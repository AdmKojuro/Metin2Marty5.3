#
# Title: Dungeon Information System
# Author: Owsap
# Description: List of all available dungeons.
# Date: 2021.01.09
# Last Update: 2021.06.03
# Version 2.0.0.2
#
# Skype: owsap.
# Discord: Owsap#0905
#
# 0x0
#
# Web: https://owsap-productions.com/
# GitHub: https://github.com/Owsap
#

import app
import ui

import localeInfo
import uiScriptLocale
import item
import nonplayer
import renderTarget
import uiToolTip
import grp
import player
import dungeonInfo
import uiCommon
import chat

from _weakref import proxy

AFFECT_DATA = {
	item.APPLY_RESIST_DARK : localeInfo.DETAILS_26,
	item.APPLY_ATTBONUS_UNDEAD : localeInfo.DETAILS_4,
	item.APPLY_RESIST_MAGIC : localeInfo.DETAILS_76,
	item.APPLY_RESIST_BOW : localeInfo.DETAILS_52,
	item.APPLY_ATTBONUS_DEVIL : localeInfo.DETAILS_9,
	item.APPLY_RESIST_ELEC : localeInfo.DETAILS_24,
	item.APPLY_RESIST_EARTH : localeInfo.DETAILS_29,
	item.APPLY_RESIST_EARTH : localeInfo.DETAILS_29,
	item.APPLY_BLOCK : localeInfo.DETAILS_63,
	item.APPLY_RESIST_WIND : localeInfo.DETAILS_28,
	item.APPLY_ATTBONUS_ORC : localeInfo.DETAILS_3,
	item.APPLY_RESIST_FIRE : localeInfo.DETAILS_27,
	item.APPLY_RESIST_ICE : localeInfo.DETAILS_25,
}

RENDER_BACKGROUND_INDEX = 2

ATT_BONUS_INDEX = 0
DEF_BONUS_INDEX = 1

RANK_SCORE = 1
RANK_TIME = 2
RANK_DAMAGE = 3

ROOT = "d:/ymir work/ui/game/dungeon_info/"
# ICON_ROOT = "%s/dungeon_info/" % app.GetLocalePath()
ICON_ROOT = "locale/common/dungeon_icon/"

def SecondToHMSGolbal(time):
	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24

	text = ""

	if hour > 9:
		text += str(hour) + ":"
	else:
		text += "0" + str(hour) + ":"

	if minute > 9:
		text += str(minute) + ":"
	else:
		text += "0" + str(minute) + ":"

	if second > 9:
		text += str(second)
	else:
		text += "0" + str(second)

	return text

def NumberToDamageString(n) :
	if n <= 0 :
		return "0"

	return "%s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]))

DUNGEON_CLOSED = 0
DUNGEON_AVAILABLE = 1
DUNGEON_COOLDOWN = 2
DUNGEON_LEVEL_HIGH = 3
DUNGEON_LEVEL_LOW = 4

class ListToggleButton(object):
	def __init__(self, parent = None, key = 0):
		self.parent = parent
		self.key = key

		self.event = lambda *arg: None
		self.arg = 0

		button = ui.RadioButton()
		button.SetParent(parent)
		button.SetPosition(4, 4 + ((50 - 2) * key))
		button.SetUpVisual(ROOT + "list_btn_default.png")
		button.SetOverVisual(ROOT + "list_btn_over.png")
		button.SetDownVisual(ROOT + "list_btn_down.png")
		button.Show()
		self.button = button

		name = ui.TextLine()
		name.SetParent(button)
		name.SetPosition(120, 8)
		name.SetText("")
		name.SetHorizontalAlignCenter()
		name.Show()
		self.name = name

		icon = ui.ImageBox()
		icon.SetParent(button)
		icon.SetPosition(3, 3)
		icon.LoadImage(ICON_ROOT + "0.png")
		icon.Show()
		self.icon = icon

		clock = ui.ImageBox()
		clock.SetParent(button)
		clock.SetPosition(190, 5)
		clock.LoadImage(ROOT + "clock.png")
		clock.Hide()
		self.clock = clock

		clockTime = ui.TextLine()
		clockTime.SetParent(clock)
		clockTime.SetPosition(10, 20)
		clockTime.SetText("00:00:00")
		clockTime.SetHorizontalAlignCenter()
		clockTime.Hide()
		self.clockTime = clockTime

		status = ui.TextLine()
		status.SetParent(button)
		status.SetPosition(120, 25)
		status.SetText(localeInfo.DUNGEON_INFO_STATUS_AVAILABLE)
		status.SetFontColor((0.00 / 255), (255.00 / 255), (0.00 / 255))
		status.SetHorizontalAlignCenter()
		status.Show()
		self.status = status

		self.cooldown = 0
		self.ticking = False

	def Show(self):
		if self.button:
			self.button.Show()

	def Hide(self):
		if self.button:
			self.button.Hide()

	def __del__(self):
		self.parent = None
		self.key = None

		self.enable = True
		self.event = lambda *arg: None
		self.arg = 0

		self.button = None
		self.icon = None

		self.status = None

		self.clock = None
		self.clockTime = None

		self.cooldown = 0
		self.ticking = False

	def SetKey(self, key):
		self.key = key

	def GetKey(self):
		return self.key

	def SetText(self, map_index):
		if self.name:
			self.name.SetText(localeInfo.GetMiniMapZoneNameByIdx(map_index))

	def SetStatus(self, dataDict):
		if self.status:
			try:
				text = dataDict[0]
				r, g, b = dataDict[1]
				self.status.SetText(text)
				self.status.SetFontColor(r, g, b)
			except KeyError:
				self.status.SetText("")

	def SetIcon(self, map_index):
		if self.icon:
			iconFile = ICON_ROOT + "%d.png" % map_index
			if app.IsExistFile(iconFile):
				self.icon.LoadImage(iconFile)
			else:
				self.icon.LoadImage(ICON_ROOT + "0.png")

	def ResetClock(self, trigger, time):
		self.ticking = False
		if not self.cooldown:
			self.SetClock(trigger, time)

	def SetClock(self, trigger, time = 0, reset = False):
		self.ShowClock()

		if not self.ticking:
			self.clockTime.SetText("%s" % SecondToHMSGolbal(time))
			self.cooldown = app.GetGlobalTimeStamp() + time

			self.ticking = True

	def ShowClock(self):
		if self.clock and self.clockTime:
			self.clock.Show()
			self.clockTime.Show()

	def HideClock(self):
		self.clock.Hide()
		self.clockTime.Hide()

		self.ticking = False

	def OnUpdate(self):
		if self.cooldown and self.ticking:
			leftSec = max(0, self.cooldown - app.GetGlobalTimeStamp())
			if leftSec > 0:
				self.clockTime.SetText("%s" % SecondToHMSGolbal(leftSec))
			else:
				self.cooldown = 0
				self.HideClock()
		else:
			self.HideClock()

	def SetEvent(self, event, arg):
		self.event = event
		self.arg = arg

		if self.button:
			self.button.SetEvent(self.event, self.arg)

	def GetObject(self):
		if self.button:
			return self.button

