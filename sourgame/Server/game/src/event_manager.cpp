#include "stdafx.h"
#include "event.h"
#include "event_manager.h"
#include "text_file_loader.h"
#include "locale_service.h"
#include "quest.h"
#include "questmanager.h"
#include "priv_manager.h"
#include "sectree_manager.h"
#include "start_position.h"
#include "char.h"
#include "regen.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "utils.h"
#include "buffer_manager.h"
#include "p2p.h"
#include "OXEvent.h"
#include "char_manager.h"

#include <algorithm>

using namespace quest;

static const CEventManager::THideAndSeek aNPCPosition[HIDE_AND_SEEK_POSITIONS] =
{
	/*
		Map Index
		Vector of positions
		Vector of hint annoucement strings
	*/

	{
		61,
		{ {238, 1427}, {819, 121}, {1425, 1011}, {1043, 693}},
		{ {"Mount Sohan", "Mount Sohan 2", "Mount Sohan 3", "Mount Sohan 4"} },
	},

	{
		62,
		{ {1044, 980}, {798, 806}, {1032, 668}, {805, 1168}},
		{ {"Fireland", "Fireland 2", "Fireland 3", "Fireland 4"} },
	},

	{
		63,
		{ {821, 787}, {435, 703}, {698, 428}, {1485, 692}},
		{ {"Sahara Desert", "Sahara Desert 2", "Sahara Desert 3", "Sahara Desert 4"} },
	},

	{
		64,
		{ {184, 121}, {1225, 279}, {130, 1219}, {1438, 1208}},
		{ {"Valley of Seungryong", "Valley of Seungryong 2", "Valley of Seungryong 3", "Valley of Seungryong 4"} },
	},
};

bool SortStartQueue(const TEventTable* a, const TEventTable* b)
{
	return a->startTime < b->startTime;
}

bool SortEndQueue(const TEventTable* a, const TEventTable* b)
{
	return a->endTime < b->endTime;
}

bool SortKingdomWar(const CEventManager::TKingdomWarCount& a, const CEventManager::TKingdomWarCount& b)
{
	return a.wCount < b.wCount;
}

CEventManager::CEventManager()
{
	m_mapEventName.clear();
	m_mapEvent.clear();
	m_dequeEventStart.clear();
	m_dequeEventEnd.clear();

	BuildEventNameMap();

	m_bReload = false;

	m_pOXEvent = nullptr;

	m_bSiegeScore = 0;

	memset(m_aKingdomWarScore, 0, sizeof(m_aKingdomWarScore));

	m_pHideAndSeekNPC = nullptr;
	m_iCurrentHideAndSeekRound = -1;
}

CEventManager::~CEventManager()
{
	for (auto& kv : m_mapEvent)
		M2_DELETE(kv.second);

	m_mapEvent.clear();
	m_dequeEventStart.clear();
	m_dequeEventEnd.clear();
}

struct FSendKingdomWarScore
{
	FSendKingdomWarScore() 
	{
	}

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;

			if (ch->IsPC())
				CEventManager::Instance().SendKingdomWarScorePacket(ch);
		}
	}
};

EVENTINFO(warp_all_to_village_event_info)
{
	DWORD	dwWarpMapIndex;
	BYTE	bEmpire;

	warp_all_to_village_event_info()
		: dwWarpMapIndex(0), bEmpire(0)
	{
	}
};

struct FWarpAllToVillage
{
	BYTE m_bEmpire;
	FWarpAllToVillage()
	{
		m_bEmpire = 0;
	}

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsPC())
			{
				BYTE bEmpire = ch->GetEmpire();
				if (bEmpire == 0)
				{
					sys_err("Unknown Empire %s %d ", ch->GetName(), ch->GetPlayerID());
					return;
				}
					    
				if (m_bEmpire)
				{
					if(m_bEmpire == bEmpire)
						ch->WarpSet(g_start_position[bEmpire][0], g_start_position[bEmpire][1]);
				}
				else
					ch->WarpSet(g_start_position[bEmpire][0], g_start_position[bEmpire][1]);
			}
		}
	}
};

EVENTFUNC(warp_all_to_village_event)
{
	warp_all_to_village_event_info* info = dynamic_cast<warp_all_to_village_event_info*>(event->info);

	if (!info)
	{
		sys_err("warp_all_to_village_event> <Factor> Null pointer");
		return 0;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(info->dwWarpMapIndex);

	if (pSecMap)
	{
		FWarpAllToVillage f;
		f.m_bEmpire = info->bEmpire;

		pSecMap->for_each(f);
	}

	return 0;
}

struct FKillSectree
{
	void operator () (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsMonster() || ch->IsStone() || mining::IsVeinOfOre(ch->GetRaceNum()))
				ch->Dead();
		}
	}
};

EVENTINFO(dynamic_spawn_cycle_event_info)
{
	DWORD dwWarpMapIndex;
	std::string strRegenPath;
	std::string strRegenPath2;
	BYTE step;

	dynamic_spawn_cycle_event_info()
		: dwWarpMapIndex(0),
		strRegenPath(""),
		strRegenPath2(""),
		step(0)
	{
	}
};

