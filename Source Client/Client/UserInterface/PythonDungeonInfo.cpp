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

#include "StdAfx.h"

#if defined(ENABLE_DUNGEON_INFO_SYSTEM)
#include "PythonDungeonInfo.h"
#include "PythonNetworkStream.h"
#include "AbstractPlayer.h"

CPythonDungeonInfo::CPythonDungeonInfo()
{
	Clear();
}

CPythonDungeonInfo::~CPythonDungeonInfo()
{
	Clear();
}

bool CPythonDungeonInfo::AddDungeon(BYTE byIndex, DungeonInfo::Packet::GCInfo TData)
{
	bool bCount = true;
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
	{
		m_vecDungeonInfoDataMap[byIndex].clear();
		bCount = false;
	}

	m_vecDungeonInfoDataMap[byIndex].emplace_back(TData);
	if (bCount)
		++m_byDungeonCount;

	m_vecDungeonRankingContainer.reserve(DungeonInfo::ERank::MAX_RANKING_COUNT);
	m_bIsLoaded = true;

	return true;
}

void CPythonDungeonInfo::Clear()
{
	m_bIsOpen = false;
	m_bIsLoaded = false;

	m_vecDungeonInfoDataMap->clear();
	m_byDungeonCount = 0;
	m_vecDungeonRankingContainer.clear();
}

void CPythonDungeonInfo::Open()
{
	//if (IsLoaded())
	//	return;

	m_bIsOpen = true;

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendDungeonInfo(DungeonInfo::EAction::OPEN);
}

void CPythonDungeonInfo::Warp(BYTE byIndex)
{
	if (!IsOpen())
		return;

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendDungeonInfo(DungeonInfo::EAction::WARP, byIndex);
}

void CPythonDungeonInfo::Close()
{
	m_bIsOpen = false;

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendDungeonInfo(DungeonInfo::EAction::CLOSE);
}

BYTE CPythonDungeonInfo::GetKey(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->byIndex;
	return 0;
}

BYTE CPythonDungeonInfo::GetType(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->byType;
	return 0;
}

long CPythonDungeonInfo::GetMapIndex(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->lMapIndex;
	return 0;
}

long CPythonDungeonInfo::GetEntryMapIndex(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->lEntryMapIndex;
	return 0;
}

DWORD CPythonDungeonInfo::GetBossVnum(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->dwBossVnum;
	return 0;
}

UINT CPythonDungeonInfo::GetLevelLimit(BYTE byIndex, BYTE byLimit)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return (byLimit ? it->sLevelLimit.iLevelMax : it->sLevelLimit.iLevelMin);
	return 0;
}

UINT CPythonDungeonInfo::GetMemberLimit(BYTE byIndex, BYTE byLimit)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return (byLimit ? it->sLevelLimit.iMemberMax : it->sLevelLimit.iMemberMin);
	return 0;
}

DWORD CPythonDungeonInfo::GetRequiredItemVnum(BYTE byIndex, BYTE bySlot)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->sRequiredItem[bySlot].dwVnum;
	return 0;
}

WORD CPythonDungeonInfo::GetRequiredItemCount(BYTE byIndex, BYTE bySlot)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->sRequiredItem[bySlot].wCount;
	return 0;
}

DWORD CPythonDungeonInfo::GetDuration(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->dwDuration;
	return 0;
}

DWORD CPythonDungeonInfo::GetCooldown(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->dwCooldown;
	return 0;
}

BYTE CPythonDungeonInfo::GetElement(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->byElement;
	return 0;
}

BYTE CPythonDungeonInfo::GetAttBonusCount(BYTE byIndex)
{
	BYTE byCount = 0;

	if (m_vecDungeonInfoDataMap->empty())
		return byCount;

	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
	{
		for (BYTE byAffect = CItemData::APPLY_NONE + 1; byAffect < CItemData::MAX_APPLY_NUM; ++byAffect)
		{
			if (std::find(it->sBonus.byAttBonus, std::end(it->sBonus.byAttBonus), byAffect)
				!= std::end(it->sBonus.byAttBonus))
			{
				++byCount;
			}

		}
	}
	return byCount;
}

