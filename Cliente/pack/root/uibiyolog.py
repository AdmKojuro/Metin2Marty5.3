import ui
import item
import constInfo
import app
import net
import chat
import player
import localeInfo
from uiToolTip import ItemToolTip
import event

OZUT_STATUS = True
UNUTKANLIK_STATUS = True
CALLBACK_MISSION = True

ROOT = "d:/ymir work/ui/game/bio/"
CACHE_SAVE_DIRECTORY = "lib/xml"

YENILEME_NOTU = 70028


bio_data = [
	# level, item, count, time, lucky, stone, afftype1, affvalue1, afftype2, affvalue2, afftype3, affvalue3 , afftype4, affvalue4, isChoose
	[ 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0], #empty
	[ 30, 30006, 10, 10 * 60, 60, 30220, item.APPLY_MAX_HP, 10000,0,0,0,0,0,0,0, 71035, 30250], #Dientes orco
	[ 40, 30047, 15, 10 * 60, 60, 30221, item.APPLY_MAX_HP, 5000,0,0,0,0,0,0,0, 71035, 30250], #Libro maldicion
	[ 50, 30015, 15, 15 * 60, 60, 30222, item.APPLY_ATTBONUS_HUMAN, 12,0,0,0,0,0,0,0, 71035, 30250], #Recuerdodeldemonio
	[ 60, 30050, 20, 15 * 60, 60, 30223, item.APPLY_CAST_SPEED, 15,0,0,0,0,0,0,1, 71035, 30250], #Bola de hielo
	[ 70, 30165, 25, 30 * 60, 60, 30224, item.APPLY_ATT_GRADE_BONUS, 30,item.APPLY_ATTBONUS_MONSTER,10,0,0,0,0,1, 71035, 30250], #Ramas Zelkova
	[ 80, 30166, 30, 30 * 60, 60, 30225, item.APPLY_MAX_HP, 1000,item.APPLY_CAST_SPEED,10,0,0,0,0,1, 71035, 30250], #Panel Tugyis
	[ 85, 30167, 40, 45 * 60, 60, 30226, item.APPLY_RESIST_WARRIOR, 10,item.APPLY_RESIST_ASSASSIN,10,item.APPLY_RESIST_SURA,10,item.APPLY_RESIST_SHAMAN,10,0, 71035, 30250], #Rama arbol Rojo Fantasma
	[ 90, 30168, 50, 60 * 60, 60, 30227, item.APPLY_ATTBONUS_WARRIOR, 10,item.APPLY_ATTBONUS_ASSASSIN,10,item.APPLY_ATTBONUS_SURA,10,item.APPLY_ATTBONUS_SHAMAN,10,0, 71035, 30250], #Notas del Lider
	[ 92, 30251, 10, 120 * 60, 60, 30228, item.APPLY_MAX_HP, 1200,item.APPLY_DEF_GRADE_BONUS,120,item.APPLY_ATT_GRADE_BONUS,50,0,0,1, 71035, 30250], #Joya de la envidia
	[ 94, 30252, 20, 120 * 60, 60, 30229, item.APPLY_MAX_HP, 1500,item.APPLY_DEF_GRADE_BONUS,140,item.APPLY_ATT_GRADE_BONUS,60,0,0,1, 71035, 30250], #Joya de la sabiduria
	[ 98, 300010, 35, 180 * 60, 60, 30230, item.APPLY_ATTBONUS_MONSTER, 10,item.APPLY_RESIST_HUMAN,10,0,0,0,0,1, 71035, 30250], #Pluma de Fenix
	[ 100, 300011, 35, 180 * 60, 60, 30231, item.APPLY_ATTBONUS_MONSTER, 10,item.APPLY_MELEE_MAGIC_ATTBONUS_PER,10,0,0,0,0,1, 71035, 30250], #Monoculo Rojo
 	[ 105, 300012, 35, 180 * 60, 60, 30232, item.APPLY_ATT_GRADE_BONUS, 50,item.APPLY_ATTBONUS_HUMAN,10,0,0,0,0,1, 71035, 30250], #Receta antigua
]

