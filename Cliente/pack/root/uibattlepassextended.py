import os
import ui
import player
import mouseModule
import net
import app
import snd
import item
import chat
import grp
import time
import uiScriptLocale
import localeInfo
import constInfo
import ime
import pack
import wndMgr
import uiToolTip
import uiCommon
import uiPickMoney
import nonplayer

from datetime import datetime
from _weakref import proxy

PREMIUM_BATTLEPASS_ITEM = 93100
ORE_VNUM_MIN = 50601
ORE_VNUM_MAX = 50619
RANKING_MAX_NUM = 8

ROOT_PATH = "d:/ymir work/ui/public/battlepass/"
GAUGE_PATH = "d:/ymir work/ui/public/battlepass/gauges/"
RANKLIST_PATH = "d:/ymir work/ui/public/battlepass/ranklist/"

NORMAL_PATH = "d:/ymir work/ui/public/battlepass/normal/"
PREMIUM_PATH = "d:/ymir work/ui/public/battlepass/premium/"
EVENT_PATH = "d:/ymir work/ui/public/battlepass/event/"

class BattlePassWindow(ui.ScriptWindow):
	def __init__(self):
		self.lastUpdateBattlePass = 0
		self.page = "NORMAL"
		self.missionInfoDict = {}
		self.generalInfoDict = {}

		self.BattlePassIDNormal = -1
		self.BattlePassIDPremium = -1
		self.BattlePassIDEvent = -1
		
		self.next_normal_battlepass_time = 0
		self.next_premium_battlepass_time = 0
		self.next_event_battlepass_time = 0
		
		self.rankingFirstOpen = 0
		self.rankingRefreshLastTime = 0
		self.rankingInfoNormal = {}
		self.rankingInfoPremium = {}
		self.rankingInfoEvent = {}
		self.rankingItems = []
		self.ranklistNormalCurrentPage = 1
		self.ranklistPremiumCurrentPage = 1
		self.ranklistEventCurrentPage = 1

		self.rewardTable = [ [[], []], [[], []], [ [], [] ] ]
		self.rewardDict = {}

		self.battlePassGeneralInfo = { 1 : {}, 2 : {}, 3 : {} }
		self.rewardSlotIndex = 0
		
		self.scrollBarPosNormal = 0.0
		self.scrollBarPosPremium = 0.0
		self.scrollBarPosEvent = 0.0
		
		self.isActivePremiumWindow = 0
		self.lastUpdateTimeGauge = 0
		
		ui.ScriptWindow.__init__(self)
		self.__LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
		
	def Destroy(self):
		ui.ScriptWindow.__del__(self)

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/BattlePassExtendedWindow.py")
		except:
			import exception
			exception.Abort("uiBattlePassExtended.Open.BattlePassExtendedWindow.py")
	
		try:
			self.tooltipItem = uiToolTip.ItemToolTip()
			self.tooltipItem.Hide()
			
			self.tooltip = uiToolTip.ToolTip()
			self.tooltip.Hide()
			
			self.GetChild("board").SetCloseEvent(self.Close)	
			
			self.missionNameDict = {
				1 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_1, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_1_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_1_DESC_2},
				2 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_2, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_2_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_2_DESC_2},
				3 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_3, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_3_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_3_DESC_2},
				4 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_4, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_4_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_4_DESC_2},
				5 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_5, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_5_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_5_DESC_2},
				6 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_6, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_6_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_6_DESC_2},
				7 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_7, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_7_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_7_DESC_2},
				8 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_8, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_8_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_8_DESC_2},
				9 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_9, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_9_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_9_DESC_2},
				10 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_10_1, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_10_1_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_10_DESC_2},
				11 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_11, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_11_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_11_DESC_2},
				12 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_12, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_12_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_12_DESC_2},
				13 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_13, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_13_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_13_DESC_2},
				14 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_14, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_14_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_14_DESC_2},
				15 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_15, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_15_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_15_DESC_2},
				16 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_16, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_16_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_16_DESC_2},
				17 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_17, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_17_DESC_1, "desc_non_condition" : ""},
				18 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_18, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_18_DESC_1, "desc_non_condition" : ""},
				19 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_19, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_19_DESC_1, "desc_non_condition" : ""},
				20 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_20, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_20_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_20_DESC_2},
				21 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_21, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_21_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_21_DESC_2},
				22 : {"name" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_22, "desc" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_22_DESC_1, "desc_non_condition" : uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_22_DESC_2},
			}
			self.mapNameDict = {
				64 : uiScriptLocale.EXTENDET_BATTLE_PASS_MAP_NAME_BY_IDX_64,
				65 : uiScriptLocale.EXTENDET_BATTLE_PASS_MAP_NAME_BY_IDX_65,
				72 : uiScriptLocale.EXTENDET_BATTLE_PASS_MAP_NAME_BY_IDX_72,
			}
			self.guildwarTypeNameDict = {
				1 : uiScriptLocale.EXTENDET_BATTLE_PASS_GUILDWAR_TYPE_NAME_1,
				2 : uiScriptLocale.EXTENDET_BATTLE_PASS_GUILDWAR_TYPE_NAME_2,
				3 : uiScriptLocale.EXTENDET_BATTLE_PASS_GUILDWAR_TYPE_NAME_3,
			}
			self.dungeonNameDict = {
				1 : uiScriptLocale.EXTENDET_BATTLE_PASS_DUNGEON_NAME_1,
				2 : uiScriptLocale.EXTENDET_BATTLE_PASS_DUNGEON_NAME_2,
				3 : uiScriptLocale.EXTENDET_BATTLE_PASS_DUNGEON_NAME_3,
				4 : uiScriptLocale.EXTENDET_BATTLE_PASS_DUNGEON_NAME_4,
				5 : uiScriptLocale.EXTENDET_BATTLE_PASS_DUNGEON_NAME_5,
				6 : uiScriptLocale.EXTENDET_BATTLE_PASS_DUNGEON_NAME_6,
				7 : uiScriptLocale.EXTENDET_BATTLE_PASS_DUNGEON_NAME_7,
			}
			self.minigameNameDict = {
				1 : uiScriptLocale.EXTENDET_BATTLE_PASS_MINIGAME_NAME_1,
				2 : uiScriptLocale.EXTENDET_BATTLE_PASS_MINIGAME_NAME_2,
				3 : uiScriptLocale.EXTENDET_BATTLE_PASS_MINIGAME_NAME_3,
				4 : uiScriptLocale.EXTENDET_BATTLE_PASS_MINIGAME_NAME_4,
				5 : uiScriptLocale.EXTENDET_BATTLE_PASS_MINIGAME_NAME_5,
			}
			
			self.uiImageDict = {
				1 : NORMAL_PATH, 
				2 : PREMIUM_PATH, 
				3 : EVENT_PATH,
			}
			
			self.tabDict = {
				"NORMAL"	: self.GetChild("tab_normal"),
				"PREMIUM"	: self.GetChild("tab_premium"),
				"EVENT"	: self.GetChild("tab_event"),
			}
			
			self.tabButtonDict = {
				"NORMAL"	: self.GetChild("tab_button_normal"),
				"PREMIUM"	: self.GetChild("tab_button_premium"),
				"EVENT"	: self.GetChild("tab_button_event"),
			}
			
			self.tabWindowDict = {
				"NORMAL"	: self.GetChild("area_normal"),
				"PREMIUM"	: self.GetChild("area_premium"),
				"EVENT"	: self.GetChild("area_event"),
			}
				
			self.subTabDict = {
				"MISSIONS"	: self.GetChild("tab_missions"),
				"GENERAL"	: self.GetChild("tab_general"),
			}
			
			self.subTabButtonDict = {
				"MISSIONS"	: self.GetChild("tab_button_missions"),
				"GENERAL"	: self.GetChild("tab_button_general"),
			}
			
			self.infoPagesList = [
				self.GetChild("NormalInfoBoard"),
				self.GetChild("NormalTextInfo"),
				self.GetChild("PremiumInfoBoard"),
				self.GetChild("InputTicketBoard"),
				self.GetChild("PremiumTextInfo"),
				self.GetChild("EventInfoBoard"),
				self.GetChild("PremiumTextInfo")
			]
			
			for i in xrange(len(self.infoPagesList)):
				self.infoPagesList[i].Hide()

			for tabValue in self.tabDict.itervalues():
				tabValue.Hide()

			for (stateKey, tabButton) in self.tabButtonDict.items():
				tabButton.SetEvent(ui.__mem_func__(self.__OnClickTabButton), stateKey)
				
			for (subStateKey, subTabButton) in self.subTabButtonDict.items():
				subTabButton.SetEvent(ui.__mem_func__(self.__OnClickSubTabButton), subStateKey)
				
			self.GetChild("BorderRanking").Hide()
			self.GetChild("RankingButton").SetEvent(ui.__mem_func__(self.__OnClickRankingButton))
			self.GetChild("RewardButton").SetEvent(ui.__mem_func__(self.__OnClickRewardButton))

			self.GetChild("reward_slots").SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			self.GetChild("reward_slots").SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

			self.GetChild("ranking_title_icon_rank").SetEvent(ui.__mem_func__(self.RanklistIconTooltip), "mouse_over_in", 1)
			self.GetChild("ranking_title_icon_rank").SetEvent(ui.__mem_func__(self.RanklistIconTooltip), "mouse_over_out", 1)
			self.GetChild("ranking_title_icon_name").SetEvent(ui.__mem_func__(self.RanklistIconTooltip), "mouse_over_in", 2)
			self.GetChild("ranking_title_icon_name").SetEvent(ui.__mem_func__(self.RanklistIconTooltip), "mouse_over_out", 2)
			self.GetChild("ranking_title_icon_time").SetEvent(ui.__mem_func__(self.RanklistIconTooltip), "mouse_over_in", 3)
			self.GetChild("ranking_title_icon_time").SetEvent(ui.__mem_func__(self.RanklistIconTooltip), "mouse_over_out", 3)
			
			self.GetChild("ticket_slot").SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlotSrc))
			self.GetChild("ticket_slot").SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlotSrc))

			self.GetChild("prev_page_button").SetEvent(ui.__mem_func__(self.__OnClickPrevRankingPage))
			self.GetChild("next_page_button").SetEvent(ui.__mem_func__(self.__OnClickNextRankingPage))
			self.GetChild("first_page_button").SetEvent(ui.__mem_func__(self.__OnClickRankingPageButton), 1)
			self.GetChild("last_page_button").SetEvent(ui.__mem_func__(self.__OnClickRankingPageButton), 5)
			self.GetChild("refresh_ranklist_button").SetEvent(ui.__mem_func__(self.__OnClickRefreshRanklist))

			for i in xrange(1, 6):
				self.GetChild("page%d_button" % int(i)).SetEvent(ui.__mem_func__(self.__OnClickRankingPageButton), i)
				
			self.scrollBar = MissionScrollBar()
			self.scrollBar.SetParent(self.GetChild("BorderScroll"))
			self.scrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
			self.scrollBar.SetUpVisual(NORMAL_PATH + "scrollbar.sub")
			self.scrollBar.SetOverVisual(NORMAL_PATH + "scrollbar.sub")
			self.scrollBar.SetDownVisual(NORMAL_PATH + "scrollbar.sub")
			self.scrollBar.SetRestrictMovementArea(12, 2, 6, 249)
			self.scrollBar.SetPosition(12, 2)
			self.scrollBar.Show()
		
			self.missionList = ListBoxMissions()
			self.missionList.SetParent(self.GetChild("BorderMissions"))
			self.missionList.SetGlobalParent(self)
			self.missionList.SetPosition(4, 4)
			self.missionList.SetSize(300, 249)
			self.missionList.Show()
			self.missionList.ShowMissionsByBattlePassType(1)
			self.SetPage("NORMAL")
			self.SetSubPage("MISSIONS")
			self.__OnClickRankingPageButton(1)
		except:
			import exception
			exception.Abort("uiBattlePassExtended.LoadWindow.BindObject")

	def __OnClickTabButton(self, stateKey):
		self.SetPage(stateKey)

	def __OnClickSubTabButton(self, subStateKey):
		self.SetSubPage(subStateKey)

	def __OnClickRewardButton(self):
		if self.page == "NORMAL":
			net.SendExtBattlePassAction(10)
		if self.page == "PREMIUM":
			net.SendExtBattlePassAction(11)
		if self.page == "EVENT":
			net.SendExtBattlePassAction(12)
		
	def __OnClickRankingButton(self):
		if self.GetChild("BorderRanking").IsShow():
			self.GetChild("BorderRanking").Hide()
		else:
			if self.rankingFirstOpen == 0:
				self.rankingFirstOpen = 1
				net.SendExtBattlePassAction(2)
				self.rankingRefreshLastTime = app.GetTime() + 10
			self.GetChild("BorderRanking").Show()
		
	def __OnClickRankingPageButton(self, pageIndex):
		new_page = min(5, pageIndex)
		new_page = max(1, new_page)
		for i in xrange(1, 6):
			if i != new_page:
				self.GetChild("page%d_button" % int(i)).SetUp()
			else:
				self.GetChild("page%d_button" % int(i)).Down()

		if self.page == "NORMAL":
			self.ranklistNormalCurrentPage = new_page
		if self.page == "PREMIUM":
			self.ranklistPremiumCurrentPage = new_page
		if self.page == "EVENT":
			self.ranklistEventCurrentPage = new_page
		self.RefreshRanklist()

	def __OnClickNextRankingPage(self):
		if self.page == "NORMAL":
			self.__OnClickRankingPageButton(self.ranklistNormalCurrentPage + 1)
		if self.page == "PREMIUM":
			self.__OnClickRankingPageButton(self.ranklistPremiumCurrentPage + 1)
		if self.page == "EVENT":
			self.__OnClickRankingPageButton(self.ranklistCurrentPage + 1)
	
	def __OnClickPrevRankingPage(self):
		if self.page == "NORMAL":
			self.__OnClickRankingPageButton(self.ranklistNormalCurrentPage - 1)
		if self.page == "PREMIUM":
			self.__OnClickRankingPageButton(self.ranklistPremiumCurrentPage - 1)
		if self.page == "EVENT":
			self.__OnClickRankingPageButton(self.ranklistEventCurrentPage - 1)
	
	def __OnClickRefreshRanklist(self):
		if self.rankingRefreshLastTime < app.GetTime():
			self.rankingRefreshLastTime = app.GetTime() + 11
			self.rankingInfoNormal = {}
			self.rankingInfoPremium = {}
			self.rankingInfoEvent = {}
			net.SendExtBattlePassAction(2)
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXTENDET_BATTLE_PASS_RANKLIST_REFRESH)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXTENDET_BATTLE_PASS_RANKLIST_WAIT_TO_REFRESH % int((self.rankingRefreshLastTime - app.GetTime()) + 1))
	
	def SetPage(self, stateKey, isClear = False):
		if self.page == "NORMAL" and not isClear:
			self.scrollBarPosNormal = self.scrollBar.GetPos()
		if self.page == "PREMIUM" and not isClear:
			self.scrollBarPosPremium = self.scrollBar.GetPos()
		if self.page == "EVENT" and not isClear:
			self.scrollBarPosEvent = self.scrollBar.GetPos()
		
		self.page = stateKey

		for (tabKey, tabButton) in self.tabButtonDict.items():
			if stateKey!=tabKey:
				tabButton.SetUp()

		for (tabKey, tabWindow) in self.tabWindowDict.items():
			if stateKey==tabKey:
				tabWindow.Show()
			else:
				tabWindow.Hide()

		for tabValue in self.tabDict.itervalues():
			tabValue.Hide()

		self.GetChild("UIArea").Show()
		for i in xrange(len(self.infoPagesList)):
			self.infoPagesList[i].Hide()
	
		self.tabDict[stateKey].Show()

		if stateKey == "NORMAL":
			self.SetNormalPage()
			self.missionList.ShowMissionsByBattlePassType(1)
			self.scrollBar.SetPos(self.scrollBarPosNormal)
			self.__OnClickRankingPageButton(self.ranklistNormalCurrentPage)
			self.SetGeneralInfo()
		if stateKey == "PREMIUM":
			self.SetPremiumPage()
			self.missionList.ShowMissionsByBattlePassType(2)
			self.scrollBar.SetPos(self.scrollBarPosPremium)
			self.__OnClickRankingPageButton(self.ranklistPremiumCurrentPage)
			self.SetGeneralInfo()
		if stateKey == "EVENT":
			self.SetEventPage()
			self.missionList.ShowMissionsByBattlePassType(3)
			self.scrollBar.SetPos(self.scrollBarPosEvent)
			self.__OnClickRankingPageButton(self.ranklistEventCurrentPage)
			self.SetGeneralInfo()

		self.missionList.SelectMissionByPage()
		self.RefreshRewardSlots()
		self.RefreshRanklist()
	
	def GetPage(self):
		return self.page
	
	def SetSubPage(self, subStateKey):
		self.subpage = subStateKey
		
		for (subTabKey, subTabButton) in self.subTabButtonDict.items():
			if subStateKey!=subTabKey:
				subTabButton.SetUp()
	
		for subTabValue in self.subTabDict.itervalues():
			subTabValue.Hide()

		self.subTabDict[subStateKey].Show()
		
		if subStateKey == "MISSIONS":
			self.GetChild("BorderInfoGeneral").Hide()
			self.GetChild("BorderInfoMission").Show()
			if self.GetChild("BorderRanking").IsShow():
				self.GetChild("BorderRanking").Hide()
		if subStateKey == "GENERAL":
			self.GetChild("BorderInfoMission").Hide()
			self.GetChild("BorderInfoGeneral").Show()
			self.SetGeneralInfo()
			self.RefreshRewardSlots()

	def SetNormalPage(self):
		if self.battlePassGeneralInfo[1].has_key(self.BattlePassIDNormal) and self.battlePassGeneralInfo[1][self.BattlePassIDNormal]["start_time"] < app.GetGlobalTimeStamp() and self.battlePassGeneralInfo[1][self.BattlePassIDNormal]["end_time"] > app.GetGlobalTimeStamp():
			self.GetChild("UIArea").Show()
			self.GetChild("NormalInfoBoard").Hide()
			self.GetChild("NormalTextInfo").Hide()
			self.__ChangeUI(1)
		else:
			self.GetChild("UIArea").Hide()
			self.GetChild("NormalInfoBoard").Show()
			self.GetChild("NormalTextInfo").Show()
			self.GetChild("NormalInfoTextTime").Hide()
			for battlepass in self.battlePassGeneralInfo[1].itervalues():
				if battlepass["start_time"] > app.GetGlobalTimeStamp():
					self.next_normal_battlepass_time = battlepass["start_time"]
					break
			if self.next_normal_battlepass_time != 0:
				self.GetChild("NormalInfoText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_PLANNED_NEXT_NORMAL_BATTLEPASS)
				self.GetChild("NormalInfoText").SetPosition(0, -17)
				self.GetChild("NormalInfoTextTime").Show()
				self.GetChild("NormalInfoTextTime").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_TIME_TO_NEXT_NORMAL_BATTLEPASS % localeInfo.SecondToDHM(self.next_normal_battlepass_time - app.GetGlobalTimeStamp()))
			else:
				self.GetChild("NormalInfoText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_NO_PLANNED_NORMAL_BATTLEPASS)
				self.GetChild("NormalInfoText").SetPosition(0, -2)
	
	def SetPremiumPage(self):
		self.isActivePremiumWindow = 0
		if self.battlePassGeneralInfo[2].has_key(self.BattlePassIDPremium) and self.battlePassGeneralInfo[2][self.BattlePassIDPremium]["start_time"] < app.GetGlobalTimeStamp() and self.battlePassGeneralInfo[2][self.BattlePassIDPremium]["end_time"] > app.GetGlobalTimeStamp():
			if player.GetPremiumBattlePassID() != self.BattlePassIDPremium:
				self.GetChild("UIArea").Hide()
				self.GetChild("PremiumInfoBoard").Show()
				self.GetChild("InputTicketBoard").Show()
				self.GetChild("ActivateTicketButton").Hide()
				self.GetChild("ActivateTicketButton").SetEvent(ui.__mem_func__(self.__OnClickSendPremiumTicket))
				self.GetChild("ticket_slot").ClearSlot(0)
				self.slotItemPos = -1
			else:
				self.GetChild("UIArea").Show()
				self.GetChild("PremiumInfoBoard").Hide()
				self.GetChild("PremiumTextInfo").Hide()
				self.GetChild("InputTicketBoard").Hide()
				self.isActivePremiumWindow = 1
				self.__ChangeUI(2)
		else:
			self.GetChild("UIArea").Hide()
			self.GetChild("PremiumInfoBoard").Show()
			self.GetChild("PremiumTextInfo").Show()
			self.GetChild("PremiumInfoTextTime").Hide()
			for battlepass in self.battlePassGeneralInfo[2].itervalues():
				if battlepass["start_time"] > app.GetGlobalTimeStamp():
					self.next_premium_battlepass_time = battlepass["start_time"]
					break
			if self.next_premium_battlepass_time != 0:
				self.GetChild("PremiumInfoText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_PLANNED_NEXT_PREMIUM_BATTLEPASS)
				self.GetChild("PremiumInfoText").SetPosition(0, -17)
				self.GetChild("PremiumInfoTextTime").Show()
				self.GetChild("PremiumInfoTextTime").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_TIME_TO_NEXT_PREMIUM_BATTLEPASS % localeInfo.SecondToDHM(self.next_premium_battlepass_time - app.GetGlobalTimeStamp()))
			else:
				self.GetChild("PremiumInfoText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_NO_PLANNED_PREMIUM_BATTLEPASS)
				self.GetChild("PremiumInfoText").SetPosition(0, -2)
	
	def SetEventPage(self):
		if self.battlePassGeneralInfo[3].has_key(self.BattlePassIDEvent) and self.battlePassGeneralInfo[3][self.BattlePassIDEvent]["start_time"] < app.GetGlobalTimeStamp() and self.battlePassGeneralInfo[3][self.BattlePassIDEvent]["end_time"] > app.GetGlobalTimeStamp():
			self.GetChild("UIArea").Show()
			self.GetChild("EventInfoBoard").Hide()
			self.GetChild("EventTextInfo").Hide()
			self.__ChangeUI(3)
		else:
			self.GetChild("UIArea").Hide()
			self.GetChild("EventInfoBoard").Show()
			self.GetChild("EventTextInfo").Show()
			self.GetChild("EventInfoTextTime").Hide()
			for battlepass in self.battlePassGeneralInfo[3].itervalues():
				if battlepass["start_time"] > app.GetGlobalTimeStamp():
					self.next_event_battlepass_time = battlepass["start_time"]
					break
			if self.next_event_battlepass_time != 0:
				self.GetChild("EventInfoText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_PLANNED_NEXT_EVENT_BATTLEPASS)
				self.GetChild("EventInfoText").SetPosition(0, -17)
				self.GetChild("EventInfoTextTime").Show()
				self.GetChild("EventInfoTextTime").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_TIME_TO_NEXT_EVENT_BATTLEPASS % localeInfo.SecondToDHM(self.next_event_battlepass_time - app.GetGlobalTimeStamp()))
			else:
				self.GetChild("EventInfoText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_NO_PLANNED_EVENT_BATTLEPASS)
				self.GetChild("EventInfoText").SetPosition(0, -2)

	def SetGeneralInfo(self):
		missionCount = self.missionList.GetMissionCount()
		completedMissionCount = self.missionList.GetCompletedMissionCount()
		battlepasstype = self.GetShowBattlePassType()
		battlepassid = self.GetActualBattlePassIDByType()
		
		if battlepassid != -1:
			self.GetChild("GeneralTitleText").SetText(self.battlePassGeneralInfo[battlepasstype][battlepassid]["name"])

			self.GetChild("GeneralInfoStartDateText").SetText(datetime.fromtimestamp(self.battlePassGeneralInfo[battlepasstype][battlepassid]["start_time"]).strftime('%d.%m.%Y %H:%M:%S'))
			self.GetChild("GeneralInfoEndDateText").SetText(datetime.fromtimestamp(self.battlePassGeneralInfo[battlepasstype][battlepassid]["end_time"]).strftime('%d.%m.%Y %H:%M:%S'))
			#self.GetChild("GeneralInfoTotalMissionsText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_SEASON_TOTAL_MISSIONS % missionCount)
			self.GetChild("GeneralInfoFinishMissionsText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_SEASON_CURRENT_MISSIONS % (completedMissionCount, missionCount))

			self.GetChild("GaugeMission").SetPercentage(completedMissionCount, missionCount)
			self.UpdateTimeGauge()
				
	def SetMissionInfo(self, mission_index, mission_type):
		(mission_name, mission_condition, actual_value, total_value) = self.missionList.GetMissionInfo(mission_index)

		self.GetChild("MissionInfoTitle").SetText(mission_name)
		
		if actual_value >= total_value:
			self.GetChild("MissionStatusText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_STATUS +  uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_STATUS_FINISH)
		else:
			self.GetChild("MissionStatusText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_STATUS +  uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_STATUS_IN_PROGRESS)

		if mission_type != 17 and mission_condition != 0 or mission_type != 18 and mission_condition != 0 or mission_type != 19 and mission_condition != 0: 
			self.GetChild("MissionInformationText1").Show()
			self.GetChild("MissionInformationText1").SetText(self.GetMissionConditionText(mission_type, mission_condition))
		else:
			self.GetChild("MissionInformationText1").Hide()

		self.GetChild("MissionInformationText2").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_COUNT % (localeInfo.NumberWithPoint(actual_value), localeInfo.NumberWithPoint(total_value)))
		if actual_value == 0:
			self.GetChild("MissionInformationText3").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_PERCENT % (float(0.00)))
		else:
			self.GetChild("MissionInformationText3").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_PERCENT % (float(actual_value) / float(total_value) * 100.0))
		
		self.SetMissionDescText(mission_type, mission_condition, total_value)

	def GetTitleTextInfo(self, mission_type, mission_condition, total_value):
		if mission_condition != 0:
			if mission_type in [1]:
				return nonplayer.GetMonsterName(mission_condition)
			if mission_type in [5, 6, 7, 8, 9, 10, 11, 12, 13]:
				item.SelectItem(mission_condition)
				return item.GetItemName()
			if mission_type in [2, 14, 15, 18, 20]:
				return localeInfo.NumberWithPoint(total_value)
			if mission_type in [21]:
				return self.dungeonNameDict[mission_condition]
			if mission_type in [22]:
				return self.minigameNameDict[mission_condition]
		else:
			if mission_type in [1]:
				return uiScriptLocale.EXTENDET_BATTLE_PASS_NONE_COND_TITLE_MOB
			if mission_type in [5, 6, 7, 8, 9, 10]:
				return uiScriptLocale.EXTENDET_BATTLE_PASS_NONE_COND_TITLE_ITEM
			if mission_type in [11, 12, 13]:
				return uiScriptLocale.EXTENDET_BATTLE_PASS_NONE_COND_TITLE_FISH
			if mission_type in [2, 14, 15, 18, 20]:
				return localeInfo.NumberWithPoint(total_value)
			if mission_type in [21]:
				return uiScriptLocale.EXTENDET_BATTLE_PASS_NONE_COND_TITLE_DUNGEON
			if mission_type in [22]:
				return uiScriptLocale.EXTENDET_BATTLE_PASS_NONE_COND_TITLE_MINIGAME

	def GetMissionConditionText(self, mission_type, mission_condition):
		if mission_type in [1, 3, 14]:
			return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_MONSTER % nonplayer.GetMonsterName(mission_condition))
		if mission_type in [2, 4]:
			return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_MINLEVEL % str(mission_condition))
		if mission_type in [5, 6, 7, 8, 9, 10]:
			item.SelectItem(mission_condition)
			if mission_type == 10 and mission_condition >= ORE_VNUM_MIN and mission_condition <= ORE_VNUM_MAX:
				return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_ORE % item.GetItemName())
			return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_ITEM % item.GetItemName())
		if mission_type in [11, 12, 13]:
			item.SelectItem(mission_condition)
			return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_FISH % item.GetItemName())
		if mission_type in [15]:
			return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_MAPNAME % (self.mapNameDict[mission_condition]))
		if mission_type in [16]:
			return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_GUILDWARTYPE % (self.guildwarTypeNameDict[mission_condition]))
		if mission_type in [20]:
			return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_PET_MINTYPE % str(mission_condition))
		if mission_type in [21]:
			return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_DUNGEON % (self.dungeonNameDict[mission_condition]))
		if mission_type in [22]:
			return (uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFORMATION_MINIGAME % (self.minigameNameDict[mission_condition]))
	
	def SetMissionDescText(self, mission_type, mission_condition, total_value):
		value = localeInfo.NumberWithPoint(total_value)
		text = ""
		if mission_condition != 0:
			if mission_type in [1 ,3, 14]:
				text = (self.missionNameDict[mission_type]["desc"] % (value, nonplayer.GetMonsterName(mission_condition)))
			if mission_type in [2 ,4]:
				text = (self.missionNameDict[mission_type]["desc"] % (value, mission_condition))
			if mission_type in [5, 6, 7, 8, 9, 10, 11, 12, 13]:
				item.SelectItem(mission_condition)
				if mission_type == 10 and mission_condition >= ORE_VNUM_MIN and mission_condition <= ORE_VNUM_MAX:
					text = (uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_10_2_DESC_1 % (value, item.GetItemName()))
				else:
					text = (self.missionNameDict[mission_type]["desc"] % (value, item.GetItemName()))
			if mission_type in [15]:
				text = (self.missionNameDict[mission_type]["desc"] % (value, self.mapNameDict[mission_condition]))
			if mission_type in [16]:
				text = (self.missionNameDict[mission_type]["desc"] % (value, self.guildwarTypeNameDict[mission_condition]))
			if mission_type in [20]:
				text = (self.missionNameDict[mission_type]["desc"] % (value, str(mission_condition)))
			if mission_type in [21]:
				text = (self.missionNameDict[mission_type]["desc"] % (self.dungeonNameDict[mission_condition], value))
			if mission_type in [22]:
				text = (self.missionNameDict[mission_type]["desc"] % (value, self.minigameNameDict[mission_condition]))
		else:
			if mission_type in [17, 18, 19]:
				text = (self.missionNameDict[mission_type]["desc"] % (value))
