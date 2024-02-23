#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "questmanager.h"
#include "sectree_manager.h"
#include "char.h"
#include "char_manager.h"
#include "desc.h"
#include "packet.h"
#include "target.h"

/////////////////////////////////////////////////////////////////////
// Packet
/////////////////////////////////////////////////////////////////////
void SendTargetCreatePacket(LPDESC d, TargetInfo * info)
{
if (!info->bSendToClient)
return;

TPacketGCTargetCreate pck;

pck.bHeader = HEADER_GC_TARGET_CREATE;
pck.lID = info->iID;
pck.bType = info->iType;
pck.dwVID = info->iArg1;
strlcpy(pck.szName, info->szTargetDesc, sizeof(pck.szName));
d->Packet(&pck, sizeof(TPacketGCTargetCreate));
}

#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
void SendTargetUpdatePacket(LPDESC d, int iID, int x, int y, bool bIsShopSearch)
#else
void SendTargetUpdatePacket(LPDESC d, int iID, int x, int y)
#endif
{
TPacketGCTargetUpdate pck;
pck.bHeader = HEADER_GC_TARGET_UPDATE;
pck.lID = iID;
pck.lX = x;
pck.lY = y;
#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
pck.bIsShopSearch = bIsShopSearch;
#endif
d->Packet(&pck, sizeof(TPacketGCTargetUpdate));
sys_log(0, "SendTargetUpdatePacket %d %dx%d", iID, x, y);
}

void SendTargetDeletePacket(LPDESC d, int iID)
{
TPacketGCTargetDelete pck;
pck.bHeader = HEADER_GC_TARGET_DELETE;
pck.lID = iID;
d->Packet(&pck, sizeof(TPacketGCTargetDelete));
}
/////////////////////////////////////////////////////////////////////
CTargetManager::CTargetManager() : m_iID(0)
{
}

CTargetManager::~CTargetManager()
{
}

EVENTFUNC(target_event)
{
	TargetInfo * info = dynamic_cast<TargetInfo *>( event->info );

	if ( info == NULL )
	{
		sys_err( "target_event> <Factor> Null pointer" );
		return 0;
	}

// <Factor> Raplaced direct pointer reference with key searching.
	//LPCHARACTER pkChr = info->pkChr;
	LPCHARACTER pkChr = CHARACTER_MANAGER::instance().FindByPID(info->dwPID);
	if (pkChr == NULL) {
		return 0; // <Factor> need to be confirmed
	}
	LPCHARACTER tch = NULL;
	int x = 0, y = 0;
	int iDist = 5000;

	if (info->iMapIndex != pkChr->GetMapIndex())
		return MINMAX(passes_per_sec / 2, iDist / (1500 / passes_per_sec), passes_per_sec * 5);

	switch (info->iType)
	{
		case TARGET_TYPE_POS:
			x = info->iArg1;
			y = info->iArg2;
			iDist = DISTANCE_APPROX(pkChr->GetX() - x, pkChr->GetY() - y);
			break;

#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
		case TARGET_TYPE_VID_SHOP_SEARCH:
#endif
			{
				tch = CHARACTER_MANAGER::instance().Find(info->iArg1);

				if (tch && tch->GetMapIndex() == pkChr->GetMapIndex())
				{
					x = tch->GetX();
					y = tch->GetY();
					iDist = DISTANCE_APPROX(pkChr->GetX() - x, pkChr->GetY() - y);
				}
			}
			break;
	}

	bool bRet = true;

	if (iDist <= 500)
		bRet = quest::CQuestManager::instance().Target(pkChr->GetPlayerID(), info->dwQuestIndex, info->szTargetName, "arrive");

#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
	if (!tch && (info->iType == TARGET_TYPE_VID || info->iType == TARGET_TYPE_VID_SHOP_SEARCH))
#else
	if (!tch && info->iType == TARGET_TYPE_VID)
#endif
	{
		quest::CQuestManager::instance().Target(pkChr->GetPlayerID(), info->dwQuestIndex, info->szTargetName, "die");
		CTargetManager::instance().DeleteTarget(pkChr->GetPlayerID(), info->dwQuestIndex, info->szTargetName);
	}

	if (event->is_force_to_end)
	{
		sys_log(0, "target_event: event canceled");
		return 0;
	}

	if (x != info->iOldX || y != info->iOldY)
	{
#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
		const bool IsShopSearch = (info->iType == TARGET_TYPE_VID_SHOP_SEARCH);
		if (info->bSendToClient)
			SendTargetUpdatePacket(pkChr->GetDesc(), info->iID, x, y, IsShopSearch);
#else
		if (info->bSendToClient)
			SendTargetUpdatePacket(pkChr->GetDesc(), info->iID, x, y);
#endif

		info->iOldX = x;
		info->iOldY = y;
	}

	if (!bRet)
		return passes_per_sec;
	else
		return MINMAX(passes_per_sec / 2, iDist / (1500 / passes_per_sec), passes_per_sec * 5);
}