class DungeonRankingWindow(ui.ScriptWindow):
	SLOT_RANKING = 0
	SLOT_NAME = 1
	SLOT_LEVEL = 2
	SLOT_POINT = 3

	MAX_LINE_COUNT = dungeonInfo.MAX_RANKING_LINES

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.Initialize()

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		self.Initialize()

	def Initialize(self):
		self.isLoaded = False
		self.type = None

		self.resultButtonList = []
		self.resultSlotList = {}
		self.myResultSlotList = []

	def Destroy(self):
		self.Initialize()

	def __LoadWindow(self):
		if self.isLoaded:
			return

		self.isLoaded = True

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/DungeonRankingWindow.py")
		except:
			import exception
			exception.Abort("DungeonRankingWindow.__LoadWindow")

		try:
			self.titleBar = self.GetChild("TitleBar")
			self.titleBarName = self.GetChild("TitleBarName")

			self.resultPosition = self.GetChild("ResultPosition")
			self.resultName = self.GetChild("ResultName")
			self.resultLevel = self.GetChild("ResultLevel")
			self.resultPoints = self.GetChild("ResultPoints")
			self.waitAniImg = self.GetChild("WaitAniImg")

		except:
			import exception
			exception.Abort("DungeonRankingWindow.__LoadWindow.SetObject")

		if self.titleBar:
			self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.MakeUIBoard()

	def MakeUIBoard(self):
		yPos = 0
		for i in range(0, self.MAX_LINE_COUNT + 1):
			yPos = 65 + i * 24
			if i == 5:
				yPos += 10

			## Position
			rankingSlotImage = ui.MakeImageBox(self, "d:/ymir work/ui/public/parameter_slot_00.sub", 23, yPos)
			rankingSlotImage.SetAlpha(0)
			rankingSlot = ui.MakeTextLine(rankingSlotImage)
			self.Children.append(rankingSlotImage)
			self.Children.append(rankingSlot)

			## Name
			nameImage = ui.MakeImageBox(self, "d:/ymir work/ui/public/parameter_slot_04.sub", 77, yPos)
			nameImage.SetAlpha(0)
			nameSlot = ui.MakeTextLine(nameImage)
			self.Children.append(nameImage)
			self.Children.append(nameSlot)

			## Level
			levelSlotImage = ui.MakeImageBox(self, "d:/ymir work/ui/public/parameter_slot_00.sub", 205, yPos)
			levelSlotImage.SetAlpha(0)
			levelSlot = ui.MakeTextLine(levelSlotImage)
			self.Children.append(levelSlotImage)
			self.Children.append(levelSlot)

			## Points (Rank Type)
			pointSlotImage = ui.MakeImageBox(self, "d:/ymir work/ui/public/parameter_slot_00.sub", 270, yPos)
			pointSlotImage.SetAlpha(0)
			pointSlot = ui.MakeTextLine(pointSlotImage)
			self.Children.append(pointSlotImage)
			self.Children.append(pointSlot)

			if i < self.MAX_LINE_COUNT:
				tempRankingSlotList = []
				tempRankingSlotList.append(rankingSlot)
				tempRankingSlotList.append(nameSlot)
				tempRankingSlotList.append(levelSlot)
				tempRankingSlotList.append(pointSlot)
				self.resultSlotList[i] = tempRankingSlotList
			else:
				self.myResultSlotList.append(rankingSlot)
				self.myResultSlotList.append(nameSlot)
				self.myResultSlotList.append(levelSlot)
				self.myResultSlotList.append(pointSlot)

			itemSlotButtonImage = ui.MakeButton(self, 21, yPos, "", "d:/ymir work/ui/game/guild/dragonlairranking/", "ranking_list_button01.sub", "ranking_list_button02.sub", "ranking_list_button02.sub")
			itemSlotButtonImage.Show()
			itemSlotButtonImage.Disable()
			self.Children.append(itemSlotButtonImage)

			if i < self.MAX_LINE_COUNT:
				self.resultButtonList.append(itemSlotButtonImage)

		self.AllClear()
		if self.waitAniImg:
			self.waitAniImg.Show()

	def RefreshRankingBoard(self):
		self.AllClear()
		if self.waitAniImg:
			self.waitAniImg.Hide()

		myRank = 0
		myPoints = 0

		for line, slot in self.resultSlotList.items():
			nowIndex = line

			if nowIndex >= dungeonInfo.GetRankingCount():
				break

			(name, level, points) = dungeonInfo.GetRankingByLine(nowIndex)

			slot[self.SLOT_RANKING].SetText(str(nowIndex + 1))
			slot[self.SLOT_NAME].SetText(name)
			slot[self.SLOT_LEVEL].SetText(str(level))

			if self.type == RANK_SCORE:
				self.resultPoints.SetText(localeInfo.DUNGEON_RANKING_TOTAL_FINISHED)
				slot[self.SLOT_POINT].SetText(str(points))
			elif self.type == RANK_TIME:
				self.resultPoints.SetText(localeInfo.DUNGEON_RANKING_FASTEST_TIME)
				slot[self.SLOT_POINT].SetText(SecondToHMSGolbal(points))
			elif self.type == RANK_DAMAGE:
				self.resultPoints.SetText(localeInfo.DUNGEON_RANKING_HIGHEST_DMG)
				slot[self.SLOT_POINT].SetText(NumberToDamageString(points))
			else:
				self.resultPoints.SetText(localeInfo.DUNGEON_RANKING_POINTS)
				slot[self.SLOT_POINT].SetText(str(points))

			self.resultButtonList[line].Show()

			if player.GetName() == name:
				myPoints = points
				self.resultButtonList[line].Down()

		self.myResultSlotList[self.SLOT_NAME].SetText(player.GetName())
		self.myResultSlotList[self.SLOT_LEVEL].SetText(str(player.GetStatus(player.LEVEL)))

		myRank = dungeonInfo.GetMyRankingLine()
		if myRank:
			self.myResultSlotList[self.SLOT_RANKING].SetText(str(myRank))

			if myPoints:
				if self.type == RANK_SCORE:
					self.myResultSlotList[self.SLOT_POINT].SetText(str(myPoints))
				elif self.type == RANK_TIME:
					self.myResultSlotList[self.SLOT_POINT].SetText(SecondToHMSGolbal(myPoints))
				elif self.type == RANK_DAMAGE:
					self.myResultSlotList[self.SLOT_POINT].SetText(NumberToDamageString(myPoints))
				else:
					self.myResultSlotList[self.SLOT_POINT].SetText(myPoints)

	def AllClear(self):
		for line, resultSlotList in self.resultSlotList.items():
			resultSlotList[self.SLOT_RANKING].SetText("")
			resultSlotList[self.SLOT_NAME].SetText("")
			resultSlotList[self.SLOT_LEVEL].SetText("")
			resultSlotList[self.SLOT_POINT].SetText("")

			self.resultButtonList[line].SetUp()
			self.resultButtonList[line].Hide()

		self.myResultSlotList[self.SLOT_RANKING].SetText("-")
		self.myResultSlotList[self.SLOT_NAME].SetText("-")
		self.myResultSlotList[self.SLOT_LEVEL].SetText("-")
		self.myResultSlotList[self.SLOT_POINT].SetText("-")

	# On refresh board.
	def Refresh(self, type):
		self.type = type

		if self.waitAniImg:
			self.waitAniImg.Show()

		self.RefreshRankingBoard()

	# On open board.
	def Show(self):
		# Load board.
		self.__LoadWindow()

		# Show board.
		ui.ScriptWindow.Show(self)
		self.SetCenterPosition()
		self.SetTop()

	# On close board.
	def Close(self):
		# Clear dungeon ranking.
		dungeonInfo.ClearRanking()

		# Hide board.
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnPressExitKey(self):
		self.Close()
		return True

