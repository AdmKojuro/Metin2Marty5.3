#include "stdafx.h"

#ifdef ENABLE_EXTENDED_BATTLE_PASS
#include "battlepass_manager.h"

#include "p2p.h"
#include "locale_service.h"
#include "char.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "questmanager.h"
#include "questlua.h"
#include "start_position.h"
#include "char_manager.h"
#include "item_manager.h"
#include "sectree_manager.h"
#include "regen.h"
#include "log.h"
#include "db.h"
#include "target.h"
#include "party.h"

#include <string>
#include <algorithm>

const std::string g_astBattlePassType[BATTLEPASS_TYPE_MAX] = {
	"BATTLEPASS_NORMAL",
	"BATTLEPASS_PREMIUM",
	"BATTLEPASS_EVENT",
};

const std::string g_astMissionType[MISSION_TYPE_MAX] = {
	"",
	"KILL_MONSTER",
	"KILL_PLAYER",
	"DAMAGE_MONSTER",
	"DAMAGE_PLAYER",
	"BP_ITEM_USE",
	"BP_ITEM_SELL",
	"BP_ITEM_CRAFT",
	"BP_ITEM_REFINE",
	"BP_ITEM_DESTROY",
	"BP_ITEM_COLLECT",
	"FISH_FISHING",
	"FISH_GRILL",
	"FISH_CATCH",
	"EXP_COLLECT",
	"YANG_COLLECT",
	"GUILD_PLAY_GUILDWAR",
	"GUILD_SPENT_EXP",
	"GAYA_CRAFT_GAYA",
	"GAYA_BUY_ITEM_GAYA_COST",
	"PET_ENCHANT",
	"COMPLETE_DUNGEON",
	"COMPLETE_MINIGAME",
};

CBattlePassManager::CBattlePassManager()
{
	m_pNormalLoader = NULL;
	m_pPremiumLoader = NULL;
	m_pEventLoader = NULL;
	
	m_dwActiveNormalBattlePassID = 0;
	m_dwActivePremiumBattlePassID = 0;
	m_dwActiveEventBattlePassID = 0;
	
	m_dwNormalBattlePassStartTime = 0;
	m_dwNormalBattlePassEndTime = 0;
	m_dwPremiumBattlePassStartTime = 0;
	m_dwPremiumBattlePassEndTime = 0;
	m_dwEventBattlePassStartTime = 0;
	m_dwEventBattlePassEndTime = 0;
}

CBattlePassManager::~CBattlePassManager()
{
	if (m_pNormalLoader)
		delete m_pNormalLoader;
	if (m_pPremiumLoader)
		delete m_pPremiumLoader;
	if (m_pEventLoader)
		delete m_pEventLoader;
	
	m_dwActiveNormalBattlePassID = 0;
	m_dwActivePremiumBattlePassID = 0;
	m_dwActiveEventBattlePassID = 0;
	
	m_dwNormalBattlePassStartTime = 0;
	m_dwNormalBattlePassEndTime = 0;
	m_dwPremiumBattlePassStartTime = 0;
	m_dwPremiumBattlePassEndTime = 0;
	m_dwEventBattlePassStartTime = 0;
	m_dwEventBattlePassEndTime = 0;
}

bool CBattlePassManager::InitializeBattlePass()
{
	// ----------------------- Normal --------------------------//
	char szNormalBattlePassFileName[256];
	snprintf(szNormalBattlePassFileName, sizeof(szNormalBattlePassFileName), "%s/battlepass/normal.txt", LocaleService_GetBasePath().c_str());
	m_pNormalLoader = new CGroupTextParseTreeLoader;
	CGroupTextParseTreeLoader& normal_loader = *m_pNormalLoader;
	
	if (false == normal_loader.Load(szNormalBattlePassFileName))
	{
		sys_err("battlepass/normal.txt initialize failure (path incorrect?)");
		return false;
	}

	if (!ReadNormalBattlePassGroup())
		return false;

	if (!ReadNormalBattlePassMissions())
		return false;
	
	// ---------------------- Premium -------------------------//
	char szPremiumBattlePassFileName[256];
	snprintf(szPremiumBattlePassFileName, sizeof(szPremiumBattlePassFileName), "%s/battlepass/premium.txt", LocaleService_GetBasePath().c_str());
	m_pPremiumLoader = new CGroupTextParseTreeLoader;
	CGroupTextParseTreeLoader& premium_loader = *m_pPremiumLoader;
	
	if (false == premium_loader.Load(szPremiumBattlePassFileName))
	{
		sys_err("battlepass/premium.txt initialize failure (path incorrect?)");
		return false;
	}

	if (!ReadPremiumBattlePassGroup())
		return false;

	if (!ReadPremiumBattlePassMissions())
		return false;
	
	// ------------------------ Event ---------------------------//
	char szEventBattlePassFileName[256];
	snprintf(szEventBattlePassFileName, sizeof(szEventBattlePassFileName), "%s/battlepass/event.txt", LocaleService_GetBasePath().c_str());
	m_pEventLoader = new CGroupTextParseTreeLoader;
	CGroupTextParseTreeLoader& event_loader = *m_pEventLoader;
	
	if (false == event_loader.Load(szEventBattlePassFileName))
	{
		sys_err("battlepass/event.txt initialize failure (path incorrect?)");
		return false;
	}

	if (!ReadEventBattlePassGroup())
		return false;

	if (!ReadEventBattlePassMissions())
		return false;
	
	// --------------- Check Battlepass Times ------------------//
	
	CheckBattlePassTimes();
	
	return true;
}

bool CBattlePassManager::ReadNormalBattlePassGroup()
{
	std::string stName;

	CGroupNode* pGroupNode = m_pNormalLoader->GetGroup("battlepass");

	if (NULL == pGroupNode)
	{
		sys_err("battle_pass.txt a besoin d'un groupe de battle pass.");
		return false;
	}

	int n = pGroupNode->GetRowCount();
	if (0 == n)
	{
		sys_err("Le group battle pass est vide.");
		return false;
	}

	std::set<BYTE> setIDs;

	for (int i = 0; i < n; i++)
	{
		const CGroupNode::CGroupNodeRow* pRow;
		pGroupNode->GetRow(i, &pRow);

		std::string stBattlePassName;
		BYTE battlePassId;
		
		//DWORD dwStartTime;
		//DWORD dwEndTime;
		
		std::vector<TExtBattlePassTimeTable> timeVector;
		TExtBattlePassTimeTable activeTimes;
		
		if (!pRow->GetValue("battlepassname", stBattlePassName))
		{
			sys_err("In Group BattlePass, No BattlePassName column.");
			return false;
		}

		if (!pRow->GetValue("battlepassid", battlePassId))
		{
			sys_err("In Group BattlePass, %s's ID is invalid", stBattlePassName.c_str());
			return false;
		}

		if (!pRow->GetValue("starttime", activeTimes.dwStartTime))
		{
			sys_err("In Group BattlePass, %s's StartTime is invalid", stBattlePassName.c_str());
			return false;
		}
		
		if (!pRow->GetValue("endtime", activeTimes.dwEndTime))
		{
			sys_err("In Group BattlePass, %s's EndTime is invalid", stBattlePassName.c_str());
			return false;
		}
		
		if (setIDs.end() != setIDs.find(battlePassId))
		{
			sys_err("In Group BattlePass, duplicated id exist.");
			return false;
		}
		//sys_err("BattlePassNormal: StartTime = %d |-| EndTime = %d", m_dwNormalBattlePassStartTime, m_dwNormalBattlePassEndTime);
		
		/* if(m_dwNormalBattlePassStartTime < get_global_time() && m_dwNormalBattlePassEndTime > get_global_time())
		{
			m_dwActiveNormalBattlePassID = battlePassId;
		} */
		
			
		setIDs.insert(battlePassId);
		
		m_map_normal_battle_pass_name.insert(TMapNormalBattlePassName::value_type(battlePassId, stBattlePassName));
		
		activeTimes.bBattlePassId = battlePassId;
		
		timeVector.push_back(activeTimes);
		m_map_normal_battle_pass_times.insert(TMapNormalBattlePassTimeTable::value_type(battlePassId, timeVector));
	}

	return true;
}