BYTE CPythonDungeonInfo::GetAttBonus(BYTE byIndex, BYTE byType)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->sBonus.byAttBonus[byType];
	return 0;
}

BYTE CPythonDungeonInfo::GetDefBonusCount(BYTE byIndex)
{
	BYTE byCount = 0;

	if (m_vecDungeonInfoDataMap->empty())
		return byCount;

	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
	{
		for (BYTE byAffect = CItemData::APPLY_NONE + 1; byAffect < CItemData::MAX_APPLY_NUM; ++byAffect)
		{
			if (std::find(it->sBonus.byDefBonus, std::end(it->sBonus.byDefBonus), byAffect)
				!= std::end(it->sBonus.byDefBonus))
			{
				++byCount;
			}
		}
	}
	return byCount;
}

BYTE CPythonDungeonInfo::GetDefBonus(BYTE byIndex, BYTE byType)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->sBonus.byDefBonus[byType];
	return 0;
}

BYTE CPythonDungeonInfo::GetBossDropCount(BYTE byIndex)
{
	BYTE byCount = 0;

	if (m_vecDungeonInfoDataMap->empty())
		return byCount;

	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
	{
		for (BYTE bySlot = 0; bySlot < DungeonInfo::MAX_BOSS_ITEM_SLOTS; ++bySlot)
		{
			if (it->sBossDropItem[bySlot].dwVnum > 0)
				byCount++;
		}
	}
	return byCount;
}

DWORD CPythonDungeonInfo::GetBossDropItemVnum(BYTE byIndex, BYTE bySlot)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->sBossDropItem[bySlot].dwVnum;
	return 0;
}

WORD CPythonDungeonInfo::GetBossDropItemCount(BYTE byIndex, BYTE bySlot)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->sBossDropItem[bySlot].wCount;
	return 0;
}

UINT CPythonDungeonInfo::GetFinish(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->sResults.dwFinish;
	return 0;
}

UINT CPythonDungeonInfo::GetFinishTime(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->sResults.dwFinishTime;
	return 0;
}

UINT CPythonDungeonInfo::GetFinishDamage(BYTE byIndex)
{
	TDungeonInfoDataMap::const_iterator it = m_vecDungeonInfoDataMap[byIndex].begin();
	if (it != m_vecDungeonInfoDataMap[byIndex].end())
		return it->sResults.dwFinishDamage;
	return 0;
}

//
// Ranking
//

void CPythonDungeonInfo::AddRanking(const char* c_szName, int iLevel, DWORD dwPoints)
{
	if (c_szName && iLevel > 0)
		m_vecDungeonRankingContainer.emplace_back(std::make_shared<SDungeonRankingData>(c_szName, iLevel, dwPoints));
}

void CPythonDungeonInfo::ClearRankingData()
{
	m_vecDungeonRankingContainer.clear();
	m_vecDungeonRankingContainer.reserve(DungeonInfo::ERank::MAX_RANKING_COUNT);
}

size_t CPythonDungeonInfo::GetRankingCount() const
{
	return m_vecDungeonRankingContainer.size();
}

CPythonDungeonInfo::SDungeonRankingData* CPythonDungeonInfo::GetRankingByLine(DWORD dwArrayIndex) const
{
	if (dwArrayIndex >= GetRankingCount())
		return NULL;

	return m_vecDungeonRankingContainer.at(dwArrayIndex).get();
}

DWORD CPythonDungeonInfo::GetMyRankingLine() const
{
	auto aIter = std::find_if(m_vecDungeonRankingContainer.begin(),
		m_vecDungeonRankingContainer.end(), [](const std::shared_ptr<SDungeonRankingData>& sRef)
		{
			return !sRef->strName.compare(IAbstractPlayer::GetSingleton().GetName());
		}
	);

	if (aIter != m_vecDungeonRankingContainer.end())
		return std::distance(m_vecDungeonRankingContainer.begin(), aIter) + 1;

	return 0;
}

#endif