EVENTFUNC(dynamic_spawn_cycle_event)
{
	dynamic_spawn_cycle_event_info* info = dynamic_cast<dynamic_spawn_cycle_event_info*>(event->info);

	if (!info)
	{
		sys_err("dynamic_spawn_cycle_event <Factor> Null pointer to event");
		return 0;
	}

	if (info->dwWarpMapIndex == 0 || info->strRegenPath == "" || info->strRegenPath2 == "")
	{
		sys_err("dynamic_spawn_cycle_event: Null pointer to event info");
		return 0;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(info->dwWarpMapIndex);

	if (pSecMap)
	{
		info->step += 1;

		/*
			Clear each entity on the map on the beggining of 1st and 6th round.
		*/
		if (info->step == CLEAR_ENTITY_STAGE1_ROUND ||
			info->step == CLEAR_ENTITY_STAGE2_ROUND)
		{
			FKillSectree f;
			pSecMap->for_each(f);
		}

		/*
			Round 1~5 -> Stage 1
			Round 6~10 -> Stage 2
			Exit after round 10
		*/
		if (info->step <= DYNAMIC_CYCLE_STAGE1)
		{
			regen_load_in_file(info->strRegenPath.c_str(), info->dwWarpMapIndex, pSecMap->m_setting.iBaseX, pSecMap->m_setting.iBaseY);
		}
		else if (info->step > DYNAMIC_CYCLE_STAGE1 &&
			info->step <= DYNAMIC_CYCLE_STAGE2)
		{
			regen_load_in_file(info->strRegenPath2.c_str(), info->dwWarpMapIndex, pSecMap->m_setting.iBaseX, pSecMap->m_setting.iBaseY);
		}
		else if (info->step > DYNAMIC_CYCLE_STAGE2)
			return 0;
	}

	return PASSES_PER_SEC(5 * 60);
}

EVENTINFO(static_spawn_cycle_event_info)
{
	DWORD dwWarpMapIndex;
	std::string strRegenPath;
	BYTE step;

	static_spawn_cycle_event_info()
		: dwWarpMapIndex(0),
		strRegenPath(""),
		step(0)
	{
	}
};

EVENTFUNC(static_spawn_cycle_event)
{
	static_spawn_cycle_event_info* info = dynamic_cast<static_spawn_cycle_event_info*>(event->info);

	if (!info)
	{
		sys_err("static_spawn_cycle_event <Factor> Null pointer to event");
		return 0;
	}

	if (info->dwWarpMapIndex == 0 || info->strRegenPath == "")
	{
		sys_err("static_spawn_cycle_event: Null pointer to event info");
		return 0;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(info->dwWarpMapIndex);

	if (pSecMap)
	{
		info->step += 1;

		/*
			Clear each entity on the map on the beggining of 1st.
			Do not clear map during Tanaka Event.
		*/
		if (info->step == CLEAR_ENTITY_STAGE1_ROUND &&
			info->dwWarpMapIndex != TANAKA_EVENT_MAP)
		{
			FKillSectree f;
			pSecMap->for_each(f);
		}

		/*
			Round 1~3 -> Stage 1
			Exit after round 3
		*/
		if (info->step <= STATIC_CYCLE_MAX_ROUND)
		{
			regen_load_in_file(info->strRegenPath.c_str(), info->dwWarpMapIndex, pSecMap->m_setting.iBaseX, pSecMap->m_setting.iBaseY);
		}
		else
			return 0;
	}

	return PASSES_PER_SEC(8 * 60); 
}

/*
	Execeute event state at both queues for the 
	first element in list. First element's time is always the
	closest one to the current time. (Sorted by time at initialization)

	Send an announcement before the start of the event.
*/
void CEventManager::Process()
{
	if (IsReloadMode())
		return;

	if (g_bChannel == EVENT_CHANNEL)
	{
		if (!m_dequeEventStart.empty())
		{
			TEventTable* pStartEvent = m_dequeEventStart.front();
			if (pStartEvent->startTime <= get_global_time())
			{

				SetEventState(pStartEvent, true);
				Dequeue(QUEUE_TYPE_START, pStartEvent->dwID);
			}

			// Send an announcment for the 5 closest upcoming events
			for (int i = 0; i < 5; ++i)
			{
				if (i >= m_dequeEventStart.size())
					break;

				TEventTable* pStartEvent = m_dequeEventStart.at(i);
				time_t lSecondsLeft = pStartEvent->startTime - get_global_time();

				if (lSecondsLeft <= 3600)
				{
					char szBuf[256 + 1];
					switch (lSecondsLeft)
					{
						case 3600:
							snprintf(szBuf, sizeof(szBuf), "%s will be starting in 1 hour.", GetEventName(GetEvent(pStartEvent->szType)).c_str());
							BroadcastNotice(szBuf);
							break;

						case 2700:
							snprintf(szBuf, sizeof(szBuf), "%s will be starting in 45 minutes.", GetEventName(GetEvent(pStartEvent->szType)).c_str());
							BroadcastNotice(szBuf);
							break;

						case 1800:
							snprintf(szBuf, sizeof(szBuf), "%s will be starting in 30 minutes.", GetEventName(GetEvent(pStartEvent->szType)).c_str());
							BroadcastNotice(szBuf);
							break;

						case 900:
							snprintf(szBuf, sizeof(szBuf), "%s will be starting in 15 minutes.", GetEventName(GetEvent(pStartEvent->szType)).c_str());
							BroadcastNotice(szBuf);
							break;

						case 300:
							snprintf(szBuf, sizeof(szBuf), "%s will be starting in 5 minutes.", GetEventName(GetEvent(pStartEvent->szType)).c_str());
							BroadcastNotice(szBuf);
							break;
					}
				}

			}
		}

		if (!m_dequeEventEnd.empty())
		{
			TEventTable* pEndEvent = m_dequeEventEnd.front();
			if (pEndEvent->endTime <= get_global_time())
			{
				SetEventState(pEndEvent, false);
				Dequeue(QUEUE_TYPE_END, pEndEvent->dwID);
			}
		}
	}
}

/*
	Function will be called at the initialization of events.
	Each game event will be added to start-event queue and 
	end-event queue (dependant on bQueue value).
*/
void CEventManager::Enqueue(BYTE bQueue, TEventTable* table)
{
	if (bQueue == QUEUE_TYPE_START)
	{
		if (table->startTime <= get_global_time())
			SetEventState(table, true);
		else
			m_dequeEventStart.push_back(table);
	}
	else if (bQueue == QUEUE_TYPE_END)
	{
		m_dequeEventEnd.push_back(table);
	}
}

/*
	Events are dequeued from their respective deque on start/end.
	At the end of an event, a packet is sent to database to update
	event completion status. Database then sents a packet back
	to update information on every game core.
*/
void CEventManager::Dequeue(BYTE bQueue, DWORD dwID)
{
	if (bQueue == QUEUE_TYPE_START)
	{
		m_dequeEventStart.pop_front();
	}
	else if (bQueue == QUEUE_TYPE_END)
	{
		m_dequeEventEnd.pop_front();

		auto eventIt = m_mapEvent.find(dwID);

		if (eventIt != m_mapEvent.end())
			db_clientdesc->DBPacket(HEADER_GD_UPDATE_EVENT_STATUS, 0, &eventIt->second->dwID, sizeof(DWORD));
	}
}

void CEventManager::CancelActiveEvents()
{
	for (auto &kv : m_mapEvent)
	{
		DWORD dwIndex = kv.first;
		TEventTable* pEvent = kv.second;

		bool bStartFound = false;
		for (auto it = m_dequeEventStart.begin(); it != m_dequeEventStart.end(); ++it) 
		{
			TEventTable* pEventQueue = *it;
			if (pEventQueue->dwID == dwIndex)
			{
				bStartFound = true;
				break;
			}
		}

		bool bEndFound = false;
		for (auto it = m_dequeEventEnd.begin(); it != m_dequeEventEnd.end(); ++it)
		{
			TEventTable* pEventQueue = *it;
			if (pEventQueue->dwID == dwIndex)
			{
				bEndFound = true;
				break;
			}
		}

		/* 
			If an event is only present in the end queue, it means it has already started.
			Event must be ended to reset the game flag for future auto-start.
		*/
		if (!bStartFound && bEndFound)
		{
			int iEvent = GetEvent(kv.second->szType);

			SetEventState(kv.second, false);
		}

		M2_DELETE(pEvent);
	}

	m_mapEvent.clear();
	m_dequeEventStart.clear();
	m_dequeEventEnd.clear();
}

void CEventManager::Initialize(TEventTable* pTab, int iSize, bool bReload /* = false */)
{
	if(g_bChannel == EVENT_CHANNEL)
		regen_free_map(EVENT_MAP_INDEX);

	if (bReload)
	{
		SetReloadMode(true);
		CancelActiveEvents();
	}

	for (int i = 0; i < iSize; ++i, ++pTab)
	{
		if (!GetEvent(pTab->szType))
		{
			sys_err("Skipping event table id %d, unknown event type %s", pTab->dwID, pTab->szType);
			continue;
		}

		if (!pTab->startTime || !pTab->endTime)
		{
			sys_err("Skipping event table id %d, unknown start/end time type %d", pTab->dwID, pTab->szType);
			continue;
		}

		TEventTable* t = M2_NEW TEventTable;
		t->dwID = pTab->dwID;
		strlcpy(t->szType, pTab->szType, sizeof(t->szType));
		t->startTime = pTab->startTime;
		t->endTime = pTab->endTime;
		t->iValue0 = pTab->iValue0;
		t->iValue1 = pTab->iValue1;
		t->bCompleted = pTab->bCompleted;

		m_mapEvent.emplace(std::make_pair(t->dwID, t));

		// Event queues are initialized on only one specific game channel
		if (g_bChannel == EVENT_CHANNEL && !t->bCompleted)
		{
			// Add event to the start queue
			Enqueue(QUEUE_TYPE_START, t);

			// Add event to the end queue
			Enqueue(QUEUE_TYPE_END, t);
		}
	}

	std::sort(m_dequeEventStart.begin(), m_dequeEventStart.end(), SortStartQueue);
	std::sort(m_dequeEventEnd.begin(), m_dequeEventEnd.end(), SortEndQueue);

	// Send a packet to reload event board on client-side incase it was a reload
	if ((g_bChannel == EVENT_CHANNEL) && bReload)
		ReloadEvent();

	SetReloadMode(false);
}

void CEventManager::UpdateEventStatus(DWORD dwID)
{
	auto eventIt = m_mapEvent.find(dwID);

	if (eventIt != m_mapEvent.end())
		eventIt->second->bCompleted = true;
}

int CEventManager::GetEvent(const std::string& strEventName)
{
	if (m_mapEventName.find(strEventName) != m_mapEventName.end())
		return m_mapEventName[strEventName];

	return EVENT_TYPE_NONE;
}

std::string CEventManager::GetEventName(int iEvent)
{
	const static std::map<int, std::string> mapEventName =
	{
		{ EVENT_TYPE_EXPERIENCE,		"Experience Event"},
		{ EVENT_TYPE_ITEM_DROP,			"Item - Drop Boost Event"},
		{ EVENT_TYPE_BOSS,				"Boss Event"},
		{ EVENT_TYPE_METIN,				"Metin Shower Event"},
		{ EVENT_TYPE_MINING,			"Mining Event"},
		{ EVENT_TYPE_GOLD_FROG,			"Golden Frog Event"},
		{ EVENT_TYPE_MOONLIGHT,			"Moonlight Chest Event"},
		{ EVENT_TYPE_HEXEGONAL_CHEST,	"Hexegonal Box Event"},
		{ EVENT_TYPE_FISHING,			"Fishing Event" },
		{ EVENT_TYPE_HIDE_AND_SEEK,		"Hide and Seek Event" },
		{ EVENT_TYPE_OX,				"OX Event" },
		{ EVENT_TYPE_TANAKA,			"Tanka Event" },
		{ EVENT_TYPE_SIEGE_WAR,			"Siege War Event" },
		{ EVENT_TYPE_KINGDOM_WAR,		"Kingdom War Event" },
	};

	return mapEventName.at(iEvent);
}

std::string CEventManager::GetEventString(int iEvent)
{
	for (auto kv : m_mapEventName)
	{
		if (kv.second == iEvent)
			return kv.first;
	}

	return "EVENT_TYPE_NONE";
}

void CEventManager::SendEventInfo(const LPCHARACTER ch, int iMonth)
{
	TEMP_BUFFER buf;

	for (const auto& kv : m_mapEvent)
	{
		const time_t startTime = kv.second->startTime;
		const struct tm* tStart = localtime(&startTime);

		// Send current month's events and events from previous months which are still active
		if (((tStart->tm_mon < iMonth) && !kv.second->bCompleted) || (tStart->tm_mon == iMonth))
		{
			TPacketEventData eventData;
			eventData.dwID = kv.second->dwID;
			eventData.bType = GetEvent(kv.second->szType);
			eventData.startTime = kv.second->startTime;
			eventData.endTime = kv.second->endTime;
			eventData.iValue0 = kv.second->iValue0;
			eventData.iValue1 = kv.second->iValue1;
			eventData.bCompleted = kv.second->bCompleted;

			buf.write(&eventData, sizeof(TPacketEventData));
		}
	}

	TPacketGCEventInfo p;
	p.bHeader = HEADER_GC_EVENT_INFO;
	p.wSize = sizeof(TPacketGCEventInfo) + buf.size();

	ch->GetDesc()->BufferedPacket(&p, sizeof(TPacketGCEventInfo));

	if(buf.size())
		ch->GetDesc()->Packet(buf.read_peek(), buf.size());
}

void CEventManager::BuildEventNameMap()
{
	m_mapEventName["EVENT_TYPE_NONE"] = EVENT_TYPE_NONE;
	m_mapEventName["EVENT_TYPE_EXPERIENCE"] = EVENT_TYPE_EXPERIENCE;
	m_mapEventName["EVENT_TYPE_ITEM_DROP"] = EVENT_TYPE_ITEM_DROP;
	m_mapEventName["EVENT_TYPE_BOSS"] = EVENT_TYPE_BOSS;
	m_mapEventName["EVENT_TYPE_METIN"] = EVENT_TYPE_METIN;
	m_mapEventName["EVENT_TYPE_MINING"] = EVENT_TYPE_MINING;
	m_mapEventName["EVENT_TYPE_GOLD_FROG"] = EVENT_TYPE_GOLD_FROG;
	m_mapEventName["EVENT_TYPE_MOONLIGHT"] = EVENT_TYPE_MOONLIGHT;
	m_mapEventName["EVENT_TYPE_HEXEGONAL_CHEST"] = EVENT_TYPE_HEXEGONAL_CHEST;
	m_mapEventName["EVENT_TYPE_FISHING"] = EVENT_TYPE_FISHING;
	m_mapEventName["EVENT_TYPE_HIDE_AND_SEEK"] = EVENT_TYPE_HIDE_AND_SEEK;
	m_mapEventName["EVENT_TYPE_OX"] = EVENT_TYPE_OX;
	m_mapEventName["EVENT_TYPE_TANAKA"] = EVENT_TYPE_TANAKA;
	m_mapEventName["EVENT_TYPE_SIEGE_WAR"] = EVENT_TYPE_SIEGE_WAR;
	m_mapEventName["EVENT_TYPE_KINGDOM_WAR"] = EVENT_TYPE_KINGDOM_WAR;
}

bool CEventManager::GetEventState(int iEventType)
{
	switch (iEventType)
	{
		case EVENT_TYPE_EXPERIENCE:
			return CQuestManager::instance().GetEventFlag("exp_event");
			break;
		case EVENT_TYPE_ITEM_DROP:
			return CQuestManager::instance().GetEventFlag("item_drop_event");
			break;
		case EVENT_TYPE_BOSS:
			return CQuestManager::instance().GetEventFlag("boss_event");
			break;
		case EVENT_TYPE_METIN:
			return CQuestManager::instance().GetEventFlag("metin_event");
			break;
		case EVENT_TYPE_MINING:
			return CQuestManager::instance().GetEventFlag("mining_event");
			break;
		case EVENT_TYPE_GOLD_FROG:
			return CQuestManager::instance().GetEventFlag("golden_frog_event");
			break;
		case EVENT_TYPE_MOONLIGHT:
			return CQuestManager::instance().GetEventFlag("moon_drop");
			break;
		case EVENT_TYPE_HEXEGONAL_CHEST:
			return CQuestManager::instance().GetEventFlag("hexegonal_drop");
			break;
		case EVENT_TYPE_FISHING:
			return CQuestManager::instance().GetEventFlag("fish_event");
			break;
		case EVENT_TYPE_HIDE_AND_SEEK:
			return CQuestManager::instance().GetEventFlag("hide_seek_event");
			break;
		case EVENT_TYPE_OX:
			return CQuestManager::instance().GetEventFlag("oxevent_status");
			break;
		case EVENT_TYPE_TANAKA:
			return CQuestManager::instance().GetEventFlag("tanaka_event");
			break;
		case EVENT_TYPE_SIEGE_WAR:
			return CQuestManager::instance().GetEventFlag("siege_war_event");
			break;
		case EVENT_TYPE_KINGDOM_WAR:
			return CQuestManager::instance().GetEventFlag("kingdom_war_event");
			break;
		default:
			return false;
			break;
	}
}

void CEventManager::SetEventState(TEventTable* pTable, bool bState)
{
	int iEvent = GetEvent(pTable->szType);

	/* 
		Return if the event state is already set, ignore
		if it is not a reload -> game boot
	*/
	if ((GetEventState(iEvent) == bState) && !IsReloadMode())
		return;

	bool ret = false;

	switch (iEvent)
	{
		case EVENT_TYPE_EXPERIENCE:
			ret = SetExperienceEvent(bState, pTable);
			break;
		case EVENT_TYPE_ITEM_DROP:
			ret = SetItemDropEvent(bState, pTable);
			break;
		case EVENT_TYPE_BOSS:
			ret = SetBossEvent(bState);
			break;
		case EVENT_TYPE_METIN:
			ret = SetMetinEvent(bState);
			break;
		case EVENT_TYPE_MINING:
			ret = SetMiningEvent(bState);
			break;
		case EVENT_TYPE_GOLD_FROG:
			ret = SetGoldFrogEvent(bState);
			break;
		case EVENT_TYPE_MOONLIGHT:
			ret = SetMoonlightEvent(bState);
			break;
		case EVENT_TYPE_HEXEGONAL_CHEST:
			ret = SetHexegonalEvent(bState);
			break;
		case EVENT_TYPE_FISHING:
			ret = SetFishingEvent(bState);
			break;
		case EVENT_TYPE_HIDE_AND_SEEK:
			ret = SetHideAndSeekEvent(bState);
			break;
		case EVENT_TYPE_OX:
			ret = SetOXEvent(bState);
			break;
		case EVENT_TYPE_TANAKA:
			ret = SetTanakaEvent(bState, pTable);
			break;
		case EVENT_TYPE_SIEGE_WAR:
			ret = SetSiegeWarEvent(bState);
			break;
		case EVENT_TYPE_KINGDOM_WAR:
			ret = SetKingdomWarEvent(bState);
			break;
	}

	if (!ret)
		sys_err("Event %s could not be started/stopped, state %d", pTable->szType, bState);
}

/*
	When reloading, game requires instant information about
	game flags. Waiting for it to retrieve from database is not an
	option so we are updating this core's game flag directly while
	also sending a packet to update db.
*/
void CEventManager::UpdateGameFlag(const char* c_szFlag, long lValue)
{
	CQuestManager::Instance().SetEventFlag(c_szFlag, (int)lValue);

	TPacketSetEventFlag p;
	strlcpy(p.szFlagName, c_szFlag, sizeof(p.szFlagName));
	p.lValue = lValue;
	db_clientdesc->DBPacket(HEADER_GD_EVENT_NOTIFICATION, 0, &p, sizeof(TPacketSetEventFlag));

	if (lValue)
		SendEventBeginNotification();
	else
		SendEventEndNotification();
}

void CEventManager::OnDead(LPCHARACTER pVictim, LPCHARACTER pKiller)
{
	if (!pKiller || !pKiller->IsPC())
		return;

	if (pVictim->IsPC())
	{
		if (GetEventState(EVENT_TYPE_KINGDOM_WAR))
		{
			++m_aKingdomWarScore[pKiller->GetEmpire()].wCount;

				LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(KINGDOM_WAR_MAP_INDEX);

				FSendKingdomWarScore f;
				pSecMap->for_each(f);

				/*
				// End the event on specific kill count

				if (m_aKingdomWarScore[pKiller->GetEmpire()].wCount >= 100)
					SetKingdomWarEvent(false);
				*/
		}
	}
	else if (pVictim->IsStone())
	{
		switch (pVictim->GetRaceNum())
		{
			case 8020:
			{
				BroadcastNotice("Shinsoo Stone has been destroyed. Shinsoo players will be teleported to their villages.");
				++m_bSiegeScore;

				// Teleport players from the map
				LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(SIEGE_MAP_INDEX);

				if (pSecMap)
				{
					warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();
					info->dwWarpMapIndex = SIEGE_MAP_INDEX;
					info->bEmpire = SHINSOO_EMPIRE;

					event_create(warp_all_to_village_event, info, PASSES_PER_SEC(3));
				}

				// End the event if the 2nd Kingdom has been defeated
				if (m_bSiegeScore == 2)
				{
					// Update the flag immidiately without P2P procedure
					CQuestManager::Instance().SetEventFlag("siege_shinsoo_score", m_bSiegeScore);
					SetSiegeWarEvent(false);
				}
				else
				{
					CQuestManager::Instance().RequestSetEventFlag("siege_shinsoo_score", m_bSiegeScore);
				}

			} break;

			case 8022:
			{
				BroadcastNotice("Chunjo Stone has been destroyed. Chunjo players will be teleported to their villages.");
				++m_bSiegeScore;

				// Teleport players from the map
				LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(SIEGE_MAP_INDEX);

				if (pSecMap)
				{
					warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();
					info->dwWarpMapIndex = SIEGE_MAP_INDEX;
					info->bEmpire = CHUNJO_EMPIRE;

					event_create(warp_all_to_village_event, info, PASSES_PER_SEC(3));
				}

				// End the event if the 2nd Kingdom has been defeated
				if (m_bSiegeScore == 2)
				{
					// Additional set-event-flag is required to update the flag immidiately without P2P procedure
					CQuestManager::Instance().SetEventFlag("siege_chunjo_score", m_bSiegeScore);
					SetSiegeWarEvent(false);
				}
				else
				{
					CQuestManager::Instance().RequestSetEventFlag("siege_chunjo_score", m_bSiegeScore);
				}
			} break;

			case 8021:
			{
				BroadcastNotice("Jinno Stone has been destroyed. Jinno players will be teleported to their villages.");
				++m_bSiegeScore;

				// Teleport players from the map
				LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(SIEGE_MAP_INDEX);

				if (pSecMap)
				{
					warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();
					info->dwWarpMapIndex = SIEGE_MAP_INDEX;
					info->bEmpire = JINNO_EMPIRE;

					event_create(warp_all_to_village_event, info, PASSES_PER_SEC(3));
				}

				// End the event if the 2nd Kingdom has been defeated
				if (m_bSiegeScore == 2)
				{
					// Additional set-event-flag is required to update the flag immidiately without P2P procedure
					CQuestManager::Instance().SetEventFlag("siege_jinno_score", m_bSiegeScore);
					SetSiegeWarEvent(false);
				}
				else
				{
					CQuestManager::Instance().RequestSetEventFlag("siege_jinno_score", m_bSiegeScore);
				}
			} break;
		}
	}
}

bool CEventManager::SetExperienceEvent(bool bActive, TEventTable* pTable)
{
	if (bActive && (pTable->iValue0 == 0))
	{
		sys_err("CEventManager::SetExperienceEvent: rate is set to null.");
		return false;
	}

	UpdateGameFlag("exp_event", bActive);

	if (bActive)
	{
		long time = pTable->endTime - pTable->startTime;
		if (time <= 0)
		{
			sys_err("CEventManager::SetExperienceEvent: time is set to null.");
			return false;
		}

		BroadcastNotice("Experience Boost Event has started. Enjoy the leveling.");
		CPrivManager::instance().RequestGiveEmpirePriv(ALL_EMPIRES, PRIV_EXP_PCT, pTable->iValue0, time);
	}
	else
	{
		BroadcastNotice("Experience Boost Event has ended.");
	}

	return true;
}

bool CEventManager::SetItemDropEvent(bool bActive, TEventTable* pTable)
{
	if (bActive && (pTable->iValue0 == 0))
	{
		sys_err("CEventManager::SetItemDropEvent: rate is set to null.");
		return false;
	}

	UpdateGameFlag("item_drop_event", bActive);

	if (bActive)
	{
		long time = pTable->endTime - pTable->startTime;
		if (time <= 0)
		{
			sys_err("CEventManager::SetExperienceEvent: time is set to null.");
			return false;
		}

		BroadcastNotice("Item-Drop Boost Event has started. Enjoy the grinding.");
		CPrivManager::instance().RequestGiveEmpirePriv(ALL_EMPIRES, PRIV_ITEM_DROP, pTable->iValue0, time);
	}
	else
		BroadcastNotice("Item-Drop Boost Event has ended.");

	return true;
}

bool CEventManager::SetBossEvent(bool bActive)
{
	UpdateGameFlag("boss_event", bActive);

	if (bActive)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		dynamic_spawn_cycle_event_info * info = AllocEventInfo<dynamic_spawn_cycle_event_info>();
		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		info->strRegenPath = "data/event/boss_event_regen_type_1.txt";
		info->strRegenPath2 = "data/event/boss_event_regen_type_2.txt";

		event_create(dynamic_spawn_cycle_event, info, PASSES_PER_SEC(1));

		BroadcastNotice("Boss Hunt Event has started. Open the Quest Scroll to join.");
	}
	else
	{
		BroadcastNotice("Boss Hunt Event has ended. Thank you for participating.");

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();
		info->dwWarpMapIndex = EVENT_MAP_INDEX;

		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(60));

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), EVENT_MAP_INDEX, false);

		FKillSectree f;
		pSecMap->for_each(f);
	}
	return true;
}