bool CBattlePassManager::ReadPremiumBattlePassGroup()
{
	std::string stName;

	CGroupNode* pGroupNode = m_pPremiumLoader->GetGroup("battlepass");

	if (NULL == pGroupNode)
	{
		sys_err("battle_pass.txt a besoin d'un groupe de battle pass.");
		return false;
	}

	int n = pGroupNode->GetRowCount();
	if (0 == n)
	{
		sys_err("Le group battle pass est vide.");
		return false;
	}

	std::set<BYTE> setIDs;

	for (int i = 0; i < n; i++)
	{
		const CGroupNode::CGroupNodeRow* pRow;
		pGroupNode->GetRow(i, &pRow);

		std::string stBattlePassName;
		BYTE battlePassId;
		
		std::vector<TExtBattlePassTimeTable> timeVector;
		TExtBattlePassTimeTable activeTimes;
		
		if (!pRow->GetValue("battlepassname", stBattlePassName))
		{
			sys_err("In Group BattlePass, No BattlePassName column.");
			return false;
		}

		if (!pRow->GetValue("battlepassid", battlePassId))
		{
			sys_err("In Group BattlePass, %s's ID is invalid", stBattlePassName.c_str());
			return false;
		}

		if (!pRow->GetValue("starttime", activeTimes.dwStartTime))
		{
			sys_err("In Group BattlePass, %s's StartTime is invalid", stBattlePassName.c_str());
			return false;
		}
		
		if (!pRow->GetValue("endtime", activeTimes.dwEndTime))
		{
			sys_err("In Group BattlePass, %s's EndTime is invalid", stBattlePassName.c_str());
			return false;
		}
		
		if (setIDs.end() != setIDs.find(battlePassId))
		{
			sys_err("In Group BattlePass, duplicated id exist.");
			return false;
		}
		//sys_err("BattlePassNormal: StartTime = %d |-| EndTime = %d", m_dwNormalBattlePassStartTime, m_dwNormalBattlePassEndTime);
		
		/* if(m_dwNormalBattlePassStartTime < get_global_time() && m_dwNormalBattlePassEndTime > get_global_time())
		{
			m_dwActiveNormalBattlePassID = battlePassId;
		} */
		
			
		setIDs.insert(battlePassId);
		
		m_map_premium_battle_pass_name.insert(TMapPremiumBattlePassName::value_type(battlePassId, stBattlePassName));
		
		activeTimes.bBattlePassId = battlePassId;
		
		timeVector.push_back(activeTimes);
		m_map_premium_battle_pass_times.insert(TMapPremiumBattlePassTimeTable::value_type(battlePassId, timeVector));
	}

	return true;
}

bool CBattlePassManager::ReadEventBattlePassGroup()
{
	std::string stName;

	CGroupNode* pGroupNode = m_pEventLoader->GetGroup("battlepass");

	if (NULL == pGroupNode)
	{
		sys_err("battle_pass.txt a besoin d'un groupe de battle pass.");
		return false;
	}

	int n = pGroupNode->GetRowCount();
	if (0 == n)
	{
		sys_err("Le group battle pass est vide.");
		return false;
	}

	std::set<BYTE> setIDs;

	for (int i = 0; i < n; i++)
	{
		const CGroupNode::CGroupNodeRow* pRow;
		pGroupNode->GetRow(i, &pRow);

		std::string stBattlePassName;
		BYTE battlePassId;

		std::vector<TExtBattlePassTimeTable> timeVector;
		TExtBattlePassTimeTable activeTimes;
		
		if (!pRow->GetValue("battlepassname", stBattlePassName))
		{
			sys_err("In Group BattlePass, No BattlePassName column.");
			return false;
		}

		if (!pRow->GetValue("battlepassid", battlePassId))
		{
			sys_err("In Group BattlePass, %s's ID is invalid", stBattlePassName.c_str());
			return false;
		}

		if (!pRow->GetValue("starttime", activeTimes.dwStartTime))
		{
			sys_err("In Group BattlePass, %s's StartTime is invalid", stBattlePassName.c_str());
			return false;
		}
		
		if (!pRow->GetValue("endtime", activeTimes.dwEndTime))
		{
			sys_err("In Group BattlePass, %s's EndTime is invalid", stBattlePassName.c_str());
			return false;
		}
		
		if (setIDs.end() != setIDs.find(battlePassId))
		{
			sys_err("In Group BattlePass, duplicated id exist.");
			return false;
		}
		//sys_err("BattlePassNormal: StartTime = %d |-| EndTime = %d", m_dwNormalBattlePassStartTime, m_dwNormalBattlePassEndTime);
		
		/* if(m_dwNormalBattlePassStartTime < get_global_time() && m_dwNormalBattlePassEndTime > get_global_time())
		{
			m_dwActiveNormalBattlePassID = battlePassId;
		} */
		
			
		setIDs.insert(battlePassId);
		
		m_map_event_battle_pass_name.insert(TMapEventBattlePassName::value_type(battlePassId, stBattlePassName));
		
		activeTimes.bBattlePassId = battlePassId;
		
		timeVector.push_back(activeTimes);
		m_map_event_battle_pass_times.insert(TMapEventBattlePassTimeTable::value_type(battlePassId, timeVector));
	}

	return true;
}

bool CBattlePassManager::ReadNormalBattlePassMissions()
{
	TMapNormalBattlePassName::iterator it = m_map_normal_battle_pass_name.begin();
	while (it != m_map_normal_battle_pass_name.end())
	{
		std::string battlePassName = (it++)->second;

		CGroupNode* pGroupNode = m_pNormalLoader->GetGroup(battlePassName.c_str());

		sys_err("battle_pass.txt : %s ", battlePassName.c_str());
		if (NULL == pGroupNode) {
			sys_err("battle_pass.txt need group %s.", battlePassName.c_str());
			return false;
		}

		int n = pGroupNode->GetChildNodeCount();
		if (n < 2) {
			sys_err("Group %s need to have at least one grup for Reward and one Mission. Row: %d", battlePassName.c_str(), n);
			return false;
		}
		
		{
			CGroupNode* pChild;
			if (NULL == (pChild = pGroupNode->GetChildNode("reward"))) {
				sys_err("In Group %s, Reward group is not defined.", battlePassName.c_str());
				return false;
			}

			int m = pChild->GetRowCount();
			std::vector<TExtBattlePassRewardItem> rewardVector;

			for (int j = 1; j <= m; j++) {
				std::stringstream ss;
				ss << j;
				const CGroupNode::CGroupNodeRow* pRow = NULL;

				pChild->GetRow(ss.str(), &pRow);
				if (NULL == pRow) {
					sys_err("In Group %s, subgroup Reward, No %d row.", battlePassName.c_str(), j);
					return false;
				}

				TExtBattlePassRewardItem itemReward;

				if (!pRow->GetValue("itemvnum", itemReward.dwVnum)) {
					sys_err("In Group %s, subgroup Reward, ItemVnum is empty.", battlePassName.c_str());
					return false;
				}

				if (!pRow->GetValue("itemcount", itemReward.bCount)) {
					sys_err("In Group %s, subgroup Reward, ItemCount is empty.", battlePassName.c_str());
					return false;
				}

				rewardVector.push_back(itemReward);
			}

			m_map_normal_battle_pass_reward.insert(TNormalMapBattlePassReward::value_type(battlePassName.c_str(), rewardVector));
		}

		std::vector<TExtBattlePassMissionInfo> missionInfoVector;

		for (int i = 1; i < n; i++) {
			std::stringstream ss;
			ss << "mission_" << i;

			CGroupNode* pChild;
			if (NULL == (pChild = pGroupNode->GetChildNode(ss.str()))) {
				sys_err("In Group %s, %s subgroup is not defined.", battlePassName.c_str(), ss.str().c_str());
				return false;
			}

			int m = pChild->GetRowCount();

			std::string stMissionSearch[] = { "", "" };
			bool bAlreadySearched = false;
			BYTE bRewardContor = 0;
			TExtBattlePassMissionInfo missionInfo;
			memset(&missionInfo, 0, sizeof(TExtBattlePassMissionInfo));

			missionInfo.bMissionIndex = i;
			
			for (int j = 0; j < m; j++) {
				const CGroupNode::CGroupNodeRow* pRow = NULL;

				pChild->GetRow(j, &pRow);
				if (NULL == pRow) {
					sys_err("In Group %s and subgroup %s null row.", battlePassName.c_str(), ss.str().c_str());
					return false;
				}

				// InfoDesc = ItemVnum from reward
				// InfoName = ItemCount from reward

				std::string stInfoDesc;
				if (!pRow->GetValue("infodesc", stInfoDesc)) {
					sys_err("In Group %s and subgroup %s InfoDesc does not exist.", battlePassName.c_str(), ss.str().c_str());
					return false;
				}

				if (stInfoDesc == "type") {
					std::string stInfoName;
					if (!pRow->GetValue("infoname", stInfoName)) {
						sys_err("In Group %s and subgroup %s InfoName does not exist.", battlePassName.c_str(), ss.str().c_str());
						return false;
					}

					missionInfo.bMissionType = GetMissionTypeByName(stInfoName);
				}

				if (missionInfo.bMissionType <= MISSION_TYPE_NONE || missionInfo.bMissionType >= MISSION_TYPE_MAX) {
					sys_err("In Group %s and subgroup %s Wrong mission type: %d.", battlePassName.c_str(), ss.str().c_str(), missionInfo.bMissionType);
					return false;
				}

				if (!bAlreadySearched) {
					GetMissionSearchName(missionInfo.bMissionType, &stMissionSearch[0], &stMissionSearch[1]);
					bAlreadySearched = true;
				}

				for (int k = 0; k < 2; k++) {
					if (stMissionSearch[k] != "") {
						if (stInfoDesc == stMissionSearch[k]) {
							if (!pRow->GetValue("infoname", missionInfo.dwMissionInfo[k])) {
								sys_err("In Group %s and subgroup %s InfoDesc %s InfoName does not exist.", battlePassName.c_str(), ss.str().c_str(), stMissionSearch[k].c_str());
								return false;
							}

							sys_log(0, "BattlePassInfo: Group %s // Subgroup %s // InfoName %s // InfoValue %d",
								battlePassName.c_str(), ss.str().c_str(), stMissionSearch[k].c_str(), missionInfo.dwMissionInfo[k]);

							stMissionSearch[k] = "";
						}
					}
				}

				if (bRewardContor >= 3) {
					sys_err("In Group %s and subgroup %s More than 3 rewards.", battlePassName.c_str(), ss.str().c_str());
					return false;
				}

				if (isdigit(*stInfoDesc.c_str())) {
					DWORD dwVnum = atoi(stInfoDesc.c_str());
					BYTE bCount = 1;

					if (!pRow->GetValue("infoname", bCount)) {
						sys_err("In Group %s and subgroup %s Wrong ItemCount.", battlePassName.c_str(), ss.str().c_str());
						return false;
					}

					missionInfo.aRewardList[bRewardContor].dwVnum = dwVnum;
					missionInfo.aRewardList[bRewardContor].bCount = bCount;
					bRewardContor++;
				}
			}

			missionInfoVector.push_back(missionInfo);
		}

		m_map_normal_battle_pass_mission_info.insert(TNormalMapBattleMissionInfo::value_type(battlePassName.c_str(), missionInfoVector));
	}
	return true;
}

