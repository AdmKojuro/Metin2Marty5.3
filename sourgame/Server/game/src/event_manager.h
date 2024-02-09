#pragma once
#include "../../common/tables.h"
#include <unordered_map>

enum EEventTypes
{
	EVENT_TYPE_NONE,
	EVENT_TYPE_EXPERIENCE,
	EVENT_TYPE_ITEM_DROP,
	EVENT_TYPE_BOSS,
	EVENT_TYPE_METIN,
	EVENT_TYPE_MINING,
	EVENT_TYPE_GOLD_FROG,
	EVENT_TYPE_MOONLIGHT,
	EVENT_TYPE_HEXEGONAL_CHEST,
	EVENT_TYPE_FISHING,
	EVENT_TYPE_HIDE_AND_SEEK,
	EVENT_TYPE_OX,
	EVENT_TYPE_TANAKA,
	EVENT_TYPE_SIEGE_WAR,
	EVENT_TYPE_KINGDOM_WAR,
	EVENT_MAX_NUM
};

enum EEvent
{
	CLEAR_ENTITY_STAGE1_ROUND = 1,
	CLEAR_ENTITY_STAGE2_ROUND = 6,

	DYNAMIC_CYCLE_STAGE1 = 5,
	DYNAMIC_CYCLE_STAGE2 = 10,

	STATIC_CYCLE_MAX_ROUND = 3,

	TANAKA_EVENT_MAP = 63,
	SIEGE_MAP_INDEX = 103,
	KINGDOM_WAR_MAP_INDEX = 103,
	EVENT_MAP_INDEX = 230,

	OX_START_WAITING_TIME = 5,
	OX_ROUND_COUNT = 3,
	OX_PLAYER_WIN_COUNT = 5,
	OX_REWARD_VNUM = 50109,
	OX_REWARD_COUNT = 1,

	HIDE_AND_SEEK_ROUNDS = 5,
	HIDE_AND_SEEK_POSITIONS = 4,
	HIDE_AND_SEEK_NPC = 20359,
	HIDE_AND_SEEK_REWARD_VNUM = 50109,
	HIDE_AND_SEEK_REWARD_COUNT = 5,

	SIEGE_EMPIRE_COUNT = 3,

	EVENT_CHANNEL = 99,
	HIDE_AND_SEEK_CHANNEL = 1,
};

class CEventManager : public singleton<CEventManager>
{
public:
	typedef struct SKingdomWarCount
	{
		BYTE bKingdom;
		WORD wCount;
	} TKingdomWarCount;

	typedef struct SMapPosition
	{
		WORD x;
		WORD y;
	} TMapPosition;

	typedef struct SHideAndSeek
	{
		DWORD dwMapIndex;
		std::vector<TMapPosition>	vec_Positions;
		std::vector<std::string>	vec_Annoucments;
	} THideAndSeek;

	enum EQueueType
	{
		QUEUE_TYPE_NONE,
		QUEUE_TYPE_START,
		QUEUE_TYPE_END,
	};

	CEventManager();
	~CEventManager();

	void	Process();
	void	Enqueue(BYTE bQueue, TEventTable* table);
	void	Dequeue(BYTE bQueue, DWORD dwID);
	void	CancelActiveEvents();

	void	Initialize(TEventTable* pTab, int iSize, bool bReload = false);
	void	UpdateEventStatus(DWORD dwID);

	int		GetEvent(const std::string& strEventName);
	std::string	GetEventName(int iEvent);
	std::string		GetEventString(int iEvent);
	void	SendEventInfo(const LPCHARACTER ch, int iMonth);

	void	BuildEventNameMap();
	bool	GetEventState(int iEvent);
	void	SetEventState(TEventTable* table, bool bState);
	void	UpdateGameFlag(const char* c_szFlag, long lValue);

	void	SetReloadMode(bool bReload) { m_bReload = bReload; }
	bool	IsReloadMode() { return m_bReload; }

	void	OnDead(LPCHARACTER pVictim, LPCHARACTER pKiller);

	bool	SetExperienceEvent(bool bActive, TEventTable* pTable);
	bool	SetItemDropEvent(bool bActive, TEventTable* pTable);
	bool	SetBossEvent(bool bActive);
	bool	SetMetinEvent(bool bActive);
	bool	SetMiningEvent(bool bActive);
	bool	SetGoldFrogEvent(bool bActive);
	bool	SetMoonlightEvent(bool bActive);
	bool	SetHexegonalEvent(bool bActive);
	bool	SetFishingEvent(bool bActive);

	bool	SetHideAndSeekEvent(bool bActive);
	bool	HideAndSeekNPC(int iPosition, int iRound);
	void	SetHideAndSeekRound(int iRound) { m_iCurrentHideAndSeekRound = iRound; }
	int		GetHideAndSeekRound() { return m_iCurrentHideAndSeekRound; }

	bool	SetOXEvent(bool bActive);
	bool	SetTanakaEvent(bool bActive, TEventTable* pTable);
	bool	SetSiegeWarEvent(bool bActive);

	bool	SetKingdomWarEvent(bool bActive);
	void	SendKingdomWarScorePacket(LPCHARACTER ch);

private:
	std::unordered_map<std::string, int> m_mapEventName;
	std::unordered_map<DWORD, TEventTable*> m_mapEvent;

	std::deque<TEventTable*>	m_dequeEventStart;
	std::deque<TEventTable*>	m_dequeEventEnd;

	bool m_bReload;

	LPEVENT		m_pOXEvent;

	BYTE		m_bSiegeScore;

	TKingdomWarCount		m_aKingdomWarScore[EMPIRE_MAX_NUM];

	LPCHARACTER				m_pHideAndSeekNPC;
	int						m_iCurrentHideAndSeekRound;
};