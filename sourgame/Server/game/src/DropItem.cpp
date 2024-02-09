#include "stdafx.h"
#include "DropItem.h"
#include "char.h"
#include "locale_service.h"
#include "packet.h"
#include "desc_client.h"
#include "mob_manager.h"
#include "item_manager.h"

CDropItem::CDropItem()
{
}

CDropItem::~CDropItem()
{
	
}

void CDropItem::Open(LPCHARACTER ch)
{
	SendDateDropItemPackets(ch,DROP_ITEM_SUB_HEADER_OPEN_RECEIVE);
}


void CDropItem::LevelItemGroupDI(int index, int vnum_item, std::vector<int> get_mobs_item_group)
{

	std::map<DWORD, CLevelItemGroup*> m_map_pkLevelItemGroup = ITEM_MANAGER::instance().GetLevelItemGroup();
	itertype(m_map_pkLevelItemGroup) it = m_map_pkLevelItemGroup.find(get_mobs_item_group[index]);
	if (it != m_map_pkLevelItemGroup.end())
	{
		int iCount = 0;
		DWORD dwPct =  0;
		int dwCountItemGroup = 0;

		__typeof__(it->second->GetVector()) v = it->second->GetVector();

		for (DWORD i = 0; i < v.size(); ++i)
		{
			if (v[i].dwVNum == vnum_item){
				if (v[i].dwPct > dwPct){
					dwPct = v[i].dwPct;
				}
				if (v[i].iCount > iCount){
					iCount = v[i].iCount;
				}
				dwCountItemGroup++;
			}
		}

		if (dwCountItemGroup > 0){
			InsetListDi(index,get_mobs_item_group,iCount,dwPct,dwCountItemGroup);
		}
	}
	return;
}

void CDropItem::MobItemGroupDI(int index, int vnum_item, std::vector<int> get_mobs_item_group)
{
	std::map<DWORD, CMobItemGroup*> m_map_pkMobItemGroup = ITEM_MANAGER::instance().GetMobItemGroup();
	itertype(m_map_pkMobItemGroup) it1 = m_map_pkMobItemGroup.find(get_mobs_item_group[index]);
	if (it1 != m_map_pkMobItemGroup.end())
	{

		int iCount = 0;
		DWORD dwPct =  0;
		int dwCountItemGroup = 0;

		__typeof__(it1->second->GetVector()) v = it1->second->GetVector();

		for (DWORD i = 0; i < v.size(); ++i)
		{

			if (v[i].dwItemVnum == vnum_item){
				if (v[i].iRarePct > dwPct){
					dwPct = v[i].iRarePct;
				}
				if (v[i].iCount > iCount){
					iCount = v[i].iCount;
				}
				dwCountItemGroup++;
			}
		}

		if (dwCountItemGroup > 0){
			InsetListDi(index,get_mobs_item_group,iCount,dwPct,dwCountItemGroup);
		}
	}
	return;
}

void CDropItem::DropItemGroupDI(int index, int vnum_item, std::vector<int> get_mobs_item_group)
{
	std::map<DWORD, CDropItemGroup*> m_map_pkDropItemGroup = ITEM_MANAGER::instance().GetDropItemGroup();
	itertype(m_map_pkDropItemGroup) it2 = m_map_pkDropItemGroup.find(get_mobs_item_group[index]);
	if (it2 != m_map_pkDropItemGroup.end())
	{
		int iCount = 0;
		DWORD dwPct =  0;
		int dwCountItemGroup = 0;

		__typeof__(it2->second->GetVector()) v = it2->second->GetVector();

		for (DWORD i = 0; i < v.size(); ++i)
		{
			if (v[i].dwVnum == vnum_item){
				if (v[i].dwPct > dwPct){
					dwPct = v[i].dwPct;
				}
				if (v[i].iCount > iCount){
					iCount = v[i].iCount;
				}
				dwCountItemGroup++;
			}
		}

		if (dwCountItemGroup > 0){
			InsetListDi(index,get_mobs_item_group,iCount,dwPct,dwCountItemGroup);
		}
	}
	return;
}

void CDropItem::InsetListDi(int index, std::vector<int> get_mobs_item_group, int CountItem, DWORD ProbItem, int CountItemGroup)
{	
	const CMob* pMob = CMobManager::instance().Get(get_mobs_item_group[index]);
	if (pMob != NULL)
	{
		itertype(list_mobs) iter = list_mobs.find(get_mobs_item_group[index]);
		if (iter == list_mobs.end())
		{	
			
	 		
	 		ProbItem = (ProbItem/10000);

	 		if (CountItemGroup > 1){
	 			CountItem = 0;
	 		}

			WStrucInfoDropItem* DropItemDates = new WStrucInfoDropItem(get_mobs_item_group[index],pMob->m_table.szLocaleName,CountItem,ProbItem);

			list_mobs.insert(std::make_pair(get_mobs_item_group[index], DropItemDates));
		}
	}
	return;
}

void CDropItem::Search(LPCHARACTER ch, int vnum_item)
{	

	list_mobs.clear();

	std::vector<int> get_mobs_item_group = ITEM_MANAGER::instance().GetMobVnumsItemGroup();

	for (int i_m = 0; i_m < get_mobs_item_group.size(); ++i_m)
	{

		LevelItemGroupDI(i_m,vnum_item,get_mobs_item_group);
		MobItemGroupDI(i_m,vnum_item,get_mobs_item_group);
		DropItemGroupDI(i_m,vnum_item,get_mobs_item_group);
		
	}

	if (list_mobs.size() > 0)
	{
		SendDateDropItemPackets(ch,DROP_ITEM_SUB_HEADER_DATES_CLEAR_RECEIVE);
		SendDateDropItemPackets(ch,DROP_ITEM_SUB_HEADER_DATES_RECEIVE);
		SendDateDropItemPackets(ch,DROP_ITEM_SUB_HEADER_DATES_LOADING);
	}else{

		ch->ChatPacket(1,"[Search Item Drop] El item no se encuentra en el drop.");
	}
	return;
}

void CDropItem::SendDateDropItemPackets(LPCHARACTER ch, BYTE subheader)
{
	TPacketGCDropItemReceive pack;
	pack.subheader = subheader;


	if(subheader == DROP_ITEM_SUB_HEADER_DATES_RECEIVE)
	{

		for (itertype(list_mobs) it = list_mobs.begin(); it != list_mobs.end(); ++it)
		{

			memcpy (pack.info_drop.name_mob, 	it->second->name_mob.c_str(), 		sizeof(pack.info_drop.name_mob));
			pack.info_drop.id_mob 			=	it->second->id_mob;
			pack.info_drop.prob_item 			=	it->second->prob_item;
			pack.info_drop.count_item 			=	it->second->count_item;

			LPDESC d = ch->GetDesc();

			if (NULL == d)
			{
				sys_err ("User CDropItem::SendDateDropItemPackets (%s)'s DESC is NULL POINT.", ch->GetName());
				return ;
			}

			d->Packet(&pack, sizeof(pack));
		}


	}else{

		LPDESC d = ch->GetDesc();

		if (NULL == d)
		{
			sys_err ("User CDropItem::SendDateDropItemPackets (%s)'s DESC is NULL POINT.", ch->GetName());
			return ;
		}

		d->Packet(&pack, sizeof(pack));
	}
	return;
}