#			else:
#				text = (self.missionNameDict[mission_type]["desc_non_condition"] % (value))
			
		self.SetDesc(text, self.GetChild("bgImageMission"))
	
	def GetShowBattlePassType(self):
		if self.page == "NORMAL":
			return 1
		if self.page == "PREMIUM":
			return 2
		if self.page == "EVENT":
			return 3
		return 0
	
	def GetActualBattlePassIDByType(self):
		if self.page == "NORMAL":
			return self.BattlePassIDNormal
		if self.page == "PREMIUM":
			return self.BattlePassIDPremium
		if self.page == "EVENT":
			return self.BattlePassIDEvent
		return -1
	
	def RefreshRewardSlots(self):
		if self.page == "NORMAL":
			reward_items = self.rewardTable[0]
		if self.page == "PREMIUM":
			reward_items = self.rewardTable[1]
		if self.page == "EVENT":
			reward_items = self.rewardTable[2]
			
		if len(reward_items[0]) != 0:
			self.rewardSlotIndex = 0
			self.rewardDict = {}
			for slot in xrange(6):
				self.GetChild("reward_slots").ClearSlot(slot)
			for i in xrange(len(reward_items[0])):
				self.GetChild("reward_slots").SetItemSlot(self.rewardSlotIndex, reward_items[0][i], reward_items[1][i])
				self.rewardDict[self.rewardSlotIndex] = [reward_items[0][i], reward_items[1][i]]
				item.SelectItem(reward_items[0][i])
				self.rewardSlotIndex += 1 if item.GetItemSize()[1] == 1 else 2

	def RecvGeneralInfo(self, BattlePassType, BattlePassName, BattlePassID, battlePassStartTime, battlePassEndTime):
		if not self.battlePassGeneralInfo[BattlePassType].has_key(BattlePassID):
			self.battlePassGeneralInfo[BattlePassType][BattlePassID] = {"name" : str(BattlePassName), "start_time" : battlePassStartTime, "end_time" : battlePassEndTime}
			if battlePassStartTime < app.GetGlobalTimeStamp() and battlePassEndTime > app.GetGlobalTimeStamp():
				if BattlePassType == 1:
					self.BattlePassIDNormal = BattlePassID
				if BattlePassType == 2:
					self.BattlePassIDPremium = BattlePassID
				if BattlePassType == 3:
					self.BattlePassIDEvent = BattlePassID

	def AddMission(self, battlepass_type, battlepass_id, mission_index, mission_type, info_value, current_value, total_value):
		if self.battlePassGeneralInfo[battlepass_type].has_key(battlepass_id) and self.battlePassGeneralInfo[battlepass_type][battlepass_id]["start_time"] < app.GetGlobalTimeStamp() and self.battlePassGeneralInfo[battlepass_type][battlepass_id]["end_time"] > app.GetGlobalTimeStamp():
			if self.missionList.HaveMission(battlepass_type, mission_index, mission_type):
				self.missionList.SetProgress(battlepass_type, mission_index, total_value, current_value)
			else:
				textInfo = self.GetTitleTextInfo(mission_type, info_value, current_value)
				
				if self.missionNameDict.has_key(mission_type):
					if mission_type in [3, 4, 16, 17, 19]:
						missionName = self.missionNameDict[mission_type]["name"] 
					else:
						if mission_type == 10 and info_value >= ORE_VNUM_MIN and info_value <= ORE_VNUM_MAX:
							missionName = (uiScriptLocale.EXTENDET_BATTLE_PASS_TYPENAME_10_2 % textInfo)
						else:
							missionName = (self.missionNameDict[mission_type]["name"] % textInfo)
				else:
					missionName = "Unknown name"
					
				self.missionList.AppendMission(47, battlepass_type, battlepass_id, mission_index, mission_type, missionName, info_value)
				self.missionList.SetProgress(battlepass_type, mission_index, total_value, current_value)

	def AddMissionReward(self, battlepass_type, battlepass_id, missionIndex, missionType, itemVnum, itemCount):
		if self.battlePassGeneralInfo[battlepass_type].has_key(battlepass_id) and self.battlePassGeneralInfo[battlepass_type][battlepass_id]["start_time"] < app.GetGlobalTimeStamp() and self.battlePassGeneralInfo[battlepass_type][battlepass_id]["end_time"] > app.GetGlobalTimeStamp():
			if self.missionList:
				self.missionList.AddMissionReward(battlepass_type, missionIndex, itemVnum, itemCount)
	
	def UpdateMission(self, battlepass_type, missionIndex, mission_type, current_value):
		if self.missionList.HaveMission(battlepass_type, missionIndex, mission_type):
			self.missionList.UpdateProgress(battlepass_type, missionIndex, current_value)
			if self.missionList.GetSelectedMission() == missionIndex:
				self.SetMissionInfo(missionIndex, mission_type)
			#self.RefreshGeneralInfo()

	def AddReward(self, battlepass_type, battlepass_id, itemVnum, itemCount):
		if self.battlePassGeneralInfo[battlepass_type].has_key(battlepass_id) and self.battlePassGeneralInfo[battlepass_type][battlepass_id]["start_time"] < app.GetGlobalTimeStamp() and self.battlePassGeneralInfo[battlepass_type][battlepass_id]["end_time"] > app.GetGlobalTimeStamp():
			self.rewardTable[battlepass_type-1][0].append(itemVnum)
			self.rewardTable[battlepass_type-1][1].append(itemCount)

	def AddRankingEntry(self, playername, battlepassType, battlepassID, startTime, endTime):
		if battlepassType == 1:
			self.rankingInfoNormal[len(self.rankingInfoNormal)] = { "playername" : playername, "battlepass_id" : battlepassID, "starttime" : startTime, "endtime" : endTime }
		if battlepassType == 2:
			self.rankingInfoPremium[len(self.rankingInfoPremium)] = { "playername" : playername, "battlepass_id" : battlepassID, "starttime" : startTime, "endtime" : endTime }
		if battlepassType == 3:
			self.rankingInfoEvent[len(self.rankingInfoEvent)] = { "playername" : playername, "battlepass_id" : battlepassID, "starttime" : startTime, "endtime" : endTime }
		self.RefreshRanklist()
	
	def RefreshRanklist(self):
		pos_y = 27
		self.rankingItems = []
		for i in xrange(RANKING_MAX_NUM):
	
			if self.page == "NORMAL":
				rank_range = i + (self.ranklistNormalCurrentPage*RANKING_MAX_NUM) - RANKING_MAX_NUM + 1
				slot_image = NORMAL_PATH + "ranklist_item.sub"
				info_list = self.rankingInfoNormal
			if self.page == "PREMIUM":
				rank_range = i + (self.ranklistPremiumCurrentPage*RANKING_MAX_NUM) - RANKING_MAX_NUM + 1
				slot_image = PREMIUM_PATH + "ranklist_item.sub"
				info_list = self.rankingInfoPremium
			if self.page == "EVENT":
				rank_range = i + (self.ranklistEventCurrentPage*RANKING_MAX_NUM) - RANKING_MAX_NUM + 1
				slot_image = EVENT_PATH + "ranklist_item.sub"
				info_list = self.rankingInfoEvent
			
			if info_list.has_key(rank_range-1):
				image = ui.ImageBox()
				image.SetParent(self.GetChild("BorderRanking"))
				image.SetPosition(3, pos_y)
				image.LoadImage(slot_image)
				image.Show()

				rankText = ui.TextLine()
				rankText.SetParent(image)
				rankText.SetHorizontalAlignCenter()
				rankText.SetPosition(21,4)
				rankText.SetText(str(rank_range))
				rankText.AddFlag("not_pick")
				rankText.Show()
				
				nameText = ui.TextLine()
				nameText.SetParent(image)
				nameText.SetHorizontalAlignCenter()
				nameText.SetPosition(107,4)
				nameText.SetText(str(info_list[rank_range-1]["playername"]))
				nameText.AddFlag("not_pick")
				nameText.Show()
				
				timeText = ui.TextLine()
				timeText.SetParent(image)
				timeText.SetHorizontalAlignCenter()
				timeText.SetPosition(236,4)
				timeText.SetText(localeInfo.SecondToDHM(info_list[rank_range-1]["endtime"] - info_list[rank_range-1]["starttime"]))
				timeText.AddFlag("not_pick")
				timeText.Show()

				self.rankingItems.append([])
				self.rankingItems[i].append(image)
				self.rankingItems[i].append(rankText)
				self.rankingItems[i].append(nameText)
				self.rankingItems[i].append(timeText)
				pos_y += 25
	
	def __ChangeUI(self, battlepass_type):
		self.GetChild("bgImageMission").LoadImage(self.uiImageDict[battlepass_type] + "mission_info_background.tga")
		self.GetChild("bgImageGeneral").LoadImage(self.uiImageDict[battlepass_type] + "mission_info_background.tga")
		self.GetChild("RankingButton").SetUpVisual(self.uiImageDict[battlepass_type] + "button_ranklist_normal.sub")
		self.GetChild("RankingButton").SetOverVisual(self.uiImageDict[battlepass_type] + "button_ranklist_hover.sub")
		self.GetChild("RankingButton").SetDownVisual(self.uiImageDict[battlepass_type] + "button_ranklist_down.sub")
		self.GetChild("RewardButton").SetUpVisual(self.uiImageDict[battlepass_type] + "button_recive_reward_normal.sub")
		self.GetChild("RewardButton").SetOverVisual(self.uiImageDict[battlepass_type] + "button_recive_reward_hover.sub")
		self.GetChild("RewardButton").SetDownVisual(self.uiImageDict[battlepass_type] + "button_recive_reward_down.sub")
		self.GetChild("reward_slot_background").LoadImage(self.uiImageDict[battlepass_type] + "reward_slots.sub")
		self.scrollBar.SetUpVisual(self.uiImageDict[battlepass_type] + "scrollbar.sub")
		self.scrollBar.SetOverVisual(self.uiImageDict[battlepass_type] + "scrollbar.sub")
		self.scrollBar.SetDownVisual(self.uiImageDict[battlepass_type] + "scrollbar.sub")
		self.GetChild("RankingTitle").LoadImage(self.uiImageDict[battlepass_type] + "ranklist_titlebar.sub")
	
	def __ClearAll(self):
		if self.missionList:
			self.missionList.ClearAll()
		self.missionInfoDict = {}
		self.generalInfoDict = {}

		self.BattlePassIDNormal = -1
		self.BattlePassIDPremium = -1
		self.BattlePassIDEvent = -1
		
		self.next_normal_battlepass_time = 0
		self.next_premium_battlepass_time = 0
		self.next_event_battlepass_time = 0
		
		self.rankingFirstOpen = 0
		self.rankingInfoNormal = {}
		self.rankingInfoPremium = {}
		self.rankingInfoEvent = {}
		self.rankingItems = []
		self.ranklistNormalCurrentPage = 1
		self.ranklistPremiumCurrentPage = 1
		self.ranklistEventCurrentPage = 1

		self.rewardTable = [ [[], []], [[], []], [ [], [] ] ]
		self.rewardDict = {}

		self.battlePassGeneralInfo = { 1 : {}, 2 : {}, 3 : {} }
		self.rewardSlotIndex = 0
		
		self.scrollBarPosNormal = 0.0
		self.scrollBarPosPremium = 0.0
		self.scrollBarPosEvent = 0.0
	
	def __OnClickSendPremiumTicket(self):
		if self.slotItemPos != -1:
			net.SendExtBattlePassPremiumItem(self.slotItemPos)
			self.slotItemPos = -1
			self.GetChild("ActivateTicketButton").Hide()
			self.GetChild("ticket_slot").ClearSlot(0)
	
	def SelectEmptySlotSrc(self, selectedSlotPos):	
		if mouseModule.mouseController.isAttached():
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()
			attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
			selectedItemVNum = player.GetItemIndex(attachedSlotPos)

			item.SelectItem(selectedItemVNum)
			
			if selectedItemVNum != PREMIUM_BATTLEPASS_ITEM:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXTENDET_BATTLE_PASS_ITS_NOT_BATTLEPASS_ITEM)
				mouseModule.mouseController.DeattachObject()
				return

			self.GetChild("ticket_slot").SetItemSlot(selectedSlotPos, attachedItemIndex, 0)
			snd.PlaySound("sound/ui/drop.wav")
			self.slotItemPos = attachedSlotPos
			self.GetChild("ActivateTicketButton").Show()
					
			mouseModule.mouseController.DeattachObject()
			return
	
	def SelectItemSlotSrc(self, itemSlotIndex):
		self.GetChild("ActivateTicketButton").Hide()
		snd.PlaySound("sound/ui/pickup_item_in_inventory.wav")
		self.GetChild("ticket_slot").ClearSlot(0)
		self.GetChild("ticket_slot").RefreshSlot()
		
	def SetDesc(self, desc, parent):
		lines = SplitDescription(desc, 40)
		if not lines:
			return
		
		self.childrenList = []
		self.toolTipHeight = 165
		for line in lines:
			if len(self.childrenList) >= 4:
				return
				
			textLine = ui.TextLine()
			textLine.SetParent(parent)
			textLine.SetText(line)
			textLine.SetOutline()
			textLine.Show()

			textLine.SetPosition(4, self.toolTipHeight)
			self.childrenList.append(textLine)
			self.toolTipHeight += 14

	def OverInToolTip(self, arg):
		arglen = len(str(arg))
		pos_x, pos_y = wndMgr.GetMousePosition()
		
		self.tooltip.ClearToolTip()
		self.tooltip.SetThinBoardSize(11 + 6 * arglen)
		self.tooltip.SetToolTipPosition(pos_x + 5, pos_y - 5)
		self.tooltip.AppendTextLine(arg, 0xffffff00)
		self.tooltip.Show()
		
	def OverOutToolTip(self):
		self.tooltip.Hide()
		
	def RanklistIconTooltip(self, event_type, idx):
		if "mouse_over_in" == str(event_type):
			if idx == 1:
				self.OverInToolTip(uiScriptLocale.EXTENDET_BATTLE_PASS_RANK_ICON_RANK)
			elif idx == 2:
				self.OverInToolTip(uiScriptLocale.EXTENDET_BATTLE_PASS_RANK_ICON_NAME)
			elif idx == 3:
				self.OverInToolTip(uiScriptLocale.EXTENDET_BATTLE_PASS_RANK_ICON_TIME)
			else:
				return 
		elif "mouse_over_out" == str(event_type) :
			self.OverOutToolTip()
		else:
			return
				
	def OverInItem(self, slotPos):
		if self.tooltipItem:
			self.tooltipItem.ClearToolTip()
			if self.rewardDict.has_key(slotPos):
				self.tooltipItem.AddItemData(self.rewardDict[slotPos][0], metinSlot = [0 for i in xrange(player.METIN_SOCKET_MAX_NUM)])
				self.tooltipItem.ShowToolTip()

	def OverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnScroll(self):
		if self.missionList:
			self.missionList.OnScroll(self.scrollBar.GetPos())

	def OnRunMouseWheel(self, nLen):
		if not self.scrollBar or not self.scrollBar.IsShow():
			return

		if nLen > 0 and self.scrollBar:
			if self.missionList.GetMissionCount() < 15:
				self.scrollBar.OnUp()
			else:
				self.scrollBar.OnUp2()
		else:
			if self.missionList.GetMissionCount() < 15:
				self.scrollBar.OnDown()
			else:
				self.scrollBar.OnDown2()

	def OnPressEscapeKey(self):
		self.Close()
		return True
	
	def OnUpdate(self):
		missionCount = self.missionList.GetMissionCount()
		completedMissionCount = self.missionList.GetCompletedMissionCount()
		battlepasstype = self.GetShowBattlePassType()
		battlepassid = self.GetActualBattlePassIDByType()
		
		if self.page == "PREMIUM" and player.GetPremiumBattlePassID() != self.BattlePassIDPremium and self.isActivePremiumWindow == 1:
			self.SetPage(self.page)
		
		if battlepassid != -1:
			if self.battlePassGeneralInfo[battlepasstype][battlepassid]["end_time"]+1 < app.GetGlobalTimeStamp():
				self.__ClearAll()
				net.SendExtBattlePassAction(1)
				self.SetPage(self.page)

		if self.next_normal_battlepass_time != 0:
			if self.next_normal_battlepass_time+1 < app.GetGlobalTimeStamp():
				self.__ClearAll()
				net.SendExtBattlePassAction(1)
				self.SetPage(self.page, True)
			else:
				self.GetChild("NormalInfoTextTime").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_TIME_TO_NEXT_NORMAL_BATTLEPASS % localeInfo.SecondToDHMS(self.next_normal_battlepass_time+2 - app.GetGlobalTimeStamp()))	

		if self.next_premium_battlepass_time != 0:
			if self.next_premium_battlepass_time+1 < app.GetGlobalTimeStamp():
				self.__ClearAll()
				net.SendExtBattlePassAction(1)
				self.SetPage(self.page, True)
			elif self.GetChild("PremiumInfoTextTime").IsShow():
				self.GetChild("PremiumInfoTextTime").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_TIME_TO_NEXT_PREMIUM_BATTLEPASS % localeInfo.SecondToDHMS(self.next_premium_battlepass_time+2 - app.GetGlobalTimeStamp()))	
				
		if self.next_event_battlepass_time != 0:
			if self.next_event_battlepass_time+1 < app.GetGlobalTimeStamp():
				self.__ClearAll()
				net.SendExtBattlePassAction(1)
				self.SetPage(self.page, True)
			else:
				self.GetChild("EventInfoTextTime").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_TIME_TO_NEXT_EVENT_BATTLEPASS % localeInfo.SecondToDHMS(self.next_event_battlepass_time+2 - app.GetGlobalTimeStamp()))

		if self.subpage == "GENERAL":
			if self.battlePassGeneralInfo[battlepasstype].has_key(battlepassid) and self.battlePassGeneralInfo[battlepasstype][battlepassid]["start_time"] < app.GetGlobalTimeStamp() and self.battlePassGeneralInfo[battlepasstype][battlepassid]["end_time"]+1 > app.GetGlobalTimeStamp():
				self.GetChild("GeneralInfoEndTimeText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_SEASON_REST_TIME % str(localeInfo.SecondToDHMS(self.battlePassGeneralInfo[battlepasstype][battlepassid]["end_time"] - app.GetGlobalTimeStamp())))
				self.GetChild("GeneralInfoFinishMissionsText").SetText(uiScriptLocale.EXTENDET_BATTLE_PASS_SEASON_CURRENT_MISSIONS % (completedMissionCount, missionCount))
				self.GetChild("GaugeMission").SetPercentage(completedMissionCount, missionCount)
				self.UpdateTimeGauge()

	def UpdateTimeGauge(self):
		battlepasstype = self.GetShowBattlePassType()
		battlepassid = self.GetActualBattlePassIDByType()
		range = self.battlePassGeneralInfo[battlepasstype][battlepassid]["end_time"] - self.battlePassGeneralInfo[battlepasstype][battlepassid]["start_time"]
		percent = float((float(app.GetGlobalTimeStamp()) - float(self.battlePassGeneralInfo[battlepasstype][battlepassid]["start_time"])) / float(range))
		if percent >= 0.9:
			self.GetChild("GaugeTime").LoadImage(GAUGE_PATH + "large_red.sub")
		else:
			if percent <= 0.7:
				self.GetChild("GaugeTime").LoadImage(GAUGE_PATH + "large_green.sub")
			else:
				self.GetChild("GaugeTime").LoadImage(GAUGE_PATH + "large_yellow.sub")
		self.GetChild("GaugeTime").SetPercentageEx(self.battlePassGeneralInfo[battlepasstype][battlepassid]["start_time"], app.GetGlobalTimeStamp(), self.battlePassGeneralInfo[battlepasstype][battlepassid]["end_time"])

class ListBoxMissions(ui.Window):
	class Item(ui.Window):
		def __init__(self, BattlePassType, battlePassID, mission_index, mission_type):
			ui.Window.__init__(self)
			self.SetWindowName("ListBoxMissions_Item")
			self.bIsSelected = False
			self.battlePassType = BattlePassType
			self.battlePassID = battlePassID
			self.missionIndex = mission_index
			self.missionId = mission_type
			self.percentActual = 0
			self.percentTotal = 0
			self.rewardCount = 0
			self.missionInfo1 = 0
			self.xBase, self.yBase = 0, 0

			self.tooltipItem = uiToolTip.ItemToolTip()
			self.tooltipItem.Hide()

			self.listImages = []
			self.rewardList = [[0, 0], [0, 0], [0, 0]]
			self.rewardImages = []
			self.rewardListCount = []
			
			if BattlePassType == 1:
				bgImage = ui.MakeExpandedImageBox(self, NORMAL_PATH + "mission_item_normal.sub", 0, 0, "not_pick")
			if BattlePassType == 2:
				bgImage = ui.MakeExpandedImageBox(self, PREMIUM_PATH + "mission_item_normal.sub", 0, 0, "not_pick")
			if BattlePassType == 3:
				bgImage = ui.MakeExpandedImageBox(self, EVENT_PATH + "mission_item_normal.sub", 0, 0, "not_pick")
			bgGauge = ui.MakeExpandedImageBox(bgImage, GAUGE_PATH + "small_background.sub", 47, 28, "not_pick")
			bgGaugeFull = ui.MakeExpandedImageBox(bgGauge, GAUGE_PATH + "small_green.sub", 8, 2, "not_pick")
			bgGaugeFull.SetWindowName("gaugeFull")

			self.listImages.append(bgImage)
			self.listImages.append(bgGauge)
			self.listImages.append(bgGaugeFull)

			for i in xrange(3):
				rewardImage = ui.MakeExpandedImageBox(self, "d:/ymir work/ui/pet/skill_button/skill_enable_button.sub", 186 + (32 * i), 6)
				rewardImage.SetEvent(ui.__mem_func__(self.OverInItem), "MOUSE_OVER_IN", i)
				rewardImage.SetEvent(ui.__mem_func__(self.OverOutItem), "MOUSE_OVER_OUT")
				self.rewardImages.append(rewardImage)

				itemCount = ui.NumberLine()
				itemCount.SetParent(rewardImage)
				itemCount.SetWindowName("itemCount_%d" % i)
				itemCount.SetHorizontalAlignRight()
				itemCount.SetPosition(32 - 4, 32 - 10)
				itemCount.Show()
				self.rewardListCount.append(itemCount)

			self.missionName = ui.AddTextLine(bgImage, 53, 6, "", 1)

		def __del__(self):
			ui.Window.__del__(self)
			self.bIsSelected = False
			self.missionId = 0
			self.battlePassType = 0
			self.missionIndex = 0
			self.battlePassID = 0
			self.percentActual = 0
			self.percentTotal = 0
			self.rewardCount = 0
			self.missionInfo1 = 0
			self.xBase, self.yBase = 0, 0
			self.missionName = None
			self.tooltipItem = None

			self.listImages = []
			self.rewardList = [[0, 0], [0, 0], [0, 0]]
			self.rewardImages = []
			self.rewardListCount = []

		def OverInItem(self, eventType, rewardIndex):
			if self.tooltipItem:
				self.tooltipItem.ClearToolTip()
				if self.rewardList[rewardIndex][0]:
					self.tooltipItem.AddItemData(self.rewardList[rewardIndex][0], metinSlot = [0 for i in xrange(player.METIN_SOCKET_MAX_NUM)])
					self.tooltipItem.ShowToolTip()

		def OverOutItem(self):
			if self.tooltipItem:
				self.tooltipItem.HideToolTip()

		def SetBasePosition(self, x, y):
			self.xBase = x
			self.yBase = y

		def GetBasePosition(self):
			return (self.xBase, self.yBase)

		def GetMissionIndex(self):
			return self.missionIndex
			
		def GetMissionId(self):
			return self.missionId

		def GetPattlePassType(self):
			return self.battlePassType
            
		def GetBattlePassId(self):
			return self.battlePassID

		def OnMouseLeftButtonUp(self):
			snd.PlaySound("sound/ui/click.wav")
			self.Select()

		def Select(self):
			self.bIsSelected = True
			self.parent.SetSelectedMission(self.missionIndex, self.missionId)

			if len(self.listImages) > 0:
				if self.battlePassType == 1:
					self.listImages[0].LoadImage(NORMAL_PATH + "mission_item_active.sub")
				if self.battlePassType == 2:
					self.listImages[0].LoadImage(PREMIUM_PATH + "mission_item_active.sub")
				if self.battlePassType == 3:
					self.listImages[0].LoadImage(EVENT_PATH + "mission_item_active.sub")

		def Deselect(self):
			self.bIsSelected = False

			if len(self.listImages) > 0:
				if self.battlePassType == 1:
					self.listImages[0].LoadImage(NORMAL_PATH + "mission_item_normal.sub")
				if self.battlePassType == 2:
					self.listImages[0].LoadImage(PREMIUM_PATH + "mission_item_normal.sub")
				if self.battlePassType == 3:
					self.listImages[0].LoadImage(EVENT_PATH + "mission_item_normal.sub")

		def SetProgress(self, progressActual, pregressTotal):
			self.percentActual = progressActual
			self.percentTotal = pregressTotal

			self.UpdateGauge()

		def UpdateProgress(self, newProgress):
			self.percentActual = newProgress
			self.UpdateGauge()

		def UpdateGauge(self):
			for image in self.listImages:
				if image.GetWindowName() == "gaugeFull":
					if self.percentActual == self.percentTotal:
						image.LoadImage(GAUGE_PATH + "small_green.sub")
					else:
						if self.percentActual <= int(self.percentTotal / 2):
							image.LoadImage(GAUGE_PATH + "small_blue.sub")
						else:
							image.LoadImage(GAUGE_PATH + "small_yellow.sub")

		def IsCompleted(self):
			if self.percentActual >= self.percentTotal:
				return True

			return False

		def SetMissionName(self, missionName):
			if self.missionName:
				self.missionName.SetText(missionName)

		def SetMissionInfo1(self, missionInfo):
			self.missionInfo1 = missionInfo

		def GetMissionInfo(self):
			return (self.missionName.GetText(), self.missionInfo1, self.percentActual, self.percentTotal)

		def AddMissionReward(self, itemVnum, itemCount):
			if self.rewardCount == -1:
				return

			if itemVnum and itemCount > 0:
				if self.rewardCount < len(self.rewardImages):
					item.SelectItem(itemVnum)
					self.rewardImages[self.rewardCount].LoadImage(item.GetIconImageFileName())
					self.rewardListCount[self.rewardCount].SetNumber(str(itemCount))
					self.rewardList[self.rewardCount] = [itemVnum, itemCount]
					self.rewardCount += 1
			else:
				self.rewardCount = -1

		def Show(self):
			ui.Window.Show(self)

		def SetParent(self, parent):
			ui.Window.SetParent(self, parent)
			self.parent = proxy(parent)

		def OnUpdate(self):
			isInMissionRewards = None
			if self.rewardImages[0].IsIn():
				isInMissionRewards = 0
			elif self.rewardImages[1].IsIn():
				isInMissionRewards = 1
			elif self.rewardImages[2].IsIn():
				isInMissionRewards = 2

			if isInMissionRewards != None:
				if self.tooltipItem:
					self.tooltipItem.ClearToolTip()
					if self.rewardList[isInMissionRewards][0] and self.rewardList[isInMissionRewards][0] != 0:
						self.tooltipItem.AddItemData(self.rewardList[isInMissionRewards][0], metinSlot = [0 for i in xrange(player.METIN_SOCKET_MAX_NUM)])
						self.tooltipItem.ShowToolTip()
					else:
						self.tooltipItem.HideToolTip()
			else:
				if self.tooltipItem:
					self.tooltipItem.HideToolTip()

			for count in self.rewardListCount:
				xList, yList = self.parent.GetGlobalPosition()
				xText, yText = count.GetGlobalPosition()
				wText, hText = count.GetWidth(), 7

				if yText < yList or (yText + hText > yList + self.parent.GetHeight()):
					count.Hide()
				else:
					count.Show()

		def OnRender(self):
			xList, yList = self.parent.GetGlobalPosition()

			for item in self.listImages + self.rewardImages:
				if item.GetWindowName() == "gaugeFull":
					if self.percentTotal == 0:
						self.percentTotal = 1
					item.SetClipRect(0.0, yList, -1.0 + float(self.percentActual) / float(self.percentTotal), yList + self.parent.GetHeight(), True)
				else:
					item.SetClipRect(xList, yList, xList + self.parent.GetWidth(), yList + self.parent.GetHeight())

			if self.missionName:
				xText, yText = self.missionName.GetGlobalPosition()
				wText, hText = self.missionName.GetTextSize()

				if yText < yList or (yText + hText > yList + self.parent.GetHeight()):
					self.missionName.Hide()
				else:
					self.missionName.Show()

			for count in self.rewardListCount:
				xList, yList = self.parent.GetGlobalPosition()
				xText, yText = count.GetGlobalPosition()
				wText, hText = count.GetWidth(), 7

				if yText < yList or (yText + hText > yList + self.parent.GetHeight()):
					count.Hide()
				else:
					count.Show()

	def __init__(self):
		ui.Window.__init__(self)
		self.SetWindowName("ListBoxMissions")
		self.missionlist_type_normal = []
		self.missionlist_type_premium = []
		self.missionlist_type_event = []
		
		self.selected_list = self.missionlist_type_normal
		
		self.selectedBattlePassType = 1
		self.selectedMissionNormal = -1
		self.selectedMissionPremium = -1
		self.selectedMissionEvent = -1

	def __del__(self):
		ui.Window.__del__(self)

		self.missionlist_type_normal = []
		self.missionlist_type_premium = []
		self.missionlist_type_event = []

		self.selectedBattlePassType = 1
		self.selectedMissionNormal = -1
		self.selectedMissionPremium = -1
		self.selectedMissionEvent =- 1
		self.globalParent = None

	def SelectMissionByPage(self):
		if self.selectedBattlePassType == 1:
			selected_mission = self.selectedMissionNormal
		if self.selectedBattlePassType == 2:
			selected_mission = self.selectedMissionPremium
		if self.selectedBattlePassType == 3:
			selected_mission = self.selectedMissionEvent

		if len(self.selected_list) != 0:
			if selected_mission == -1:
				selected_mission = self.selected_list[0].GetMissionIndex()
			for item in self.selected_list:
				if selected_mission == item.GetMissionIndex():
					item.Select()
				else:
					item.Deselect()

	def SetSelectedMission(self, missionIndex, missionId):
		if self.selectedBattlePassType == 1:
			self.selectedMissionNormal = missionIndex
		if self.selectedBattlePassType == 2:
			self.selectedMissionPremium = missionIndex
		if self.selectedBattlePassType == 3:
			self.selectedMissionEvent = missionIndex
		
		if len(self.selected_list) != 0:
			for item in self.selected_list:
				if missionIndex != item.GetMissionIndex():
					item.Deselect()

			if self.globalParent:
				self.globalParent.SetMissionInfo(missionIndex, missionId)

	def GetSelectedMission(self):
		if self.selectedBattlePassType == 1:
			return self.selectedMissionNormal
		if self.selectedBattlePassType == 2:
			return self.selectedMissionPremium
		if self.selectedBattlePassType == 3:
			return self.selectedMissionEvent

	def HaveMission(self, battlePassType, missionIndex, missionId):
		if battlePassType == 1:
			select_list = self.missionlist_type_normal
		if battlePassType == 2:
			select_list = self.missionlist_type_premium
		if battlePassType == 3:
			select_list = self.missionlist_type_event
		for item in select_list:
			if missionIndex == item.GetMissionIndex():
				return True

		return False

	def GetMissionInfo(self, mission_index):
		for item in self.selected_list:
			if mission_index == item.GetMissionIndex():
				return item.GetMissionInfo()

		return (0, 0, 0)

	def GetMissionCount(self):
		return len(self.selected_list)

	def GetCompletedMissionCount(self):
		completedCount = 0
		for item in self.selected_list:
			if item.IsCompleted():
				completedCount += 1

		return completedCount

	def SetProgress(self, battlePassType, missionIndex, progressActual, pregressTotal):
		if battlePassType == 1:
			select_list = self.missionlist_type_normal
		if battlePassType == 2:
			select_list = self.missionlist_type_premium
		if battlePassType == 3:
			select_list = self.missionlist_type_event
		for item in select_list:
			if missionIndex == item.GetMissionIndex():
				item.SetProgress(progressActual, pregressTotal)

	def UpdateProgress(self, battlePassType, missionIndex, newProgress):
		if battlePassType == 1:
			select_list = self.missionlist_type_normal
		if battlePassType == 2:
			select_list = self.missionlist_type_premium
		if battlePassType == 3:
			select_list = self.missionlist_type_event
		for item in select_list:
			if missionIndex == item.GetMissionIndex():
				item.UpdateProgress(newProgress)

	def AddMissionReward(self, battlePassType, missionIndex, itemVnum, itemCount):
		if battlePassType == 1:
			select_list = self.missionlist_type_normal
		if battlePassType == 2:
			select_list = self.missionlist_type_premium
		if battlePassType == 3:
			select_list = self.missionlist_type_event
		for item in select_list:
			if missionIndex == item.GetMissionIndex():
				item.AddMissionReward(itemVnum, itemCount)

	def SetGlobalParent(self, parent):
		self.globalParent = proxy(parent)

	def OnRunMouseWheel(self, nLen):
		if nLen > 0:
			self.scrollBar.OnUp()
		else:
			self.scrollBar.OnDown()

	def OnScroll(self, scrollPos):
		totalHeight = 0
		for itemH in self.selected_list:
			totalHeight += itemH.GetHeight() 

		totalHeight -= self.GetHeight()

		for i in xrange(len(self.selected_list)):
			x, y = self.selected_list[i].GetLocalPosition()
			xB, yB = self.selected_list[i].GetBasePosition()
			setPos = yB - int(scrollPos * totalHeight)
			self.selected_list[i].SetPosition(xB, setPos)

	def AppendMission(self, itemHeight, battlePassType, battlePassID, mission_index, mission_type, missionName, missionInfo1):
		if battlePassType == 1:
			selected_list = self.missionlist_type_normal
		if battlePassType == 2:
			selected_list = self.missionlist_type_premium
		if battlePassType == 3:
			selected_list = self.missionlist_type_event
	
		item = self.Item(battlePassType, battlePassID, mission_index, mission_type)
		item.SetParent(self)
		item.SetSize(self.GetWidth() - 3, itemHeight)
		item.SetMissionName(missionName)
		item.SetMissionInfo1(missionInfo1)

		if len(selected_list) == 0:
			item.SetPosition(0, 0)
			item.SetBasePosition(0, 0)
		else:
			x, y = selected_list[-1].GetLocalPosition()
			item.SetPosition(0, y + selected_list[-1].GetHeight())
			item.SetBasePosition(0, y + selected_list[-1].GetHeight())

		selected_list.append(item)

	def ShowMissionsByBattlePassType(self, BattlePassType):
		self.selectedBattlePassType = BattlePassType
		for i in xrange(len(self.missionlist_type_normal)):
			self.missionlist_type_normal[i].Hide()
		for i in xrange(len(self.missionlist_type_premium)):
			self.missionlist_type_premium[i].Hide()
		for i in xrange(len(self.missionlist_type_event)):
			self.missionlist_type_event[i].Hide()
		if self.globalParent.scrollBar:
			self.globalParent.scrollBar.Show()
		if BattlePassType == 1:
			if len(self.missionlist_type_normal) <= 5:
				self.globalParent.scrollBar.Hide()
			for i in xrange(len(self.missionlist_type_normal)):
				self.missionlist_type_normal[i].Show()
				self.selected_list = self.missionlist_type_normal
		if BattlePassType == 2:
			if len(self.missionlist_type_premium) <= 5:
				self.globalParent.scrollBar.Hide()
			for i in xrange(len(self.missionlist_type_premium)):
				self.missionlist_type_premium[i].Show()
				self.selected_list = self.missionlist_type_premium
		if BattlePassType == 3:
			if len(self.missionlist_type_event) <= 5:
				self.globalParent.scrollBar.Hide()
			for i in xrange(len(self.missionlist_type_event)):
				self.missionlist_type_event[i].Show()
				self.selected_list = self.missionlist_type_event
		
	def ClearAll(self):
		self.missionlist_type_normal = []
		self.missionlist_type_premium = []
		self.missionlist_type_event = []
		self.selectedBattlePassType = 1
		self.selectedMissionNormal = -1
		self.selectedMissionPremium = -1
		self.selectedMissionEvent = -1

class MissionScrollBar(ui.DragButton):
	def __init__(self):
		ui.DragButton.__init__(self)
		self.AddFlag("float")
		self.AddFlag("movable")
		self.AddFlag("restrict_x")

		self.eventScroll = lambda *arg: None
		self.currentPos = 0.0
		self.scrollStep = 0.10
		self.scrollStep2 = 0.05

	def __del__(self):
		ui.DragButton.__del__(self)
		self.currentPos = 0.0
		self.scrollStep = 0.10
		self.scrollStep2 = 0.05
		self.eventScroll = lambda *arg: None

	def SetScrollEvent(self, event):
		self.eventScroll = event

	def SetScrollStep(self, step):
		self.scrollStep = step

	def SetPos(self, pos):
		pos = max(0.0, pos)
		pos = min(1.0, pos)

		yPos = float(pos * 165)

		self.SetPosition(12, yPos + 2)
		self.OnMove()

	def GetPos(self):
		return self.currentPos

	def OnUp(self):
		self.SetPos(self.currentPos - self.scrollStep)

	def OnDown(self):
		self.SetPos(self.currentPos + self.scrollStep)

	def OnUp2(self):
		self.SetPos(self.currentPos - self.scrollStep2)

	def OnDown2(self):
		self.SetPos(self.currentPos + self.scrollStep2)
		
	def OnMove(self):
		(xLocal, yLocal) = self.GetLocalPosition()
		self.currentPos = float(yLocal - 2) / float(165) 

		self.eventScroll()

def SplitDescription(desc, limit):
	total_tokens = desc.split()
	line_tokens = []
	line_len = 0
	lines = []
	for token in total_tokens:
		line_len += len(token)
		if len(line_tokens) + line_len > limit:
			lines.append(" ".join(line_tokens))
			line_len = len(token)
			line_tokens = [token]
		else:
			line_tokens.append(token)

	if line_tokens:
		lines.append(" ".join(line_tokens))

	return lines