bool CEventManager::SetMetinEvent(bool bActive)
{
	UpdateGameFlag("metin_event", bActive);

	if (bActive)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		dynamic_spawn_cycle_event_info * info = AllocEventInfo<dynamic_spawn_cycle_event_info>();
		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		info->strRegenPath = "data/event/metin_event_regen_type_1.txt";
		info->strRegenPath2 = "data/event/metin_event_regen_type_2.txt";

		event_create(dynamic_spawn_cycle_event, info, PASSES_PER_SEC(1));

		BroadcastNotice("Metin Shower Event has started. Open the Quest Scroll to join.");
	}
	else
	{
		BroadcastNotice("Metin Shower Event has ended. Thank you for participating.");
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();

		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(15));

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), EVENT_MAP_INDEX, false);

		FKillSectree f;
		pSecMap->for_each(f);
	}

	return true;
}

bool CEventManager::SetMiningEvent(bool bActive)
{
	UpdateGameFlag("mining_event", bActive);

	if (bActive)
	{

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		regen_free_map(EVENT_MAP_INDEX);

		if (!regen_load("data/event/mining_event_regen_type_0.txt", EVENT_MAP_INDEX, pSecMap->m_setting.iBaseX, pSecMap->m_setting.iBaseY))
			return false;

		BroadcastNotice("Mining Event has started. Open the Quest Scroll to join.");

	}
	else
	{
		BroadcastNotice("Mining Event has ended. Thank you for participating.");
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();

		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(15));

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), EVENT_MAP_INDEX, false);

		regen_free_map(EVENT_MAP_INDEX);

		FKillSectree f;
		pSecMap->for_each(f);
	}

	return true;
}

