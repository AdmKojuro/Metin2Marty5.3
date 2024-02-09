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

#pragma once

#if defined(ENABLE_DUNGEON_INFO_SYSTEM)
#include "Packet.h"

namespace DungeonInfo
{
	enum EMisc
	{
		MAX_DUNGEONS = 100,
		MAX_REQUIRED_ITEMS = 3,
		MAX_BOSS_ITEM_SLOTS = 16,
		MAX_BOSS_DROP_ITEMS = 255,
		MAX_DUNGEON_SCROLL = 10,
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
		MAX_RANKING_LINES = 5,
		MAX_RANKING_COUNT = 100,
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
		using GCRank = struct {
			BYTE byHeader;
			char szName[CHARACTER_NAME_MAX_LEN + 1];
			int iLevel;
			DWORD dwPoints;
		};
	}
}

class CPythonDungeonInfo : public CSingleton<CPythonDungeonInfo>
{
public:
	typedef std::vector<DungeonInfo::Packet::GCInfo> TDungeonInfoDataMap;

	struct SDungeonRankingData
	{
		SDungeonRankingData(const char* c_szName, int iLevel, DWORD dwPoints)
			: strName(c_szName), iLevel(iLevel), dwPoints(dwPoints) {}
		std::string strName;
		int iLevel;
		DWORD dwPoints;
	};
	std::vector<std::shared_ptr<SDungeonRankingData>> m_vecDungeonRankingContainer;

	CPythonDungeonInfo();
	virtual ~CPythonDungeonInfo();

	bool AddDungeon(BYTE byIndex, DungeonInfo::Packet::GCInfo TData);
	void Clear();

	bool IsLoaded() { return m_bIsLoaded; }
	void Unload() { m_bIsLoaded = false; }

	bool IsOpen() { return m_bIsOpen; }
	void Open();
	void Warp(BYTE byIndex);
	void Close();

	BYTE GetCount() { return m_byDungeonCount; }

	BYTE GetKey(BYTE byIndex);
	BYTE GetType(BYTE byIndex);

	long GetMapIndex(BYTE byIndex);
	long GetEntryMapIndex(BYTE byIndex);

	DWORD GetBossVnum(BYTE byIndex);

	// Limits
	UINT GetLevelLimit(BYTE byIndex, BYTE byLimit = 0);
	UINT GetMemberLimit(BYTE byIndex, BYTE byLimit = 0);

	// Required Item
	DWORD GetRequiredItemVnum(BYTE byIndex, BYTE bySlot);
	WORD GetRequiredItemCount(BYTE byIndex, BYTE bySlot);

	DWORD GetDuration(BYTE byIndex);
	DWORD GetCooldown(BYTE byIndex);
	BYTE GetElement(BYTE byIndex);

	// Attack Bonus
	BYTE GetAttBonusCount(BYTE byIndex);
	BYTE GetAttBonus(BYTE byIndex, BYTE byType);

	// Defense Bonus
	BYTE GetDefBonusCount(BYTE byIndex);
	BYTE GetDefBonus(BYTE byIndex, BYTE byType);

	// Boss Drop Item
	BYTE GetBossDropCount(BYTE byIndex);
	DWORD GetBossDropItemVnum(BYTE byIndex, BYTE bySlot);
	WORD GetBossDropItemCount(BYTE byIndex, BYTE bySlot);

	// Results
	UINT GetFinish(BYTE byIndex);
	UINT GetFinishTime(BYTE byIndex);
	UINT GetFinishDamage(BYTE byIndex);

	void AddRanking(const char* c_szName, int iLevel, DWORD dwPoints);
	void ClearRankingData();

	size_t GetRankingCount() const;
	SDungeonRankingData* GetRankingByLine(DWORD dwArrayIndex) const;
	DWORD GetMyRankingLine() const;

protected:
	TDungeonInfoDataMap m_vecDungeonInfoDataMap[255];
	BYTE m_byDungeonCount;
	bool m_bIsOpen;
	bool m_bIsLoaded;
};
#endif
