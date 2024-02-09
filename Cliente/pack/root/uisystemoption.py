import dbg
import ui
import snd
import systemSetting
import net
import chat
import app
import localeInfo
import constInfo
import chrmgr
import player
import musicInfo

import uiSelectMusic
import background
import uiPhaseCurtain

import uiCommon

import sys
import uiScriptLocale
import wndMgr

MUSIC_FILENAME_MAX_LEN = 25
if app.ENABLE_GRAPHIC_ON_OFF:
	GRAPHIC_LEVEL_MAX_NUM = 5

if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
	import sys
	import uiScriptLocale
	import wndMgr

	def ReloadModule(name):
		if name in sys.modules:
			del sys.modules[name]

			if name in locals():
				del locals()[name]

			if name in globals():
				del globals()[name]

			module = __import__(name)

			if not module in locals():
				locals()[name] = module

			if not module in globals():
				globals()[name] = module

	LOCALE_LANG_DICT = {
		app.LOCALE_EN : { "name" : uiScriptLocale.LANGUAGE_EN, "locale" : "en", "code_page" : 1252 },
		app.LOCALE_PT : { "name" : uiScriptLocale.LANGUAGE_PT, "locale" : "pt", "code_page" : 1252 },
		app.LOCALE_ES : { "name" : uiScriptLocale.LANGUAGE_ES, "locale" : "es", "code_page" : 1252 },
		app.LOCALE_FR : { "name" : uiScriptLocale.LANGUAGE_FR, "locale" : "fr", "code_page" : 1252 },
		app.LOCALE_DE : { "name" : uiScriptLocale.LANGUAGE_DE, "locale" : "de", "code_page" : 1252 },
		app.LOCALE_RO : { "name" : uiScriptLocale.LANGUAGE_RO, "locale" : "ro", "code_page" : 1250 },
		app.LOCALE_TR : { "name" : uiScriptLocale.LANGUAGE_TR, "locale" : "tr", "code_page" : 1254 },
	}

blockMode = 0

class OptionDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()
		if app.ENABLE_GRAPHIC_ON_OFF:
			self.RefreshPetStatus()
			self.RefreshNpcNameStatus()
		#if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
		#	self.__CreateLanguageSelectWindow()
		if app.ENABLE_ENVIRONMENT_OPTION:
			self.__InitNightModeOption()
			self.__InitSnowModeOption()
			self.__InitSnowTextureModeOption()

