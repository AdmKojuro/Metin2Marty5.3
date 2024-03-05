import ui
import uiScriptLocale
import wndMgr
import player
import miniMap
import localeInfo
import net
import app
import colorInfo
import constInfo
import background
import uicommon
import systemSetting
import time
import serverInfo as serverInfo
if app.ENABLE_ATLAS_BOSS:
	import grp
if app.BL_KILL_BAR:
	import playersettingmodule
	import item

class MapTextToolTip(ui.Window):
	def __init__(self):
		ui.Window.__init__(self)

		textLine = ui.TextLine()
		textLine.SetParent(self)
		if not app.ENABLE_ATLAS_BOSS:
			textLine.SetHorizontalAlignCenter()

		textLine.SetOutline()
		if not app.ENABLE_ATLAS_BOSS:
			textLine.SetHorizontalAlignRight()
		else:
			textLine.SetHorizontalAlignLeft()

		textLine.Show()
		self.textLine = textLine
		if app.ENABLE_ATLAS_BOSS:
			textLine2 = ui.TextLine()
			textLine2.SetParent(self)
			textLine2.SetOutline()
			textLine2.SetHorizontalAlignLeft()
			textLine2.Show()
			self.textLine2 = textLine2

	def __del__(self):
		ui.Window.__del__(self)

	def SetText(self, text):
		self.textLine.SetText(text)

	if app.ENABLE_ATLAS_BOSS:
		def SetText2(self, text):
			self.textLine2.SetText(text)

		def ShowText2(self):
			self.textLine2.Show()

		def HideText2(self):
			self.textLine2.Hide()

	def SetTooltipPosition(self, PosX, PosY):
		if app.ENABLE_ATLAS_BOSS:
			PosY -= 24

		if localeInfo.IsARABIC():
			w, h = self.textLine.GetTextSize()
			self.textLine.SetPosition(PosX - w - 5, PosY)
			if app.ENABLE_ATLAS_BOSS:
				self.textLine2.SetPosition(PosX - w - 5, PosY + 10)
		else:
			self.textLine.SetPosition(PosX - 5, PosY)
			if app.ENABLE_ATLAS_BOSS:
				self.textLine2.SetPosition(PosX - 5, PosY + 10)

	def SetTextColor(self, TextColor):
		self.textLine.SetPackedFontColor(TextColor)
		if app.ENABLE_ATLAS_BOSS:
			self.textLine2.SetPackedFontColor(TextColor)

	def GetTextSize(self):
		return self.textLine.GetTextSize()

