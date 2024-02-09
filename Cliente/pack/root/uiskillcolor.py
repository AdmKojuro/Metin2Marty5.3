# (c) 2019 Owsap Productions

import app
import ui
import uiCommon
import grp
import wndMgr
import uiScriptLocale
import localeInfo
import player
import chat
import uiToolTip
import skill
import cfg
import uiCommon
import sys

COLOR_PRESET_SELECT_UI_SHOW_MAX = 10
HEX_CODE_LENGTH = 7
COLOR_PRESET_SAVE_MAX = (COLOR_PRESET_SELECT_UI_SHOW_MAX * (HEX_CODE_LENGTH + 1)) - 1

COLOR_PRESET_TOOLTIP_LIST = [
	localeInfo.SKILL_COLOR_PRESET_TOOLTIP_1,
	localeInfo.SKILL_COLOR_PRESET_TOOLTIP_2,
	localeInfo.SKILL_COLOR_PRESET_TOOLTIP_3
]

CUSTOM_COLOR_HEX_CODE_TOOLTIP_LIST = [
	localeInfo.SKILL_COLOR_CUSTOM_HEX_CODE_TOOLTIP_1,
	localeInfo.SKILL_COLOR_CUSTOM_HEX_CODE_TOOLTIP_2
]

class SkillColorWindow(ui.ScriptWindow):
	def __init__(self, skillSlot, skillIndex):
		ui.Window.__init__(self)
		self.isLoaded = False

		self.skillSlot = skillSlot
		self.skillIndex = skillIndex

		self.pickerPos = (0, 0)
		self.genColor = None
		self.tmpGenColor = None
		self.colorMarker = None
		self.toolTip = None
		self.popup = None
		self.questionDialog = None
		self.selectedColor = False
		self.page = 1

		self.colorPresetDict = {}
		self.colorPresetListOpen = False
		self.colorPresetWindowHeight = 0
		self.colorPresetPreview = False
		self.selectedColorPreset = 0

		self.selectedColorTab = 0
		self.updatesSinceColorUpdate = 0

		self.skillColors = []

	def __del__(self):
		ui.Window.__del__(self)

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/SkillColorWindow.py")
		except:
			import exception
			exception.Abort("SkillColorWindow.__LoadWindow.LoadScriptFile")

		try:
			self.__BindObject()
		except:
			import exception
			exception.Abort("SkillColorWindow.__LoadWindow.__BindObject")

		try:
			self.__BindEvent()
		except:
			import exception
			exception.Abort("SkillColorWindow.__LoadWindow.__BindEvent")

	def __BindObject(self):
		self.GetChild("Board").SetCloseEvent(ui.__mem_func__(self.Close))

		self.thinBoard = self.GetChild("ThinBoard")

		self.bgColorBar = self.GetChild("BGColorBar")
		self.bgImg = self.GetChild("BGImage")

		self.bgColorPickerImg = self.GetChild("BGColorPickerImage")
		self.bgColorPickerButton = self.GetChild("BGColorPickerButton")
		self.bgColorPickerDotImg = self.GetChild("BGColorPickerDotImage")
		self.bgColorPickerDotImg.Hide()

		self.bg2Img = self.GetChild("BG2Image")

		self.bg2ColorPresetButton = self.GetChild("BG2ColorPresetButton")
		self.bg2ColorPresetWindow = self.GetChild("BG2ColorPresetWindow")
		self.bg2ColorPresetEditLine = self.GetChild("BG2ColorPresetEditLine")
		self.bg2ColorPresetToolTip = self.GetChild("BG2ColorPresetToolTip")

		self.bg2ColorPresetSaveButton = self.GetChild("BG2ColorPresetSaveButton")
		self.bg2ColorPresetClearButton = self.GetChild("BG2ColorPresetClearButton")

		self.bg2MouseOverImage = self.GetChild("BG2MouseOverImage")
		self.bg2MouseOverImage.Hide()

		self.bg2CustonColorInputSlotImg = self.GetChild("BG2CustomColorInputSlotImage")
		self.bg2CustomColorEditLine = self.GetChild("BG2CustomColorEditLine")
		self.bg2CustomColorToolTip = self.GetChild("BG2CustomColorToolTip")

		self.defaultButton = self.GetChild("DefaultButton")

		self.prevPageButton = self.GetChild("PrevPageButton")
		self.nextPageButton = self.GetChild("NextPageButton")

		self.confirmButton = self.GetChild("ConfirmButton")
		self.cancelButton = self.GetChild("CancelButton")

		self.bgColorLayerButton = []
		self.bgColorLayerButton.append(self.GetChild("ColorLayer1Button"))
		self.bgColorLayerButton.append(self.GetChild("ColorLayer2Button"))
		self.bgColorLayerButton.append(self.GetChild("ColorLayer3Button"))
		self.bgColorLayerButton.append(self.GetChild("ColorLayer4Button"))
		self.bgColorLayerButton.append(self.GetChild("ColorLayer5Button"))

	def __BindEvent(self):
		if self.bgColorPickerButton:
			self.bgColorPickerButton.SetEvent(ui.__mem_func__(self.OnClickColorPicker))

		if self.confirmButton:
			self.confirmButton.SetEvent(ui.__mem_func__(self.OnClickConfirmButton))

		if self.cancelButton:
			self.cancelButton.SetEvent(ui.__mem_func__(self.OnClickCancelButton))

		if self.defaultButton:
			self.defaultButton.SetEvent(ui.__mem_func__(self.OnClickDefaultButton))

		if self.prevPageButton:
			self.prevPageButton.SetEvent(ui.__mem_func__(self.OnClickPrevButton))

		if self.nextPageButton:
			self.nextPageButton.SetEvent(ui.__mem_func__(self.OnClickNextButton))

		if self.bg2CustomColorEditLine:
			self.bg2CustomColorEditLine.OnIMEUpdate = ui.__mem_func__(self.OnUpdateHex)

		## Color Preset List
		if self.bg2ColorPresetEditLine and self.bg2ColorPresetButton:
			self.bg2ColorPresetEditLine.SetEvent(ui.__mem_func__(self.OnClickColorPresetButton))
			self.bg2ColorPresetButton.SetEvent(ui.__mem_func__(self.OnClickColorPresetButton))
			self.bg2ColorPresetEditLine.SetTextAlignLeft(self.bg2ColorPresetEditLine.GetText(), 2)

		if self.bg2ColorPresetSaveButton:
			self.bg2ColorPresetSaveButton.SetEvent(ui.__mem_func__(self.OnClickSaveColorButton))
			self.bg2ColorPresetSaveButton.Disable()
			self.bg2ColorPresetSaveButton.Down()

		if self.bg2ColorPresetClearButton:
			self.bg2ColorPresetClearButton.SetEvent(ui.__mem_func__(self.OnClickClearColorButton))
			self.bg2ColorPresetClearButton.Disable()
			self.bg2ColorPresetClearButton.Down()

		## ToolTips
		if self.bg2ColorPresetToolTip:
			self.colorPresetToolTip = self.__CreateGameTypeToolTip(localeInfo.SKILL_COLOR_PRESET_TOOLTIP_TITLE, COLOR_PRESET_TOOLTIP_LIST)

		if self.bg2CustomColorToolTip:
			self.customColorToolTip = self.__CreateGameTypeToolTip(localeInfo.SKILL_COLOR_CUSTOM_HEX_CODE_TOOLTIP_TITLE, CUSTOM_COLOR_HEX_CODE_TOOLTIP_LIST)

		i = 0
		for btn in self.bgColorLayerButton:
			btn.SetEvent(ui.__mem_func__(self.OnClickColorTab), i)
			i = i + 1

		self.toolTip = uiToolTip.ToolTip()
		self.toolTip.ClearToolTip()

		self.popup = uiCommon.PopupDialog()

		self.currentSkillColor = []
		tmpSkillColor = player.GetSkillColor(self.skillSlot)
		for tmpColor in tmpSkillColor:
			self.currentSkillColor.append(tmpColor)
			self.skillColors.append(tmpColor)

		self.GetCurrentColor(self.skillSlot)
		self.ReloadSavedColorPreset(self.skillSlot)
		self.ReloadPage()
		self.OnClickColorTab(0)

	def OnClickColorTab(self, colorID):
		for btn in self.bgColorLayerButton:
			btn.Enable()

		self.bgColorLayerButton[colorID].Disable()
		self.selectedColorTab = colorID

		decColor = 0

		if self.currentSkillColor[colorID] > 0:
			decColor = self.currentSkillColor[colorID]

		skillColorCode = hex(decColor).split('x')[-1]
		self.OnUpdateHex(True, skillColorCode)

	def GetCurrentColor(self, skillSlot):
		currentSkillColor = player.GetSkillColor(skillSlot)
		decColor = 0

		if currentSkillColor[0] > 0:
			decColor = currentSkillColor[0]

		elif currentSkillColor[1] > 0:
			decColor = currentSkillColor[1]

		elif currentSkillColor[2] > 0:
			decColor = currentSkillColor[2]

		elif currentSkillColor[3] > 0:
			decColor = currentSkillColor[3]

		elif currentSkillColor[4] > 0:
			decColor = currentSkillColor[4]

		else:
			return # no color data from current skill slot index

		skillColorCode = hex(decColor).split('x')[-1]
		self.OnUpdateHex(True, skillColorCode)

	def GetSavedColorPresets(self, skillSlot):
		skillSlotColorCfg = cfg.Get(cfg.SAVE_SKILL_COLOR, str(skillSlot))
		hexCodeList = skillSlotColorCfg.split()

		tmpColorPresetSaves = []

		index = 0
		while index < len(hexCodeList):
			tmpColorPresetSaves.append(hexCodeList[index])
			index += 1

		return tmpColorPresetSaves

	def OnClickSaveColorButton(self):
		if not self.genColor or not self.selectedColor or not self.tmpGenColor:
			self.popup.SetText(localeInfo.SKILL_COLOR_SELECT_FIRST)
			self.popup.Open()
			return

		r, g, b = (self.tmpGenColor[0], self.tmpGenColor[1], self.tmpGenColor[2])
		hexCode = "#{:02x}{:02x}{:02x}".format(int(r), int(g), int(b))

		skillColorCfg = cfg.Get(cfg.SAVE_SKILL_COLOR, str(self.skillSlot))
		if len(skillColorCfg) >= COLOR_PRESET_SAVE_MAX:
			self.popup.SetText(localeInfo.SKILL_COLOR_CANNOT_SAVE)
			self.popup.Open()
			return

		if len(skillColorCfg) < HEX_CODE_LENGTH:
			cfg.Set(cfg.SAVE_SKILL_COLOR, str(self.skillSlot), hexCode)
		else:
			cfg.Set(cfg.SAVE_SKILL_COLOR, str(self.skillSlot), skillColorCfg + " " + hexCode)

		self.ReloadSavedColorPreset(self.skillSlot)

	def ReloadSavedColorPreset(self, skillColor):
		if self.colorPresetListOpen:
			self.__ColorPresetWindow(False)

		if self.bg2ColorPresetEditLine:
			if not self.GetSavedColorPresets(self.skillSlot):
				self.bg2ColorPresetEditLine.SetText(localeInfo.SKILL_COLOR_SELECT_PRESET_NONE)
			else:
				self.bg2ColorPresetEditLine.SetText(localeInfo.SKILL_COLOR_SELECT_PRESET)

		self.colorPresetDict = {}
		self.colorPresetListOpen = False
		self.colorPresetWindowHeight = 0
		self.colorPresetPreview = False
		self.selectedColorPreset = 0

		self.bg2MouseOverImage.SetParent(self)

	def OnClickClearColorButton(self):
		skillColorPreset = self.GetSavedColorPresets(self.skillSlot)
		selectedColorCode = skillColorPreset[self.selectedColorPreset][:HEX_CODE_LENGTH]

		skillColorCfg = cfg.Get(cfg.SAVE_SKILL_COLOR, str(self.skillSlot))
		skillColorCfgStr = skillColorCfg.replace(selectedColorCode, '')

		cfg.Set(cfg.SAVE_SKILL_COLOR, str(self.skillSlot), skillColorCfgStr)

		self.ClearColors()

		self.ReloadSavedColorPreset(self.skillSlot)

	def ClearColors(self):
		if self.genColor:
			self.genColor = (0, 0, 0)

		if self.tmpGenColor:
			self.tmpGenColor = (0, 0, 0)

		self.selectedColorPreset = 0

		if self.bg2CustomColorEditLine:
			self.bg2CustomColorEditLine.SetText("")

	def OnClickColorPresetButton(self):
		self.__CreateColorPresetButton()

		if self.colorPresetListOpen:
			self.__ColorPresetWindow(False)
		else:
			self.__ColorPresetWindow(True)

	def __ColorPresetWindow(self, show):
		if True == show:
			self.colorPresetListOpen = True
			if self.bg2ColorPresetWindow:
				self.bg2ColorPresetWindow.SetSize(131, self.colorPresetWindowHeight)

			for button in self.colorPresetDict.values():
				button.Show()
		else:
			self.colorPresetListOpen = False
			if self.bg2ColorPresetWindow:
				self.bg2ColorPresetWindow.SetSize(131, 0)

			for button in self.colorPresetDict.values():
				button.Hide()

	def __CreateColorPresetButton(self):
		if not self.bg2ColorPresetWindow:
			return

		if self.colorPresetDict:
			return

		colorPresetList = self.GetSavedColorPresets(self.skillSlot)
		if not colorPresetList:
			return

		buttonHeight = 16
		dictLen = len(colorPresetList)
		self.colorPresetWindowHeight = dictLen * buttonHeight

		for i in xrange(min(COLOR_PRESET_SELECT_UI_SHOW_MAX, dictLen)):
			key = i
			button = ui.Button()
			button.SetParent(self.bg2ColorPresetWindow)
			button.SetPosition(0, buttonHeight * i - 1)

			if 1 == dictLen:
				button.SetUpVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_only.sub")
				button.SetDownVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_only.sub")
				button.SetOverVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_only.sub")
			elif i == 0:
				button.SetUpVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_top.sub")
				button.SetDownVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_top.sub")
				button.SetOverVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_top.sub")
			elif i >= COLOR_PRESET_SELECT_UI_SHOW_MAX - 1:
				button.SetUpVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_bottom.sub")
				button.SetDownVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_bottom.sub")
				button.SetOverVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_bottom.sub")
			else:
				button.SetUpVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_middle.sub")
				button.SetDownVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_middle.sub")
				button.SetOverVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_middle.sub")

			button.SetEvent(ui.__mem_func__(self.OnClickColorPreset), key)
			button.SetOverEvent(ui.__mem_func__(self.OnColorPresetOver), key)
			button.SetOverOutEvent(ui.__mem_func__(self.OnColorPresetOut), key)
			button.SetListText(colorPresetList[key][:HEX_CODE_LENGTH])
			button.Hide()

			self.colorPresetDict[key] = button

		self.bg2MouseOverImage.SetParent(self.bg2ColorPresetWindow)

	def OnClickColorPreset(self, index):
		for button in self.colorPresetDict.values():
			button.Hide()

		self.__ColorPresetWindow(False)

		skillColorPreset = self.GetSavedColorPresets(self.skillSlot)
		skillColorPresetCode = skillColorPreset[index][:HEX_CODE_LENGTH]

		self.selectedColorPreset = index

		if self.bg2ColorPresetEditLine:
			self.bg2ColorPresetEditLine.SetText(skillColorPresetCode)

		if self.bg2ColorPresetClearButton:
			self.bg2ColorPresetClearButton.Enable()
			self.bg2ColorPresetClearButton.SetUp()

		self.OnUpdateHex(True, skillColorPresetCode)

	def OnColorPresetOver(self, index):
		if not self.bg2MouseOverImage:
			return

		button = self.colorPresetDict[index]
		if 0 == button:
			return

		skillColorPreset = self.GetSavedColorPresets(self.skillSlot)
		skillColorPresetCode = skillColorPreset[index][:HEX_CODE_LENGTH]
		self.__PreviewColorPreset(skillColorPresetCode)

		(buttonX, buttonY) = button.GetLocalPosition()
		self.bg2MouseOverImage.SetParent(self.bg2ColorPresetWindow)
		self.bg2MouseOverImage.SetPosition(buttonX, buttonY)
		self.bg2MouseOverImage.Show()

	def OnColorPresetOut(self, index):
		if not self.bg2MouseOverImage:
			return

		self.colorPresetPreview = False

		self.bg2MouseOverImage.Hide()

	def __PreviewColorPreset(self, hexCode):
		if hexCode == "":
			return

		color = str(hexCode).split("#")
		rgbColor = self.HexToRGB(str(color[1]))

		if rgbColor[0] <= 20 and rgbColor[1] <= 20 and rgbColor[2] <= 20:
			rgbColorNew = list(rgbColor)
			rgbColorNew[0] = 0
			rgbColorNew[1] = 0
			rgbColorNew[2] = 0
			rgbColor = tuple(rgbColorNew)

		r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)

		self.colorPresetPreview = True

		if self.bgColorBar:
			self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

	def OnUpdateHex(self, loadPreset = False, hexCode = ""):
		if loadPreset == True:
			text = hexCode
		else:
			ui.EditLine.OnIMEUpdate(self.bg2CustomColorEditLine)
			text = self.bg2CustomColorEditLine.GetText()

		if len(text):
			self.bg2CustomColorEditLine.SetText(str(text))
			self.bgColorPickerDotImg.Hide()

		strLen = len(str(text))
		if strLen >= HEX_CODE_LENGTH - 1:
			if text.find("#") == -1:
				self.bg2CustomColorEditLine.SetText("")
				self.bg2CustomColorEditLine.SetText("#" + str(text) + "")

			color = str(self.bg2CustomColorEditLine.GetText()).split("#")
			rgbColor = self.HexToRGB(str(color[1]))

			if rgbColor[0] <= 20 and rgbColor[1] <= 20 and rgbColor[2] <= 20:
				rgbColorNew = list(rgbColor)
				rgbColorNew[0] = 0
				rgbColorNew[1] = 0
				rgbColorNew[2] = 0
				rgbColor = tuple(rgbColorNew)
				self.selectedColor = False
			else:
				self.selectedColor = True

			r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)
			self.genColor = (r, g, b)

			if self.bgColorBar:
				self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

			if self.bg2ColorPresetSaveButton:
				self.bg2ColorPresetSaveButton.Enable()
				self.bg2ColorPresetSaveButton.SetUp()

			tmpR, tmpG, tmpB = (float(rgbColor[0]), float(rgbColor[1]), float(rgbColor[2]))
			self.skillColors[self.selectedColorTab] = grp.GenerateColor(tmpR / 255, tmpG / 255, tmpB / 255, 0.0) # i think?
			self.currentSkillColor[self.selectedColorTab] = grp.GenerateColor(tmpR / 255, tmpG / 255, tmpB / 255, 0.0) # i think?
			self.tmpGenColor = (tmpR, tmpG, tmpB)
		else:
			if self.bg2ColorPresetSaveButton:
				self.bg2ColorPresetSaveButton.Disable()
				self.bg2ColorPresetSaveButton.Down()

			self.selectedColor = False

	def OnClickPrevButton(self):
		if self.page <= 1:
			return

		self.page -= 1
		self.ReloadPage()

	def OnClickNextButton(self):
		if self.page >= 2:
			return

		self.page += 1
		self.ReloadPage()

	def ReloadPage(self):
		if self.colorPresetListOpen:
			self.__ColorPresetWindow(False)

		page = self.page

		if page >= 2:
			self.prevPageButton.Show()
			self.nextPageButton.Hide()

			self.bgColorPickerImg.Hide()
			self.bg2Img.Show()

			for btn in self.bgColorLayerButton:
				btn.Hide()
		else:
			self.prevPageButton.Hide()
			self.nextPageButton.Show()

			self.bgColorPickerImg.Show()
			self.bg2Img.Hide()

			for btn in self.bgColorLayerButton:
				btn.Show()

	def OnClickCancelButton(self):
		self.Close()

	def OnClickDefaultButton(self):
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.SKILL_COLOR_DO_YOU_RESET)
		questionDialog.SetAcceptEvent(lambda arg = 0 : ui.__mem_func__(self.OnAcceptQuestionDialog)(arg))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def OnClickConfirmButton(self):
		if not self.genColor or not self.selectedColor:
			self.popup.SetText(localeInfo.SKILL_COLOR_SELECT_FIRST)
			self.popup.Open()
			return

		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.SKILL_COLOR_DO_YOU_CHANGE)
		questionDialog.SetAcceptEvent(lambda arg = 1 : ui.__mem_func__(self.OnAcceptQuestionDialog)(arg))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def OnAcceptQuestionDialog(self, arg):
		if arg == 0:
			player.SetSkillColor(self.skillSlot, 0, 0, 0, 0, 0)
		else:
			player.SetSkillColor(self.skillSlot, self.skillColors[0], self.skillColors[1],\
				self.skillColors[2], self.skillColors[3], self.skillColors[4])

		self.Close()

		self.OnCloseQuestionDialog()
		return True

	def OnCloseQuestionDialog(self):
		self.questionDialog.Close()
		self.questionDialog = None
		return True

	def OnPressEscapeKey(self):
		self.Close()
		self.toolTip.Hide()
		return True

	def Close(self):
		self.Hide()

	def Show(self, page = 1):
		ui.ScriptWindow.Show(self)

		if not self.isLoaded:
			self.page = page
			self.__LoadWindow()

		self.SetCenterPosition()
		self.SetTop()

	def OnClickColorPicker(self):
		rgbColor = self.GetRGBColor()

		if rgbColor[0] <= 20 and rgbColor[1] <= 20 and rgbColor[2] <= 20:
			rgbColorNew = list(rgbColor)
			rgbColorNew[0] = 0
			rgbColorNew[1] = 0
			rgbColorNew[2] = 0
			rgbColor = tuple(rgbColorNew)
			self.selectedColor = False
		else:
			self.selectedColor = True

		r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)
		self.genColor = (r, g, b)

		if self.bgColorBar:
			self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

		if self.bg2ColorPresetSaveButton:
			self.bg2ColorPresetSaveButton.Enable()
			self.bg2ColorPresetSaveButton.SetUp()

		if self.bgColorPickerDotImg:
			self.bgColorPickerDotImg.SetPosition(self.pickerPos[0] - (self.bgColorPickerDotImg.GetWidth()/2), self.pickerPos[1] - (self.bgColorPickerDotImg.GetHeight()/2))
			self.bgColorPickerDotImg.Show()

			colorMarker = ui.TextLine()
			colorMarker.SetParent(self.bgColorPickerDotImg)
			colorMarker.SetPosition(5, -15)
			colorMarker.SetHorizontalAlignCenter()
			r, g, b = self.GetRGBColor()
			hexCode = "#{:02x}{:02x}{:02x}".format(int(r), int(g), int(b))
			colorMarker.SetText("%s" % hexCode)
			colorMarker.Show()
			self.colorMarker = colorMarker

		tmpR, tmpG, tmpB = (float(rgbColor[0]), float(rgbColor[1]), float(rgbColor[2]))
		self.skillColors[self.selectedColorTab] = grp.GenerateColor(tmpR / 255, tmpG / 255, tmpB / 255, 0.0)
		self.currentSkillColor[self.selectedColorTab] = grp.GenerateColor(tmpR / 255, tmpG / 255, tmpB / 255, 0.0) # i think?
		self.tmpGenColor = (tmpR, tmpG, tmpB)

		if self.bg2CustomColorEditLine:
			r, g, b = (self.tmpGenColor[0], self.tmpGenColor[1], self.tmpGenColor[2])
			hexCode = "#{:02x}{:02x}{:02x}".format(int(r), int(g), int(b))
			self.bg2CustomColorEditLine.SetText(str(hexCode))

	def HexToRGB(self, strValue):
		strValue = strValue.lstrip("#")
		lv = len(strValue)
		rgbCode = (0, 0 ,0)
		try:
			rgbCode = tuple(int(strValue[i:i+int(lv/3)], 16) for i in range(0, lv, int(lv/3)))
		except:
			pass # unvalid hex string

		return rgbCode

	def GetRGBColor(self):
		xMouse, yMouse = wndMgr.GetRealMousePosition()
		return wndMgr.GetColorAtPosition(xMouse, yMouse)

	def ChangeColor(self, x, y):
		if x > 255:
			x = 255

		if y > 255:
			y = 255

		rgbColor = self.GetRGBColor()
		r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)

		self.updatesSinceColorUpdate = 0

		if self.bgColorBar:
			self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

	def OnUpdate(self):
		self.updatesSinceColorUpdate = self.updatesSinceColorUpdate + 1
		if self.bgColorPickerButton.IsIn():
			xBtn, yBtn = self.bgColorPickerButton.GetGlobalPosition()
			btnHeight = self.bgColorPickerButton.GetHeight()
			xMousePos, yMousePos = wndMgr.GetMousePosition()

			if yMousePos - yBtn < btnHeight - 1:
				xMouse = xMousePos - xBtn
				yMouse = yMousePos - yBtn

				if xMouse > 255:
					xMouse = 255

				if yMouse > 255:
					yMouse = 255

				self.pickerPos = (xMouse, yMouse)
				if self.updatesSinceColorUpdate > 5:
					self.ChangeColor(xMouse, yMouse)

					if self.toolTip:
						try:
							r, g, b = self.GetRGBColor()
							hexCode = "#{:02x}{:02x}{:02x}".format(int(r), int(g), int(b))
							toolTipText = localeInfo.SKILL_COLOR_PICK_TOOLTIP % (skill.GetSkillName(self.skillIndex, 0), hexCode)
							arglen = len(str(toolTipText))
							self.toolTip.ClearToolTip()
							self.toolTip.SetThinBoardSize(5 * arglen)
							self.toolTip.AppendTextLine(toolTipText, 0xffffff00)
							self.toolTip.Show()
						except:
							return # skill.GetSkillName - Failed to find skill index
					else:
						self.toolTip.Hide()
		elif self.bg2ColorPresetToolTip.IsIn():
			if self.colorPresetToolTip:
				self.colorPresetToolTip.Show()
			else:
				self.colorPresetToolTip.Hide()
		elif self.bg2CustomColorToolTip.IsIn():
			if self.customColorToolTip:
				self.customColorToolTip.Show()
			else:
				self.customColorToolTip.Hide()
		else:
			if not self.colorPresetPreview:
				if self.genColor and self.bgColorBar:
					self.bgColorBar.SetColor(grp.GenerateColor(self.genColor[0], self.genColor[1], self.genColor[2], 1.0))
				else:
					self.bgColorBar.SetColor(grp.GenerateColor(0, 0, 0, 0.0))

			if self.toolTip:
				self.toolTip.Hide()

			if self.colorPresetToolTip:
				self.colorPresetToolTip.Hide()

			if self.customColorToolTip:
				self.customColorToolTip.Hide()

	def __CreateGameTypeToolTip(self, title, descList):
		tooltip = uiToolTip.ToolTip()
		tooltip.SetTitle(title)
		tooltip.AppendSpace(7)

		for desc in descList:
			tooltip.AutoAppendTextLine(desc)

		tooltip.AlignHorizonalCenter()
		tooltip.SetTop()
		return tooltip