#Environment
		self.curtain = uiPhaseCurtain.PhaseCurtain()
		self.curtain.speed = 0.03
		self.curtain.Hide()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE SYSTEM OPTION DIALOG"

	def __Initialize(self):
		if not app.ENABLE_DISABLE_SOFTWARE_TILING:
			self.tilingMode = 0
		self.titleBar = 0
		self.changeMusicButton = 0
		self.selectMusicFile = 0
		self.ctrlMusicVolume = 0
		self.ctrlSoundVolume = 0
		self.musicListDlg = 0
		if not app.ENABLE_DISABLE_SOFTWARE_TILING:
			self.tilingApplyButton = 0
		self.cameraModeButtonList = []
		self.fogModeButtonList = []
		if app.ENABLE_FOG_FIX:
			self.fogButtonList = []
		if app.ENABLE_SHADOW_RENDER_QUALITY_OPTION:
			self.shadowQualityButtonList = []
			self.shadowTargetButtonList = []
		if not app.ENABLE_DISABLE_SOFTWARE_TILING:
			self.tilingModeButtonList = []
		self.ctrlShadowQuality = 0
		if app.ENABLE_ENVIRONMENT_OPTION:
			self.NightModeOption = []
			self.SnowModeOption = []
			self.SnowTextureModeOption = []
		if app.ENABLE_GRAPHIC_ON_OFF:
			self.effectLevel = 0
			self.effectLevelApplyButton = 0
			self.effectLevelButtonList = []

			self.privateShopLevel = 0
			self.privateShopLevelApplyButton = 0
			self.privateShopLevelButtonList = []

			self.dropItemLevel = 0
			self.dropItemLevelApplyButton = 0
			self.dropItemLevelButtonList = []

			self.petStatusButtonList = []
			self.npcNameStatusButtonList = []
		#if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
		#	self.language_change_window = None
		#	self.language_select_button = None
		#	self.language_change_button = None
		#
		#	self.cur_language_text = None
		#	self.cur_language_text_window = None
		#
		#	self.language_button_dict = {}
		#	self.language_select_list_open = False
		#	self.language_select_window_height = 0
		#
		#	self.language_select_window_pos = ((wndMgr.GetScreenWidth() / 2) - 153, (wndMgr.GetScreenHeight() / 2) - 231 - 32)
		#	self.language_select_window_bar = None
		#
		#	self.mouse_over_image = None
		#
		#	self.selected_language = 0

		self.IsShow = False

	def Destroy(self):
		self.ClearDictionary()

		self.__Initialize()
		print " -------------------------------------- DESTROY SYSTEM OPTION DIALOG"

	def __Load_LoadScript(self, fileName):
		try:
			pyScriptLoader = ui.PythonScriptLoader()
			pyScriptLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("System.OptionDialog.__Load_LoadScript")

	def __Load_BindObject(self):
		try:
			GetObject = self.GetChild
			self.titleBar = GetObject("titlebar")
			self.selectMusicFile = GetObject("bgm_file")
			self.changeMusicButton = GetObject("bgm_button")
			self.ctrlMusicVolume = GetObject("music_volume_controller")
			self.ctrlSoundVolume = GetObject("sound_volume_controller")
			self.cameraModeButtonList.append(GetObject("camera_short"))
			self.cameraModeButtonList.append(GetObject("camera_long"))
			if app.ENABLE_GRAPHIC_ON_OFF:
				for i in xrange(1, GRAPHIC_LEVEL_MAX_NUM + 1):
					self.effectLevelButtonList.append(GetObject("effect_level%d" % i))
					self.privateShopLevelButtonList.append(GetObject("privateShop_level%d" % i))
					self.dropItemLevelButtonList.append(GetObject("dropItem_level%d" % i))

				self.effectLevelApplyButton=GetObject("effect_apply")
				self.privateShopLevelApplyButton=GetObject("privateShop_apply")
				self.dropItemLevelApplyButton=GetObject("dropItem_apply")

				self.petStatusButtonList.append(GetObject("pet_on"))
				self.petStatusButtonList.append(GetObject("pet_off"))

				self.npcNameStatusButtonList.append(GetObject("npcName_on"))
				self.npcNameStatusButtonList.append(GetObject("npcName_off"))
			if app.ENABLE_ENVIRONMENT_OPTION:
				self.NightModeOption.append(GetObject("night_mode_off"))
				self.NightModeOption.append(GetObject("night_mode_on"))
				self.SnowModeOption.append(GetObject("snow_mode_off"))
				self.SnowModeOption.append(GetObject("snow_mode_on"))
				self.SnowTextureModeOption.append(GetObject("snow_texture_mode_off"))
				self.SnowTextureModeOption.append(GetObject("snow_texture_mode_on"))
			#if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
			#	self.language_select_button = self.GetChild("language_select_button")
			#	self.language_change_window = self.GetChild("language_change_window")
			#	self.cur_language_text = self.GetChild("cur_language_text")
			#	self.cur_language_text_window = self.GetChild("cur_language_text_window")
			#	self.language_change_button = self.GetChild("language_change_button")

			if app.ENABLE_FOG_FIX:
				self.fogButtonList.append(GetObject("fog_off"))
				self.fogButtonList.append(GetObject("fog_on"))
			else:
				self.fogModeButtonList.append(GetObject("fog_level0"))
				self.fogModeButtonList.append(GetObject("fog_level1"))
				self.fogModeButtonList.append(GetObject("fog_level2"))

			if not app.ENABLE_DISABLE_SOFTWARE_TILING:
				self.tilingModeButtonList.append(GetObject("tiling_cpu"))
				self.tilingModeButtonList.append(GetObject("tiling_gpu"))
				self.tilingApplyButton=GetObject("tiling_apply")
			if app.ENABLE_SHADOW_RENDER_QUALITY_OPTION:
				self.shadowQualityButtonList.append(GetObject("shadow_quality_bad"))
				self.shadowQualityButtonList.append(GetObject("shadow_quality_average"))
				self.shadowQualityButtonList.append(GetObject("shadow_quality_good"))

				self.shadowTargetButtonList.append(GetObject("shadow_target_none"))
				self.shadowTargetButtonList.append(GetObject("shadow_target_ground"))
				self.shadowTargetButtonList.append(GetObject("shadow_target_ground_and_solo"))
				self.shadowTargetButtonList.append(GetObject("shadow_target_all"))
			# self.ctrlShadowQuality = GetObject("shadow_bar")

		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("uiscript/systemoptiondialog.py")
		self.__Load_BindObject()

		self.SetCenterPosition()

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.ctrlMusicVolume.SetSliderPos(float(systemSetting.GetMusicVolume()))
		self.ctrlMusicVolume.SetEvent(ui.__mem_func__(self.OnChangeMusicVolume))

		self.ctrlSoundVolume.SetSliderPos(float(systemSetting.GetSoundVolume()) / 5.0)
		self.ctrlSoundVolume.SetEvent(ui.__mem_func__(self.OnChangeSoundVolume))