class BiologWindow(ui.BoardWithTitleBar):
	def __del__(self):
		ui.BoardWithTitleBar.__del__(self)

	def Destroy(self):
		self.__Initialize()
		ui.BoardWithTitleBar.Destroy(self)

	def __Initialize(self):
		self.children = []
		self.missionIcon=None
		self.missionName=None
		self.statusText=None
		self.missionGiftText=None
		self.missionStatusText=None
		self.giftFirst=None
		self.giftSecond=None
		self.giftThird=None
		self.giftFourth=None
		self.canSelectBonus=False

		self.bioLevel = 0
		self.bioCount = 0
		self.bioTime = 0
		self.needUpdate = False

		self.unutkanlikIcon = None
		self.checkBoxUnutkanlik = None
		self.ozutIcon = None
		self.checkBoxOzut = None
		self.checkBoxCall = None

	def __init__(self):
		ui.BoardWithTitleBar.__init__(self)
		self.AddFlag("float")
		self.AddFlag("attach")
		self.AddFlag("movable")
		self.SetWindowName("BiologWindow")

		self.SetTitleName(localeInfo.BIO_TITLE)
		self.SetCloseEvent(ui.__mem_func__(self.Close))

		self.__Initialize()
		self.__LoadWindow()

	def __LoadWindow(self):
		wallPaper = ui.ImageBox()
		wallPaper.SetParent(self)
		wallPaper.AddFlag("not_pick")
		wallPaper.LoadImage(ROOT+"bg.tga")
		wallPaper.SetPosition(12,32)
		wallPaper.Show()
		self.children.append(wallPaper)
		self.SetSize(12+wallPaper.GetWidth()+12, 32+wallPaper.GetHeight()+10)
		self.SetCenterPosition()

		missionName = ui.TextLine()
		missionName.AddFlag("not_pick")
		missionName.SetParent(wallPaper)
		missionName.SetHorizontalAlignCenter()
		missionName.SetPosition(wallPaper.GetWidth()/2,10)
		missionName.SetOutline()
		missionName.Show()
		self.missionName = missionName

		missionIconSlot = ui.ImageBox()
		missionIconSlot.SetParent(wallPaper)
		missionIconSlot.AddFlag("attach")
		missionIconSlot.LoadImage(ROOT+"gold_slot.tga")
		missionIconSlot.SetPosition(12,39)
		missionIconSlot.Show()
		self.children.append(missionIconSlot)

		missionIcon = ui.ImageBox()
		missionIcon.SetParent(missionIconSlot)
		missionIcon.SetPosition(5,6)
		missionIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
		missionIcon.Show()
		self.missionIcon = missionIcon

		clickBtn = ui.Button()
		clickBtn.SetParent(wallPaper)
		clickBtn.SetUpVisual(ROOT+"button_0.tga")
		clickBtn.SetOverVisual(ROOT+"button_1.tga")
		clickBtn.SetDownVisual(ROOT+"button_2.tga")
		clickBtn.SetText(localeInfo.BIO_GIVE_MISSION)
		clickBtn.SAFE_SetEvent(self.GiveMission)
		clickBtn.SetPosition(61,59)
		clickBtn.Show()
		self.children.append(clickBtn)

		missionStatusText = ui.TextLine()
		missionStatusText.AddFlag("not_pick")
		missionStatusText.SetParent(wallPaper)
		missionStatusText.SetHorizontalAlignCenter()
		missionStatusText.SetPosition(110,40)
		missionStatusText.SetOutline()
		missionStatusText.Show()
		self.missionStatusText = missionStatusText

		statusText = ui.TextLine()
		statusText.AddFlag("not_pick")
		statusText.SetParent(wallPaper)
		statusText.SetHorizontalAlignCenter()
		statusText.SetPosition(133,98)
		statusText.SetText(localeInfo.BIO_CAN_GIVE_ITEM)
		statusText.SetOutline()
		statusText.Show()
		self.statusText = statusText

		ozutIconSlot = ui.ImageBox()
		ozutIconSlot.SetParent(wallPaper)
		ozutIconSlot.AddFlag("attach")
		ozutIconSlot.LoadImage(ROOT+"grey_slot.tga")
		ozutIconSlot.SetPosition(168,40)
		ozutIconSlot.Show()
		self.children.append(ozutIconSlot)

		ozutIcon = ui.ImageBox()
		ozutIcon.SetParent(ozutIconSlot)
		ozutIcon.SetPosition(5,6)
		ozutIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
		ozutIcon.Show()
		self.ozutIcon = ozutIcon

		checkBoxOzut = ui.CheckBox()
		checkBoxOzut.SetParent(wallPaper)
		checkBoxOzut.SetEvent(ui.__mem_func__(self.CheckBoxEvent), "ON_CHECK", "OZUT",True)
		checkBoxOzut.SetEvent(ui.__mem_func__(self.CheckBoxEvent), "ON_UNCKECK","OZUT", False)
		#checkBoxOzut.SetCheckStatus(OZUT_STATUS)
		checkBoxOzut.SetPosition(168, 40)
		checkBoxOzut.Show()
		self.checkBoxOzut=checkBoxOzut

		unutkanlikIconSlot = ui.ImageBox()
		unutkanlikIconSlot.SetParent(wallPaper)
		unutkanlikIconSlot.AddFlag("attach")
		unutkanlikIconSlot.LoadImage(ROOT+"grey_slot.tga")
		unutkanlikIconSlot.SetPosition(215,40)
		unutkanlikIconSlot.Show()
		self.children.append(unutkanlikIconSlot)

		unutkanlikIcon = ui.ImageBox()
		unutkanlikIcon.SetParent(unutkanlikIconSlot)
		unutkanlikIcon.SetPosition(5,6)
		unutkanlikIcon.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.OverOutItem)
		unutkanlikIcon.Show()
		self.unutkanlikIcon = unutkanlikIcon

		checkBoxUnutkanlik = ui.CheckBox()
		checkBoxUnutkanlik.SetParent(wallPaper)
		checkBoxUnutkanlik.SetEvent(ui.__mem_func__(self.CheckBoxEvent), "ON_CHECK", "UNUTKANLIK",True)
		checkBoxUnutkanlik.SetEvent(ui.__mem_func__(self.CheckBoxEvent), "ON_UNCKECK","UNUTKANLIK", False)
		#checkBoxUnutkanlik.SetCheckStatus(UNUTKANLIK_STATUS)
		checkBoxUnutkanlik.SetPosition(215, 40)
		checkBoxUnutkanlik.Show()
		self.checkBoxUnutkanlik=checkBoxUnutkanlik

		shopBtn = ui.Button()
		shopBtn.SetParent(wallPaper)
		#shopBtn.SetUpVisual(ROOT+"shop_0.tga")
		#shopBtn.SetOverVisual(ROOT+"shop_1.tga")
		#shopBtn.SetDownVisual(ROOT+"shop_2.tga")
		shopBtn.SAFE_SetEvent(self.OpenShop)
		shopBtn.SetPosition(262,40)
		shopBtn.Show()
		self.children.append(shopBtn)

		checkBoxCall = ui.CheckBox()
		checkBoxCall.SetParent(wallPaper)
		checkBoxCall.SetPosition(259,100)
		checkBoxCall.SetEvent(ui.__mem_func__(self.CheckBoxEvent), "ON_CHECK", "CALL",True)
		checkBoxCall.SetEvent(ui.__mem_func__(self.CheckBoxEvent), "ON_UNCKECK","CALL", False)
		#checkBoxCall.SetCheckStatus(CALLBACK_MISSION)
		checkBoxCall.Show()
		self.checkBoxCall = checkBoxCall

		giftText = ui.TextLine()
		giftText.SetParent(wallPaper)
		giftText.AddFlag("not_pick")
		giftText.SetHorizontalAlignCenter()
		giftText.SetPosition(wallPaper.GetWidth()/2,132)
		giftText.SetText(localeInfo.BIO_GIFT)
		giftText.SetOutline()
		giftText.Show()
		self.children.append(giftText)

		giftTextWindow = ui.Window()
		giftTextWindow.SetParent(wallPaper)
		giftTextWindow.AddFlag("attach")
		giftTextWindow.SetPosition(3,151)
		giftTextWindow.SetSize(305,103)
		giftTextWindow.Show()
		self.children.append(giftTextWindow)

		missionGiftText = MultiTextLine()
		missionGiftText.SetParent(giftTextWindow)
		missionGiftText.AddFlag("not_pick")
		missionGiftText.SetPosition(giftTextWindow.GetWidth()/2,5)
		missionGiftText.SetTextType("horizontal#center")
		missionGiftText.SetTextRange(20)
		missionGiftText.Show()
		self.missionGiftText = missionGiftText

		giftFirst = ui.Button()
		giftFirst.SetParent(giftTextWindow)
		giftFirst.SetUpVisual(ROOT+"large_button_0.tga")
		giftFirst.SetOverVisual(ROOT+"large_button_1.tga")
		giftFirst.SetDownVisual(ROOT+"large_button_2.tga")
		giftFirst.SetEvent(lambda x = 0: self.ClickGift(x))
		giftFirst.SetPosition(23,5)
		self.giftFirst = giftFirst

		giftSecond = ui.Button()
		giftSecond.SetParent(giftTextWindow)
		giftSecond.SetUpVisual(ROOT+"large_button_0.tga")
		giftSecond.SetOverVisual(ROOT+"large_button_1.tga")
		giftSecond.SetDownVisual(ROOT+"large_button_2.tga")
		giftSecond.SetEvent(lambda x = 1: self.ClickGift(x))
		giftSecond.SetPosition(23,5+((giftFirst.GetHeight()+2)*1))
		self.giftSecond = giftSecond

		giftThird = ui.Button()
		giftThird.SetParent(giftTextWindow)
		giftThird.SetUpVisual(ROOT+"large_button_0.tga")
		giftThird.SetOverVisual(ROOT+"large_button_1.tga")
		giftThird.SetDownVisual(ROOT+"large_button_2.tga")
		giftThird.SetEvent(lambda x = 2: self.ClickGift(x))
		giftThird.SetPosition(23,5+((giftFirst.GetHeight()+2)*2))
		self.giftThird = giftThird

		giftFourth = ui.Button()
		giftFourth.SetParent(giftTextWindow)
		giftFourth.SetUpVisual(ROOT+"large_button_0.tga")
		giftFourth.SetOverVisual(ROOT+"large_button_1.tga")
		giftFourth.SetDownVisual(ROOT+"large_button_2.tga")
		giftFourth.SetEvent(lambda x = 3: self.ClickGift(x))
		giftFourth.SetPosition(23,5+((giftFirst.GetHeight()+2)*3))
		self.giftFourth = giftFourth
		
		self.LoadBioCacheData()

	def LoadEmpty(self):
		self.canSelectBonus=False
		self.missionIcon.Hide()
		self.unutkanlikIcon.Hide()
		self.checkBoxUnutkanlik.Hide()
		self.ozutIcon.Hide()
		self.checkBoxOzut.Hide()
		self.missionName.SetText("-")
		self.missionGiftText.SetText("")
		self.missionStatusText.SetText("")
		self.statusText.SetText(localeInfo.BIO_DONT_HAVE_MISSION)
		self.bioLevel = 0
		self.bioCount = 0
		self.bioTime = 0
		self.needUpdate = True
		self.giftFirst.Hide()
		self.giftSecond.Hide()
		self.giftThird.Hide()
		self.giftFourth.Hide()

	def CheckDirectory(self,directory):
		try:
			os.makedirs(directory)
		except:
			pass

	def SaveBioCacheData(self):
		global CACHE_SAVE_DIRECTORY
		self.CheckDirectory(CACHE_SAVE_DIRECTORY)
		fileName = "%s/%s"%(CACHE_SAVE_DIRECTORY,player.GetName())
		try:
			file = open(fileName, "w+")
			file.write("%d#%d#%d"%(int(OZUT_STATUS),int(UNUTKANLIK_STATUS),int(CALLBACK_MISSION)))
			file.close()
		except:
			pass

	def LoadBioCacheData(self):
		global CACHE_SAVE_DIRECTORY
		global OZUT_STATUS
		global UNUTKANLIK_STATUS
		global CALLBACK_MISSION
		self.CheckDirectory(CACHE_SAVE_DIRECTORY)
		fileName = "%s/%s"%(CACHE_SAVE_DIRECTORY,player.GetName())
		try:
			lines = open(fileName, "r").readlines()
			for line in lines:
				lineSplit = line.split("#")
				if len(lineSplit) != 3:
					OZUT_STATUS = True
					UNUTKANLIK_STATUS = True
					CALLBACK_MISSION = True
				else:
					OZUT_STATUS = int(lineSplit[0])
					UNUTKANLIK_STATUS = int(lineSplit[1])
					CALLBACK_MISSION = int(lineSplit[2])
				break
		except:
			OZUT_STATUS = True
			UNUTKANLIK_STATUS = True
			CALLBACK_MISSION = True

		self.checkBoxOzut.SetCheckStatus(OZUT_STATUS)
		self.checkBoxUnutkanlik.SetCheckStatus(UNUTKANLIK_STATUS)
		self.checkBoxCall.SetCheckStatus(CALLBACK_MISSION)


	def CheckBoxEvent(self, name, type, flag):
		global OZUT_STATUS
		global UNUTKANLIK_STATUS
		global CALLBACK_MISSION
		if type == "OZUT":
			OZUT_STATUS=flag
		elif type == "UNUTKANLIK":
			UNUTKANLIK_STATUS=flag
		elif type == "CALL":
			CALLBACK_MISSION=flag
		self.SaveBioCacheData()

	def BindInterfaceClass(self, interface):
		self.interface = interface

	def OverOutItem(self):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			if interface.tooltipItem:
				interface.tooltipItem.HideToolTip()

	def OverInItem(self, index):
		interface = constInfo.GetInterfaceInstance()
		if interface:
			if interface.tooltipItem:
				interface.tooltipItem.SetItemToolTip(index)

	def GetAffectString(self, affectType, affectValue):
		if not affectType:
			return "None"
		try:
			return ItemToolTip.AFFECT_DICT[affectType](affectValue)
		except TypeError:
			return "UNKNOWN_VALUE[{}] {}".format(affectType, affectValue)
		except KeyError:
			return "UNKNOWN_TYPE[{}] {}".format(affectType, affectValue)


	def LoadData(self, level, count, time):
		self.canSelectBonus=False
		global bio_data
		if level == 0 or level > len(bio_data):
			self.LoadEmpty()
			return
		if level == self.bioLevel:
			self.SetStatusText(level, count)

			self.bioCount = count
			self.bioTime = time
			self.needUpdate = False

			return
		else:
			if level != self.bioLevel:
				item.SelectItem(bio_data[level][15])
				if item.GetIconImageFileName().find("gr2") == -1:
					self.ozutIcon.LoadImage(item.GetIconImageFileName())
					self.ozutIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,bio_data[level][15])
					self.ozutIcon.Show()
					self.checkBoxOzut.Show()
				else:
					self.ozutIcon.Hide()
					self.checkBoxOzut.Hide()
				item.SelectItem(bio_data[level][16])
				if item.GetIconImageFileName().find("gr2") == -1:
					self.unutkanlikIcon.LoadImage(item.GetIconImageFileName())
					self.unutkanlikIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,bio_data[level][16])
					self.unutkanlikIcon.Show()
					self.checkBoxUnutkanlik.Show()
				else:
					self.unutkanlikIcon.Hide()
					self.checkBoxUnutkanlik.Hide()
				self.ShowBonusText()

		self.bioLevel = level
		self.bioCount = count
		self.bioTime = time
		self.needUpdate = False

		self.SetWindowTitle(level)
		self.SetStatusText(level, count)
		self.SetMissionIcon(level, count)
		self.ShowBonusText()

		if self.checkBoxCall.GetCheckStatus():
			if not self.IsShow():
				game = constInfo.GetGameInstance()
				if game != None:
					game.AddLoopEvent("bio",ui.__mem_func__(self.OnUpdate))


	def ShowBonusText(self):
		level = self.bioLevel

		if bio_data[level][14] != 0:
			self.missionGiftText.Hide()

			bonusCount = 0
			buttonList = [self.giftFirst,self.giftSecond,self.giftThird,self.giftFourth]
			for j in xrange(len(buttonList)):
				btn = buttonList[j]
				index = 6+(j*2)
				if bio_data[level][index] != 0:
					bonusCount+=1
					btn.SetText(self.GetAffectString(bio_data[level][index],bio_data[level][index+1]))
					btn.SetUpVisual(ROOT+"large_button_2.tga")
					btn.SetOverVisual(ROOT+"large_button_2.tga")
					btn.SetDownVisual(ROOT+"large_button_2.tga")
					btn.Show()
				else:
					btn.Hide()

			_yPos = 40
			if bonusCount == 2:
				_yPos = 30
			elif bonusCount == 3:
				_yPos = 17
			elif bonusCount == 4:
				_yPos = 5
			for j in xrange(bonusCount):
				buttonList[j].SetPosition(23,_yPos)
				_yPos+=buttonList[j].GetHeight()+2
		else:
			lineCount = 0
			affText = ""
			if bio_data[level][6] != 0:
				affText += self.GetAffectString(bio_data[level][6],bio_data[level][7])+"\n"
				lineCount+=1
			if bio_data[level][8] != 0:
				affText += self.GetAffectString(bio_data[level][8],bio_data[level][9])+"\n"
				lineCount+=1
			if bio_data[level][10] != 0:
				affText += self.GetAffectString(bio_data[level][10],bio_data[level][11])+"\n"
				lineCount+=1
			if bio_data[level][12] != 0:
				affText += self.GetAffectString(bio_data[level][12],bio_data[level][13])+"\n"
				lineCount+=1

			if affText == "":
				self.missionGiftText.SetText(localeInfo.BIO_EMPTY_BONUS)
				lineCount+=1
			else:
				self.missionGiftText.SetText(affText)

			(_x,_y) = (305/2,5)
			_y = (103/2)-((lineCount-1)*20)
			if lineCount == 3:
				_y += 20
			elif lineCount == 4:
				_y += 25

			self.missionGiftText.SetPosition(_x,_y)
			self.missionGiftText.Show()

			btnList = [self.giftFirst,self.giftSecond,self.giftThird,self.giftFourth]
			for btn in btnList:
				btn.Hide()

	def LoadStone(self, level):
		self.canSelectBonus=False
		global bio_data
		if level == 0 or level > len(bio_data):
			self.LoadEmpty()
			return

		self.bioLevel = level
		self.bioCount = 0
		self.bioTime = 0
		self.needUpdate = True

		self.SetWindowTitle(level)
		self.SetStatusText(level, bio_data[level][2])
		self.SetMissionIcon(level, bio_data[level][2])
		self.ShowBonusText()

		self.unutkanlikIcon.Hide()
		self.checkBoxUnutkanlik.Hide()
		self.ozutIcon.Hide()
		self.checkBoxOzut.Hide()

		self.statusText.SetText(localeInfo.BIO_FIND_STONE)

	def SetWindowTitle(self, level):
		global bio_data
		item.SelectItem(bio_data[level][1])
		self.missionName.SetText(localeInfo.BIO_MISSION_TITLE % (bio_data[level][0], item.GetItemName()))

	def SetStatusText(self, level, count):
		global bio_data
		self.missionStatusText.SetText("%d / %d"%(count,bio_data[level][2]))

	def SetMissionIcon(self, level, count):
		global bio_data
		itemVnum = bio_data[level][1]
		if bio_data[level][2] == count:
			if bio_data[level][5] != 0:
				itemVnum = bio_data[level][5]
		item.SelectItem(itemVnum)
		if item.GetIconImageFileName().find("gr2") == -1:
			self.missionIcon.LoadImage(item.GetIconImageFileName())
			self.missionIcon.SAFE_SetStringEvent("MOUSE_OVER_IN",self.OverInItem,itemVnum)
			self.missionIcon.Show()
		else:
			self.missionIcon.Hide()

	def LoadGift(self, level):
		self.canSelectBonus=True
		global bio_data
		if level == 0 or level > len(bio_data):
			self.LoadEmpty()
			return

		self.bioLevel = level
		self.bioCount = 0
		self.bioTime = 0
		self.needUpdate = True

		self.SetWindowTitle(level)
		self.SetStatusText(level, bio_data[level][2])
		self.SetMissionIcon(level, 1)

		self.statusText.SetText(localeInfo.BIO_CHOOSE_GIFT)

		self.missionGiftText.Hide()
		self.unutkanlikIcon.Hide()
		self.checkBoxUnutkanlik.Hide()
		self.ozutIcon.Hide()
		self.checkBoxOzut.Hide()

		bonusCount = 0
		buttonList = [self.giftFirst,self.giftSecond,self.giftThird,self.giftFourth]
		for j in xrange(len(buttonList)):
			btn = buttonList[j]
			index = 6+(j*2)
			if bio_data[level][index] != 0:
				bonusCount+=1
				btn.SetText(self.GetAffectString(bio_data[level][index],bio_data[level][index+1]))
				btn.SetUpVisual(ROOT+"large_button_0.tga")
				btn.SetOverVisual(ROOT+"large_button_1.tga")
				btn.SetDownVisual(ROOT+"large_button_2.tga")
				btn.Show()
			else:
				btn.Hide()

		_yPos = 40
		if bonusCount == 2:
			_yPos = 30
		elif bonusCount == 3:
			_yPos = 17
		elif bonusCount == 4:
			_yPos = 5
		for j in xrange(bonusCount):
			buttonList[j].SetPosition(23,_yPos)
			_yPos+=buttonList[j].GetHeight()+2

	def OpenShop(self):
		net.SendChatPacket("/open_shop 8")

	def GiveMission(self):
		if self.bioLevel == 0:
			return
		elif player.GetStatus(player.LEVEL) < bio_data[self.bioLevel][0]:
			#chat.AppendChat(chat.CHAT_TYPE_INFO, "710 %d"%bio_data[self.bioLevel][0])
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.BIO_NEED_LEVEL%bio_data[self.bioLevel][0])
			return
		net.SendChatPacket("/bio mission %d %d"%(self.checkBoxOzut.GetCheckStatus(),self.checkBoxUnutkanlik.GetCheckStatus()))

	def ClickGift(self, index):
		if self.canSelectBonus==False:
			return
		net.SendChatPacket("/bio gift %d"%index)

	def OnUpdate(self):
		if not self.needUpdate:
			if self.statusText == None:
				return True
			if self.bioTime == 0:
				self.statusText.SetText(localeInfo.BIO_CAN_GIVE_ITEM)
				self.needUpdate = True
				return True
			time = self.bioTime-app.GetGlobalTimeStamp()
			if time <= 0:
				if not self.IsShow():
					game = constInfo.GetGameInstance()
					if game != None:
						game.BINARY_OnRecvBulkWhisper("[Biolog] - "+localeInfo.BIO_CAN_GIVE_ITEM)

				self.statusText.SetText(localeInfo.BIO_CAN_GIVE_ITEM)
				self.needUpdate = True
				return True
			else:
				text = localeInfo.BIO_TIME_TEXT%(constInfo.minutetoday(time),constInfo.minutetohour(time),constInfo.minutetominute(time),constInfo.minutetosecond(time))
				self.statusText.SetText(text)
				return False
		return True

	def Open(self):
		self.Show()
		self.SetTop()

		game = constInfo.GetGameInstance()
		if game != None:
			game.RemoveLoopEvent("bio")

	def Close(self):
		self.Hide()

		if self.checkBoxCall.GetCheckStatus():
			if not self.IsShow():
				game = constInfo.GetGameInstance()
				if game != None:
					game.AddLoopEvent("bio",ui.__mem_func__(self.OnUpdate))

	def OnPressEscapeKey(self):
		self.Close()


