/*
* Title: Dungeon Information System
* Author: Owsap
* Description: List of all available dungeons.
* Date: 2021.01.09
* Last Update: 2021.06.03
* Version 2.0.0.2
*
* Skype: owsap.
* Discord: Owsap#0905
*
* 0x0
*
* Web: https://owsap-productions.com/
* GitHub: https://github.com/Owsap
*/

#if defined(__DUNGEON_INFO_SYSTEM__)

#if !defined(_DUNGEON_INFO_MANAGER_H_)
#define _DUNGEON_INFO_MANAGER_H_

#define DUNGEON_TOKEN(string) if (!str_cmp(szTokenString, string))

#include "locale_service.h"
#include "char.h"
#include "desc.h"
#include "packet.h"
#include "quest.h"
#include "questmanager.h"
#include "utils.h"
#include "config.h"
#include "desc_manager.h"
#include "db.h"
#include "buffer_manager.h"

namespace DungeonInfo
{
	enum EMisc
	{
		MAX_REQUIRED_ITEMS = 3,
		MAX_BOSS_ITEM_SLOTS = 16,
		MAX_DUNGEONS = 100,
		MAX_REFRESH_DELAY = 3,
	};

	enum EAction
	{
		CLOSE,
		OPEN,
		WARP,
		RANK
	};

	enum ERank
	{
		MAX_RANKING_LINES = 10,
		MAX_RANKING_COUNT = 10,
	};

	enum ERankTypes
	{
		RANKING_TYPE_COMPLETED = 1,
		RANKING_TYPE_TIME = 2,
		RANKING_TYPE_DAMAGE = 3,

		MAX_RANKING_TYPE,
	};

	using LevelLimit = struct { int iLevelMin, iLevelMax, iMemberMin, iMemberMax; };
	using Item = struct { DWORD dwVnum; WORD wCount; };
	using Bonus = struct { BYTE byAttBonus[POINT_MAX_NUM]; BYTE byDefBonus[POINT_MAX_NUM]; };
	using Results = struct { DWORD dwFinish, dwFinishTime, dwFinishDamage; };

	namespace Packet
	{
		enum EHeader
		{
			HEADER_GC_DUNGEON_INFO = 140,
			HEADER_CG_DUNGEON_INFO = 140,
			HEADER_GC_DUNGEON_RANKING = 141,
		};
		enum ESubHeader
		{
			SUBHEADER_DUNGEON_INFO_SEND,
			SUBHEADER_DUNGEON_INFO_OPEN,
		};
		using CGInfo = struct { BYTE byHeader, bySubHeader, byIndex, byRankType; };
		using GCInfo = struct SDungeonInfo {
			SDungeonInfo() : byHeader(HEADER_GC_DUNGEON_INFO), bySubHeader(0), byIndex(0), byType(0),
				bReset(false),
				lMapIndex(0), lEntryMapIndex(0),
				dwBossVnum(0),
				dwDuration(0), dwCooldown(0), byElement(0)
			{
				memset(&sLevelLimit, 0, sizeof(sLevelLimit));
				memset(&sRequiredItem, 0, sizeof(sRequiredItem));
				memset(&sBonus.byAttBonus, 0, sizeof(sBonus.byAttBonus));
				memset(&sBonus.byDefBonus, 0, sizeof(sBonus.byDefBonus));
				memset(&sBossDropItem, 0, sizeof(sBossDropItem));
				memset(&sResults, 0, sizeof(sResults));
			}
			BYTE byHeader, bySubHeader, byIndex, byType;
			bool bReset;
			long lMapIndex, lEntryMapIndex;
			DWORD dwBossVnum;
			LevelLimit sLevelLimit;
			Item sRequiredItem[MAX_REQUIRED_ITEMS];
			DWORD dwDuration;
			DWORD dwCooldown;
			BYTE byElement;
			Bonus sBonus;
			Item sBossDropItem[MAX_BOSS_ITEM_SLOTS];
			Results sResults;
		};
		using GCRank = struct SRank {
			SRank() { std::strncpy(szName, "", sizeof(szName)); }
			SRank(const char* c_szName, const int c_iLevel, const DWORD c_dwPoints)
				: iLevel(c_iLevel), dwPoints(c_dwPoints)
			{
				std::strncpy(szName, c_szName, sizeof(szName));
			}
			BYTE byHeader = HEADER_GC_DUNGEON_RANKING;
			char szName[CHARACTER_NAME_MAX_LEN + 1];
			int iLevel = 0;
			DWORD dwPoints = 0;
		};
	}
}