void CTargetManager::CreateTarget(DWORD dwPID,
		DWORD dwQuestIndex,
		const char * c_pszTargetName,
		int iType,
		int iArg1,
		int iArg2,
		int iMapIndex,
		const char * c_pszTargetDesc,
		int iSendFlag)
{
	sys_log(0, "CreateTarget : target pid %u quest %u name %s arg %d %d %d",
			dwPID, dwQuestIndex, c_pszTargetName, iType, iArg1, iArg2);

	LPCHARACTER pkChr = CHARACTER_MANAGER::instance().FindByPID(dwPID);

	if (!pkChr)
	{
		sys_err("Cannot find character ptr by PID %u", dwPID);
		return;
	}

	if (pkChr->GetMapIndex() != iMapIndex)
		return;

	itertype(m_map_kListEvent) it = m_map_kListEvent.find(dwPID);

	if (it != m_map_kListEvent.end())
	{
		std::list<LPEVENT>::const_iterator it2 = it->second.begin();

		while (it2 != it->second.end())
		{
			LPEVENT pkEvent = *(it2++);
			TargetInfo* existInfo = dynamic_cast<TargetInfo*>(pkEvent->info);

			if (NULL == existInfo)
			{
				sys_err("CreateTarget : event already exist, but have no info");
				return;
			}

			if (existInfo->dwQuestIndex == dwQuestIndex && !strcmp(existInfo->szTargetName, c_pszTargetName))
			{
				sys_log(0, "CreateTarget : same target will be replaced");

				if (existInfo->bSendToClient)
					SendTargetDeletePacket(pkChr->GetDesc(), existInfo->iID);

				if (c_pszTargetDesc)
				{
					strlcpy(existInfo->szTargetDesc, c_pszTargetDesc, sizeof(existInfo->szTargetDesc));
				}
				else
				{
					*existInfo->szTargetDesc = '\0';
				}

				existInfo->iID = ++m_iID;
				existInfo->iType = iType;
				existInfo->iArg1 = iArg1;
				existInfo->iArg2 = iArg2;
				existInfo->iOldX = 0;
				existInfo->iOldY = 0;
				existInfo->bSendToClient = iSendFlag ? true : false;

				SendTargetCreatePacket(pkChr->GetDesc(), existInfo);
				return;
			}
		}
	}

	TargetInfo* newInfo = AllocEventInfo<TargetInfo>();

	if (c_pszTargetDesc)
	{
		strlcpy(newInfo->szTargetDesc, c_pszTargetDesc, sizeof(newInfo->szTargetDesc));
	}
	else
	{
		*newInfo->szTargetDesc = '\0';
	}

	newInfo->iID = ++m_iID;
	// <Factor> Removed pkChr
	//newInfo->pkChr = pkChr;
	newInfo->dwPID = dwPID;
	newInfo->dwQuestIndex = dwQuestIndex;
	strlcpy(newInfo->szTargetName, c_pszTargetName, sizeof(newInfo->szTargetName));
	newInfo->iType = iType;
	newInfo->iArg1 = iArg1;
	newInfo->iArg2 = iArg2;
	newInfo->iMapIndex = iMapIndex;
	newInfo->iOldX = 0;
	newInfo->iOldY = 0;
	newInfo->bSendToClient = iSendFlag ? true : false;

	LPEVENT event = event_create(target_event, newInfo, PASSES_PER_SEC(1));

	if (NULL != event)
	{
		m_map_kListEvent[dwPID].push_back(event);

		SendTargetCreatePacket(pkChr->GetDesc(), newInfo);
	}
}