class DungeonInfoWindow(ui.ScriptWindow):
	class ItemGrid(object):
		def __init__(self, width, height):
			self.grid = {}
			self.gridWidth = width
			self.gridHeight = height
			self.gridSize = width * height
			self.Clear()

		def __del__(self):
			self.grid = {}
			self.gridWidth = 0
			self.gridHeight = 0
			self.gridSize = 0

		def Clear(self):
			for pos in range(self.gridSize):
				self.grid[pos] = False

		def IsEmpty(self, pos, width, height):
			row = pos / self.gridWidth

			if row + height > self.gridHeight:
				return False

			if pos + width > row * self.gridWidth + self.gridWidth:
				return False

			for y in range(height):
				start = pos + (y * self.gridWidth)
				if self.grid[start] == True:
					return False

				x = 1
				while x < width:
					x =+ 1
					if self.grid[start + x] == True:
						return False

			return True

		def FindBlank(self, width, height):
			if width > self.gridWidth or height > self.gridHeight:
				return -1

			for row in range(self.gridHeight):
				for col in range(self.gridWidth):
					index = row * self.gridWidth + col
					if self.IsEmpty(index, width, height):
						return index

			return -1

		def Put(self, pos, width, height):
			if not self.IsEmpty(pos, width, height):
				return False

			for y in range(height):
				start = pos + (y * self.gridWidth)
				self.grid[start] = True

				x = 1
				while x < width:
					x += 1
					self.grid[start + x] = True

			return True

	DUNGEON_TYPE_DICT = {
		0 : localeInfo.DUNGEON_INFO_TYPE_01,
		1 : localeInfo.DUNGEON_INFO_TYPE_02,
		2 : localeInfo.DUNGEON_INFO_TYPE_03,
	}

	DUNGEON_ELEMENT_DICT = {
		0 : ROOT + "unkown-element.png", # RACE_UNKOWN
		1 : "d:/ymir work/ui/game/12zi/element/elect.sub", # RACE_FLAG_ATT_ELEC
		2 : "d:/ymir work/ui/game/12zi/element/fire.sub", # RACE_FLAG_ATT_FIRE
		3 : "d:/ymir work/ui/game/12zi/element/ice.sub", # RACE_FLAG_ATT_ICE
		4 : "d:/ymir work/ui/game/12zi/element/wind.sub", # RACE_FLAG_ATT_WIND
		5 : "d:/ymir work/ui/game/12zi/element/earth.sub", # RACE_FLAG_ATT_EARTH
		6 : "d:/ymir work/ui/game/12zi/element/dark.sub", # RACE_FLAG_ATT_DARK
	}

	DUNGEON_HELP_TOOLTIP_DICT = {
		1 : localeInfo.DUNGEON_INFO_TOOL_TIP_01,
		2 : localeInfo.DUNGEON_INFO_TOOL_TIP_02,
		3 : localeInfo.DUNGEON_INFO_TOOL_TIP_03,
		4 : localeInfo.DUNGEON_INFO_TOOL_TIP_04,
		5 : "[ENTER]",
		6 : localeInfo.DUNGEON_INFO_TOOL_TIP_05,
		7 : localeInfo.DUNGEON_INFO_TOOL_TIP_06,
		8 : localeInfo.DUNGEON_INFO_TOOL_TIP_07,
		9 : localeInfo.DUNGEON_INFO_TOOL_TIP_08,
	}

	DUNGEON_STATUS_DICT = {
		DUNGEON_CLOSED : [
			localeInfo.DUNGEON_INFO_STATUS_CLOSED,
			((255.00 / 255), (0.00 / 255), (0.00 / 255))
		],
		DUNGEON_AVAILABLE : [
			localeInfo.DUNGEON_INFO_STATUS_AVAILABLE,
			((0.00 / 255), (255.00 / 255), (0.00 / 255))
		],
		DUNGEON_COOLDOWN : [
			localeInfo.DUNGEON_INFO_STATUS_COOLDOWN,
			((255.00 / 255), (0.00 / 255), (0.00 / 255))
		],
		DUNGEON_LEVEL_HIGH : [
			localeInfo.DUNGEON_INFO_STATUS_HIGH_LEVEL,
			((255.00 / 255), (0.00 / 255), (0.00 / 255))
		],
		DUNGEON_LEVEL_LOW : [
			localeInfo.DUNGEON_INFO_STATUS_LOW_LEVEL,
			((255.00 / 255), (0.00 / 255), (0.00 / 255))
		],
	}

	DUNGEON_BOX_MAX_SLOTS = 16

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.toolTip = uiToolTip.ToolTip()
		self.toolTipItem = uiToolTip.ItemToolTip()
		self.toolTipHelp = None

		self.boxGrid = self.ItemGrid(4, 4)

		self.rankWnd = DungeonRankingWindow()

		self.Init()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

		self.toolTip = None
		self.toolTipItem = None
		self.toolTipHelp = None

		self.boxGrid = {}

		self.rankWnd = None

		self.Init()

	def Destroy(self):
		if self.rankWnd:
			self.rankWnd.Destroy()
			self.rankWnd = None

		self.Init()

	def Init(self):
		self.isLoaded = False

		self.tabIndex = -1
		self.listIndex = -1
		self.toggleButtonObjList = []

		self.reqItemVnumList = [0 for slot in range(0, 3)]
		self.reqItemCountList = [0 for slot in range(0, 3)]

		self.previewTools = False
		self.previewZoom = False
		self.previewRenderModelVnum = {}

		self.scrollPos = 0

		self.showBoxWnd = False
		self.boxItems = {}

		self.questionDialog = None
		self.rankType = 0

		self.popUp = None

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/DungeonInfoWindow.py")
		except:
			import exception
			exception.Abort("DungeonInfoWindow.__LoadWindow")

		try:
			self.__BindObject()
		except:
			import exception
			exception.Abort("DungeonInfoWindow.__BindObject")

		try:
			self.__BindEvent()
		except:
			import exception
			exception.Abort("DungeonInfoWindow.__BindEvent")

		self.isLoaded = True

	def Initialize(self):
		self.__OnClickTabButton(0)

		self.toggleButtonObjList = []

		if dungeonInfo.GetCount() > 0:
			self.OnLockButtons(False)
		else:
			self.OnLockButtons(True)

			popUp = uiCommon.PopupDialog()
			popUp.SetText(localeInfo.DUNGEON_INFO_NOT_FOUND)
			popUp.SetAcceptEvent(self.Close)
			popUp.Open()
			self.popUp = popUp

		for key in xrange(min(dungeonInfo.MAX_DUNGEON_SCROLL, dungeonInfo.GetCount())):
			toggleButton = ListToggleButton(self.buttonListThinBoard, key)

			toggleButton.SetKey(key)
			toggleButton.SetText(dungeonInfo.GetMapIndex(key))
			toggleButton.SetIcon(dungeonInfo.GetMapIndex(key))
			if dungeonInfo.GetCooldown(key) > 0:
				toggleButton.SetClock(True, dungeonInfo.GetCooldown(key), True)
			toggleButton.SetEvent(ui.__mem_func__(self.__OnClickListButton), key)

			self.toggleButtonObjList.append(toggleButton)

		if self.toggleButtonObjList:
			self.__OnClickListButton(0)

		if dungeonInfo.GetCount() > dungeonInfo.MAX_DUNGEON_SCROLL:
			self.scrollBar.SetMiddleBarSize(float(dungeonInfo.MAX_DUNGEON_SCROLL) / float(dungeonInfo.GetCount()))
			self.scrollBar.SetScrollEvent(self.OnScroll)
			self.scrollBar.Show()
			self.scrollButton.Show()
		else:
			self.scrollBar.Hide()
			self.scrollButton.Hide()

	"""
	# Server Reply
	"""

	def OnRefreshRanking(self):
		if self.rankType != 0:
			if self.rankWnd:
				self.rankWnd.Refresh(self.rankType)

	def OnOpen(self):
		# Check if buttons aren't already set.
		if not self.toggleButtonObjList:
			# Clear the entire list and reload all data.
			self.Initialize()

	def OnReload(self, onReset):
		# NOTE : Reserved function.
		# In order to keep this function safe we should not reset the entire list, instead we
		# must create a newer function when "/reload dungeon" is called.
		#
		# This function is reserved only for updateing "texts, images and models"
		#

		if onReset:
			self.Initialize()

		if not self.toggleButtonObjList:
			return

		try:
			key = self.listIndex
			self.__OnLoadInformation(key, True) # < key, reloadRender? >
		except IndexError:
			return

	def OnScroll(self):
		if not self.toggleButtonObjList:
			return

		count = dungeonInfo.GetCount()
		scrollLineCount = max(0, count - dungeonInfo.MAX_DUNGEON_SCROLL)
		self.scrollPos = int(scrollLineCount * self.scrollBar.GetPos())

		for key in xrange(min(dungeonInfo.MAX_DUNGEON_SCROLL, dungeonInfo.GetCount())):
			scrollKey = key + self.scrollPos

			try:
				toggleButton = self.toggleButtonObjList[key]

				toggleButton.SetKey(scrollKey)
				toggleButton.SetText(dungeonInfo.GetMapIndex(scrollKey))
				toggleButton.SetIcon(dungeonInfo.GetMapIndex(scrollKey))
				if dungeonInfo.GetCooldown(scrollKey) > 0:
					toggleButton.SetClock(True, dungeonInfo.GetCooldown(scrollKey), True)
				else:
					toggleButton.HideClock()
				toggleButton.SetEvent(ui.__mem_func__(self.__OnClickListButton), key)

			except KeyError:
				return

		self.__OnLoadInformation(self.listIndex, False)

	def GetRealKey(self):
		key = self.listIndex
		try:
			toggleBtn = self.toggleButtonObjList[self.listIndex]
			key = toggleBtn.GetKey()
		except KeyError:
			return key
		return key

	def __OnClickListButton(self, key):
		try:
			toggleBtn = self.toggleButtonObjList[key].GetObject()
		except IndexError:
			return

		for eachToggleBtn in self.toggleButtonObjList:
			eachToggleBtn.GetObject().SetUp()

		realKey = key

		toggleBtn.Down()

		#try:
		#	realKey = self.toggleButtonObjList[key].GetKey()
		#except IndexError:
		#	return

		self.listIndex = realKey

		self.__OnLoadInformation(realKey)

	def OnUpdate(self):
		for eachToggleBtn in self.toggleButtonObjList:
			eachToggleBtn.OnUpdate()

		self.CheckDungeonStatus()

		if self.previewTools:
			renderTarget.SetZoom(RENDER_BACKGROUND_INDEX, self.previewZoom)

	def CheckDungeonStatus(self):
		for key in xrange(len(self.toggleButtonObjList)):
			try:
				toggleBtn = self.toggleButtonObjList[key]
				realKey = toggleBtn.GetKey()

				if dungeonInfo.GetType(realKey) == -1:
					toggleBtn.SetStatus(self.DUNGEON_STATUS_DICT[DUNGEON_CLOSED])

				elif player.GetLevel() < dungeonInfo.GetLevelLimit(realKey, 0):
					toggleBtn.SetStatus(self.DUNGEON_STATUS_DICT[DUNGEON_LEVEL_HIGH])

				elif player.GetLevel() > dungeonInfo.GetLevelLimit(realKey, 1):
					toggleBtn.SetStatus(self.DUNGEON_STATUS_DICT[DUNGEON_LEVEL_LOW])

				else:
					if dungeonInfo.GetCooldown(realKey) > 0:
						toggleBtn.SetStatus(self.DUNGEON_STATUS_DICT[DUNGEON_COOLDOWN])
						toggleBtn.SetClock(True, dungeonInfo.GetCooldown(realKey), True)
					else:
						toggleBtn.SetStatus(self.DUNGEON_STATUS_DICT[DUNGEON_AVAILABLE])
						toggleBtn.HideClock()

			except KeyError:
				return

	def __OnLoadInformation(self, key, reloadRender = False):
		self.previewRender.Show()

		try:
			toggleBtn = self.toggleButtonObjList[key]
			key = toggleBtn.GetKey()
			print key

			self.titleNameText.SetText("%s" % localeInfo.GetMiniMapZoneNameByIdx(dungeonInfo.GetMapIndex(key)))
			self.previewNameText.SetText("%s" % nonplayer.GetMonsterName(dungeonInfo.GetBossVnum(key)))

			self.typeText.SetText(localeInfo.DUNGEON_INFO_TYPE % self.DUNGEON_TYPE_DICT[dungeonInfo.GetType(key)])

			minLevel = dungeonInfo.GetLevelLimit(key, 0)
			maxLevel = dungeonInfo.GetLevelLimit(key, 1)
			self.levelLimitText.SetText(localeInfo.DUNGEON_INFO_LEVEL_LIMIT % (minLevel, maxLevel))

			if not reloadRender:
				if player.GetLevel() < minLevel or player.GetLevel() > maxLevel:
					self.warpButton.Disable()
					self.warpButton.Down()
				else:
					self.warpButton.Enable()
					self.warpButton.SetUp()

			self.memberLimitText.SetText(localeInfo.DUNGEON_INFO_PARTY_LIMIT % (dungeonInfo.GetMemberLimit(key, 0), dungeonInfo.GetMemberLimit(key, 1)))

			if dungeonInfo.GetDuration(key) > 0:
				self.durationText.SetText(localeInfo.DUNGEON_INFO_DURATION % SecondToHMSGolbal(dungeonInfo.GetDuration(key)))
			else:
				self.durationText.SetText(localeInfo.DUNGEON_INFO_DURATION % localeInfo.DUNGEON_INFO_NONE)

			# if dungeonInfo.GetCooldown(key) > 0:
				# self.cooldownText.SetText(localeInfo.DUNGEON_INFO_COOLDOWN % SecondToHMSGolbal(dungeonInfo.GetCooldown(key)))
			# else:
				# self.cooldownText.SetText(localeInfo.DUNGEON_INFO_COOLDOWN % localeInfo.DUNGEON_INFO_NONE)

			self.locationText.SetText(localeInfo.DUNGEON_INFO_LOCATION % localeInfo.GetMiniMapZoneNameByIdx(dungeonInfo.GetMapIndex(key)))
			self.entranceText.SetText(localeInfo.DUNGEON_INFO_ENTRACE % localeInfo.GetMiniMapZoneNameByIdx(dungeonInfo.GetEntryMapIndex(key)))
			self.elementalImg.LoadImage(self.DUNGEON_ELEMENT_DICT[dungeonInfo.GetElement(key)])

			self.totalFinishedText.SetText(localeInfo.DUNGEON_INFO_TOTAL_FINISHED % dungeonInfo.GetFinish(key))
			self.fastestTimeText.SetText(localeInfo.DUNGEON_INFO_FASTEST_TIME % SecondToHMSGolbal(dungeonInfo.GetFinishTime(key)))
			self.highestDamageText.SetText(localeInfo.DUNGEON_INFO_HIGHEST_DMG % NumberToDamageString(dungeonInfo.GetFinishDamage(key)))

			#
			# NOTE : Model update.
			# We don't need to check for the background render.
			renderTarget.SetBackground(RENDER_BACKGROUND_INDEX, "d:/ymir work/ui/game/myshop_deco/model_view_bg.sub")
			#
			# Since this function is called every x seconds we don't want to keep updating the render model because
			# it will keep refreshing (flashing) the model.
			# We must check if the previous model vnum is equal to the newer vnum.
			# <
			renderUpdate = True
			if key in self.previewRenderModelVnum and reloadRender:
				if self.previewRenderModelVnum[key] == dungeonInfo.GetBossVnum(key):
					renderUpdate = False

			if renderUpdate:
				self.previewRenderModelVnum[key] = dungeonInfo.GetBossVnum(key)
				renderTarget.SelectModel(RENDER_BACKGROUND_INDEX, dungeonInfo.GetBossVnum(key))
			# />

			# Required Item Slot
			for slotPos in xrange(self.reqItemSlot.GetSlotCount()):
				itemVnum = dungeonInfo.GetRequiredItemVnum(key, slotPos)
				itemCount = dungeonInfo.GetRequiredItemCount(key, slotPos)

				if itemVnum != 0:
					self.reqItemVnumList[slotPos] = itemVnum
					self.reqItemCountList[slotPos] = itemVnum

					item.SelectItem(itemVnum)
					itemIcon = item.GetIconImage()
					(width, height) = item.GetItemSize()
					self.reqItemSlot.SetSlot(slotPos, 0, width, height, itemIcon, (1.0, 1.0, 1.0, 0.5))
					self.reqItemSlot.SetSlotCount(slotPos, itemCount)
					self.reqItemSlot.HideSlotBaseImage(slotPos)
				else:
					self.reqItemVnumList[slotPos] = 0
					self.reqItemCountList[slotPos] = 0

					self.reqItemSlot.ClearSlot(slotPos)
					self.reqItemSlot.ShowSlotBaseImage(slotPos)

			self.reqItemSlot.RefreshSlot()

			## Box Inventory
			if self.boxWnd and self.showBoxWnd:
				self.RefreshBoxInventory()

		except IndexError:
			return

	# Bind objects.
	def __BindObject(self):
		Child = self.GetChild
		self.board = Child("Board")
		self.titleBar = Child("TitleBar")

		self.boardContainer = Child("BoardContainer")
		self.titleBackgroundImg = Child("TitleBackgroundImage")
		self.titleNameText = Child("TitleNameText")
		self.helpToolTipButton = Child("HelpToolTipButton")

		self.previewBackgroundImg = Child("PreviewBackgroundImg")
		self.previewNameText = Child("PreviewNameText")

		self.previewRenderZoomInButton = Child("PreviewRenderZoomInButton")
		self.previewRenderZoomOutButton = Child("PreviewRenderZoomOutButton")

		self.previewRender = Child("PreviewRender")
		self.previewRender.Hide()

		self.buttonListThinBoard = Child("ButtonListThinBoard")
		self.scrollBar = Child("ScrollBar")
		self.scrollButton = Child("ScrollButton")
		#self.listButton = Child("ListButton")

		self.infoWnd = Child("InformationWindow")
		# self.rankScoreButton = Child("RankScoreButton")
		# self.rankTimeButton = Child("RankTimeButton")
		# self.rankDamageButton = Child("RankDamageButton")

		self.reqItemBackgroundImg = Child("RequiredItemBackgroundImg")
		self.reqItemText = Child("RequiredItemText")
		self.reqItemSlot = Child("RequiredItemSlot")

		self.infoThinBoard = Child("InformationThinBoard")
		self.typeText = Child("TypeText")
		self.levelLimitText = Child("LevelLimitText")
		self.memberLimitText = Child("MemberLimitText")
		self.durationText = Child("DurationText")
		self.cooldownText = Child("CooldownText")
		self.locationText = Child("LocationText")
		self.entranceText = Child("EntraceText")
		self.elementalImg = Child("ElementalImage")
		self.warpButton = Child("WarpButton")
		self.boxButton = Child("BoxButton")
		self.boxWnd = Child("BoxWindow")
		self.boxWnd.Hide()
		self.boxBackgroundImg = Child("BoxBackgroundImg")
		self.boxItemSlot = Child("BoxItemSlot")

		self.myPointsWnd = Child("MyPointsWindow")
		self.myPointsWnd.Hide()

		self.myPointsThinBoard = Child("MyPointsThinBoard")
		self.totalFinishedText = Child("TotalFinishedText")
		self.fastestTimeText = Child("FastestTimeText")
		self.highestDamageText = Child("HighestDamageText")

		self.tabButtonImg = Child("TabButtonImage")
		self.tabButton1 = Child("TabButton1")
		self.tabButton2 = Child("TabButton2")

	# Bind events to objects.
	def __BindEvent(self):
		# Board titlebar.
		if self.titleBar:
			self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		# Required item slots.
		if self.reqItemSlot:
			self.reqItemSlot.SetOverInItemEvent(ui.__mem_func__(self.__SlotOverInItem))
			self.reqItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.__SlotOverOutItem))

		# Help tooltip button.
		if self.helpToolTipButton:
			self.toolTipHelp = self.__CreateGameTypeToolTip(localeInfo.DUNGEON_INFO_TOOL_TIP, self.DUNGEON_HELP_TOOLTIP_DICT)
			self.toolTipHelp.SetTop()
			self.helpToolTipButton.SetToolTipWindow(self.toolTipHelp)

		# Tab buttom image.
		if self.tabButtonImg and self.tabButton1 and self.tabButton2:
			self.tabButton1.SetEvent(ui.__mem_func__(self.__OnClickTabButton), 0)
			self.tabButton2.SetEvent(ui.__mem_func__(self.__OnClickTabButton), 1)

		# Element (bonus) image.
		if self.elementalImg:
			self.elementalImg.OnMouseOverIn = lambda : ui.__mem_func__(self.__OnOverElementImg)()
			self.elementalImg.OnMouseOverOut = lambda : ui.__mem_func__(self.__OnOverOutElementImg)()

		# Rank Buttons.
		# if self.rankScoreButton:
			# self.rankScoreButton.SetEvent(lambda arg = RANK_SCORE : ui.__mem_func__(self.__OnClickRankingButton)(arg))

		# if self.rankTimeButton:
			# self.rankTimeButton.SetEvent(lambda arg = RANK_TIME : ui.__mem_func__(self.__OnClickRankingButton)(arg))

		# if self.rankDamageButton:
			# self.rankDamageButton.SetEvent(lambda arg = RANK_DAMAGE : ui.__mem_func__(self.__OnClickRankingButton)(arg))

		# Warp button.
		if self.warpButton:
			self.warpButton.SetEvent(ui.__mem_func__(self.__OnWarpButton))

		# Box icon button.
		if self.boxButton:
			self.boxButton.SetToggleUpEvent(ui.__mem_func__(self.__OnClickBoxButton))
			self.boxButton.SetToggleDownEvent(ui.__mem_func__(self.__OnClickBoxButton))

		# Box item slot.
		if self.boxItemSlot:
			self.boxItemSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInBoxItem))
			self.boxItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutBoxItem))

		# Preview render zoom buttons.
		if self.previewRenderZoomInButton and self.previewRenderZoomOutButton:
			self.previewRenderZoomInButton.SetToggleUpEvent(ui.__mem_func__(self.__OnRenderZoomIn), False)
			self.previewRenderZoomInButton.SetToggleDownEvent(ui.__mem_func__(self.__OnRenderZoomIn), True)

			self.previewRenderZoomOutButton.SetToggleUpEvent(ui.__mem_func__(self.__OnRenderZoomOut), False)
			self.previewRenderZoomOutButton.SetToggleDownEvent(ui.__mem_func__(self.__OnRenderZoomOut), True)

	def OnLockButtons(self, lock):
		if lock:
			if self.warpButton:
				self.warpButton.Disable()
				self.warpButton.Down()

			# if self.rankScoreButton:
				# self.rankScoreButton.Disable()
				# self.rankScoreButton.Down()

			# if self.rankTimeButton:
				# self.rankTimeButton.Disable()
				# self.rankTimeButton.Down()

			# if self.rankDamageButton:
				# self.rankDamageButton.Disable()
				# self.rankDamageButton.Down()
		else:
			if self.warpButton:
				self.warpButton.Enable()
				self.warpButton.SetUp()

			# if self.rankScoreButton:
				# self.rankScoreButton.Enable()
				# self.rankScoreButton.SetUp()

			# if self.rankTimeButton:
				# self.rankTimeButton.Enable()
				# self.rankTimeButton.SetUp()

			# if self.rankDamageButton:
				# self.rankDamageButton.Enable()
				# self.rankDamageButton.SetUp()

	# On rank button
	def __OnClickRankingButton(self, type):
		if type > 0 and self.listIndex != -1:
			self.rankType = type

			if not self.rankWnd:
				self.rankWnd = DungeonRankingWindow()

			if not self.rankWnd.IsShow():
				self.rankWnd.Show()
				dungeonInfo.Ranking(self.GetRealKey(), self.rankType)

	# On warp button.
	def __OnWarpButton(self):
		key = self.GetRealKey()

		# Get map name.
		mapName = localeInfo.GetMiniMapZoneNameByIdx(dungeonInfo.GetMapIndex(key))

		# Get <min, max> level limit.
		minLevel= dungeonInfo.GetLevelLimit(key, 0)
		maxLevel = dungeonInfo.GetLevelLimit(key, 1)

		# Check player level.
		if player.GetLevel() < minLevel:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DUNGEON_INFO_STATUS_HIGH_LEVEL)
			return

		if player.GetLevel() > maxLevel:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DUNGEON_INFO_STATUS_LOW_LEVEL)
			return

		# Create teleport question dialog.
		self.questionDialog = uiCommon.QuestionDialogWithTimeLimit()
		self.questionDialog.Open(localeInfo.DUNGEON_INFO_DO_YOU_TELEPORT % mapName, 5)
		self.questionDialog.SetAcceptText(localeInfo.UI_ACCEPT)
		self.questionDialog.SetCancelText(localeInfo.UI_DENY)
		self.questionDialog.SetAcceptEvent(lambda arg = True : ui.__mem_func__(self.OnAnswerTeleport)(arg))
		self.questionDialog.SetCancelEvent(lambda arg = False : ui.__mem_func__(self.OnAnswerTeleport)(arg))
		self.questionDialog.key = key
		self.questionDialog.SetCancelOnTimeOver()
		self.questionDialog.SetTop()

	# On answer teleport question dialog.
	def OnAnswerTeleport(self, answer):
		# Check if dialog exists.
		if not self.questionDialog:
			return

		# Check answer.
		if answer == True:
			dungeonInfo.Warp(self.questionDialog.key)

		# Close dialog.
		self.questionDialog.Close()
		self.questionDialog = None

	# Render in zoom.
	def __OnRenderZoomIn(self, trigger):
		# Unset zoom preview.
		self.previewZoom = False
		if trigger:
			self.previewTools = True
			self.previewRenderZoomInButton.Down()
			self.previewRenderZoomOutButton.SetUp()
		else:
			self.previewTools = False
			self.previewRenderZoomInButton.SetUp()

	# Render out zoom.
	def __OnRenderZoomOut(self, trigger):
		# Set zoom preview.
		self.previewZoom = True
		if trigger:
			self.previewTools = True
			self.previewRenderZoomOutButton.Down()
			self.previewRenderZoomInButton.SetUp()
		else:
			self.previewTools = False
			self.previewRenderZoomOutButton.SetUp()

	# Mouse in event of box item.
	def __OnOverInBoxItem(self, slotIndex):
		# Show box item tooltip.
		self.__ShowBoxItemToolTip(slotIndex)

	# Mouse out event of box item.
	def __OnOverOutBoxItem(self):
		# Hide tooltip.
		if self.toolTipItem:
			self.toolTipItem.HideToolTip()

	# Show box item tooltip.
	def __ShowBoxItemToolTip(self, slotIndex):
		# Check if object exists.
		if self.toolTipItem:
			# Clear tooltip data.
			self.toolTipItem.ClearToolTip()

			# Set metin slot.
			metinSlot = []
			for i in xrange(player.METIN_SOCKET_MAX_NUM):
				metinSlot.append(0)

			# Set attr. slot.
			attrSlot = []
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				attrSlot.append((0, 0))

			# Set tooltip item data.
			self.toolTipItem.AddItemData(self.boxItems[slotIndex][0], metinSlot, attrSlot)

	# On running mouse wheel.
	def OnRunMouseWheel(self, nLen):
		if nLen > 0:
			# Set scroll step up.
			if self.scrollBar:
				self.scrollBar.OnUp()
		else:
			# Set scroll step down.
			if self.scrollBar:
				self.scrollBar.OnDown()

	# Hide info window.
	def HideInfoWindow(self):
		# Check if objects exist.
		if self.infoThinBoard and self.elementalImg:
			# Hide info thinboard and info objects.
			self.infoThinBoard.Hide()
			self.elementalImg.Hide()

		# Hide warp button.
		if self.warpButton:
			self.warpButton.Hide()

	# Show info window.
	def ShowInfoWindow(self):
		# Check if objects exist.
		if self.infoThinBoard and self.elementalImg:
			# Show info thinboard and info objects.
			self.infoThinBoard.Show()
			self.elementalImg.Show()

		# Show warp button.
		if self.warpButton:
			self.warpButton.Show()

	# On click box icon button.
	def __OnClickBoxButton(self, refresh = True):
		# Show box window if isn't showing already.
		if not self.showBoxWnd:
			self.OnShowBoxWindow()

			if refresh:
				# Hide info window and refresh box inventory.
				self.HideInfoWindow()
				self.RefreshBoxInventory()

		else:
			self.OnHideBoxWindow()

			if refresh:
				# Show info window.
				self.ShowInfoWindow()

	def OnShowBoxWindow(self):
		self.showBoxWnd = True

		# Show box window and set box icon button down.
		self.boxWnd.Show()
		self.boxButton.Down()

	def OnHideBoxWindow(self):
		self.showBoxWnd = False

		# Hide box window and set box icon button up.
		self.boxButton.SetUp()
		self.boxWnd.Hide()

	# Clear boss box inventory.
	def ClearBoxInventory(self):
		# Clear all slots from box inventory.
		for slotPos in xrange(self.boxItemSlot.GetSlotCount()):
			self.boxItemSlot.ClearSlot(slotPos)
			self.boxItemSlot.HideSlotBaseImage(slotPos)
			self.boxItemSlot.EnableCoverButton(slotPos)

		# Clear box grid.
		if self.boxGrid:
			self.boxGrid.Clear()

		# Clear box item dictionary.
		if self.boxItems:
			self.boxItems = {}

	# Get box grid global slot position.
	#def __GetBoxGridGlobalSlotPos(self, slotPos):
	#	return self.boxScrollPos + slotPos

	# Refresh boss box inventory.
	def RefreshBoxInventory(self):
		# Clear box inventory always on refresh.
		self.ClearBoxInventory()

		key = self.GetRealKey()

		# Get boss drop (item) count.
		bossDropCount = dungeonInfo.GetBossDropCount(key)

		# Check if there are drops.
		if bossDropCount != 0:
			for slotPos in xrange(self.boxItemSlot.GetSlotCount()):
				try:
					globalPos = slotPos #self.__GetBoxGridGlobalSlotPos(slotPos)

					# Get vnum and count of item.
					itemVnum = dungeonInfo.GetBossDropItemVnum(key, globalPos)
					itemCount = dungeonInfo.GetBossDropItemCount(key, globalPos)
					if itemCount <= 1:
						itemCount = 0

				except IndexError:
					return

				# Get item icon and size.
				item.SelectItem(itemVnum)
				itemIcon = item.GetIconImage()
				(width, height) = item.GetItemSize()

				pos = self.boxGrid.FindBlank(width, height)
				if pos == -1:
					break

				self.boxGrid.Put(pos, width, height)
				self.boxItems.update({ pos : [ itemVnum, itemCount] })
				self.boxItemSlot.SetItemSlot(pos, itemVnum, itemCount)
				self.boxItemSlot.HideSlotBaseImage(pos)

			self.boxItemSlot.RefreshSlot()

	# Mouse in event of element (bonus) image.
	def __OnOverElementImg(self):
		# Clean tooltip data.
		self.toolTip.ClearToolTip()

		key = self.GetRealKey()

		# Get att & def bonus count.
		attBonusCount = dungeonInfo.GetAttBonusCount(key)
		defBonusCount = dungeonInfo.GetDefBonusCount(key)

		# Check if there are att. or def. bonuses.
		if attBonusCount != 0 or defBonusCount != 0:
			hasAttBonus = False # Has att. bonus?

			if attBonusCount != 0:
				self.toolTip.SetTitle(localeInfo.DUNGEON_INFO_ELEMENT_ATK_BONUS)

				for index in xrange(attBonusCount):
					# Get affect type of att. bonus.
					affectType = dungeonInfo.GetAttBonus(key, index)
					if affectType != 0 and affectType < 255:
						try:
							desc = AFFECT_DATA[affectType]
						except KeyError:
							desc = "UNKNOWN_KEY[%d]" % affectType

						hasAttBonus = True # Set att. bonus.

						# Show att. bonus text.
						self.toolTip.AutoAppendTextLine(desc, grp.GenerateColor(0.5411, 0.7254, 0.5568, 1.0))
						self.toolTip.AppendSpace(1)

			# Check if there att. or def. bonuses.
			if defBonusCount != 0:
				if hasAttBonus:
					# If there was previously an att. bonus show a horizontal line for def. bonus.
					self.toolTip.AppendHorizontalLine()

				self.toolTip.SetTitle(localeInfo.DUNGEON_INFO_ELEMENT_DEF_AND_RES_BONUS)

				for index in range(0, defBonusCount):
					# Get affect type of def. bonus.
					affectType = dungeonInfo.GetDefBonus(key, index + 1)
					if affectType != 0 and affectType < 255:
						try:
							desc = AFFECT_DATA[affectType]
						except KeyError:
							desc = "UNKNOWN_KEY[%d]" % affectType

						# Show def. bonus text.
						self.toolTip.AutoAppendTextLine(desc, grp.GenerateColor(0.5411, 0.7254, 0.5568, 1.0))
						self.toolTip.AppendSpace(1)
		else:
			# No bonus, show no bonus information.
			self.toolTip.AutoAppendTextLine(localeInfo.DUNGEON_INFO_ELEMENT_UNKOWN_BONUS)
			self.toolTip.AppendSpace(1)

		self.toolTip.TextAlignHorizonalCenter()
		self.toolTip.SetTop()
		self.toolTip.Show()

		#if self.infoThinBoard:
		#	self.infoThinBoard.Hide()

	# Mouse out event of element (bonus) image.
	def __OnOverOutElementImg(self):
		# Hide tooltip.
		if self.toolTip:
			self.toolTip.Hide()

		#if self.infoThinBoard:
		#	self.infoThinBoard.Show()

	# Hide all first tab objects.
	def HideAllFirstTabObjects(self):
		# Hide required item grid.
		if self.reqItemBackgroundImg:
			self.reqItemBackgroundImg.Hide()

		# Hide box icon button.
		if self.boxButton:
			self.boxButton.Hide()

		# Hide entire info thinboard.
		if self.infoThinBoard:
			self.infoThinBoard.Hide()

		# Hide element (bonus) image.
		if self.elementalImg:
			self.elementalImg.Hide()

		# Hide warp button.
		if self.warpButton:
			self.warpButton.Hide()

	# Show all first tab objects.
	def ShowAllFirstTabObjects(self):
		# Show required item grid.
		if self.reqItemBackgroundImg:
			self.reqItemBackgroundImg.Show()

		# Show box icon button.
		if self.boxButton:
			self.boxButton.Show()

		# Show entire info thinboard.
		if self.infoThinBoard:
			self.infoThinBoard.Show()

		# Show element (bonus) image.
		if self.elementalImg:
			self.elementalImg.Show()

		# Show warp button.
		if self.warpButton:
			self.warpButton.Show()

	# On click tab button.
	def __OnClickTabButton(self, index):
		# Prevent clicking the current tab.
		if index == self.tabIndex:
			return

		if index > 0:
			self.tabButtonImg.LoadImage(ROOT + "tab2.png")

			# Is showing box window?
			if self.showBoxWnd:
				self.OnHideBoxWindow()

			# Hide all objects from first tab.
			self.HideAllFirstTabObjects()

			# Show my points window.
			self.myPointsWnd.Show()

		else:
			self.tabButtonImg.LoadImage(ROOT + "tab1.png")

			# Is showing box window?
			if self.showBoxWnd:
				self.OnHideBoxWindow()

			# Show all objects from first tab.
			self.ShowAllFirstTabObjects()

			# Hide my points window.
			self.myPointsWnd.Hide()

	# Create game type info tooltip.
	def __CreateGameTypeToolTip(self, title, descList):
		toolTip = uiToolTip.ToolTip()
		toolTip.SetTitle(title)

		for desc in descList.itervalues():
			if desc == "[ENTER]":
				# Show horizontal line if token is found in desc.
				toolTip.AppendHorizontalLine()
			else:
				toolTip.AutoAppendTextLine(desc)
				toolTip.AppendSpace(1)

		toolTip.TextAlignHorizonalCenter()
		toolTip.SetTop()
		return toolTip

	# Mouse in event of required item.
	def __SlotOverInItem(self, slotIndex):
		# Check if object exists.
		if self.toolTipItem:
			# Show tooltip.
			if self.reqItemVnumList and self.reqItemVnumList[slotIndex] > 0:
				# Set item tooltip.
				self.toolTipItem.SetItemToolTip(self.reqItemVnumList[slotIndex])

	# Mouse out event of required item.
	def __SlotOverOutItem(self):
		# Check if object exists.
		if self.toolTipItem:
			# Hide tooltip.
			self.toolTipItem.HideToolTip()

	# Set item tooltip.
	def SetItemToolTip(self, toolTip):
		self.toolTipItem = toolTip

	# Key escape.
	def OnPressEscapeKey(self):
		# Close board.
		self.Close()
		return True

	# Key exit.
	def OnPressExitKey(self):
		# Close board.
		self.Close()
		return True

	# Close board.
	def Close(self):
		# Hide render target.
		renderTarget.SetVisibility(RENDER_BACKGROUND_INDEX, False)

		# Close dungeon info (data).
		dungeonInfo.Close()

		# Hide ranking board.
		if self.rankWnd:
			self.rankWnd.Hide()

		# Hide board.
		self.Hide()

	def Open(self):
		# Check if window isn't loaded already.
		if not self.isLoaded:
			# Load window.
			self.__LoadWindow()

		# Show render target.
		renderTarget.SetVisibility(RENDER_BACKGROUND_INDEX, True)

		# Open dungeon info (data).
		dungeonInfo.Open()

		# Set board position @ center and on top.
		self.SetCenterPosition()
		self.SetTop()

		# Show board.
		ui.ScriptWindow.Show(self)