bool CEventManager::SetGoldFrogEvent(bool bActive)
{
	UpdateGameFlag("golden_frog_event", bActive);

	if (bActive)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		static_spawn_cycle_event_info * info = AllocEventInfo<static_spawn_cycle_event_info>();
		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		info->strRegenPath = "data/event/golden_frog_event_regen_type_0.txt";

		event_create(static_spawn_cycle_event, info, PASSES_PER_SEC(1));
		BroadcastNotice("Golden Frog Event has started. Open the Quest Scroll to join.");

	}
	else
	{
		BroadcastNotice("Golden Frog Event has ended.Thank you for participating.");

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();

		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(15));

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), EVENT_MAP_INDEX, false);

		FKillSectree f;
		pSecMap->for_each(f);
	}

	return true;
}

bool CEventManager::SetMoonlightEvent(bool bActive)
{
	UpdateGameFlag("moon_drop", bActive);

	if (bActive)
	{
		BroadcastNotice("Moonlight Chest Event has started! Defeat enemies and grab your rewards!");
	}
	else
	{
		BroadcastNotice("Moonlight Chest Event has ended!");
	}

	return true;
}

bool CEventManager::SetHexegonalEvent(bool bActive)
{
	UpdateGameFlag("hexegonal_drop", bActive);

	if (bActive)
	{
		BroadcastNotice("Hexegonal Box Event has started! Defeat enemies and grab your rewards!");
	}
	else
	{
		BroadcastNotice("Hexegonal Box Event has ended!");
	}

	return true;
}