#		self.ctrlShadowQuality.SetSliderPos(float(systemSetting.GetShadowLevel()) / 5.0)
#		self.ctrlShadowQuality.SetEvent(ui.__mem_func__(self.OnChangeShadowQuality))

		self.changeMusicButton.SAFE_SetEvent(self.__OnClickChangeMusicButton)

		self.cameraModeButtonList[0].SAFE_SetEvent(self.__OnClickCameraModeShortButton)
		self.cameraModeButtonList[1].SAFE_SetEvent(self.__OnClickCameraModeLongButton)

		if app.ENABLE_FOG_FIX:
			self.fogButtonList[0].SAFE_SetEvent(self.__OnClickFogModeOffButton)
			self.fogButtonList[1].SAFE_SetEvent(self.__OnClickFogModeOnButton)
		else:
			self.fogModeButtonList[0].SAFE_SetEvent(self.__OnClickFogModeLevel0Button)
			self.fogModeButtonList[1].SAFE_SetEvent(self.__OnClickFogModeLevel1Button)
			self.fogModeButtonList[2].SAFE_SetEvent(self.__OnClickFogModeLevel2Button)

		if not app.ENABLE_DISABLE_SOFTWARE_TILING:
			self.tilingModeButtonList[0].SAFE_SetEvent(self.__OnClickTilingModeCPUButton)
			self.tilingModeButtonList[1].SAFE_SetEvent(self.__OnClickTilingModeGPUButton)

			self.tilingApplyButton.SAFE_SetEvent(self.__OnClickTilingApplyButton)

			self.__SetCurTilingMode()

		if app.ENABLE_ENVIRONMENT_OPTION:
			self.NightModeOption[0].SAFE_SetEvent(self.__OnNightModeOffButton)
			self.NightModeOption[1].SAFE_SetEvent(self.__OnNightModeOnButton)

			self.SnowModeOption[0].SAFE_SetEvent(self.__OnSnowModeOffButton)
			self.SnowModeOption[1].SAFE_SetEvent(self.__OnSnowModeOnButton)

			self.SnowTextureModeOption[0].SAFE_SetEvent(self.__OnSnowTextureModeOffButton)
			self.SnowTextureModeOption[1].SAFE_SetEvent(self.__OnSnowTextureModeOnButton)

		self.__ClickRadioButton(self.fogModeButtonList, constInfo.GET_FOG_LEVEL_INDEX())
		self.__ClickRadioButton(self.cameraModeButtonList, constInfo.GET_CAMERA_MAX_DISTANCE_INDEX())

		if app.ENABLE_GRAPHIC_ON_OFF:
			self.__ClickRadioButton(self.effectLevelButtonList, systemSetting.GetEffectLevel())
			self.__ClickRadioButton(self.privateShopLevelButtonList, systemSetting.GetPrivateShopLevel())
			self.__ClickRadioButton(self.dropItemLevelButtonList, systemSetting.GetDropItemLevel())

			self.__ClickRadioButton(self.petStatusButtonList, systemSetting.IsPetStatus())
			self.__ClickRadioButton(self.npcNameStatusButtonList, systemSetting.IsNpcNameStatus())

		if app.ENABLE_FOG_FIX:
			self.__ClickRadioButton(self.fogButtonList, background.GetFogMode())

		if app.ENABLE_SHADOW_RENDER_QUALITY_OPTION:
			self.shadowQualityButtonList[background.SHADOW_QUALITY_BAD].SAFE_SetEvent(self.__OnClickChangeShadowQuality, background.SHADOW_QUALITY_BAD)
			self.shadowQualityButtonList[background.SHADOW_QUALITY_AVERAGE].SAFE_SetEvent(self.__OnClickChangeShadowQuality, background.SHADOW_QUALITY_AVERAGE)
			self.shadowQualityButtonList[background.SHADOW_QUALITY_GOOD].SAFE_SetEvent(self.__OnClickChangeShadowQuality, background.SHADOW_QUALITY_GOOD)
			self.__ClickRadioButton(self.shadowQualityButtonList, systemSetting.GetShadowQualityLevel())

			self.shadowTargetButtonList[background.SHADOW_TARGET_NONE].SAFE_SetEvent(self.__OnClickChangeShadowTarget, background.SHADOW_TARGET_NONE)
			self.shadowTargetButtonList[background.SHADOW_TARGET_GROUND].SAFE_SetEvent(self.__OnClickChangeShadowTarget, background.SHADOW_TARGET_GROUND)
			self.shadowTargetButtonList[background.SHADOW_TARGET_GROUND_AND_SOLO].SAFE_SetEvent(self.__OnClickChangeShadowTarget, background.SHADOW_TARGET_GROUND_AND_SOLO)
			self.shadowTargetButtonList[background.SHADOW_TARGET_ALL].SAFE_SetEvent(self.__OnClickChangeShadowTarget, background.SHADOW_TARGET_ALL)
			self.__ClickRadioButton(self.shadowTargetButtonList, systemSetting.GetShadowTargetLevel())
		#if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
		#	if self.language_select_button:
		#		self.language_select_button.SetEvent(ui.__mem_func__(self.__OnClickLanguageSelectButton))
		#		self.cur_language_text_window.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.__OnClickLanguageSelectButton))
		#
		#	if self.language_change_button:
		#		self.language_change_button.SetEvent(ui.__mem_func__(self.__OnClickLanguageChangeButton))
		#
		#	self.mouse_over_image = self.GetChild("mouse_over_image")
		#	self.mouse_over_image.Hide()

		if musicInfo.fieldMusic==musicInfo.METIN2THEMA:
			self.selectMusicFile.SetText(uiSelectMusic.DEFAULT_THEMA)
		else:
			self.selectMusicFile.SetText(musicInfo.fieldMusic[:MUSIC_FILENAME_MAX_LEN])

		if app.ENABLE_GRAPHIC_ON_OFF:
			for i in xrange(GRAPHIC_LEVEL_MAX_NUM):
				self.effectLevelButtonList[i].SAFE_SetEvent(self.__OnClickEffectLevelButton, i)
				self.privateShopLevelButtonList[i].SAFE_SetEvent(self.__OnClickPrivateShopLevelButton, i)
				self.dropItemLevelButtonList[i].SAFE_SetEvent(self.__OnClickDropItemLevelButton, i)

			self.petStatusButtonList[0].SAFE_SetEvent(self.__OnClickPetStatusButton, 0)
			self.petStatusButtonList[1].SAFE_SetEvent(self.__OnClickPetStatusButton, 1)

			self.npcNameStatusButtonList[0].SAFE_SetEvent(self.__OnClickNpcNameStatusButton, 0)
			self.npcNameStatusButtonList[1].SAFE_SetEvent(self.__OnClickNpcNameStatusButton, 1)

			self.effectLevelApplyButton.SAFE_SetEvent(self.__OnClickEffectApplyButton)
			self.privateShopLevelApplyButton.SAFE_SetEvent(self.__OnClickPrivateShopApplyButton)
			self.dropItemLevelApplyButton.SAFE_SetEvent(self.__OnClickDropItemApplyButton)


	if not app.ENABLE_DISABLE_SOFTWARE_TILING:
		def __OnClickTilingModeCPUButton(self):
			self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_CPU_TILING_1)
			self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_CPU_TILING_2)
			self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_CPU_TILING_3)
			self.__SetTilingMode(0)

		def __OnClickTilingModeGPUButton(self):
			self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_GPU_TILING_1)
			self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_GPU_TILING_2)
			self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_GPU_TILING_3)
			self.__SetTilingMode(1)

		def __OnClickTilingApplyButton(self):
			self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_TILING_EXIT)
			if 0==self.tilingMode:
				background.EnableSoftwareTiling(1)
			else:
				background.EnableSoftwareTiling(0)

			net.ExitGame()

	def __OnClickChangeMusicButton(self):
		if not self.musicListDlg:

			self.musicListDlg=uiSelectMusic.FileListDialog()
			self.musicListDlg.SAFE_SetSelectEvent(self.__OnChangeMusic)

		self.musicListDlg.Open()


	def __ClickRadioButton(self, buttonList, buttonIndex):
		try:
			selButton=buttonList[buttonIndex]
		except IndexError:
			return

		for eachButton in buttonList:
			eachButton.SetUp()

		selButton.Down()


	if not app.ENABLE_DISABLE_SOFTWARE_TILING:
		def __SetTilingMode(self, index):
			self.__ClickRadioButton(self.tilingModeButtonList, index)
			self.tilingMode=index

	def __SetCameraMode(self, index):
		constInfo.SET_CAMERA_MAX_DISTANCE_INDEX(index)
		self.__ClickRadioButton(self.cameraModeButtonList, index)

	def __SetFogLevel(self, index):
		constInfo.SET_FOG_LEVEL_INDEX(index)
		self.__ClickRadioButton(self.fogModeButtonList, index)

	def __OnClickCameraModeShortButton(self):
		self.__SetCameraMode(0)

	def __OnClickCameraModeLongButton(self):
		self.__SetCameraMode(1)

	def __OnClickFogModeLevel0Button(self):
		self.__SetFogLevel(0)

	def __OnClickFogModeLevel1Button(self):
		self.__SetFogLevel(1)

	def __OnClickFogModeLevel2Button(self):
		self.__SetFogLevel(2)

	if app.ENABLE_FOG_FIX:
		def __OnClickFogModeOnButton(self):
			systemSetting.SetFogMode(True)
			background.SetFogMode(True)
			self.__ClickRadioButton(self.fogButtonList, 1)

		def __OnClickFogModeOffButton(self):
			systemSetting.SetFogMode(False)
			background.SetFogMode(False)
			self.__ClickRadioButton(self.fogButtonList, 0)

	if app.ENABLE_ENVIRONMENT_OPTION:
		def __OnNightModeOffButton(self):
			systemSetting.SetNightModeOption(False)
			self.__SetNightMode(0)
			self.__ClickRadioButton(self.NightModeOption, 0)

		def __OnNightModeOnButton(self):
			systemSetting.SetNightModeOption(True)
			self.__SetNightMode(1)
			self.__ClickRadioButton(self.NightModeOption, 1)

		def __SetNightMode(self, index):
			systemSetting.SetNightModeOption(index)

			if 1 == index:
				self.curtain.SAFE_FadeOut(self.__DayMode_OnCompleteChangeToDark)
			else:
				self.curtain.SAFE_FadeOut(self.__DayMode_OnCompleteChangeToLight)

		def __DayMode_OnCompleteChangeToLight(self):
			background.ChangeEnvironmentData(background.DAY_MODE_LIGHT)
			background.SetEnvironmentData(0)
			self.curtain.FadeIn()

		def __DayMode_OnCompleteChangeToDark(self):
			#background.RegisterEnvironmentData(background.DAY_MODE_DARK, constInfo.ENVIRONMENT_NIGHT)
			#background.ChangeEnvironmentData(background.DAY_MODE_DARK)

			background.RegisterEnvironmentData(1, constInfo.ENVIRONMENT_NIGHT)
			background.SetEnvironmentData(1)
			self.curtain.FadeIn()

		def __OnSnowModeOffButton(self):
			systemSetting.SetSnowModeOption(False)
			self.__ClickRadioButton(self.SnowModeOption, 0)

		def __OnSnowModeOnButton(self):
			systemSetting.SetSnowModeOption(True)
			self.__ClickRadioButton(self.SnowModeOption, 1)

		def __OnSnowTextureModeOffButton(self):
			systemSetting.SetSnowTextureModeOption(False)
			self.__ClickRadioButton(self.SnowTextureModeOption, 0)

		def __OnSnowTextureModeOnButton(self):
			systemSetting.SetSnowTextureModeOption(True)
			self.__ClickRadioButton(self.SnowTextureModeOption, 1)

		def __InitNightModeOption(self):
			NightModeOn = systemSetting.GetNightModeOption()
			if NightModeOn:
				self.__ClickRadioButton(self.NightModeOption, 1)
			else:
				self.__ClickRadioButton(self.NightModeOption, 0)

		def __InitSnowModeOption(self):
			SnowModeOn = systemSetting.GetSnowModeOption()
			if SnowModeOn:
				self.__ClickRadioButton(self.SnowModeOption, 1)
			else:
				self.__ClickRadioButton(self.SnowModeOption, 0)

		def __InitSnowTextureModeOption(self):
			SnowTextureModeOn = systemSetting.GetSnowTextureModeOption()
			if SnowTextureModeOn:
				self.__ClickRadioButton(self.SnowTextureModeOption, 1)
			else:
				self.__ClickRadioButton(self.SnowTextureModeOption, 0)


	def __OnChangeMusic(self, fileName):
		self.selectMusicFile.SetText(fileName[:MUSIC_FILENAME_MAX_LEN])

		if musicInfo.fieldMusic != "":
			snd.FadeOutMusic("BGM/"+ musicInfo.fieldMusic)

		if fileName==uiSelectMusic.DEFAULT_THEMA:
			musicInfo.fieldMusic=musicInfo.METIN2THEMA
		else:
			musicInfo.fieldMusic=fileName

		musicInfo.SaveLastPlayFieldMusic()

		if musicInfo.fieldMusic != "":
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

	if app.ENABLE_SHADOW_RENDER_QUALITY_OPTION:
		def __OnClickChangeShadowQuality(self, shadow_quality):
			self.__ClickRadioButton(self.shadowQualityButtonList, shadow_quality)
			self.__SetShadowQualityLevel(shadow_quality)

		def __SetShadowQualityLevel(self, index):
			systemSetting.SetShadowQualityLevel(index)
			background.SetShadowQualityLevel(index)

		def __OnClickChangeShadowTarget(self, shadow_target):
			self.__ClickRadioButton(self.shadowTargetButtonList, shadow_target)
			self.__SetShadowTargetLevel(shadow_target)

		def __SetShadowTargetLevel(self, index):
			systemSetting.SetShadowTargetLevel(index)
			background.SetShadowTargetLevel(index)
	if app.ENABLE_GRAPHIC_ON_OFF:
		def __OnClickEffectLevelButton(self, index):
			self.__ClickRadioButton(self.effectLevelButtonList, index)
			self.effectLevel=index

		def __OnClickEffectApplyButton(self):
			systemSetting.SetEffectLevel(self.effectLevel)

		def __OnClickPrivateShopLevelButton(self, index):
			self.__ClickRadioButton(self.privateShopLevelButtonList, index)
			self.privateShopLevel=index

		def __OnClickPrivateShopApplyButton(self):
			systemSetting.SetPrivateShopLevel(self.privateShopLevel)

		def __OnClickDropItemLevelButton(self, index):
			self.__ClickRadioButton(self.dropItemLevelButtonList, index)
			self.dropItemLevel=index

		def __OnClickDropItemApplyButton(self):
			systemSetting.SetDropItemLevel(self.dropItemLevel)

		def __OnClickPetStatusButton(self, flag):
			self.__ClickRadioButton(self.petStatusButtonList, flag)
			systemSetting.SetPetStatusFlag(flag)
			self.RefreshPetStatus()

		def RefreshPetStatus(self):
			if systemSetting.IsPetStatus():
				self.petStatusButtonList[0].SetUp()
				self.petStatusButtonList[1].Down()
			else:
				self.petStatusButtonList[0].Down()
				self.petStatusButtonList[1].SetUp()

		def __OnClickNpcNameStatusButton(self, flag):
			self.__ClickRadioButton(self.npcNameStatusButtonList, flag)
			systemSetting.SetNpcNameStatusFlag(flag)
			self.RefreshNpcNameStatus()

		def RefreshNpcNameStatus(self):
			if systemSetting.IsNpcNameStatus():
				self.npcNameStatusButtonList[0].SetUp()
				self.npcNameStatusButtonList[1].Down()
			else:
				self.npcNameStatusButtonList[0].Down()
				self.npcNameStatusButtonList[1].SetUp()

	def OnChangeMusicVolume(self):
		pos = self.ctrlMusicVolume.GetSliderPos()
		snd.SetMusicVolume(pos * net.GetFieldMusicVolume())
		systemSetting.SetMusicVolume(pos)

	def OnChangeSoundVolume(self):
		pos = self.ctrlSoundVolume.GetSliderPos()
		snd.SetSoundVolumef(pos)
		systemSetting.SetSoundVolumef(pos)

	def OnChangeShadowQuality(self):
		pos = self.ctrlShadowQuality.GetSliderPos()
		systemSetting.SetShadowLevel(int(pos / 0.2))

	def OnCloseInputDialog(self):
		self.inputDialog.Close()
		self.inputDialog = None
		return True

	def OnCloseQuestionDialog(self):
		self.questionDialog.Close()
		self.questionDialog = None
		return True

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		if not app.ENABLE_DISABLE_SOFTWARE_TILING:
			self.__SetCurTilingMode()
		self.Hide()
		self.IsShow = False

		#if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
		#	self.__LanguageSelectShowHide(False)

	if not app.ENABLE_DISABLE_SOFTWARE_TILING:
		def __SetCurTilingMode(self):
			if background.IsSoftwareTiling():
				self.__SetTilingMode(0)
			else:
				self.__SetTilingMode(1)

	def __NotifyChatLine(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, text)

	if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
		""" Based on dumped meta data"""
		# 2019.12.28.OSP (aka. Owsap)
		# Multi Language
		def LanguageChange(self):
			if self.selected_language != 0:
				locale = LOCALE_LANG_DICT[self.selected_language]["locale"]
				code_page = LOCALE_LANG_DICT[self.selected_language]["code_page"]

				if self.__SaveLoca(code_page, locale):
					# Notes:
					# Selected locale may not load the full pack file, regarding map names, some locale text and other images.
					# A full client restart is required.
					ReloadModule("localeInfo")
					ReloadModule("uiScriptLocale")
					ReloadModule("introLogin")
					ReloadModule("introSelect")
					ReloadModule("introCreate")
					ReloadModule("introEmpire")
					# -------------------------------------------------------------------------------------------------------

					net.SendChangeLanguagePacket(self.selected_language)
					net.ExitGame() # net.ExitGameLanguageChange() # ReloadModule

		#def __AdjustLanguageSelectWindowPosition(self):
		#	x, y = self.language_select_window_pos
		#
		#	(lx, ly) = self.language_change_window.GetLocalPosition()
		#	if self.language_select_window_bar:
		#		self.language_select_window_bar.SetPosition(x + lx + 30, y + ly + 36)

