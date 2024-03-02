import dbg
import app
import net
import ui
import ime
import snd
import wndMgr
import musicinfo
import ServerStateChecker
import serverinfo
import systemSetting
import constinfo
import uicommon
import time
import servercommandparser
import uiscriptlocale
import os
import uiweb
import localeinfo
from _weakref import proxy

g_ip = serverinfo.host

# DISCORDBUTTON = "https://discord.gg/UhwueCdukU/"
# WEBBUTTON = "https://andra.global/"
# BOARDBUTTON = "https://andra.global/"

import _winreg
REG_PATH = r"SOFTWARE\ISRAEL"

def set_reg(name, value):
	try:
		_winreg.CreateKey(_winreg.HKEY_CURRENT_USER, REG_PATH)
		registry_key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, REG_PATH, 0, _winreg.KEY_WRITE)
		_winreg.SetValueEx(registry_key, name, 0, _winreg.REG_SZ, value)
		_winreg.CloseKey(registry_key)
		return True
	except WindowsError:
		return False

def get_reg(name):
	try:
		registry_key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, REG_PATH, 0, _winreg.KEY_READ)
		value, regtype = _winreg.QueryValueEx(registry_key, name)
		_winreg.CloseKey(registry_key)
		return str(value)
	except WindowsError:
		return None
		