bool CEventManager::SetFishingEvent(bool bActive)
{
	UpdateGameFlag("fish_event", bActive);

	if (bActive)
	{
		BroadcastNotice("Fishing Event has started. Bring Fishbones to the Fisherman and receive your reward.");
	}
	else
	{
		BroadcastNotice("Fishing Event has ended. Thank you for participating!");
	}

	return true;
}

bool CEventManager::SetHideAndSeekEvent(bool bActive)
{
	UpdateGameFlag("hide_seek_event", bActive);

	if (bActive)
	{
		BroadcastNotice("Hide and Seek Event has started. Search for the Old Box to receive a reward!");
		SetHideAndSeekRound(HIDE_AND_SEEK_ROUNDS);

		TPacketGGEventHideAndSeek packet;
		packet.bHeader = HEADER_GG_EVENT_HIDE_AND_SEEK;
		packet.iPosition = number(0, HIDE_AND_SEEK_POSITIONS - 1);
		packet.iRound = HIDE_AND_SEEK_ROUNDS;

		P2P_MANAGER::Instance().Send(&packet, sizeof(packet));
	}
	else
	{
		BroadcastNotice("Hide and Seek Event has ended. Thank you for participating!");

		CQuestManager::Instance().RequestSetEventFlag("hide_seek_vid", 0);
		SetHideAndSeekRound(0);

		// Send a packet to other game cores to remove the NPC in case it was not found by players
		TPacketGGEventHideAndSeek packet;
		packet.bHeader = HEADER_GG_EVENT_HIDE_AND_SEEK;
		packet.iPosition = -1;
		packet.iRound = -1;
		P2P_MANAGER::Instance().Send(&packet, sizeof(packet));
	}

	return true;
}