#		def __CreateLanguageSelectWindow(self):
#			if self.language_button_dict:
#				return
#
#			languageList = LOCALE_LANG_DICT
#			if not LOCALE_LANG_DICT:
#				return
#
#			self.cur_language_text.SetText(LOCALE_LANG_DICT[self.__GetCurLanguageKey()]["name"])
#
#			button_height = 16
#			dict_len = len(languageList)
#			self.language_select_window_height = dict_len * button_height
#
#			self.language_select_window_bar = ui.Bar("TOP_MOST")
#			self.language_select_window_bar.SetSize(210, self.language_select_window_height)
#			self.language_select_window_bar.Hide()
#
#			for index, key in enumerate(LOCALE_LANG_DICT):
#				button = ui.Button()
#				button.SetParent(self.language_select_window_bar)
#				button.SetPosition(0, button_height * index)
#
#				if 1 == dict_len:
#					button.SetUpVisual("d:/ymir work/ui/quest_re/button_middle.sub")
#					button.SetDownVisual("d:/ymir work/ui/quest_re/button_middle.sub")
#					button.SetOverVisual("d:/ymir work/ui/quest_re/button_middle.sub")
#				elif index == 0:
#					button.SetUpVisual("d:/ymir work/ui/quest_re/button_middle.sub")
#					button.SetDownVisual("d:/ymir work/ui/quest_re/button_middle.sub")
#					button.SetOverVisual("d:/ymir work/ui/quest_re/button_middle.sub")
#				elif index == dict_len - 1:
#					button.SetUpVisual("d:/ymir work/ui/quest_re/button_bottom.sub")
#					button.SetDownVisual("d:/ymir work/ui/quest_re/button_bottom.sub")
#					button.SetOverVisual("d:/ymir work/ui/quest_re/button_bottom.sub")
#				else:
#					button.SetUpVisual("d:/ymir work/ui/quest_re/button_middle.sub")
#					button.SetDownVisual("d:/ymir work/ui/quest_re/button_middle.sub")
#					button.SetOverVisual("d:/ymir work/ui/quest_re/button_middle.sub")
#
#				button.SetEvent(ui.__mem_func__(self.__OnClickLanguageSelect), key)
#				button.SetOverEvent(ui.__mem_func__(self.__OnClickLanguageButtonOver), key)
#				button.SetOverOutEvent(ui.__mem_func__(self.__OnClickLanguageButtonOverOut), key)
#				button.SetText(LOCALE_LANG_DICT[key]["name"])
#				button.Hide()
#
#				self.language_button_dict[key] = button
#
#			self.mouse_over_image.SetParent(self.language_select_window_bar)
#
#			self.__AdjustLanguageSelectWindowPosition()

		def __GetCurLanguageKey(self):
			for key in LOCALE_LANG_DICT.keys():
				localeName = LOCALE_LANG_DICT[key]["locale"]
				if app.GetLocaleName() == localeName:
					return key

		def __GetStringCurLanguage(self):
			for key in LOCALE_LANG_DICT.keys():
				localeName = LOCALE_LANG_DICT[key]["locale"]
				if app.GetLocaleName() == localeName:
					return localeName