bool CBattlePassManager::ReadPremiumBattlePassMissions()
{
	TMapPremiumBattlePassName::iterator it = m_map_premium_battle_pass_name.begin();
	while (it != m_map_premium_battle_pass_name.end())
	{
		std::string battlePassName = (it++)->second;

		CGroupNode* pGroupNode = m_pPremiumLoader->GetGroup(battlePassName.c_str());

		sys_err("battle_pass.txt : %s ", battlePassName.c_str());
		if (NULL == pGroupNode) {
			sys_err("battle_pass.txt need group %s.", battlePassName.c_str());
			return false;
		}

		int n = pGroupNode->GetChildNodeCount();
		if (n < 2) {
			sys_err("Group %s need to have at least one grup for Reward and one Mission. Row: %d", battlePassName.c_str(), n);
			return false;
		}

		{
			CGroupNode* pChild;
			if (NULL == (pChild = pGroupNode->GetChildNode("reward"))) {
				sys_err("In Group %s, Reward group is not defined.", battlePassName.c_str());
				return false;
			}

			int m = pChild->GetRowCount();
			std::vector<TExtBattlePassRewardItem> rewardVector;

			for (int j = 1; j <= m; j++) {
				std::stringstream ss;
				ss << j;
				const CGroupNode::CGroupNodeRow* pRow = NULL;

				pChild->GetRow(ss.str(), &pRow);
				if (NULL == pRow) {
					sys_err("In Group %s, subgroup Reward, No %d row.", battlePassName.c_str(), j);
					return false;
				}

				TExtBattlePassRewardItem itemReward;

				if (!pRow->GetValue("itemvnum", itemReward.dwVnum)) {
					sys_err("In Group %s, subgroup Reward, ItemVnum is empty.", battlePassName.c_str());
					return false;
				}

				if (!pRow->GetValue("itemcount", itemReward.bCount)) {
					sys_err("In Group %s, subgroup Reward, ItemCount is empty.", battlePassName.c_str());
					return false;
				}

				rewardVector.push_back(itemReward);
			}

			m_map_premium_battle_pass_reward.insert(TPremiumMapBattlePassReward::value_type(battlePassName.c_str(), rewardVector));
		}

		std::vector<TExtBattlePassMissionInfo> missionInfoVector;

		for (int i = 1; i < n; i++) {
			std::stringstream ss;
			ss << "mission_" << i;

			CGroupNode* pChild;
			if (NULL == (pChild = pGroupNode->GetChildNode(ss.str()))) {
				sys_err("In Group %s, %s subgroup is not defined.", battlePassName.c_str(), ss.str().c_str());
				return false;
			}

			
			int m = pChild->GetRowCount();

			std::string stMissionSearch[] = { "", "" };
			bool bAlreadySearched = false;
			BYTE bRewardContor = 0;
			TExtBattlePassMissionInfo missionInfo;
			memset(&missionInfo, 0, sizeof(TExtBattlePassMissionInfo));

			missionInfo.bMissionIndex = i;
	
			for (int j = 0; j < m; j++) {
				const CGroupNode::CGroupNodeRow* pRow = NULL;

				pChild->GetRow(j, &pRow);
				if (NULL == pRow) {
					sys_err("In Group %s and subgroup %s null row.", battlePassName.c_str(), ss.str().c_str());
					return false;
				}

				// InfoDesc = ItemVnum from reward
				// InfoName = ItemCount from reward

				std::string stInfoDesc;
				if (!pRow->GetValue("infodesc", stInfoDesc)) {
					sys_err("In Group %s and subgroup %s InfoDesc does not exist.", battlePassName.c_str(), ss.str().c_str());
					return false;
				}

				if (stInfoDesc == "type") {
					std::string stInfoName;
					if (!pRow->GetValue("infoname", stInfoName)) {
						sys_err("In Group %s and subgroup %s InfoName does not exist.", battlePassName.c_str(), ss.str().c_str());
						return false;
					}

					missionInfo.bMissionType = GetMissionTypeByName(stInfoName);
				}

				if (missionInfo.bMissionType <= MISSION_TYPE_NONE || missionInfo.bMissionType >= MISSION_TYPE_MAX) {
					sys_err("In Group %s and subgroup %s Wrong mission type: %d.", battlePassName.c_str(), ss.str().c_str(), missionInfo.bMissionType);
					return false;
				}

				if (!bAlreadySearched) {
					GetMissionSearchName(missionInfo.bMissionType, &stMissionSearch[0], &stMissionSearch[1]);
					bAlreadySearched = true;
				}

				for (int k = 0; k < 2; k++) {
					if (stMissionSearch[k] != "") {
						if (stInfoDesc == stMissionSearch[k]) {
							if (!pRow->GetValue("infoname", missionInfo.dwMissionInfo[k])) {
								sys_err("In Group %s and subgroup %s InfoDesc %s InfoName does not exist.", battlePassName.c_str(), ss.str().c_str(), stMissionSearch[k].c_str());
								return false;
							}

							sys_log(0, "BattlePassInfo: Group %s // Subgroup %s // InfoName %s // InfoValue %d",
								battlePassName.c_str(), ss.str().c_str(), stMissionSearch[k].c_str(), missionInfo.dwMissionInfo[k]);

							stMissionSearch[k] = "";
						}
					}
				}

				if (bRewardContor >= 3) {
					sys_err("In Group %s and subgroup %s More than 3 rewards.", battlePassName.c_str(), ss.str().c_str());
					return false;
				}

				if (isdigit(*stInfoDesc.c_str())) {
					DWORD dwVnum = atoi(stInfoDesc.c_str());
					BYTE bCount = 1;

					if (!pRow->GetValue("infoname", bCount)) {
						sys_err("In Group %s and subgroup %s Wrong ItemCount.", battlePassName.c_str(), ss.str().c_str());
						return false;
					}

					missionInfo.aRewardList[bRewardContor].dwVnum = dwVnum;
					missionInfo.aRewardList[bRewardContor].bCount = bCount;
					bRewardContor++;
				}
			}

			missionInfoVector.push_back(missionInfo);
		}

		m_map_premium_battle_pass_mission_info.insert(TPremiumMapBattleMissionInfo::value_type(battlePassName.c_str(), missionInfoVector));
	}
	return true;
}