class LoginWindow(ui.ScriptWindow):
	def __init__(self, stream):
		ui.ScriptWindow.__init__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
		net.SetAccountConnectorHandler(self)
		if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
			self.languageList = [ None, None, None, None, None, None, None, None, None, None ] #x10 languages
		self.popupX = 520
		self.popupY = 420
		self.stream = stream
		self.isHide = 1
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
		net.ClearPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
		net.SetAccountConnectorHandler(0)

	

	def Open(self):
		# dracaryS-28042020
		ServerStateChecker.Initialize()
		ServerStateChecker.Create(self)
		ServerStateChecker.AddChannel(1, serverinfo.host, 30003)
		ServerStateChecker.AddChannel(2, serverinfo.host, 30257)
		ServerStateChecker.AddChannel(3, serverinfo.host, 30251)
		ServerStateChecker.AddChannel(4, serverinfo.host, 30255)
		ServerStateChecker.Request()

		self.loginFailureMsgDict={
			"ALREADY"	: localeinfo.LOGIN_FAILURE_ALREAY,
			"NOID"		: localeinfo.LOGIN_FAILURE_NOT_EXIST_ID,
			"WRONGPWD"	: localeinfo.LOGIN_FAILURE_WRONG_PASSWORD,
			"FULL"		: localeinfo.LOGIN_FAILURE_TOO_MANY_USER,
			"SHUTDOWN"	: localeinfo.LOGIN_FAILURE_SHUTDOWN,
			"REPAIR"	: localeinfo.LOGIN_FAILURE_REPAIR_ID,
			"BLOCK"		: localeinfo.LOGIN_FAILURE_BLOCK_ID,
			"WRONGMAT"	: localeinfo.LOGIN_FAILURE_WRONG_MATRIX_CARD_NUMBER,
			"QUIT"		: localeinfo.LOGIN_FAILURE_WRONG_MATRIX_CARD_NUMBER_TRIPLE,
			"BESAMEKEY"	: localeinfo.LOGIN_FAILURE_BE_SAME_KEY,
			"NOTAVAIL"	: localeinfo.LOGIN_FAILURE_NOT_AVAIL,
			"NOBILL"	: localeinfo.LOGIN_FAILURE_NOBILL,
			"BLKLOGIN"	: localeinfo.LOGIN_FAILURE_BLOCK_LOGIN,
			"WEBBLK"	: localeinfo.LOGIN_FAILURE_WEB_BLOCK,
		}

		# self.loginFailureMsgDict["WRONGHWI"] = localeinfo.LOGIN_FAILURE_WRONG_HWID

		self.loginFailureFuncDict = {
			"WRONGPWD"	: localeinfo.LOGIN_FAILURE_WRONG_PASSWORD,
			"WRONGMAT"	: localeinfo.LOGIN_FAILURE_WRONG_MATRIX_CARD_NUMBER,
			"QUIT"		: app.Exit,
		}

		self.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
		self.SetWindowName("LoginWindow")

		self.__LoadScript("israel/loginwindow/loginwindow.py")
		
		if musicinfo.loginMusic != "":
			snd.SetMusicVolume(systemSetting.GetMusicVolume())
			snd.FadeInMusic("BGM/" + musicinfo.loginMusic)

		snd.SetSoundVolume(systemSetting.GetSoundVolume())
		self.CheckAccount()

		ime.AddExceptKey(91)
		ime.AddExceptKey(93)
		self.SetChannel(0)
		if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
			self.RefreshLanguagesBtn()
		self.Show()
		app.ShowCursor()

	#if app.ENABLE_MULTI_LANGUAGE_SYSTEM_SYSTEM:
	#	import uiSystemOption
	#	localeName = app.GetLoca()
	#	for i, k in uiSystemOption.LOCALE_LANG_DICT.items():
	#		if localeName in uiSystemOption.LOCALE_LANG_DICT[i]["locale"]:
	#			net.SetLanguage(i)
	#			break

	def Close(self):
		# dracaryS-28042020
		ServerStateChecker.Initialize()

		if musicinfo.loginMusic != "" and musicinfo.selectMusic != "":
			snd.FadeOutMusic("BGM/"+musicinfo.loginMusic)

		if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
			self.languageList = [ None, None, None, None, None, None, None, None, None, None ] #x10 languages

		if self.stream.popupWindow:
			self.stream.popupWindow.Close()

		self.Hide()
		app.HideCursor()
		ime.ClearExceptKey()

	def OnConnectFailure(self):
		snd.PlaySound("sound/ui/loginfail.wav")
		self.PopupNotifyMessage(localeinfo.LOGIN_CONNECT_FAILURE, self.EmptyFunc)

	def OnHandShake(self):
		snd.PlaySound("sound/ui/loginok.wav")
		self.PopupDisplayMessage(localeinfo.LOGIN_CONNECT_SUCCESS)

	def OnLoginStart(self):
		self.PopupDisplayMessage(localeinfo.LOGIN_PROCESSING)

	def OnLoginFailure(self, error):
		try:
			loginFailureMsg = self.loginFailureMsgDict[error]
		except KeyError:
		
			loginFailureMsg = localeinfo.LOGIN_FAILURE_UNKNOWN  + error

		loginFailureFunc = self.loginFailureFuncDict.get(error, self.EmptyFunc)

		self.PopupNotifyMessage(loginFailureMsg, loginFailureFunc)

		snd.PlaySound("sound/ui/loginfail.wav")

		# if error == "WRONG_CL":
			# os.system("start Autopatcher.exe")
			# app.Exit()

	def __LoadScript(self, fileName):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("LoginWindow.__LoadScript.LoadObject")

		try:
			self.board_main	= self.GetChild("board_main")
			self.idEditLine = self.GetChild("id")
			self.pwdEditLine = self.GetChild("pwd")
			self.pwdHideText = self.GetChild("HideText")
			self.pwdHideText.SetEvent(ui.__mem_func__(self.__OnClickHideText))
			self.loginButton = self.GetChild("login_button")
			self.exitButton = self.GetChild("exit_button")
			# self.exitButton1 = self.GetChild("exit_button1")
			self.channelButton = {
				0 : self.GetChild("ch1"),
				1 :	self.GetChild("ch2"),
				2 : self.GetChild("ch3"),
				3 : self.GetChild("ch4"),}
			
			self.accountData = {
				0 : [[self.GetChild("delete_button_0"), self.GetChild("save_button_0"), self.GetChild("load_button_0")], self.GetChild("account_0_text")],
				1 : [[self.GetChild("delete_button_1"), self.GetChild("save_button_1"), self.GetChild("load_button_1")], self.GetChild("account_1_text")],
				2 : [[self.GetChild("delete_button_2"), self.GetChild("save_button_2"), self.GetChild("load_button_2")], self.GetChild("account_2_text")],
				3 : [[self.GetChild("delete_button_3"), self.GetChild("save_button_3"), self.GetChild("load_button_3")], self.GetChild("account_3_text")]}
		except:
			import exception
			exception.Abort("LoginWindow.__LoadScript.BindObject")
			
		for (key, item) in list(self.accountData.items()):
			if isinstance(item[0], list):
				item[0][0].SetEvent(ui.__mem_func__(self.DeleteAccount), key)
				item[0][1].SetEvent(ui.__mem_func__(self.SaveAccount), key)
				item[0][2].SetEvent(ui.__mem_func__(self.LoadAccount), key)

		for (channelID, channelButtons) in list(self.channelButton.items()):
			channelButtons.SetEvent(ui.__mem_func__(self.SetChannel), channelID)
		self.loginButton.SetEvent(ui.__mem_func__(self.__OnClickLoginButton))
		self.exitButton.SetEvent(ui.__mem_func__(self.OnPressExitKey))
		# self.exitButton1.SetEvent(ui.__mem_func__(self.OnPressExitKey1))
		# self.clickOnTextLinks()
		self.idEditLine.SetReturnEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))
		self.idEditLine.SetTabEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))
		self.pwdEditLine.SetReturnEvent(ui.__mem_func__(self.__OnClickLoginButton))
		self.pwdEditLine.SetTabEvent(ui.__mem_func__(self.idEditLine.SetFocus))
		self.idEditLine.SetFocus()
			
		GetObject=self.GetChild
		# self.DiscordButton = self.GetChild("discord_btn")
		# self.DiscordButton.SetEvent(ui.__mem_func__(self.__OnClickDiscordButton))
		
		# self.ForgotButton = self.GetChild("web_btn")
		# self.ForgotButton.SetEvent(ui.__mem_func__(self.__OnClickWebButton))

		# self.ForgotButton = self.GetChild("board_btn")
		# self.ForgotButton.SetEvent(ui.__mem_func__(self.__OnClickBoardButton))
		if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
			self.languageList[0] = GetObject("btn_lang_en")
			self.languageList[1] = GetObject("btn_lang_ro")
			self.languageList[2] = GetObject("btn_lang_it")
			self.languageList[3] = GetObject("btn_lang_tr")
			self.languageList[4] = GetObject("btn_lang_de")
			self.languageList[5] = GetObject("btn_lang_pl")
			self.languageList[6] = GetObject("btn_lang_pt")
			self.languageList[7] = GetObject("btn_lang_es")
			self.languageList[8] = GetObject("btn_lang_cz")
			self.languageList[9] = GetObject("btn_lang_hu")
		self.languageList[0].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("en"))
		self.languageList[1].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("ro"))
		self.languageList[2].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("it"))
		self.languageList[3].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("tr"))
		self.languageList[4].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("de"))
		self.languageList[5].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("pl"))
		self.languageList[6].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("pt"))
		self.languageList[7].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("es"))
		self.languageList[8].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("cz"))
		self.languageList[9].SetEvent(lambda:ui.__mem_func__(self.__QuestionChangeLanguage)("hu"))

	if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
		def RefreshLanguagesBtn(self):
			langs = ["en", "ro", "it", "tr", "de", "pl", "pt", "es", "cz", "hu"]
			for i in xrange(10):
				if app.GetLocaleName() != langs[i]:
					self.languageList[i].SetUp()
				else:
					self.languageList[i].Down()

		def OnClickLanguageButton(self, lang):
			if app.GetLocaleName() != lang:
				with open("locale.cfg", "w+") as localeConfig:
					localeConfig.write("10022 %d %s" % (self.GetCodePage(lang), lang))
				
				app.SetDefaultCodePage(self.GetCodePage(lang))
				app.ForceSetLocale(lang, "locale/%s" % (lang))
				app.ShellExecute("Andra.exe")
				self.OnCloseQuestionDialog()
				app.Exit()

		def OnCloseQuestionDialog(self):
			if not self.questionDialog:
				return
			
			self.RefreshLanguagesBtn()
			self.questionDialog.Close()
			self.questionDialog = None
			constinfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

		def __QuestionChangeLanguage(self, lang):
			if app.GetLocaleName() == lang:
				return
			
			langs = ["en", "ro", "it", "tr", "de", "pl", "pt", "es", "cz", "hu"]
			for i in xrange(10):
				if lang != langs[i]:
					self.languageList[i].SetUp()
				else:
					self.languageList[i].Down()
			
			self.stream.popupWindow.Close()
			self.questionDialog = uicommon.QuestionDialog()
			self.questionDialog.SetText(localeinfo.QUESTION_DLG_CHANGELANGUAGE)
			self.questionDialog.SetAcceptEvent(lambda arg = lang: self.OnClickLanguageButton(arg))
			self.questionDialog.SetCancelEvent(self.OnCloseQuestionDialog)
			self.questionDialog.Open()
			self.questionDialog.SetPosition(self.popupX - 29, self.popupY - 29)
			constinfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
			
	def GetCodePage(self, lang):
		codePageDict = {
			"en" : 1252,
			"ro" : 1250,
			"it" : 1252,
			"tr" : 1254,
			"de" : 1252,
			"pl" : 1250,
			"pt" : 1252,
			"es" : 1252,
			"cz" : 1250,
			"hu" : 1250,
		}
		try:
			return codePageDict[lang]
		except:
			return 1252

	def GetIndexByName(self, lang):
		langCode = {
			"en" : 0, # 0
			"ro" : 1, # 1
			"it" : 2, # 2
			"tr" : 3, # 3
			"de" : 4, # 4
			"pl" : 5, # 5
			"pt" : 6, # 6
			"es" : 7, # 7
			"cz" : 8, # 8
			"hu" : 9, # 9
		}
		try:
			return langCode[lang]
		except:
			return 2


	def __ClickRadioButton(self, buttonList, buttonIndex):
		try:
			selButton = buttonList[buttonIndex]
		except IndexError:
			return
		for eachButton in buttonList:
			eachButton.SetUp()
		selButton.Down()

	def __OnClickHideText(self):
		self.pwdEditLine.SetFocus()
		if self.isHide == 1:
			self.pwdEditLine.SetSecret(False)
			text = self.pwdEditLine.GetText()
			self.pwdEditLine.SetText("")
			ime.PasteString(text)
			self.isHide=0
		else:
			self.pwdEditLine.SetSecret(True)
			text = self.pwdEditLine.GetText()
			self.pwdEditLine.SetText("")
			ime.PasteString(text)
			self.isHide=1

	def CheckAccount(self):
		for i in range(4):
			if get_reg("id_%d" % i):
				self.accountData[i][1].SetText(str(get_reg("id_%d" % i)))
				self.accountData[i][0][1].Hide()
				self.accountData[i][0][0].Show()
			else:
				self.accountData[i][1].SetText(uiscriptlocale.LOGIN_ACCOUNT_EMPTY)
				self.accountData[i][0][1].Show()
				self.accountData[i][0][0].Hide()
				
	def DeleteAccount(self, key):
		if get_reg("id_%d" % key):
			set_reg("id_%d" % key, "")
			set_reg("pwd_%d" % key, "")
			self.PopupNotifyMessage(uiscriptlocale.LOGIN_ACCOUNT_DELETE)
		else:
			self.PopupNotifyMessage(uiscriptlocale.LOGIN_ACCOUNT_D_S)
			
		self.CheckAccount()
		
	def LoadAccount(self, key):
		if get_reg("id_%d" % key):
			self.idEditLine.SetText(str(get_reg("id_%d" % key)))
			self.pwdEditLine.SetText(str(get_reg("pwd_%d" % key)))
			self.pwdEditLine.SetFocus()
		else:
			self.PopupNotifyMessage(uiscriptlocale.LOGIN_ACCOUNT_N_S)
			
	def SaveAccount(self, key):
		if get_reg("id_%d" % key):
			self.PopupNotifyMessage(uiscriptlocale.LOGIN_ACCOUNT_N_S_D)
			return
		
		if self.idEditLine.GetText() == "" or self.pwdEditLine.GetText() == "":
			self.PopupNotifyMessage(uiscriptlocale.LOGIN_ACCOUNT_SAVE)
			return
		
		set_reg("id_%d" % key, self.idEditLine.GetText())
		set_reg("pwd_%d" % key, self.pwdEditLine.GetText())
		self.PopupNotifyMessage(uiscriptlocale.LOGIN_ACCOUNT_SAVED)
		self.CheckAccount()

	def SetChannel(self, ch):
		for key, button in list(self.channelButton.items()):
			button.SetUp()
			
		self.channelButton[ch].Down()

		self.stream.SetConnectInfo(g_ip, self.ChannelPort(ch, 0), g_ip, self.ChannelPort("LOGIN"))
		net.SetMarkServer(g_ip, self.ChannelPort("LOGO"))
		app.SetGuildMarkPath("10.tga")
		app.SetGuildSymbolPath("10")
		net.SetServerInfo(self.ChannelPort(ch, 2))
		
	def ChannelPort(self, ch, value=0):
		channel = {
			0	:	30003,
			1	:	30257,
			2	:	30251,
			3	:	30255,}
		if ch == "LOGIN":
			return 30001
		elif ch == "LOGO":
			return channel[0]
		elif value == 2:
			return "ISRAEL, Channel %s" % (ch+1)
		else:
			return channel[ch]
				
	def Connect(self, id, pwd):
		if constinfo.SEQUENCE_PACKET_ENABLE:
			net.SetPacketSequenceMode()
		constinfo.id = id
		constinfo.pwd = pwd
			
		constinfo.LastAccount = id.lower()
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(localeinfo.LOGIN_CONNETING, self.EmptyFunc, localeinfo.UI_CANCEL)

		self.stream.SetLoginInfo(id, pwd)
		self.stream.Connect()
		
	def PopupDisplayMessage(self, msg):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg)

	def PopupNotifyMessage(self, msg, func=0):
		if not func:
			func = self.EmptyFunc
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, func, localeinfo.UI_OK)


	# def OnPressExitKey1(self):
		# if self.stream.popupWindow:
			# self.stream.popupWindow.Close()
		# self.stream.SetPhaseWindow(0)
		# return TRUE
		
	def OnPressExitKey(self):
		if self.stream.popupWindow:
			self.stream.popupWindow.Close()
		self.stream.SetPhaseWindow(0)
		return TRUE

	def EmptyFunc(self):
		pass

	# def __OnClickDiscordButton(self):
		# os.system("start " + DISCORDBUTTON)

	# def __OnClickWebButton(self):
		# os.system("start " + WEBBUTTON)

	# def __OnClickBoardButton(self):
		# os.system("start " + BOARDBUTTON)

	def __OnClickLoginButton(self):
		id = self.idEditLine.GetText()
		pwd = self.pwdEditLine.GetText()

		if len(id)==0:
			self.PopupNotifyMessage(localeinfo.LOGIN_INPUT_ID, self.EmptyFunc)
			return

		if len(pwd)==0:
			self.PopupNotifyMessage(localeinfo.LOGIN_INPUT_PASSWORD, self.EmptyFunc)
			return

		self.Connect(id, pwd)

	def AskGuardCode(self):
		GuardInput = uiCommon.InputDialog()
		GuardInput.SetTitle("Enter the code sent to your e-mail address")
		GuardInput.SetAcceptEvent(ui.__mem_func__(self.ConfirmGuardCode))
		GuardInput.SetCancelEvent(ui.__mem_func__(self.CancelGuardCode))
		GuardInput.Open()

		self.GuardInput = GuardInput

	def ConfirmGuardCode(self):
		self.__OnClickLoginButton()

		self.GuardInput.Close()
		self.GuardInput = None
		return True

	def CancelGuardCode(self):
		self.GuardInput.Close()
		self.GuardInput = None
		return True

	def CheckGuardCode(self):
		if constinfo.GUARD_TRY_COUNT == 3:
			self.PopupNotifyMessage("You already tried too much", self.__ExitGame)

		constinfo.GUARD_TRY_COUNT += 1

	# dracaryS-28042020
	def __LoginDirectly(self, j):
		self.LoadAccount(j-1)
		self.__OnClickLoginButton()
	def OnKeyDown(self, key):
		if app.DIK_F1 == key:
			self.__LoginDirectly(1)
		elif app.DIK_F2 == key:
			self.__LoginDirectly(2)
		elif app.DIK_F3 == key:
			self.__LoginDirectly(3)
		elif app.DIK_F4 == key:
			self.__LoginDirectly(4)
		return True
	def OnUpdate(self):
		ServerStateChecker.Update()
	def NotifyChannelState(self, addrKey, state):
		if addrKey >= 1 and addrKey<=4:
			STATE_DICT = {0 : "....",1 : "NORMAL",2 : "BUSY",3 : "FULL"}
			self.GetChild("ch%d_status"%addrKey).SetFontColor(0.0, 1.0, 0.0)
			self.GetChild("ch%d_status"%addrKey).SetText(STATE_DICT[state])