#		def __LanguageSelectShowHide(self, is_show):
#			if True == is_show:
#				self.language_select_list_open = True
#
#				if self.language_select_window_bar:
#					self.language_select_window_bar.SetSize(210, self.language_select_window_height)
#					self.language_select_window_bar.Show()
#
#				for button in self.language_button_dict.values():
#					button.Show()
#			else:
#				self.language_select_list_open = False
#
#				if self.language_select_window_bar:
#					self.language_select_window_bar.SetSize(210, 0)
#
#				for button in self.language_button_dict.values():
#					button.Hide()

#		def __OnClickLanguageButtonOver(self, index):
#			if not self.mouse_over_image:
#				return
#
#			button = self.language_button_dict.get(index, 0)
#			if 0 == button:
#				return
#
#			(button_x, button_y) = button.GetLocalPosition()
#			self.mouse_over_image.SetPosition(button_x, button_y)
#			self.mouse_over_image.Show()

		def __OnClickLanguageButtonOverOut(self, index):
			if not self.mouse_over_image:
				return

			self.mouse_over_image.Hide()

		def __OnClickLanguageChangeButton(self):
			if self.__GetCurLanguageKey() == self.selected_language:
				return

			if self.selected_language != 0:
				if constInfo.MULTI_LANGUAGE_NEED_RESTART_CLIENT == True:
					self.ConfirmLanguageChange()
				else:
					self.LanguageChange()