class AtlasWindow(ui.ScriptWindow):
	BOSS_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 1.0)

	class AtlasRenderer(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.AddFlag("not_pick")

		def __del__(self):
			ui.Window.__del__(self)
		def OnUpdate(self):
			miniMap.UpdateAtlas()

		def OnRender(self):
			(x, y) = self.GetGlobalPosition()
			fx = float(x)
			fy = float(y)
			miniMap.RenderAtlas(fx, fy)

		def HideAtlas(self):
			miniMap.HideAtlas()

		def ShowAtlas(self):
			miniMap.ShowAtlas()

	def __init__(self):
		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Hide()
		self.infoGuildMark = ui.MarkBox()
		self.infoGuildMark.Hide()
		self.AtlasMainWindow = None
		self.mapName = ""
		self.board = 0
		self.questionDialog = None

		ui.ScriptWindow.__init__(self)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def SetMapName(self, mapName):
		if 949==app.GetDefaultCodePage():
			try:
				self.board.SetTitleName(localeInfo.MINIMAP_ZONE_NAME_DICT[mapName])
			except:
				pass

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/AtlasWindow.py")
		except:
			import exception
			exception.Abort("AtlasWindow.LoadWindow.LoadScript")

		try:
			self.board = self.GetChild("board")

		except:
			import exception
			exception.Abort("AtlasWindow.LoadWindow.BindObject")

		self.AtlasMainWindow = self.AtlasRenderer()
		self.board.SetCloseEvent(self.Hide)
		self.AtlasMainWindow.SetParent(self.board)
		self.AtlasMainWindow.SetPosition(7, 30)
		self.tooltipInfo.SetParent(self.board)
		self.infoGuildMark.SetParent(self.board)
		self.SetPosition(wndMgr.GetScreenWidth() - 136 - 256 - 10, 0)
		self.board.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.EventMouseLeftButtonUp))
		if self.questionDialog != None:
			del self.questionDialog
		
		self.questionDialog = uicommon.QuestionDialog2()
		self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCancelQuestion))
		self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnAcceptQuestion))
		self.questionDialog.iPosX = 0
		self.questionDialog.iPosY = 0
		self.questionDialog.Hide()
		self.Hide()

		miniMap.RegisterAtlasWindow(self)

	def Destroy(self):
		miniMap.UnregisterAtlasWindow()
		self.ClearDictionary()
		self.AtlasMainWindow = None
		self.tooltipAtlasClose = 0
		self.tooltipInfo = None
		self.infoGuildMark = None
		self.board = None
		if self.questionDialog != None:
			del self.questionDialog
		
		self.questionDialog = None

	def OnCancelQuestion(self):
		if self.questionDialog == None:
			return
		elif not self.questionDialog.IsShow():
			return
		
		self.questionDialog.iPosX = 0
		self.questionDialog.iPosY = 0
		self.questionDialog.Close()

	def OnAcceptQuestion(self):
		if self.questionDialog == None:
			return
		
		net.SendChatPacket("/gotoxy %d %d" % (self.questionDialog.iPosX, self.questionDialog.iPosY))
		self.OnCancelQuestion()

	def OnMoveWindow(self, x, y):
		self.OnCancelQuestion()

	def EventMouseLeftButtonUp(self):
		(mouseX, mouseY) = wndMgr.GetMousePosition()
		if app.ENABLE_ATLAS_BOSS:
			(bFind, sName, iPosX, iPosY, dwTextColor, dwGuildID, time) = miniMap.GetAtlasInfo(mouseX, mouseY)
		else:
			(bFind, sName, iPosX, iPosY, dwTextColor, dwGuildID) = miniMap.GetAtlasInfo(mouseX, mouseY)
		
		if self.questionDialog.IsShow():
			self.questionDialog.SetTop()
		
		if False == bFind:
			return 1
		
		self.questionDialog.SetText1(localeInfo.ATLASINFO_QUESTIONDIALOG1 % (sName))
		self.questionDialog.SetText2(localeInfo.ATLASINFO_QUESTIONDIALOG2)
		self.questionDialog.iPosX = iPosX
		self.questionDialog.iPosY = iPosY
		self.questionDialog.SetWidth(170 + len(sName * 5))
		self.questionDialog.Open()
		return 1
	def OnUpdate(self):

		if not self.tooltipInfo:
			return

		if not self.infoGuildMark:
			return

		self.infoGuildMark.Hide()
		self.tooltipInfo.Hide()

		if False == self.board.IsIn():
			return

		(mouseX, mouseY) = wndMgr.GetMousePosition()
		if app.ENABLE_ATLAS_BOSS:
			(bFind, sName, iPosX, iPosY, dwTextColor, dwGuildID, time) = miniMap.GetAtlasInfo(mouseX, mouseY)
		else:
			(bFind, sName, iPosX, iPosY, dwTextColor, dwGuildID) = miniMap.GetAtlasInfo(mouseX, mouseY)

		if False == bFind:
			return

		if "empty_guild_area" == sName:
			sName = localeInfo.GUILD_EMPTY_AREA

		if localeInfo.IsARABIC() and sName[-1].isalnum():
			self.tooltipInfo.SetText("(%s)%d, %d" % (sName, iPosX, iPosY))
			if app.ENABLE_ATLAS_BOSS:
				self.tooltipInfo.SetText2(localeInfo.MINIMAP_BOSS_RESPAWN_TIME % (time / 60))
		else:
			self.tooltipInfo.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))
			if app.ENABLE_ATLAS_BOSS:
				self.tooltipInfo.SetText2(localeInfo.MINIMAP_BOSS_RESPAWN_TIME % (time / 60))

		(x, y) = self.GetGlobalPosition()
		self.tooltipInfo.SetTooltipPosition(mouseX - x, mouseY - y)
		if app.ENABLE_ATLAS_BOSS:
			if time > 0:
				self.tooltipInfo.SetTextColor(self.BOSS_COLOR)
				self.tooltipInfo.ShowText2()
			else:
				self.tooltipInfo.SetTextColor(dwTextColor)
				self.tooltipInfo.HideText2()
		else:
			self.tooltipInfo.SetTextColor(dwTextColor)
		self.tooltipInfo.Show()
		self.tooltipInfo.SetTop()

		if 0 != dwGuildID:
			textWidth, textHeight = self.tooltipInfo.GetTextSize()
			self.infoGuildMark.SetIndex(dwGuildID)
			self.infoGuildMark.SetPosition(mouseX - x - textWidth - 18 - 5, mouseY - y)
			self.infoGuildMark.Show()

	def Hide(self):
		if self.AtlasMainWindow:
			self.AtlasMainWindow.HideAtlas()
			self.AtlasMainWindow.Hide()
		
		self.OnCancelQuestion()
		ui.ScriptWindow.Hide(self)

	def Show(self):
		if self.AtlasMainWindow:
			(bGet, iSizeX, iSizeY) = miniMap.GetAtlasSize()
			if bGet:
				self.SetSize(iSizeX + 15, iSizeY + 38)

				if localeInfo.IsARABIC():
					self.board.SetPosition(iSizeX+15, 0)

				self.board.SetSize(iSizeX + 15, iSizeY + 38)
				#self.AtlasMainWindow.SetSize(iSizeX, iSizeY)
				self.AtlasMainWindow.ShowAtlas()
				self.AtlasMainWindow.Show()

		ui.ScriptWindow.Show(self)
		self.SetCenterPosition()

	def SetCenterPositionAdjust(self, x, y):
		self.SetPosition((wndMgr.GetScreenWidth() - self.GetWidth()) / 2 + x, (wndMgr.GetScreenHeight() - self.GetHeight()) / 2 + y)

	def OnPressEscapeKey(self):
		self.Hide()
		return True