bool CBattlePassManager::ReadEventBattlePassMissions()
{
	TMapEventBattlePassName::iterator it = m_map_event_battle_pass_name.begin();
	while (it != m_map_event_battle_pass_name.end())
	{
		std::string battlePassName = (it++)->second;

		CGroupNode* pGroupNode = m_pEventLoader->GetGroup(battlePassName.c_str());

		sys_err("battle_pass.txt : %s ", battlePassName.c_str());
		if (NULL == pGroupNode) {
			sys_err("battle_pass.txt need group %s.", battlePassName.c_str());
			return false;
		}

		int n = pGroupNode->GetChildNodeCount();
		if (n < 2) {
			sys_err("Group %s need to have at least one grup for Reward and one Mission. Row: %d", battlePassName.c_str(), n);
			return false;
		}

		{
			CGroupNode* pChild;
			if (NULL == (pChild = pGroupNode->GetChildNode("reward"))) {
				sys_err("In Group %s, Reward group is not defined.", battlePassName.c_str());
				return false;
			}

			int m = pChild->GetRowCount();
			std::vector<TExtBattlePassRewardItem> rewardVector;

			for (int j = 1; j <= m; j++) {
				std::stringstream ss;
				ss << j;
				const CGroupNode::CGroupNodeRow* pRow = NULL;

				pChild->GetRow(ss.str(), &pRow);
				if (NULL == pRow) {
					sys_err("In Group %s, subgroup Reward, No %d row.", battlePassName.c_str(), j);
					return false;
				}

				TExtBattlePassRewardItem itemReward;

				if (!pRow->GetValue("itemvnum", itemReward.dwVnum)) {
					sys_err("In Group %s, subgroup Reward, ItemVnum is empty.", battlePassName.c_str());
					return false;
				}

				if (!pRow->GetValue("itemcount", itemReward.bCount)) {
					sys_err("In Group %s, subgroup Reward, ItemCount is empty.", battlePassName.c_str());
					return false;
				}

				rewardVector.push_back(itemReward);
			}

			m_map_event_battle_pass_reward.insert(TEventMapBattlePassReward::value_type(battlePassName.c_str(), rewardVector));
		}

		std::vector<TExtBattlePassMissionInfo> missionInfoVector;

		for (int i = 1; i < n; i++) {
			std::stringstream ss;
			ss << "mission_" << i;

			CGroupNode* pChild;
			if (NULL == (pChild = pGroupNode->GetChildNode(ss.str()))) {
				sys_err("In Group %s, %s subgroup is not defined.", battlePassName.c_str(), ss.str().c_str());
				return false;
			}

			int m = pChild->GetRowCount();

			std::string stMissionSearch[] = { "", "" };
			bool bAlreadySearched = false;
			BYTE bRewardContor = 0;
			TExtBattlePassMissionInfo missionInfo;
			memset(&missionInfo, 0, sizeof(TExtBattlePassMissionInfo));

			missionInfo.bMissionIndex = i;

			for (int j = 0; j < m; j++) {
				const CGroupNode::CGroupNodeRow* pRow = NULL;

				pChild->GetRow(j, &pRow);
				if (NULL == pRow) {
					sys_err("In Group %s and subgroup %s null row.", battlePassName.c_str(), ss.str().c_str());
					return false;
				}

				// InfoDesc = ItemVnum from reward
				// InfoName = ItemCount from reward

				std::string stInfoDesc;
				if (!pRow->GetValue("infodesc", stInfoDesc)) {
					sys_err("In Group %s and subgroup %s InfoDesc does not exist.", battlePassName.c_str(), ss.str().c_str());
					return false;
				}

				if (stInfoDesc == "type") {
					std::string stInfoName;
					if (!pRow->GetValue("infoname", stInfoName)) {
						sys_err("In Group %s and subgroup %s InfoName does not exist.", battlePassName.c_str(), ss.str().c_str());
						return false;
					}

					missionInfo.bMissionType = GetMissionTypeByName(stInfoName);
				}

				if (missionInfo.bMissionType <= MISSION_TYPE_NONE || missionInfo.bMissionType >= MISSION_TYPE_MAX) {
					sys_err("In Group %s and subgroup %s Wrong mission type: %d.", battlePassName.c_str(), ss.str().c_str(), missionInfo.bMissionType);
					return false;
				}

				if (!bAlreadySearched) {
					GetMissionSearchName(missionInfo.bMissionType, &stMissionSearch[0], &stMissionSearch[1]);
					bAlreadySearched = true;
				}

				for (int k = 0; k < 2; k++) {
					if (stMissionSearch[k] != "") {
						if (stInfoDesc == stMissionSearch[k]) {
							if (!pRow->GetValue("infoname", missionInfo.dwMissionInfo[k])) {
								sys_err("In Group %s and subgroup %s InfoDesc %s InfoName does not exist.", battlePassName.c_str(), ss.str().c_str(), stMissionSearch[k].c_str());
								return false;
							}

							sys_log(0, "BattlePassInfo: Group %s // Subgroup %s // InfoName %s // InfoValue %d",
								battlePassName.c_str(), ss.str().c_str(), stMissionSearch[k].c_str(), missionInfo.dwMissionInfo[k]);

							stMissionSearch[k] = "";
						}
					}
				}

				if (bRewardContor >= 3) {
					sys_err("In Group %s and subgroup %s More than 3 rewards.", battlePassName.c_str(), ss.str().c_str());
					return false;
				}

				if (isdigit(*stInfoDesc.c_str())) {
					DWORD dwVnum = atoi(stInfoDesc.c_str());
					BYTE bCount = 1;

					if (!pRow->GetValue("infoname", bCount)) {
						sys_err("In Group %s and subgroup %s Wrong ItemCount.", battlePassName.c_str(), ss.str().c_str());
						return false;
					}

					missionInfo.aRewardList[bRewardContor].dwVnum = dwVnum;
					missionInfo.aRewardList[bRewardContor].bCount = bCount;
					bRewardContor++;
				}
			}

			missionInfoVector.push_back(missionInfo);
		}

		m_map_event_battle_pass_mission_info.insert(TEventMapBattleMissionInfo::value_type(battlePassName.c_str(), missionInfoVector));
	}
	return true;
}

void CBattlePassManager::CheckBattlePassTimes()
{
	{
		bool findActiveBattlePass = false;
		int cachedBattlePassID, cachedStartTime, cachedEndTime;
		for(TMapNormalBattlePassTimeTable::iterator it = m_map_normal_battle_pass_times.begin(); it != m_map_normal_battle_pass_times.end(); ++it)
		{
			std::vector<TExtBattlePassTimeTable> timeInfo = it->second;
			if (time(0) > timeInfo[0].dwStartTime and time(0) <timeInfo[0].dwEndTime) {
				findActiveBattlePass = true;
				cachedBattlePassID = timeInfo[0].bBattlePassId;
				cachedStartTime = timeInfo[0].dwStartTime;
				cachedEndTime = timeInfo[0].dwEndTime;
			}
		}
		if (findActiveBattlePass)
		{
			m_dwActiveNormalBattlePassID = cachedBattlePassID;
			m_dwNormalBattlePassStartTime = cachedStartTime;
			m_dwNormalBattlePassEndTime = cachedEndTime;
		}
		else
		{
			m_dwActiveNormalBattlePassID = 0;
			m_dwNormalBattlePassStartTime = 0;
			m_dwNormalBattlePassEndTime = 0;
		}
	}
	
	{
		bool findActiveBattlePass = false;
		int cachedBattlePassID, cachedStartTime, cachedEndTime;
		for(TMapPremiumBattlePassTimeTable::iterator it = m_map_premium_battle_pass_times.begin(); it != m_map_premium_battle_pass_times.end(); ++it)
		{
			std::vector<TExtBattlePassTimeTable> timeInfo = it->second;
			if (time(0) > timeInfo[0].dwStartTime and time(0) <timeInfo[0].dwEndTime) {
				findActiveBattlePass = true;
				cachedBattlePassID = timeInfo[0].bBattlePassId;
				cachedStartTime = timeInfo[0].dwStartTime;
				cachedEndTime = timeInfo[0].dwEndTime;
			}
		}
		if (findActiveBattlePass)
		{
			m_dwActivePremiumBattlePassID = cachedBattlePassID;
			m_dwPremiumBattlePassStartTime = cachedStartTime;
			m_dwPremiumBattlePassEndTime = cachedEndTime;
		}
		else
		{
			m_dwActivePremiumBattlePassID = 0;
			m_dwPremiumBattlePassStartTime = 0;
			m_dwPremiumBattlePassEndTime = 0;
		}
	}
	
	{
		bool findActiveBattlePass = false;
		int cachedBattlePassID, cachedStartTime, cachedEndTime;
		for(TMapEventBattlePassTimeTable::iterator it = m_map_event_battle_pass_times.begin(); it != m_map_event_battle_pass_times.end(); ++it)
		{
			std::vector<TExtBattlePassTimeTable> timeInfo = it->second;
			if (time(0) > timeInfo[0].dwStartTime and time(0) <timeInfo[0].dwEndTime) {
				findActiveBattlePass = true;
				cachedBattlePassID = timeInfo[0].bBattlePassId;
				cachedStartTime = timeInfo[0].dwStartTime;
				cachedEndTime = timeInfo[0].dwEndTime;
			}
		}
		if (findActiveBattlePass)
		{
			m_dwActiveEventBattlePassID = cachedBattlePassID;
			m_dwEventBattlePassStartTime = cachedStartTime;
			m_dwEventBattlePassEndTime = cachedEndTime;
		}
		else
		{
			m_dwActiveEventBattlePassID = 0;
			m_dwEventBattlePassStartTime = 0;
			m_dwEventBattlePassEndTime = 0;
		}
	}
}