/*
	If iPosition is equal to -1, it will only try to find an unfound NPC
	and purge it from the game. If iPosition is greater than -1, an NPC
	will be spawned if the map data that corresponds to NPCPosition[iPosition]
	is valid.
*/
bool CEventManager::HideAndSeekNPC(int iPosition, int iRound)
{
	// Delete the spawned NPC
	{
		if (m_pHideAndSeekNPC)
		{
			M2_DESTROY_CHARACTER(m_pHideAndSeekNPC);
			m_pHideAndSeekNPC = nullptr;
		}

		// If position is negative, we only need to remove the NPC
		if (iPosition < 0)
			return true;
	}

	// Close the event in case there are no rounds left
	if (iRound <= 0)
	{
		TEventTable eventTable;
		memset(&eventTable, 0, sizeof(eventTable));
		strlcpy(eventTable.szType, "EVENT_TYPE_HIDE_AND_SEEK", sizeof(eventTable.szType));

		SetEventState(&eventTable, false);
		return true;
	}

	// Decrease the round count
	SetHideAndSeekRound(--iRound);

	const THideAndSeek table = aNPCPosition[iPosition];
	if (!table.dwMapIndex)
	{
		sys_err("could not find map data on position %d", iPosition);
		return false;
	}

	// Return if there is no map in the game core
	if (!map_allow_find(table.dwMapIndex))
		return false;

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(table.dwMapIndex);

	if (!pSecMap)
		return false;

	// Get a random coordinates index for the NPC
	const BYTE bLocationIndex = number(0, table.vec_Positions.size() - 1);

	LPCHARACTER ch = CHARACTER_MANAGER::Instance().SpawnMob(HIDE_AND_SEEK_NPC, 
		table.dwMapIndex,
		pSecMap->m_setting.iBaseX + table.vec_Positions[bLocationIndex].x * 100, 
		pSecMap->m_setting.iBaseY + table.vec_Positions[bLocationIndex].y * 100, 
		0);

	if (!ch)
	{
		sys_err("could not spawn NPC on map %d pos %d %d", table.dwMapIndex, table.vec_Positions[bLocationIndex].x, table.vec_Positions[bLocationIndex].y);
		return false;
	}

	// Assign the hidden NPC and save it's VID
	m_pHideAndSeekNPC = ch;
	CQuestManager::Instance().RequestSetEventFlag("hide_seek_vid", ch->GetVID());

	// Select a random hint announcement and display it to players
	std::string strHintAnnouncement = table.vec_Annoucments.at(bLocationIndex);  //number(0, table.vec_Annoucments.size() - 1) 
	BroadcastNotice(strHintAnnouncement.c_str());

	return true;
}