class MultiTextLine(ui.Window):
	def __del__(self):
		ui.Window.__del__(self)
	def Destroy(self):
		self.children = []
		self.rangeText = 0
	def __init__(self):
		ui.Window.__init__(self)
		self.children = []
		self.rangeText = 15
		self.textType = ""
	def SetTextType(self, textType):
		self.textType = textType
		for text in self.children:
			self.AddTextType(self.textType.split("#"),text)
	def SetTextRange(self, range):
		self.rangeText = range
		yPosition = 0
		for text in self.children:
			text.SetPosition(0,yPosition)
			yPosition+=self.rangeText
	def AddTextType(self, typeArg, text):
		if len(typeArg) > 1:
			if typeArg[0] == "vertical":
				if typeArg[1] == "top":
					text.SetVerticalAlignTop()
				elif typeArg[1] == "bottom":
					text.SetVerticalAlignBottom()
				elif typeArg[1] == "center":
					text.SetVerticalAlignCenter()
			elif typeArg[0] == "horizontal":
				if typeArg[1] == "left":
					text.SetHorizontalAlignLeft()
				elif typeArg[1] == "right":
					text.SetHorizontalAlignRight()
				elif typeArg[1] == "center":
					text.SetHorizontalAlignCenter()
	def SetText(self, cmd):
		if len(self.children) > 1:
			self.children=[]
		multi_arg = cmd.split("\n")
		yPosition = 0
		for text in multi_arg:
			childText = ui.TextLine()
			childText.SetParent(self)
			childText.SetPosition(0,yPosition)
			if self.textType != "":
				if self.textType == "all_align":
					childText.SetHorizontalAlignCenter()
					childText.SetVerticalAlignCenter()
					childText.SetWindowHorizontalAlignCenter()
					childText.SetWindowVerticalAlignCenter()
				else:
					self.AddTextType(self.textType.split("#"),childText)
			childText.SetText(str(text))
			childText.Show()
			self.children.append(childText)
			yPosition+=self.rangeText