void CTargetManager::DeleteTarget(DWORD dwPID, DWORD dwQuestIndex, const char * c_pszTargetName)
{
	itertype(m_map_kListEvent) it = m_map_kListEvent.find(dwPID);

	if (it == m_map_kListEvent.end())
		return;

	std::list<LPEVENT>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
		LPEVENT pkEvent = *it2;
		TargetInfo * info = dynamic_cast<TargetInfo*>(pkEvent->info);

		if ( info == NULL )
		{
			sys_err( "CTargetManager::DeleteTarget> <Factor> Null pointer" );
			++it2;
			continue;
		}

		if (dwQuestIndex == info->dwQuestIndex)
		{
			if (!c_pszTargetName || !strcmp(info->szTargetName, c_pszTargetName))
			{
				if (info->bSendToClient) {
					// <Factor> Removed pkChr
					//SendTargetDeletePacket(info->pkChr->GetDesc(), info->iID);
					LPCHARACTER pkChr = CHARACTER_MANAGER::instance().FindByPID(info->dwPID);
					if (pkChr != NULL) {
						SendTargetDeletePacket(pkChr->GetDesc(), info->iID);
					}
				}

				event_cancel(&pkEvent);
				it2 = it->second.erase(it2);
				continue;
			}
		}

		it2++;
	}
}

LPEVENT CTargetManager::GetTargetEvent(DWORD dwPID, DWORD dwQuestIndex, const char * c_pszTargetName)
{
	itertype(m_map_kListEvent) it = m_map_kListEvent.find(dwPID);

	if (it == m_map_kListEvent.end())
		return NULL;

	std::list<LPEVENT>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
		LPEVENT pkEvent = *(it2++);
		TargetInfo * info = dynamic_cast<TargetInfo*>(pkEvent->info);

		if ( info == NULL )
		{
			sys_err( "CTargetManager::GetTargetEvent> <Factor> Null pointer" );

			continue;
		}

		if (info->dwQuestIndex != dwQuestIndex)
			continue;

		if (strcmp(info->szTargetName, c_pszTargetName))
			continue;

		return pkEvent;
	}

	return NULL;
}

TargetInfo * CTargetManager::GetTargetInfo(DWORD dwPID, int iType, int iArg1)
{
	itertype(m_map_kListEvent) it = m_map_kListEvent.find(dwPID);

	if (it == m_map_kListEvent.end())
		return NULL;

	std::list<LPEVENT>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
		LPEVENT pkEvent = *(it2++);
		TargetInfo * info = dynamic_cast<TargetInfo*>(pkEvent->info);

		if ( info == NULL )
		{
			sys_err( "CTargetManager::GetTargetInfo> <Factor> Null pointer" );

			continue;
		}

		if (!IS_SET(info->iType, iType))
			continue;

		if (info->iArg1 != iArg1)
			continue;

		return info;
	}

	return NULL;
}

void CTargetManager::Logout(DWORD dwPID)
{
	itertype(m_map_kListEvent) it = m_map_kListEvent.find(dwPID);

	if (it == m_map_kListEvent.end())
		return;

	std::list<LPEVENT>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
		event_cancel(&(*(it2++)));

	m_map_kListEvent.erase(it);
}

#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
void CTargetManager::DeleteShopSearchTarget(DWORD dwPID)
{
	std::vector<DWORD> vPID;
	for (std::map<DWORD, std::list<LPEVENT>>::const_iterator it = m_map_kListEvent.begin(); it != m_map_kListEvent.end(); ++it)
	{
		for (std::list<LPEVENT>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			LPEVENT event = *(it2);
			if (event == NULL)
				continue;

			TargetInfo* eventInfo = dynamic_cast<TargetInfo*>(event->info);
			if (eventInfo != NULL && eventInfo->iArg1 == static_cast<int>(dwPID))
				vPID.push_back(it->first);
		}
	}
	for (std::vector<DWORD>::const_iterator it = vPID.begin(); it != vPID.end(); ++it)
		DeleteTarget(*(it), SHOP_SEARCH_INDEX, "__SHOPSEARCH_TARGET__");
}
#endif