enum EQuestType
{
	QUEST_FLAG_PC,
	QUEST_FLAG_GLOBAL
};

struct SDungeonLimits
{
	UINT iMin, iMax;
	bool operator == (const SDungeonLimits& c_sRef)
	{
		return (this->iMin == c_sRef.iMin)
			&& (this->iMax == c_sRef.iMax);
	}
};

struct SDungeonEntryPosition
{
	long lBaseX, lBaseY;
	bool operator == (const SDungeonEntryPosition& c_sRef)
	{
		return (this->lBaseX == c_sRef.lBaseX)
			&& (this->lBaseY == c_sRef.lBaseY);
	}
};

struct SDungeonBonus
{
	BYTE byAttBonus, byDefBonus;
	bool operator == (const SDungeonBonus& c_sRef)
	{
		return (this->byAttBonus == c_sRef.byAttBonus)
			&& (this->byDefBonus == c_sRef.byDefBonus);
	}
};

struct SDungeonItem
{
	DWORD dwVnum;
	WORD wCount;

	bool operator == (const SDungeonItem& c_sRef)
	{
		return (this->wCount == c_sRef.wCount)
			&& (this->dwVnum == c_sRef.dwVnum);
	}
};

struct SDungeonQuest
{
	std::string strQuest;
	std::string strQuestFlag;
	BYTE byType;

	bool operator == (const SDungeonQuest& c_sRef)
	{
		return (this->byType == c_sRef.byType) &&
			(this->strQuestFlag == c_sRef.strQuestFlag)
			&& (this->strQuest == c_sRef.strQuest);
	}
};

struct SDungeonData
{
	BYTE byType = 0;

	long lMapIndex = 0;
	long lEntryMapIndex = 0;

	std::vector<SDungeonEntryPosition> vecEntryPosition;

	DWORD dwBossVnum = 0;

	std::vector<SDungeonLimits> vecLevelLimit;
	std::vector<SDungeonLimits> vecMemberLimit;
	std::vector<SDungeonItem> vecRequiredItem;

	DWORD dwDuration = 0;
	DWORD dwCooldown = 0;
	BYTE byElement = 0;

	std::vector<SDungeonBonus> vecBonus;
	std::vector<SDungeonItem> vecBossDropItem;

	std::vector<SDungeonQuest> vecQuest;

	SDungeonData();
};

class CDungeonInfoManager : public singleton<CDungeonInfoManager>
{
public:
	CDungeonInfoManager();
	virtual ~CDungeonInfoManager();

	void Destroy();

	void Initialize();
	bool LoadFile(const char* c_szFileName);

	void Reload();

	void SendInfo(LPCHARACTER pkCh, BOOL bReset = FALSE);
	void Warp(LPCHARACTER pkCh, BYTE byIndex);
	UINT GetResult(LPCHARACTER pkCh, std::string strQuest, std::string strFlag);
	void Ranking(LPCHARACTER pkCh, BYTE byIndex, BYTE byRankType = 0);
	void UpdateRanking(LPCHARACTER pkCh, const std::string c_strQuestName);
};

#endif /* _DUNGEON_INFO_MANAGER_H_ */

#endif // __DUNGEON_INFO_SYSTEM__