bool CEventManager::SetTanakaEvent(bool bActive, TEventTable* pTable)
{
	if (g_bChannel == EVENT_CHANNEL)
	{
		UpdateGameFlag("tanaka_event", bActive);

		if (bActive)
		{
			TPacketGGEvent packet;
			packet.bHeader = HEADER_GG_EVENT;
			packet.table = *pTable;
			packet.bState = bActive;

			P2P_MANAGER::Instance().Send(&packet, sizeof(packet));

			BroadcastNotice("Pirate Tanaka Event has started. Tanakas took over the desert, hunt them down!");
		}
		else
			BroadcastNotice("Pirate Tanaka Event has ended. Thank you for participating!");

		return true;
	}

	if (!map_allow_find(TANAKA_EVENT_MAP))
		return true;

	if (bActive)
	{

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(TANAKA_EVENT_MAP);

		if (!pSecMap)
			return false;

		static_spawn_cycle_event_info * info = AllocEventInfo<static_spawn_cycle_event_info>();
		info->dwWarpMapIndex = TANAKA_EVENT_MAP;
		info->strRegenPath = "data/event/tanaka_event_regen_type_0.txt";

		event_create(static_spawn_cycle_event, info, PASSES_PER_SEC(1));
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
//  OX Event
///////////////////////////////////////////////////////////////////////////////////////
EVENTINFO(ox_event_process_info)
{
	BYTE bState;
	BYTE bRegistrationTimeLeft;
	BYTE bRound;
	ox_event_process_info()
		: bState(0), bRegistrationTimeLeft(0), bRound(0)
	{
	}
};

EVENTFUNC(ox_event_process)
{
	ox_event_process_info* info = dynamic_cast<ox_event_process_info*>(event->info);

	if (!info)
	{
		sys_err("ox_event_process <Factor> Null pointer to event");
		return 0;
	}

	switch (info->bState)
	{
		// Start Phase
		case OXEVENT_OPEN:
		{
			--info->bRegistrationTimeLeft;

			if (info->bRegistrationTimeLeft)
			{
				char buf[128 + 1];
				snprintf(buf, sizeof(buf), "OX Event will be starting in %d minute(s).", info->bRegistrationTimeLeft);

				BroadcastNotice(buf);

				return PASSES_PER_SEC(60);
			}
			else
			{
				info->bState = OXEVENT_CLOSE;
				--info->bRound;	// Decrease available rounds count on each start

				// Restrict players from entering the event
				COXEventManager::Instance().SetStatus(OXEVENT_CLOSE);

				// Start Quiz phase in 15 secs
				return PASSES_PER_SEC(15);
			}
		} break;

		// Quiz Phase
		case OXEVENT_CLOSE:
		{
			if (COXEventManager::Instance().GetAttenderCount() <= OX_PLAYER_WIN_COUNT)
			{
				COXEventManager::Instance().GiveItemToAttender(OX_REWARD_VNUM, OX_REWARD_COUNT);
				info->bState = OXEVENT_FINISH;

				SendNoticeMap("Participants will now be teleported to the city.", OXEVENT_MAP_INDEX, true);

				return PASSES_PER_SEC(15);
			}

			if (!COXEventManager::Instance().Quiz(1, 30))
			{
				sys_err("Could not start an OX quiz.");

				COXEventManager::instance().CloseEvent();
				CEventManager::Instance().SetOXEvent(false);
			}

			/*
				Question Time:			15 Secs
				Result Time:			10 Secs
				Player Cleanup Time:	5 Secs
				-------------------------------
				Question Interval:		35 Secs
			*/
			return PASSES_PER_SEC(35);
		} break;

		// End Phase
		case OXEVENT_FINISH:
		{
			/* 
				Continue the event until there are no available rounds left.
				End the event in case there are no rounds left.
			*/
			if (info->bRound)
			{
				BroadcastNotice("The next round of OX will start again shortly.");

				// Clear the event map
				COXEventManager::instance().CloseEvent();

				info->bState = OXEVENT_OPEN;
				info->bRegistrationTimeLeft = OX_START_WAITING_TIME;

				COXEventManager::Instance().SetStatus(OXEVENT_OPEN);

				// Back to the Start Phase..
				return PASSES_PER_SEC(60);
			}
			else
			{

				// End the event
				CEventManager::Instance().SetOXEvent(false);
			}

			return 0;
		} break;
	}

	return 0;
}

bool CEventManager::SetOXEvent(bool bActive)
{
	if (bActive)
	{
		// Return if an OX Event is still in process
		if ((COXEventManager::Instance().GetStatus() != OXEVENT_FINISH) || m_pOXEvent)
		{
			sys_err("OX Event is already running");
			return false;
		}

		COXEventManager::instance().ClearQuiz();

		char szFilePath[256];
		snprintf(szFilePath, sizeof(szFilePath), "%s/oxquiz.lua", LocaleService_GetBasePath().c_str());
		int result = lua_dofile(quest::CQuestManager::instance().GetLuaState(), szFilePath);
		if (result != 0)
		{
			sys_err("Could not load quiz file %s", szFilePath);
			return false;
		}

		// Start OX Event
		BroadcastNotice("OX Event will be starting soon, prepare yourselves.");
		UpdateGameFlag("oxevent_status", bActive);

		ox_event_process_info* info = AllocEventInfo<ox_event_process_info>();
		info->bState = OXEVENT_OPEN;
		info->bRegistrationTimeLeft = OX_START_WAITING_TIME;
		info->bRound = OX_ROUND_COUNT;
		m_pOXEvent = event_create(ox_event_process, info, PASSES_PER_SEC(60));

		COXEventManager::Instance().SetStatus(OXEVENT_OPEN);
	}
	else
	{
		BroadcastNotice("OX Event has ended. Thank you for participating!");
		UpdateGameFlag("oxevent_status", bActive);

		COXEventManager::Instance().SetStatus(OXEVENT_FINISH);
		COXEventManager::instance().CloseEvent();

		event_cancel(&m_pOXEvent);
		m_pOXEvent = nullptr;
	}

	return true;
}

bool CEventManager::SetSiegeWarEvent(bool bActive)
{
	if (bActive)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(SIEGE_MAP_INDEX);

		if (!pSecMap)
			return false;

		const static std::pair<long, long> s_aStonePos[SIEGE_EMPIRE_COUNT] =
		{
			{224, 302}, // Shinsoo
			{433, 566}, // Jinno
			{467, 216}, // Chunjo
		};

		// Spawn Kingdom Stones
		for (int i = 0; i < SIEGE_EMPIRE_COUNT; ++i)
		{
			LPCHARACTER pMetinStone = CHARACTER_MANAGER::Instance().SpawnMob(8020 + i, SIEGE_MAP_INDEX, 
				pSecMap->m_setting.iBaseX + s_aStonePos[i].first*100, pSecMap->m_setting.iBaseY + s_aStonePos[i].second*100, 0);

			if (!pMetinStone)
				return false;
		}

		// Clean the event data
		m_bSiegeScore = 0;
		CQuestManager::instance().RequestSetEventFlag("siege_shinsoo_score", 0);
		CQuestManager::instance().RequestSetEventFlag("siege_chunjo_score", 0);
		CQuestManager::instance().RequestSetEventFlag("siege_jinno_score", 0);

		BroadcastNotice("Siege War Event has started. Coordinate with the kingdom to become victorious.");
		UpdateGameFlag("siege_war_event", bActive);
	}
	else
	{
		int iShinsooScore = CQuestManager::instance().GetEventFlag("siege_shinsoo_score");
		int iChunjoScore = CQuestManager::instance().GetEventFlag("siege_chunjo_score");
		int iJinnoScore = CQuestManager::instance().GetEventFlag("siege_jinno_score");

		/*
			Score 1 -> Rank 3
			Score 2 -> Rank 2
			Score 3 -> Rank 1

			Status is NULL if the empire's Metin has not been destroyed.
			In this case remaining kingdoms share the next reward.
		*/

		if (iShinsooScore <= 0)
			iShinsooScore = m_bSiegeScore + 1;

		if (iChunjoScore <= 0)
			iChunjoScore = m_bSiegeScore + 1;

		if (iJinnoScore <= 0)
			iJinnoScore = m_bSiegeScore + 1;

		switch (iShinsooScore)
		{
			case 1:
				CPrivManager::instance().RequestGiveEmpirePriv(SHINSOO_EMPIRE, PRIV_EXP_PCT, 25, 60 * 60 * 48);
				break;
			case 2:
				CPrivManager::instance().RequestGiveEmpirePriv(SHINSOO_EMPIRE, PRIV_EXP_PCT, 50, 60 * 60 * 48);
				break;
			case 3:
				CPrivManager::instance().RequestGiveEmpirePriv(SHINSOO_EMPIRE, PRIV_EXP_PCT, 100, 60 * 60 * 48);
				CPrivManager::instance().RequestGiveEmpirePriv(SHINSOO_EMPIRE, PRIV_ITEM_DROP, 50, 60 * 60 * 48);
				break;
		}

		switch (iChunjoScore)
		{
			case 1:
				CPrivManager::instance().RequestGiveEmpirePriv(CHUNJO_EMPIRE, PRIV_EXP_PCT, 25, 60 * 60 * 48);
				break;
			case 2:
				CPrivManager::instance().RequestGiveEmpirePriv(CHUNJO_EMPIRE, PRIV_EXP_PCT, 50, 60 * 60 * 48);
				break;
			case 3:
				CPrivManager::instance().RequestGiveEmpirePriv(CHUNJO_EMPIRE, PRIV_EXP_PCT, 100, 60 * 60 * 48);
				CPrivManager::instance().RequestGiveEmpirePriv(CHUNJO_EMPIRE, PRIV_ITEM_DROP, 50, 60 * 60 * 48);
				break;
		}

		switch (iJinnoScore)
		{
			case 1:
				CPrivManager::instance().RequestGiveEmpirePriv(JINNO_EMPIRE, PRIV_EXP_PCT, 25, 60 * 60 * 48);
				break;
			case 2:
				CPrivManager::instance().RequestGiveEmpirePriv(JINNO_EMPIRE, PRIV_EXP_PCT, 50, 60 * 60 * 48);
				break;
			case 3:
				CPrivManager::instance().RequestGiveEmpirePriv(JINNO_EMPIRE, PRIV_EXP_PCT, 100, 60 * 60 * 48);
				CPrivManager::instance().RequestGiveEmpirePriv(JINNO_EMPIRE, PRIV_ITEM_DROP, 50, 60 * 60 * 48);
				break;
		}


		// Reset event flags at the end of the event 
		m_bSiegeScore = 0;
		CQuestManager::instance().RequestSetEventFlag("siege_shinsoo_score", 0);
		CQuestManager::instance().RequestSetEventFlag("siege_chunjo_score", 0);
		CQuestManager::instance().RequestSetEventFlag("siege_jinno_score", 0);

		UpdateGameFlag("siege_war_event", bActive);
		BroadcastNotice("Siege War Event has ended. Thank you for participating!");

		// Teleport players to the city
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(SIEGE_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();
		info->dwWarpMapIndex = SIEGE_MAP_INDEX;

		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(60));

		FKillSectree f;
		pSecMap->for_each(f);

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), SIEGE_MAP_INDEX, false);
	}

	return true;
}