BYTE CBattlePassManager::GetMissionIndex(BYTE bBattlePassType, BYTE bMissionType, DWORD dwCondition)
{
	bool isMinMissionType = false;
	if (bMissionType == 2 or bMissionType == 4 or bMissionType == 20)
		isMinMissionType = true;

	if (bBattlePassType == 1)
	{
		TMapNormalBattlePassName::iterator it_normal = m_map_normal_battle_pass_name.find(m_dwActiveNormalBattlePassID);

		if (it_normal != m_map_normal_battle_pass_name.end())
		{
			std::string normalBattlePassName = it_normal->second;
			
			TNormalMapBattleMissionInfo::iterator itNormalInfo = m_map_normal_battle_pass_mission_info.find(normalBattlePassName);
			std::vector<TExtBattlePassMissionInfo> normalMissionInfo = itNormalInfo->second;

			for (int i = 0; i < normalMissionInfo.size(); i++) {
				//sys_err("NORMAL: TableSize = %d |-|-| Type : %d |-| Condition %d = %d |-| MissionIndex = %d", normalMissionInfo.size(), normalMissionInfo[i].bMissionType, normalMissionInfo[i].dwMissionInfo[0], dwCondition, normalMissionInfo[i].bMissionIndex);
				if (normalMissionInfo[i].dwMissionInfo[0] != 0 and isMinMissionType == false) {
					if (normalMissionInfo[i].bMissionType == bMissionType and normalMissionInfo[i].dwMissionInfo[0] == dwCondition)
						return normalMissionInfo[i].bMissionIndex;
				}
				else {
					if (normalMissionInfo[i].bMissionType == bMissionType)
						return normalMissionInfo[i].bMissionIndex;
				}
			}
			return 0;
		}
	}
	else if (bBattlePassType == 2)
	{
		TMapPremiumBattlePassName::iterator it_premium = m_map_premium_battle_pass_name.find(m_dwActivePremiumBattlePassID);

		if (it_premium != m_map_premium_battle_pass_name.end())
		{
			std::string premiumBattlePassName = it_premium->second;
			
			TPremiumMapBattleMissionInfo::iterator itPremiumInfo = m_map_premium_battle_pass_mission_info.find(premiumBattlePassName);
			std::vector<TExtBattlePassMissionInfo> premiumMissionInfo = itPremiumInfo->second;
			for (int i = 0; i < premiumMissionInfo.size(); i++) {
				//sys_err("PREMIUM: TableSize = %d |-|-| Type : %d |-| Condition %d = %d |-| MissionIndex = %d", premiumMissionInfo.size(), premiumMissionInfo[i].bMissionType, premiumMissionInfo[i].dwMissionInfo[0], dwCondition, premiumMissionInfo[i].bMissionIndex);
				if (premiumMissionInfo[i].dwMissionInfo[0] != 0 and isMinMissionType == false) {
					if (premiumMissionInfo[i].bMissionType == bMissionType and premiumMissionInfo[i].dwMissionInfo[0] == dwCondition)
						return premiumMissionInfo[i].bMissionIndex;
				}
				else {
					if (premiumMissionInfo[i].bMissionType == bMissionType)
						return premiumMissionInfo[i].bMissionIndex;
				}
			}
			return 0;
		}
	}
	else if (bBattlePassType == 3)
	{
		TMapEventBattlePassName::iterator it_event = m_map_event_battle_pass_name.find(m_dwActiveEventBattlePassID);

		if (it_event != m_map_event_battle_pass_name.end())
		{
			std::string eventBattlePassName = it_event->second;
			
			TEventMapBattleMissionInfo::iterator itEventInfo = m_map_event_battle_pass_mission_info.find(eventBattlePassName);
			std::vector<TExtBattlePassMissionInfo> eventMissionInfo = itEventInfo->second;

			for (int i = 0; i < eventMissionInfo.size(); i++) {
				sys_err("EVENT: TableSize = %d |-|-| Type : %d |-| Condition %d = %d |-| MissionIndex = %d", eventMissionInfo.size(), eventMissionInfo[i].bMissionType, eventMissionInfo[i].dwMissionInfo[0], dwCondition, eventMissionInfo[i].bMissionIndex);
				if (eventMissionInfo[i].dwMissionInfo[0] != 0 and isMinMissionType == false) {
					if (eventMissionInfo[i].bMissionType == bMissionType and eventMissionInfo[i].dwMissionInfo[0] == dwCondition)
						return eventMissionInfo[i].bMissionIndex;
				}
				else {
					if (eventMissionInfo[i].bMissionType == bMissionType)
						return eventMissionInfo[i].bMissionIndex;
				}
			}
			return 0;
		}
	}
	else
		sys_err("GetMissionIndex: False BattlePassType (%d)", bBattlePassType);
	return 0;
}

BYTE CBattlePassManager::GetMissionTypeByName(std::string stMissionName)
{
	for (int i = 0; i < MISSION_TYPE_MAX; i++)
	{
		if (g_astMissionType[i] == stMissionName)
			return i;
	}

	return 0;
}

BYTE CBattlePassManager::GetMissionTypeByIndex(BYTE bBattlePassType, DWORD dwIndex)
{
	if (bBattlePassType == 1)
	{
		TMapNormalBattlePassName::iterator it_normal = m_map_normal_battle_pass_name.find(m_dwActiveNormalBattlePassID);

		if (it_normal != m_map_normal_battle_pass_name.end()) {
			std::string normalBattlePassName = it_normal->second;
			
			TNormalMapBattleMissionInfo::iterator itNormalInfo = m_map_normal_battle_pass_mission_info.find(normalBattlePassName);
			std::vector<TExtBattlePassMissionInfo> normalMissionInfo = itNormalInfo->second;

			for (int i = 0; i < normalMissionInfo.size(); i++) {
				if (normalMissionInfo[i].dwMissionInfo[0] != 0 and normalMissionInfo[i].bMissionIndex == dwIndex) {
					return normalMissionInfo[i].bMissionType;
				}
			}
			return 0;
		}
	}
	else if (bBattlePassType == 2)
	{
		TMapPremiumBattlePassName::iterator it_premium = m_map_premium_battle_pass_name.find(m_dwActivePremiumBattlePassID);

		if (it_premium != m_map_premium_battle_pass_name.end()) {
			std::string premiumBattlePassName = it_premium->second;
			
			TPremiumMapBattleMissionInfo::iterator itPremiumInfo = m_map_premium_battle_pass_mission_info.find(premiumBattlePassName);
			std::vector<TExtBattlePassMissionInfo> premiumMissionInfo = itPremiumInfo->second;
			
			for (int i = 0; i < premiumMissionInfo.size(); i++) {
				if (premiumMissionInfo[i].dwMissionInfo[0] != 0 and premiumMissionInfo[i].bMissionIndex == dwIndex) {
					return premiumMissionInfo[i].bMissionType;
				}
			}
			return 0;
		}
	}
	else if (bBattlePassType == 3)
	{
		TMapEventBattlePassName::iterator it_event = m_map_event_battle_pass_name.find(m_dwActiveEventBattlePassID);

		if (it_event != m_map_event_battle_pass_name.end()) {
			std::string eventBattlePassName = it_event->second;
			
			TEventMapBattleMissionInfo::iterator itEventInfo = m_map_event_battle_pass_mission_info.find(eventBattlePassName);
			std::vector<TExtBattlePassMissionInfo> eventMissionInfo = itEventInfo->second;
			
			for (int i = 0; i < eventMissionInfo.size(); i++) {
				if (eventMissionInfo[i].dwMissionInfo[0] != 0 and eventMissionInfo[i].bMissionIndex == dwIndex) {
					return eventMissionInfo[i].bMissionType;
				}
			}
			return 0;
		}
	}
	else
		sys_err("GetMissionIndex: False BattlePassType (%d)", bBattlePassType);
	return 0;
}

std::string CBattlePassManager::GetMissionNameByType(BYTE bType)
{
	for (int i = 0; i < MISSION_TYPE_MAX; i++)
	{
		if (i == bType)
			return g_astMissionType[i];
	}

	return "";
}

std::string CBattlePassManager::GetNormalBattlePassNameByID(BYTE bID)
{
	TMapNormalBattlePassName::iterator it = m_map_normal_battle_pass_name.find(bID);

	if (it == m_map_normal_battle_pass_name.end())
	{
		return "";
	}

	return it->second;
}

std::string CBattlePassManager::GetPremiumBattlePassNameByID(BYTE bID)
{
	TMapPremiumBattlePassName::iterator it = m_map_premium_battle_pass_name.find(bID);

	if (it == m_map_premium_battle_pass_name.end())
	{
		return "";
	}

	return it->second;
}

