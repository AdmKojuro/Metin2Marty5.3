#pragma once

#ifdef ENABLE_EXTENDED_BATTLE_PASS
#ifndef __INC_METIN_II_BATTLE_PASS_H__
#define __INC_METIN_II_BATTLE_PASS_H__

#include <boost/unordered_map.hpp>

#include "../../common/stl.h"
#include "../../common/length.h"
#include "../../common/tables.h"
#include "group_text_parse_tree.h"

#include "packet.h"

class CBattlePassManager : public singleton<CBattlePassManager>
{
	public:
		CBattlePassManager();
		virtual ~CBattlePassManager();

		bool InitializeBattlePass();
		
		bool ReadNormalBattlePassGroup();
		bool ReadNormalBattlePassMissions();
		bool ReadPremiumBattlePassGroup();
		bool ReadPremiumBattlePassMissions();
		bool ReadEventBattlePassGroup();
		bool ReadEventBattlePassMissions();
		
		void CheckBattlePassTimes();
		BYTE GetMissionIndex(BYTE bBattlePassType, BYTE bMissionType, DWORD dwCondition);
		BYTE GetMissionTypeByName(std::string stMissionName);
		BYTE GetMissionTypeByIndex(BYTE bBattlePassType, DWORD dwIndex);
		std::string GetMissionNameByType(BYTE bType);
		
		std::string GetNormalBattlePassNameByID(BYTE bID);
		std::string GetPremiumBattlePassNameByID(BYTE bID);
		std::string GetEventBattlePassNameByID(BYTE bID);
		
		void GetMissionSearchName(BYTE bMissionType, std::string*, std::string*);

		void BattlePassRequestOpen(LPCHARACTER pkChar, bool bAfterOpen = true);
		void BattlePassRewardMission(LPCHARACTER pkChar, BYTE bBattlePassType, DWORD bBattlePassId, BYTE bMissionIndex);
		bool BattlePassMissionGetInfo(BYTE bBattlePassType, BYTE bMissionIndex, BYTE bBattlePassId, BYTE bMissionType, DWORD* dwFirstInfo, DWORD* dwSecondInfo);

		void BattlePassRequestReward(LPCHARACTER pkChar, BYTE bBattlePassType);
		void BattlePassReward(LPCHARACTER pkChar, BYTE bBattlePassType, BYTE bBattlePassID);

		DWORD	GetNormalBattlePassID() 	const { return m_dwActiveNormalBattlePassID; }
		DWORD	GetPremiumBattlePassID()	const { return m_dwActivePremiumBattlePassID; }
		DWORD	GetEventBattlePassID()		const { return m_dwActiveEventBattlePassID; }
		
		DWORD	GetNormalBattlePassStartTime()		const { return m_dwNormalBattlePassStartTime; }
		DWORD	GetNormalBattlePassEndTime()		const { return m_dwNormalBattlePassEndTime; }
		DWORD	GetPremiumBattlePassStartTime()	const { return m_dwPremiumBattlePassStartTime; }
		DWORD	GetPremiumBattlePassEndTime()		const { return m_dwPremiumBattlePassEndTime; }
		DWORD	GetEventBattlePassStartTime()		const { return m_dwEventBattlePassStartTime; }
		DWORD	GetEventBattlePassEndTime()			const { return m_dwEventBattlePassEndTime; }
		
	protected:
		DWORD	m_dwNormalBattlePassStartTime;
		DWORD	m_dwNormalBattlePassEndTime;
		DWORD	m_dwPremiumBattlePassStartTime;
		DWORD	m_dwPremiumBattlePassEndTime;
		DWORD	m_dwEventBattlePassStartTime;
		DWORD	m_dwEventBattlePassEndTime;
		
		DWORD	m_dwActiveNormalBattlePassID;
		DWORD	m_dwActivePremiumBattlePassID;
		DWORD	m_dwActiveEventBattlePassID;

	private:
		CGroupTextParseTreeLoader* m_pNormalLoader;
		CGroupTextParseTreeLoader* m_pPremiumLoader;
		CGroupTextParseTreeLoader* m_pEventLoader;

		typedef std::map <BYTE, std::string> TMapNormalBattlePassName;
		typedef std::map <std::string, std::vector<TExtBattlePassRewardItem>> TNormalMapBattlePassReward;
		typedef std::map <std::string, std::vector<TExtBattlePassMissionInfo>> TNormalMapBattleMissionInfo;

		TMapNormalBattlePassName		m_map_normal_battle_pass_name;
		TNormalMapBattlePassReward		m_map_normal_battle_pass_reward;
		TNormalMapBattleMissionInfo		m_map_normal_battle_pass_mission_info;
		
		typedef std::map <BYTE, std::string> TMapPremiumBattlePassName;
		typedef std::map <std::string, std::vector<TExtBattlePassRewardItem>> TPremiumMapBattlePassReward;
		typedef std::map <std::string, std::vector<TExtBattlePassMissionInfo>> TPremiumMapBattleMissionInfo;

		TMapPremiumBattlePassName		m_map_premium_battle_pass_name;
		TPremiumMapBattlePassReward	m_map_premium_battle_pass_reward;
		TPremiumMapBattleMissionInfo	m_map_premium_battle_pass_mission_info;
		
		typedef std::map <BYTE, std::string> TMapEventBattlePassName;
		typedef std::map <std::string, std::vector<TExtBattlePassRewardItem>> TEventMapBattlePassReward;
		typedef std::map <std::string, std::vector<TExtBattlePassMissionInfo>> TEventMapBattleMissionInfo;
		
		TMapEventBattlePassName		m_map_event_battle_pass_name;
		TEventMapBattlePassReward		m_map_event_battle_pass_reward;
		TEventMapBattleMissionInfo		m_map_event_battle_pass_mission_info;
		
		typedef std::map <BYTE, std::vector<TExtBattlePassTimeTable>> TMapNormalBattlePassTimeTable;
		typedef std::map <BYTE, std::vector<TExtBattlePassTimeTable>> TMapPremiumBattlePassTimeTable;
		typedef std::map <BYTE, std::vector<TExtBattlePassTimeTable>> TMapEventBattlePassTimeTable;
		TMapNormalBattlePassTimeTable		m_map_normal_battle_pass_times;
		TMapPremiumBattlePassTimeTable	m_map_premium_battle_pass_times;
		TMapEventBattlePassTimeTable		m_map_event_battle_pass_times;
};

#endif
#endif