bool CEventManager::SetKingdomWarEvent(bool bActive)
{
	if (bActive)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(KINGDOM_WAR_MAP_INDEX);

		if (!pSecMap)
			return false;

		// Initialize kingdom score array
		for (int i = 0; i < EMPIRE_MAX_NUM; ++i)
		{
			m_aKingdomWarScore[i].bKingdom = i;
			m_aKingdomWarScore[i].wCount = 0;
		}

		BroadcastNotice("Kingdom War Event has started. Sharpen your weapons and join the battlefield.");
		UpdateGameFlag("kingdom_war_event", bActive);
	}
	else
	{
		// Sort the kingdoms from lowest to highest
		std::sort(m_aKingdomWarScore, m_aKingdomWarScore + EMPIRE_MAX_NUM, SortKingdomWar);

		/*
			Position 1 -> Rank 3
			Position 2 -> Rank 2
			Position 3 -> Rank 1
		*/

		// m_aKingdomWarScore[0] is ignored, its there just for the correct kingdom's position in array
		for (int i = 1; i < EMPIRE_MAX_NUM; ++i)
		{
			switch (i)
			{
				case 1:
					CPrivManager::instance().RequestGiveEmpirePriv(m_aKingdomWarScore[i].bKingdom, PRIV_EXP_PCT, 25, 60 * 60 * 48);
					break;
				case 2:
					CPrivManager::instance().RequestGiveEmpirePriv(m_aKingdomWarScore[i].bKingdom, PRIV_EXP_PCT, 50, 60 * 60 * 48);
					break;
				case 3:
					CPrivManager::instance().RequestGiveEmpirePriv(m_aKingdomWarScore[i].bKingdom, PRIV_EXP_PCT, 100, 60 * 60 * 48);
					CPrivManager::instance().RequestGiveEmpirePriv(m_aKingdomWarScore[i].bKingdom, PRIV_ITEM_DROP, 50, 60 * 60 * 48);
					break;
			}
		}

		memset(m_aKingdomWarScore, 0, sizeof(m_aKingdomWarScore));
		UpdateGameFlag("kingdom_war_event", bActive);

		BroadcastNotice("Kingdom War Event has ended. Thank you for participating!");

		// Teleport players to the city
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::Instance().GetMap(KINGDOM_WAR_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();
		info->dwWarpMapIndex = KINGDOM_WAR_MAP_INDEX;

		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(60));

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), KINGDOM_WAR_MAP_INDEX, false);
	}

	return true;
}

void CEventManager::SendKingdomWarScorePacket(LPCHARACTER ch)
{
	if (ch->GetDesc())
	{
		TPacketGCEventKWScore packet;
		packet.bHeader = HEADER_GC_EVENT_KW_SCORE;
		packet.wKingdomScores[0] = m_aKingdomWarScore[SHINSOO_EMPIRE].wCount;
		packet.wKingdomScores[1] = m_aKingdomWarScore[CHUNJO_EMPIRE].wCount;
		packet.wKingdomScores[2] = m_aKingdomWarScore[JINNO_EMPIRE].wCount;

		ch->GetDesc()->Packet(&packet, sizeof(packet));
	}
}