std::string CBattlePassManager::GetEventBattlePassNameByID(BYTE bID)
{
	TMapEventBattlePassName::iterator it = m_map_event_battle_pass_name.find(bID);

	if (it == m_map_event_battle_pass_name.end())
	{
		return "";
	}

	return it->second;
}

void CBattlePassManager::GetMissionSearchName(BYTE bMissionType, std::string* st_name_1, std::string* st_name_2)
{
	switch (bMissionType)
	{
		case KILL_MONSTER:
		case DAMAGE_MONSTER:
			*st_name_1 = "mob_vnum";
			*st_name_2 = "value";
			break;
		
		case KILL_PLAYER:
		case DAMAGE_PLAYER:
			*st_name_1 = "min_level";
			*st_name_2 = "value";
			break;
		
		case BP_ITEM_USE:
		case BP_ITEM_SELL:
		case BP_ITEM_CRAFT:
		case BP_ITEM_REFINE:
		case BP_ITEM_DESTROY:
		case BP_ITEM_COLLECT:
		case FISH_FISHING:
		case FISH_GRILL:
		case FISH_CATCH:
			*st_name_1 = "item_vnum";
			*st_name_2 = "count";
			break;
	
		case EXP_COLLECT:
			*st_name_1 = "mob_vnum";
			*st_name_2 = "value";
			break;

		case YANG_COLLECT:
			*st_name_1 = "map_idx";
			*st_name_2 = "value";
			break;

		case GUILD_PLAY_GUILDWAR:
			*st_name_1 = "war_type";
			*st_name_2 = "count";
			break;
		
		case GUILD_SPENT_EXP:
		case GAYA_CRAFT_GAYA:
		case GAYA_BUY_ITEM_GAYA_COST:
			*st_name_1 = "";
			*st_name_2 = "value";
			break;

		case PET_ENCHANT:
			*st_name_1 = "min_type";
			*st_name_2 = "value";
			break;

		case COMPLETE_DUNGEON:
			*st_name_1 = "dungeon_id";
			*st_name_2 = "count";
			break;

		case COMPLETE_MINIGAME:
			*st_name_1 = "minigame_id";
			*st_name_2 = "count";
			break;

		default:
			*st_name_1 = "";
			*st_name_2 = "";
			break;
	}
}

void CBattlePassManager::BattlePassRequestOpen(LPCHARACTER pkChar, bool bAfterOpen)
{
	if (!pkChar) {
		sys_err("pkChar null");
		return;
	}

	if (!pkChar->GetDesc()) {
		sys_err("pkChar GetDesc null");
		return;
	}
	
	if(get_dword_time() < pkChar->GetLastReciveExtBattlePassInfoTime())
		return;
	
	pkChar->SetLastReciveExtBattlePassInfoTime(get_dword_time() + 1000);
	
	{
		// --- NORMAL
		const char* season_name;
		TMapNormalBattlePassName::iterator it = m_map_normal_battle_pass_name.find(m_dwActiveNormalBattlePassID);
		if (it != m_map_normal_battle_pass_name.end()) {
			std::string BattlePassName = it->second;
			season_name = LC_TEXT(BattlePassName.c_str());
		}
		else
			season_name = "NoName";

		// --- GENERAL INFOS
		for(TMapNormalBattlePassTimeTable::iterator it = m_map_normal_battle_pass_times.begin(); it != m_map_normal_battle_pass_times.end(); ++it)
		{
			std::vector<TExtBattlePassTimeTable> timeInfo = it->second;
			if (!timeInfo.empty()) {
				TPacketGCExtBattlePassGeneralInfo packet;
				packet.bHeader = HEADER_GC_EXT_BATTLE_PASS_GENERAL_INFO;
				packet.bBattlePassType = 1;
				strcpy(packet.szSeasonName, season_name);
				packet.dwBattlePassID = timeInfo[0].bBattlePassId;
				packet.dwBattlePassStartTime = timeInfo[0].dwStartTime;
				packet.dwBattlePassEndTime = timeInfo[0].dwEndTime;

				pkChar->GetDesc()->Packet(&packet, sizeof(TPacketGCExtBattlePassGeneralInfo));
			}
		}
	}
	
	{
		const char* season_name;
		TMapPremiumBattlePassName::iterator it = m_map_premium_battle_pass_name.find(m_dwActivePremiumBattlePassID);
		if (it != m_map_premium_battle_pass_name.end()) {
			std::string BattlePassName = it->second;
			season_name = LC_TEXT(BattlePassName.c_str());
		}
		else
			season_name = "NoName";
		
		for(TMapPremiumBattlePassTimeTable::iterator it = m_map_premium_battle_pass_times.begin(); it != m_map_premium_battle_pass_times.end(); ++it)
		{
			std::vector<TExtBattlePassTimeTable> timeInfo = it->second;
			if (!timeInfo.empty()) {
				TPacketGCExtBattlePassGeneralInfo packet;
				packet.bHeader = HEADER_GC_EXT_BATTLE_PASS_GENERAL_INFO;
				packet.bBattlePassType = 2;
				strcpy(packet.szSeasonName, season_name);
				packet.dwBattlePassID = timeInfo[0].bBattlePassId;
				packet.dwBattlePassStartTime = timeInfo[0].dwStartTime;
				packet.dwBattlePassEndTime = timeInfo[0].dwEndTime;

				pkChar->GetDesc()->Packet(&packet, sizeof(TPacketGCExtBattlePassGeneralInfo));
			}
		}
	}
	
	{
		const char* season_name;
		TMapEventBattlePassName::iterator it = m_map_event_battle_pass_name.find(m_dwActiveEventBattlePassID);
		if (it != m_map_event_battle_pass_name.end()) {
			std::string BattlePassName = it->second;
			season_name = LC_TEXT(BattlePassName.c_str());
		}
		else
			season_name = "NoName";
			
		for(TMapEventBattlePassTimeTable::iterator it = m_map_event_battle_pass_times.begin(); it != m_map_event_battle_pass_times.end(); ++it)
		{
			std::vector<TExtBattlePassTimeTable> timeInfo = it->second;
			if (!timeInfo.empty()) {
				TPacketGCExtBattlePassGeneralInfo packet;
				packet.bHeader = HEADER_GC_EXT_BATTLE_PASS_GENERAL_INFO;
				packet.bBattlePassType = 3;
				strcpy(packet.szSeasonName, season_name);
				packet.dwBattlePassID = timeInfo[0].bBattlePassId;
				packet.dwBattlePassStartTime = timeInfo[0].dwStartTime;
				packet.dwBattlePassEndTime = timeInfo[0].dwEndTime;

				pkChar->GetDesc()->Packet(&packet, sizeof(TPacketGCExtBattlePassGeneralInfo));
			}
		}
	}

	// --- NORMAL
	TMapNormalBattlePassName::iterator it_normal = m_map_normal_battle_pass_name.find(m_dwActiveNormalBattlePassID);

	if (it_normal != m_map_normal_battle_pass_name.end())
	{
		std::string normalBattlePassName = it_normal->second;
		
		TNormalMapBattleMissionInfo::iterator itNormalInfo = m_map_normal_battle_pass_mission_info.find(normalBattlePassName);
		TNormalMapBattlePassReward::iterator itNormalReward = m_map_normal_battle_pass_reward.find(normalBattlePassName);

		std::vector<TExtBattlePassMissionInfo> normalMissionInfo = itNormalInfo->second;
		std::vector<TExtBattlePassRewardItem> normalRewardInfo = itNormalReward->second;

		for (int i = 0; i < normalMissionInfo.size(); i++) {
			normalMissionInfo[i].dwMissionInfo[2] = pkChar->GetExtBattlePassMissionProgress(1, normalMissionInfo[i].bMissionIndex, normalMissionInfo[i].bMissionType);
		}
		
		if (!normalMissionInfo.empty()) {
			TPacketGCExtBattlePassMissionInfo packet;
			packet.bHeader = HEADER_GC_EXT_BATTLE_PASS_MISSION_INFO;
			packet.wSize = sizeof(packet) + sizeof(TExtBattlePassMissionInfo) * normalMissionInfo.size();
			packet.wRewardSize = sizeof(TExtBattlePassRewardItem) * normalRewardInfo.size();
			packet.bBattlePassType = 1;
			packet.dwBattlePassID = m_dwActiveNormalBattlePassID;

			pkChar->GetDesc()->BufferedPacket(&packet, sizeof(packet));
			pkChar->GetDesc()->BufferedPacket(&normalMissionInfo[0], sizeof(TExtBattlePassMissionInfo) * normalMissionInfo.size());
			pkChar->GetDesc()->Packet(&normalRewardInfo[0], sizeof(TExtBattlePassRewardItem) * normalRewardInfo.size());
		}
	}
	
	//--- PREMIUM
	if (pkChar->GetExtBattlePassPremiumID() == m_dwActivePremiumBattlePassID)
	{
		TMapPremiumBattlePassName::iterator it_premium = m_map_premium_battle_pass_name.find(m_dwActivePremiumBattlePassID);

		if (it_premium != m_map_premium_battle_pass_name.end())
		{
			std::string premiumBattlePassName = it_premium->second;
			
			TPremiumMapBattleMissionInfo::iterator itPremiumInfo = m_map_premium_battle_pass_mission_info.find(premiumBattlePassName);
			TPremiumMapBattlePassReward::iterator itPremiumReward = m_map_premium_battle_pass_reward.find(premiumBattlePassName);

			std::vector<TExtBattlePassRewardItem> premiumRewardInfo = itPremiumReward->second;
			std::vector<TExtBattlePassMissionInfo> premiumMissionInfo = itPremiumInfo->second;

			for (int i = 0; i < premiumMissionInfo.size(); i++) {
				premiumMissionInfo[i].dwMissionInfo[2] = pkChar->GetExtBattlePassMissionProgress(2, premiumMissionInfo[i].bMissionIndex, premiumMissionInfo[i].bMissionType);
			}
			
			if (!premiumMissionInfo.empty()) {
				TPacketGCExtBattlePassMissionInfo packet;
				packet.bHeader = HEADER_GC_EXT_BATTLE_PASS_MISSION_INFO;
				packet.wSize = sizeof(packet) + sizeof(TExtBattlePassMissionInfo) * premiumMissionInfo.size();
				packet.wRewardSize = sizeof(TExtBattlePassRewardItem) * premiumRewardInfo.size();
				packet.bBattlePassType = 2;
				packet.dwBattlePassID = m_dwActivePremiumBattlePassID;

				pkChar->GetDesc()->BufferedPacket(&packet, sizeof(packet));
				pkChar->GetDesc()->BufferedPacket(&premiumMissionInfo[0], sizeof(TExtBattlePassMissionInfo) * premiumMissionInfo.size());
				pkChar->GetDesc()->Packet(&premiumRewardInfo[0], sizeof(TExtBattlePassRewardItem) * premiumRewardInfo.size());
			}
		}
	}
	
	//--- EVENT
	TMapEventBattlePassName::iterator it_event = m_map_event_battle_pass_name.find(m_dwActiveEventBattlePassID);

	if (it_event != m_map_event_battle_pass_name.end())
	{
		std::string eventBattlePassName = it_event->second;
		
		TEventMapBattleMissionInfo::iterator itEventInfo = m_map_event_battle_pass_mission_info.find(eventBattlePassName);
		TEventMapBattlePassReward::iterator itEventReward = m_map_event_battle_pass_reward.find(eventBattlePassName);

		std::vector<TExtBattlePassRewardItem> eventRewardInfo = itEventReward->second;
		std::vector<TExtBattlePassMissionInfo> eventMissionInfo = itEventInfo->second;

		for (int i = 0; i < eventMissionInfo.size(); i++) {
			eventMissionInfo[i].dwMissionInfo[2] = pkChar->GetExtBattlePassMissionProgress(3, eventMissionInfo[i].bMissionIndex, eventMissionInfo[i].bMissionType);
		}

		if (!eventMissionInfo.empty()) {
			TPacketGCExtBattlePassMissionInfo packet;
			packet.bHeader = HEADER_GC_EXT_BATTLE_PASS_MISSION_INFO;
			packet.wSize = sizeof(packet) + sizeof(TExtBattlePassMissionInfo) * eventMissionInfo.size();
			packet.wRewardSize = sizeof(TExtBattlePassRewardItem) * eventRewardInfo.size();
			packet.bBattlePassType = 3;
			packet.dwBattlePassID = m_dwActiveEventBattlePassID;

			pkChar->GetDesc()->BufferedPacket(&packet, sizeof(packet));
			pkChar->GetDesc()->BufferedPacket(&eventMissionInfo[0], sizeof(TExtBattlePassMissionInfo) * eventMissionInfo.size());
			pkChar->GetDesc()->Packet(&eventRewardInfo[0], sizeof(TExtBattlePassRewardItem) * eventRewardInfo.size());
		}
	}
	
	if (bAfterOpen)
	{
		//--- OPEN GUI
		TPacketGCExtBattlePassOpen packet;
		packet.bHeader = HEADER_GC_EXT_BATTLE_PASS_OPEN;
		pkChar->GetDesc()->Packet(&packet, sizeof(packet));
	}
}