#		def __OnClickLanguageSelect(self, index):
#			for button in self.language_button_dict.values():
#				button.Hide()
#
#			self.__LanguageSelectShowHide(False)
#
#			self.selected_language = index
#
#			if self.cur_language_text:
#				self.cur_language_text.SetText(LOCALE_LANG_DICT[index]["name"])

		#def __OnClickLanguageSelectButton(self):
		#	self.__CreateLanguageSelectWindow()
		#
		#	if self.language_select_list_open:
		#		self.__LanguageSelectShowHide(False)
		#	else:
		#		self.__LanguageSelectShowHide(True)

		def __OnLanguageSelectScroll(self):
			pass # todo for > 5 languages

		def __SaveLoca(self, code_page, locale):
			if app.SetLoca(code_page, locale):
				return True

			return False

#		def OnTop(self):
#			if self.language_select_window_bar:
#				self.language_select_window_bar.SetTop()

#		def OnMoveWindow(self, x, y):
#			self.language_select_window_pos = x, y
#
#			self.__AdjustLanguageSelectWindowPosition()

		def ConfirmLanguageChange(self):
			questionDialog = uiCommon.QuestionDialog2()
			questionDialog.SetText1(localeInfo.RESTART_CLIENT_DO_YOU_ACCEPT_1)
			questionDialog.SetText2(localeInfo.RESTART_CLIENT_DO_YOU_ACCEPT_2)
			questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnAcceptLanguageChange))
			questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
			questionDialog.SetWidth(450)
			questionDialog.Open()
			self.questionDialog = questionDialog

		def OnEndCountDown(self):
			net.Disconnect()
			app.Exit()
			import os
			os.system("start metin2client.exe")

		def OnPressExitKey(self):
			pass

		def OnAcceptLanguageChange(self):
			self.OnCloseQuestionDialog()

			if self.selected_language != 0:
				locale = LOCALE_LANG_DICT[self.selected_language]["locale"]
				code_page = LOCALE_LANG_DICT[self.selected_language]["code_page"]

				if self.__SaveLoca(code_page, locale):
					net.SendChangeLanguagePacket(self.selected_language)

					import introLogin
					self.popUpTimer = introLogin.ConnectingDialog()
					self.popUpTimer.Open(3.0)
					self.popUpTimer.SetText(localeInfo.LEFT_TIME)
					self.popUpTimer.SAFE_SetTimeOverEvent(self.OnEndCountDown)
					self.popUpTimer.SAFE_SetExitEvent(self.OnPressExitKey)
			else:
				pass # no language selected