def __RegisterMiniMapColor(type, rgb):
	miniMap.RegisterColor(type, rgb[0], rgb[1], rgb[2])

class MiniMap(ui.ScriptWindow):

	CANNOT_SEE_INFO_MAP_DICT = {
		"metin2_map_monkeydungeon" : False,
		"metin2_map_monkeydungeon_02" : False,
		"metin2_map_monkeydungeon_03" : False,
		"metin2_map_devilsCatacomb" : False,
	}

	if app.BL_KILL_BAR:
		KILL_BAR_COOLTIME = 4.0
		KILL_BAR_MOVE_SPEED = 3.0
		KILL_BAR_MOVE_DISTANCE = 33.0
		KILL_BAR_MAX_ITEM = 5

		KILL_BAR_RACE = {
			playersettingmodule.RACE_WARRIOR_M: "|Ekill_bar/warrior_m|e",
			playersettingmodule.RACE_ASSASSIN_W	: "|Ekill_bar/assassin_w|e",
			playersettingmodule.RACE_SURA_M		: "|Ekill_bar/sura_m|e",
			playersettingmodule.RACE_SHAMAN_W	: "|Ekill_bar/shaman_w|e",
			playersettingmodule.RACE_WARRIOR_W	: "|Ekill_bar/warrior_w|e",
			playersettingmodule.RACE_ASSASSIN_M	: "|Ekill_bar/assassin_m|e",
			playersettingmodule.RACE_SURA_W		: "|Ekill_bar/sura_w|e",
			playersettingmodule.RACE_SHAMAN_M	: "|Ekill_bar/shaman_m|e",
		}

		KILL_BAR_WEAPON_TYPE = {
			"FIST": "|Ekill_bar/fist|e",
			item.WEAPON_SWORD: "|Ekill_bar/sword|e",
			item.WEAPON_DAGGER: "|Ekill_bar/dagger|e",
			item.WEAPON_BOW: "|Ekill_bar/bow|e",
			item.WEAPON_TWO_HANDED: "|Ekill_bar/twohand|e",
			item.WEAPON_BELL: "|Ekill_bar/bell|e",
			item.WEAPON_FAN: "|Ekill_bar/fan|e",
		}

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__Initialize()

		miniMap.Create()
		miniMap.SetScale(2.0)

		self.AtlasWindow = AtlasWindow()
		self.AtlasWindow.LoadWindow()
		self.AtlasWindow.Hide()

		self.tooltipMiniMapOpen = MapTextToolTip()
		self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)
		self.tooltipMiniMapOpen.Show()
		self.tooltipMiniMapClose = MapTextToolTip()
		self.tooltipMiniMapClose.SetText(localeInfo.UI_CLOSE)
		self.tooltipMiniMapClose.Show()
		self.tooltipScaleUp = MapTextToolTip()
		self.tooltipScaleUp.SetText(localeInfo.MINIMAP_INC_SCALE)
		self.tooltipScaleUp.Show()
		self.tooltipScaleDown = MapTextToolTip()
		self.tooltipScaleDown.SetText(localeInfo.MINIMAP_DEC_SCALE)
		self.tooltipScaleDown.Show()
		self.tooltipAtlasOpen = MapTextToolTip()
		self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		self.tooltipAtlasOpen.Show()
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.tooltipDungeonInfoOpen = MapTextToolTip()
			self.tooltipDungeonInfoOpen.SetText(localeInfo.DUNGEON_INFO_MINIMAP_TOOL_TIP)
			self.tooltipDungeonInfoOpen.Show()
		if app.ENABLE_BIYOLOG:
			self.tooltipBiolog = MapTextToolTip()
			self.tooltipBiolog.SetText(localeInfo.BIO_TITLE)
			#self.tooltipBiolog.Show()

		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Show()

		if miniMap.IsAtlas():
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		else:
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_CAN_NOT_SHOW_AREAMAP)

		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Show()

		self.mapName = ""

		self.isLoaded = 0
		self.canSeeInfo = True

		# AUTOBAN
		self.imprisonmentDuration = 0
		self.imprisonmentEndTime = 0
		self.imprisonmentEndTimeText = ""
		self.infoValue1 = None
		# END_OF_AUTOBAN

	def __del__(self):
		miniMap.Destroy()
		ui.ScriptWindow.__del__(self)

	def __Initialize(self):
		self.positionInfo = 0
		self.observerCount = 0

		self.OpenWindow = 0
		self.CloseWindow = 0
		self.ScaleUpButton = 0
		self.ScaleDownButton = 0
		self.MiniMapHideButton = 0
		self.MiniMapShowButton = 0
		self.AtlasShowButton = 0

		self.tooltipMiniMapOpen = 0
		self.tooltipMiniMapClose = 0
		self.tooltipScaleUp = 0
		self.tooltipScaleDown = 0
		self.tooltipAtlasOpen = 0
		self.tooltipInfo = None
		self.serverInfo = None
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.interface = None
			self.DungeonInfoShowButton = 0
			self.tooltipDungeonInfoOpen = 0	
		if app.ENABLE_EVENT_MANAGER:
			self.InGameEventButton = 0
			self.interface = None
		if app.ENABLE_BIYOLOG:
			self.tooltipBiolog = None
		if app.BL_KILL_BAR:
			self.KillList = list()
		if app.ENABLE_DATETIME_UNDER_MINIMAP:
			self.minimapclock = 0

	def SetMapName(self, mapName):
		self.mapName=mapName
		self.AtlasWindow.SetMapName(mapName)

		if self.CANNOT_SEE_INFO_MAP_DICT.has_key(mapName):
			self.canSeeInfo = False
			self.HideMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP_CANNOT_SEE)
		else:
			self.canSeeInfo = True
			self.ShowMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)

	# AUTOBAN
	def SetImprisonmentDuration(self, duration):
		self.imprisonmentDuration = duration
		self.imprisonmentEndTime = app.GetGlobalTimeStamp() + duration

		self.__UpdateImprisonmentDurationText()

	def __UpdateImprisonmentDurationText(self):
		restTime = max(self.imprisonmentEndTime - app.GetGlobalTimeStamp(), 0)

		imprisonmentEndTimeText = localeInfo.SecondToDHM(restTime)
		if imprisonmentEndTimeText != self.imprisonmentEndTimeText:
			self.imprisonmentEndTimeText = imprisonmentEndTimeText
			self.serverInfo.SetText("%s: %s" % (uiScriptLocale.AUTOBAN_QUIZ_REST_TIME, self.imprisonmentEndTimeText))
	# END_OF_AUTOBAN

	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			if localeInfo.IsARABIC():
				pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "Minimap.py")
			else:
				pyScrLoader.LoadScriptFile(self, "UIScript/MiniMap.py")
		except:
			import exception
			exception.Abort("MiniMap.LoadWindow.LoadScript")

		try:
			self.OpenWindow = self.GetChild("OpenWindow")
			self.MiniMapWindow = self.GetChild("MiniMapWindow")
			self.ScaleUpButton = self.GetChild("ScaleUpButton")
			self.ScaleDownButton = self.GetChild("ScaleDownButton")
			self.MiniMapHideButton = self.GetChild("MiniMapHideButton")
			self.AtlasShowButton = self.GetChild("AtlasShowButton")
			self.CloseWindow = self.GetChild("CloseWindow")
			self.MiniMapShowButton = self.GetChild("MiniMapShowButton")
			self.positionInfo = self.GetChild("PositionInfo")
			self.observerCount = self.GetChild("ObserverCount")
			self.serverInfo = self.GetChild("ServerInfo")
			self.infoValue1 = self.GetChild("textInfoValue1")
			self.infoValue2 = self.GetChild("textInfoValue2")
			if app.ENABLE_EVENT_MANAGER:
				self.InGameEventButton = self.GetChild("InGameEventButton")
				self.InGameEventButton.SetEvent(ui.__mem_func__(self.ToggleInGameEvent))
			if app.ENABLE_DUNGEON_INFO_SYSTEM:
				self.DungeonInfoShowButton = self.GetChild("DungeonInfoShowButton")
			if app.ENABLE_BIYOLOG:
				self.GetChild("bio").SetEvent(ui.__mem_func__(self.OpenBio))
			if app.ENABLE_DATETIME_UNDER_MINIMAP:
				self.minimapclock = self.GetChild("MiniMapClock")
		except:
			import exception
			exception.Abort("MiniMap.LoadWindow.Bind")

		if constInfo.MINIMAP_POSITIONINFO_ENABLE==0:
			self.positionInfo.Hide()

		if app.ENABLE_DATETIME_UNDER_MINIMAP:
			self.minimapclock.Show()

		self.serverInfo.SetText(net.GetServerInfo())
		self.ScaleUpButton.SetEvent(ui.__mem_func__(self.ScaleUp))
		self.ScaleDownButton.SetEvent(ui.__mem_func__(self.ScaleDown))
		self.MiniMapHideButton.SetEvent(ui.__mem_func__(self.HideMiniMap))
		self.MiniMapShowButton.SetEvent(ui.__mem_func__(self.ShowMiniMap))

		if app.ENABLE_BIYOLOG:
			(ButtonPosX, ButtonPosY) = self.GetChild("bio").GetGlobalPosition()
			self.tooltipBiolog.SetTooltipPosition(ButtonPosX, ButtonPosY)

		if miniMap.IsAtlas():
			self.AtlasShowButton.SetEvent(ui.__mem_func__(self.ShowAtlas))

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.DungeonInfoShowButton.SetEvent(ui.__mem_func__(self.ShowDungeonInfo))

		(ButtonPosX, ButtonPosY) = self.MiniMapShowButton.GetGlobalPosition()
		self.tooltipMiniMapOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.MiniMapHideButton.GetGlobalPosition()
		self.tooltipMiniMapClose.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleUpButton.GetGlobalPosition()
		self.tooltipScaleUp.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleDownButton.GetGlobalPosition()
		self.tooltipScaleDown.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.AtlasShowButton.GetGlobalPosition()
		self.tooltipAtlasOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			(ButtonPosX, ButtonPosY) = self.DungeonInfoShowButton.GetGlobalPosition()
			self.tooltipDungeonInfoOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)

		self.ShowMiniMap()

	if app.ENABLE_BIYOLOG:
		def OpenBio(self):
			self.interface.OpenBiologWindow()

	def Destroy(self):
		self.HideMiniMap()

		self.AtlasWindow.Destroy()
		self.AtlasWindow = None

		if app.BL_KILL_BAR:
			self.KillList = None

		self.infoValue1 = None
		self.infoValue2 = None

		self.ClearDictionary()

		self.__Initialize()

	if app.ENABLE_EVENT_MANAGER:
		def BindInterfaceClass(self, interface):
			self.interface = interface
			
		def ToggleInGameEvent(self):
			self.interface.ToggleInGameEvent()

	def UpdateObserverCount(self, observerCount):
		if observerCount>0:
			self.observerCount.Show()
		elif observerCount<=0:
			self.observerCount.Hide()

		self.observerCount.SetText(localeInfo.MINIMAP_OBSERVER_COUNT % observerCount)

	def OnUpdate(self):
		if app.ENABLE_DATETIME_UNDER_MINIMAP:
			if systemSetting.GetTimePm():
				self.minimapclock.SetText(time.strftime('[%I:%M:%S %p - %d/%m/%Y]'))
			else:
				self.minimapclock.SetText(time.strftime('[%X - %d/%m/%Y]'))
		
		(x, y, z) = player.GetMainCharacterPosition()
		miniMap.Update(x, y)
		
		added = False
		if self.MiniMapWindow.IsIn() == True:
			(mouseX, mouseY) = wndMgr.GetMousePosition()
			(bFind, sName, iPosX, iPosY, dwTextColor) = miniMap.GetInfo(mouseX, mouseY)
			if bFind != 0:
				if self.canSeeInfo:
					self.infoValue1.SetText("%s" % (sName))
					self.infoValue2.SetText("(%d, %d)" % (iPosX, iPosY))
				else:
					self.infoValue1.SetText(sName)
					self.infoValue2.SetText("(%s)" % (localeInfo.UI_POS_UNKNOWN))
				
				self.infoValue1.SetPackedFontColor(dwTextColor)
				self.infoValue2.SetPackedFontColor(dwTextColor)
				added = True
		
		if not added:
			self.infoValue1.SetText(player.GetName())
			self.infoValue1.SetPackedFontColor(-10420)
			self.infoValue2.SetText("(%d, %d)" % (int(x / 100), int(y / 100)))
			self.infoValue2.SetPackedFontColor(-10420)

			# AUTOBAN
			if self.imprisonmentDuration:
				self.__UpdateImprisonmentDurationText()
			# END_OF_AUTOBAN

		if True == self.MiniMapShowButton.IsIn():
			self.tooltipMiniMapOpen.Show()
		else:
			self.tooltipMiniMapOpen.Hide()

		if True == self.MiniMapHideButton.IsIn():
			self.tooltipMiniMapClose.Show()
		else:
			self.tooltipMiniMapClose.Hide()

		if True == self.ScaleUpButton.IsIn():
			self.tooltipScaleUp.Show()
		else:
			self.tooltipScaleUp.Hide()

		if True == self.ScaleDownButton.IsIn():
			self.tooltipScaleDown.Show()
		else:
			self.tooltipScaleDown.Hide()

		if True == self.AtlasShowButton.IsIn():
			self.tooltipAtlasOpen.Show()
		else:
			self.tooltipAtlasOpen.Hide()

		if app.ENABLE_BIYOLOG:
			if True == self.GetChild("bio").IsIn():
				self.tooltipBiolog.Show()
			else:
				self.tooltipBiolog.Hide()
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if True == self.DungeonInfoShowButton.IsIn():
				self.tooltipDungeonInfoOpen.Show()
			else:
				self.tooltipDungeonInfoOpen.Hide()

		if app.BL_KILL_BAR:
			if self.KillList:
				self.KillList = filter(
					lambda obj: obj["CoolTime"] > app.GetTime(), self.KillList)
				for obj in self.KillList:
					(xLocal, yLocal) = obj["ThinBoard"].GetLocalPosition()
					if obj["MOVE_X"] > 0.0:
						obj["ThinBoard"].SetPosition(xLocal - MiniMap.KILL_BAR_MOVE_SPEED, yLocal)
						obj["MOVE_X"] -= MiniMap.KILL_BAR_MOVE_SPEED
					if obj["MOVE_Y"] > 0.0:
						obj["ThinBoard"].SetPosition(xLocal, yLocal + MiniMap.KILL_BAR_MOVE_SPEED)
						obj["MOVE_Y"] -= MiniMap.KILL_BAR_MOVE_SPEED

	def OnRender(self):
		(x, y) = self.GetGlobalPosition()
		fx = float(x)
		fy = float(y)
		miniMap.Render(fx + 4.0, fy + 5.0)

	def Close(self):
		self.HideMiniMap()

	if app.BL_KILL_BAR:
		def RepositionKillBar(self, obj):
			obj["MOVE_Y"] += MiniMap.KILL_BAR_MOVE_DISTANCE
			return obj

		def AddKillInfo(self, killer, victim, killer_race, victim_race, weapon_type):
			if len(self.KillList) >= MiniMap.KILL_BAR_MAX_ITEM:
				self.KillList.sort(
					key=lambda obj: obj["CoolTime"], reverse=True)
				del self.KillList[-1]
			
			if self.KillList:
				self.KillList = map(self.RepositionKillBar, self.KillList)

			TBoard = ui.ThinBoard()
			TBoard.SetParent(self)
			TBoard.SetSize(155, 10)
			TBoard.SetPosition(1, 200)
			TBoard.Show()

			KillText = ui.TextLine()
			KillText.SetText("{} {} {} {} {}".format(MiniMap.KILL_BAR_RACE.get(int(killer_race), ""), killer, MiniMap.KILL_BAR_WEAPON_TYPE.get(
				int(weapon_type), MiniMap.KILL_BAR_WEAPON_TYPE.get("FIST")), victim, MiniMap.KILL_BAR_RACE.get(int(victim_race), "")))
			KillText.SetParent(TBoard)
			KillText.SetWindowHorizontalAlignCenter()
			KillText.SetWindowVerticalAlignCenter()
			KillText.SetHorizontalAlignCenter()
			KillText.SetVerticalAlignCenter()
			KillText.Show()

			KillDict = dict()
			KillDict["ThinBoard"] = TBoard
			KillDict["TextLine"] = KillText
			KillDict["CoolTime"] = app.GetTime() + MiniMap.KILL_BAR_COOLTIME
			KillDict["MOVE_X"] = MiniMap.KILL_BAR_MOVE_DISTANCE
			KillDict["MOVE_Y"] = 0.0

			self.KillList.append(KillDict)

	def HideMiniMap(self):
		miniMap.Hide()
		self.OpenWindow.Hide()
		self.CloseWindow.Show()

		if app.ENABLE_BIYOLOG:
			self.GetChild("bio").Hide()

	def ShowMiniMap(self):
		if not self.canSeeInfo:
			return

		miniMap.Show()
		self.OpenWindow.Show()
		self.CloseWindow.Hide()

		if app.ENABLE_BIYOLOG:
			self.GetChild("bio").Show()

	def isShowMiniMap(self):
		return miniMap.isShow()

	def ScaleUp(self):
		miniMap.ScaleUp()

	def ScaleDown(self):
		miniMap.ScaleDown()

	def ShowAtlas(self):
		if not miniMap.IsAtlas():
			return
		if not self.AtlasWindow.IsShow():
			self.AtlasWindow.Show()

	def ToggleAtlasWindow(self):
		if not miniMap.IsAtlas():
			return
		if self.AtlasWindow.IsShow():
			self.AtlasWindow.Hide()
		else:
			self.AtlasWindow.Show()

	if app.ENABLE_DUNGEON_INFO_SYSTEM:
		def ShowDungeonInfo(self):
			self.interface.ToggleDungeonInfoWindow()

		def BindInterfaceClass(self, interface):
			from _weakref import proxy
			self.interface = proxy(interface)