void CBattlePassManager::BattlePassRewardMission(LPCHARACTER pkChar, BYTE bBattlePassType, DWORD bBattlePassId, BYTE bMissionIndex)
{
	if (!pkChar)
		return;

	if (!pkChar->GetDesc())
		return;

	if (bBattlePassType == 1)
	{
		TMapNormalBattlePassName::iterator it = m_map_normal_battle_pass_name.find(bBattlePassId);
		if (it == m_map_normal_battle_pass_name.end())
			return;

		std::string battlePassName = it->second;
		TNormalMapBattleMissionInfo::iterator itInfo = m_map_normal_battle_pass_mission_info.find(battlePassName);
		if (itInfo == m_map_normal_battle_pass_mission_info.end())
			return;

		std::vector<TExtBattlePassMissionInfo> missionInfo = itInfo->second;
		for (int i = 0; i < missionInfo.size(); i++) {
			if (missionInfo[i].bMissionIndex == bMissionIndex) {
				for (int j = 0; j < 3; j++) {
					if (missionInfo[i].aRewardList[j].dwVnum && missionInfo[i].aRewardList[j].bCount > 0)
						pkChar->AutoGiveItem(missionInfo[i].aRewardList[j].dwVnum, missionInfo[i].aRewardList[j].bCount);
				}
				break;
			}
		}
	}
	if (bBattlePassType == 2)
	{
		TMapPremiumBattlePassName::iterator it = m_map_premium_battle_pass_name.find(bBattlePassId);
		if (it == m_map_premium_battle_pass_name.end())
			return;

		std::string battlePassName = it->second;
		TPremiumMapBattleMissionInfo::iterator itInfo = m_map_premium_battle_pass_mission_info.find(battlePassName);
		if (itInfo == m_map_premium_battle_pass_mission_info.end())
			return;

		std::vector<TExtBattlePassMissionInfo> missionInfo = itInfo->second;
		for (int i = 0; i < missionInfo.size(); i++) {
			if (missionInfo[i].bMissionIndex == bMissionIndex) {
				for (int j = 0; j < 3; j++) {
					if (missionInfo[i].aRewardList[j].dwVnum && missionInfo[i].aRewardList[j].bCount > 0)
						pkChar->AutoGiveItem(missionInfo[i].aRewardList[j].dwVnum, missionInfo[i].aRewardList[j].bCount);
				}
				break;
			}
		}
	}
	if (bBattlePassType == 3)
	{
		TMapEventBattlePassName::iterator it = m_map_event_battle_pass_name.find(bBattlePassId);
		if (it == m_map_event_battle_pass_name.end())
			return;

		std::string battlePassName = it->second;
		TEventMapBattleMissionInfo::iterator itInfo = m_map_event_battle_pass_mission_info.find(battlePassName);
		if (itInfo == m_map_event_battle_pass_mission_info.end())
			return;

		std::vector<TExtBattlePassMissionInfo> missionInfo = itInfo->second;
		for (int i = 0; i < missionInfo.size(); i++) {
			if (missionInfo[i].bMissionIndex == bMissionIndex) {
				for (int j = 0; j < 3; j++) {
					if (missionInfo[i].aRewardList[j].dwVnum && missionInfo[i].aRewardList[j].bCount > 0)
						pkChar->AutoGiveItem(missionInfo[i].aRewardList[j].dwVnum, missionInfo[i].aRewardList[j].bCount);
				}
				break;
			}
		}
	}
}

void CBattlePassManager::BattlePassRequestReward(LPCHARACTER pkChar, BYTE bBattlePassType)
{
	if (!pkChar)
		return;

	if (!pkChar->GetDesc())
		return;

	BYTE bBattlePassID;

	if (bBattlePassType == 1)
	{
		bBattlePassID = GetNormalBattlePassID();

		TMapNormalBattlePassName::iterator it = m_map_normal_battle_pass_name.find(bBattlePassID);
		if (it == m_map_normal_battle_pass_name.end())
			return;

		std::string battlePassName = it->second;
		TNormalMapBattleMissionInfo::iterator itInfo = m_map_normal_battle_pass_mission_info.find(battlePassName);

		if (itInfo == m_map_normal_battle_pass_mission_info.end())
			return;

		std::vector<TExtBattlePassMissionInfo> missionInfo = itInfo->second;

		for (int i = 0; i < missionInfo.size(); i++) {
			if (!pkChar->IsExtBattlePassCompletedMission(bBattlePassType, missionInfo[i].bMissionIndex, missionInfo[i].bMissionType)) {
				pkChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("BATTLEPASS_NOT_COMPLETED_ALL_NORMAL_MISSIONS"));
				return;
			}
		}
	}
	if (bBattlePassType == 2)
	{
		bBattlePassID = GetPremiumBattlePassID();

		TMapPremiumBattlePassName::iterator it = m_map_premium_battle_pass_name.find(bBattlePassID);
		if (it == m_map_premium_battle_pass_name.end())
			return;

		std::string battlePassName = it->second;
		TPremiumMapBattleMissionInfo::iterator itInfo = m_map_premium_battle_pass_mission_info.find(battlePassName);

		if (itInfo == m_map_premium_battle_pass_mission_info.end())
			return;

		std::vector<TExtBattlePassMissionInfo> missionInfo = itInfo->second;

		for (int i = 0; i < missionInfo.size(); i++) {
			if (!pkChar->IsExtBattlePassCompletedMission(bBattlePassType, missionInfo[i].bMissionIndex, missionInfo[i].bMissionType)) {
				pkChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("BATTLEPASS_NOT_COMPLETED_ALL_PREMIUM_MISSIONS"));
				return;
			}
		}
	}
	if (bBattlePassType == 3)
	{
		bBattlePassID = GetEventBattlePassID();

		TMapEventBattlePassName::iterator it = m_map_event_battle_pass_name.find(bBattlePassID);
		if (it == m_map_event_battle_pass_name.end())
			return;

		std::string battlePassName = it->second;
		TEventMapBattleMissionInfo::iterator itInfo = m_map_event_battle_pass_mission_info.find(battlePassName);

		if (itInfo == m_map_event_battle_pass_mission_info.end())
			return;

		std::vector<TExtBattlePassMissionInfo> missionInfo = itInfo->second;

		for (int i = 0; i < missionInfo.size(); i++) {
			if (!pkChar->IsExtBattlePassCompletedMission(bBattlePassType, missionInfo[i].bMissionIndex, missionInfo[i].bMissionType)) {
				pkChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("BATTLEPASS_NOT_COMPLETED_ALL_EVENT_MISSIONS"));
				return;
			}
		}
	}

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT battlepass_completed FROM player.battlepass_playerindex WHERE player_id = %d and battlepass_type = %d and battlepass_id = %d", pkChar->GetPlayerID(), bBattlePassType, bBattlePassID));
	if (pMsg->uiSQLErrno)
		return;
	
	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	BYTE bHasRecived = 0;
	str_to_number(bHasRecived, row[0]);
	
	if (bHasRecived != 0) {
		pkChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("BATTLEPASS_ALREADY_EARNED_BATTLEPASS_REWARD"));
		return;
	}
	
	DBManager::instance().DirectQuery("UPDATE player.battlepass_playerindex SET battlepass_completed = 1, end_time = NOW() WHERE player_id = %d and battlepass_type = %d and battlepass_id = %d", pkChar->GetPlayerID(), bBattlePassType, bBattlePassID);
	
	if (bBattlePassID)
		BattlePassReward(pkChar, bBattlePassType, bBattlePassID);
}

void CBattlePassManager::BattlePassReward(LPCHARACTER pkChar, BYTE bBattlePassType, BYTE bBattlePassID)
{
	if (bBattlePassType == 1)
	{
		TMapNormalBattlePassName::iterator it = m_map_normal_battle_pass_name.find(bBattlePassID);
		if (it == m_map_normal_battle_pass_name.end())
			return;

		std::string battlePassName = it->second;
		TNormalMapBattlePassReward::iterator itReward = m_map_normal_battle_pass_reward.find(battlePassName);
		if (itReward == m_map_normal_battle_pass_reward.end())
			return;

		std::vector<TExtBattlePassRewardItem> rewardInfo = itReward->second;
		for (int i = 0; i < rewardInfo.size(); i++) {
			pkChar->AutoGiveItem(rewardInfo[i].dwVnum, rewardInfo[i].bCount);
		}
		pkChar->EffectPacket(SE_EFFECT_BP_NORMAL_BATTLEPASS_COMPLETED);
	}
	if (bBattlePassType == 2)
	{
		TMapPremiumBattlePassName::iterator it = m_map_premium_battle_pass_name.find(bBattlePassID);
		if (it == m_map_premium_battle_pass_name.end())
			return;

		std::string battlePassName = it->second;
		TPremiumMapBattlePassReward::iterator itReward = m_map_premium_battle_pass_reward.find(battlePassName);
		if (itReward == m_map_premium_battle_pass_reward.end())
			return;

		std::vector<TExtBattlePassRewardItem> rewardInfo = itReward->second;
		for (int i = 0; i < rewardInfo.size(); i++) {
			pkChar->AutoGiveItem(rewardInfo[i].dwVnum, rewardInfo[i].bCount);
		}
		pkChar->EffectPacket(SE_EFFECT_BP_PREMIUM_BATTLEPASS_COMPLETED);
	}
	if (bBattlePassType == 3)
	{
		TMapEventBattlePassName::iterator it = m_map_event_battle_pass_name.find(bBattlePassID);
		if (it == m_map_event_battle_pass_name.end())
			return;

		std::string battlePassName = it->second;
		TEventMapBattlePassReward::iterator itReward = m_map_event_battle_pass_reward.find(battlePassName);
		if (itReward == m_map_event_battle_pass_reward.end())
			return;

		std::vector<TExtBattlePassRewardItem> rewardInfo = itReward->second;
		for (int i = 0; i < rewardInfo.size(); i++) {
			pkChar->AutoGiveItem(rewardInfo[i].dwVnum, rewardInfo[i].bCount);
		}
		pkChar->EffectPacket(SE_EFFECT_BP_EVENT_BATTLEPASS_COMPLETED);
	}
}

bool CBattlePassManager::BattlePassMissionGetInfo(BYTE bBattlePassType, BYTE bMissionIndex, BYTE bBattlePassId, BYTE bMissionType, DWORD* dwFirstInfo, DWORD* dwSecondInfo)
{
	if (bBattlePassType == 1)
	{
		TMapNormalBattlePassName::iterator it = m_map_normal_battle_pass_name.find(bBattlePassId);
		if (it == m_map_normal_battle_pass_name.end())
			return false;

		std::string battlePassName = it->second;
		TNormalMapBattleMissionInfo::iterator itInfo = m_map_normal_battle_pass_mission_info.find(battlePassName);

		if (itInfo == m_map_normal_battle_pass_mission_info.end())
			return false;

		std::vector<TExtBattlePassMissionInfo> missionInfo = itInfo->second;

		for (int i = 0; i < missionInfo.size(); i++)
		{
			if (missionInfo[i].bMissionIndex == bMissionIndex && missionInfo[i].bMissionType == bMissionType)
			{
				*dwFirstInfo = missionInfo[i].dwMissionInfo[0];
				*dwSecondInfo = missionInfo[i].dwMissionInfo[1];
				return true;
			}
		}

		return false;
	}
	else if (bBattlePassType == 2)
	{
		TMapPremiumBattlePassName::iterator it = m_map_premium_battle_pass_name.find(bBattlePassId);
		if (it == m_map_premium_battle_pass_name.end())
			return false;

		std::string battlePassName = it->second;
		TPremiumMapBattleMissionInfo::iterator itInfo = m_map_premium_battle_pass_mission_info.find(battlePassName);

		if (itInfo == m_map_premium_battle_pass_mission_info.end())
			return false;

		std::vector<TExtBattlePassMissionInfo> missionInfo = itInfo->second;

		for (int i = 0; i < missionInfo.size(); i++)
		{
			if (missionInfo[i].bMissionIndex == bMissionIndex && missionInfo[i].bMissionType == bMissionType)
			{
				*dwFirstInfo = missionInfo[i].dwMissionInfo[0];
				*dwSecondInfo = missionInfo[i].dwMissionInfo[1];
				return true;
			}
		}
		return false;
	}
	else if (bBattlePassType == 3)
	{
		TMapEventBattlePassName::iterator it = m_map_event_battle_pass_name.find(bBattlePassId);
		if (it == m_map_event_battle_pass_name.end())
			return false;

		std::string battlePassName = it->second;
		TEventMapBattleMissionInfo::iterator itInfo = m_map_event_battle_pass_mission_info.find(battlePassName);

		if (itInfo == m_map_event_battle_pass_mission_info.end())
			return false;

		std::vector<TExtBattlePassMissionInfo> missionInfo = itInfo->second;

		for (int i = 0; i < missionInfo.size(); i++)
		{
			if (missionInfo[i].bMissionIndex == bMissionIndex && missionInfo[i].bMissionType == bMissionType)
			{
				*dwFirstInfo = missionInfo[i].dwMissionInfo[0];
				*dwSecondInfo = missionInfo[i].dwMissionInfo[1];
				return true;
			}
		}

		return false;
	}
	else
	{
		sys_err("Unknown BattlePassType (%d)", bBattlePassType);
		return false;
	}
}
#endif