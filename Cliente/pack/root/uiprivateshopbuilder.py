import ui
import snd
import shop
import mouseModule
import player
import chr
import net
import uiCommon
import localeInfo
import chat
import item
import systemSetting #±Ë¡ÿ»£
import player #±Ë¡ÿ»£
import constInfo
import app
import os
import app

##### DISABLE/ENABLE SHOP VISIT COLOR ###
SHOP_VISIT=True
##### SHOP VISIT COLOR #####
SHOP_VISIT_COLOR=0xFF00C8FF
###########################                          
g_isBuildingPrivateShop = False

g_itemPriceDict={}
if app.ENABLE_CHEQUE_SYSTEM:
	g_itemChequeDict={}

g_privateShopAdvertisementBoardDict={}
SHOP_RANGE=False
def GetShopNamesRange():
	global SHOP_RANGE
	if not SHOP_RANGE:
		return LoadShopNamesRange()
	return SHOP_RANGE

def LoadShopNamesRange():
	global SHOP_RANGE
	try:
		with open("shop.cfg", 'r') as f:
			SHOP_RANGE=float(f.read().replace('\n', ''))
	except IOError:
		SHOP_RANGE = 1.000
	return SHOP_RANGE

def SetShopNamesRange(pos):
	with open("shop.cfg", 'w+') as f:
		f.write(str(pos))
		f.close()
	LoadShopNamesRange()
def Clear():
	if app.ENABLE_CHEQUE_SYSTEM:
		global g_itemChequeDict
	global g_itemPriceDict
	global g_isBuildingPrivateShop
	g_itemPriceDict={}
	if app.ENABLE_CHEQUE_SYSTEM:
		g_itemChequeDict={}
	g_isBuildingPrivateShop = False
	# @fixme007 BEGIN
	global g_privateShopAdvertisementBoardDict
	g_privateShopAdvertisementBoardDict={}
	# @fixme007 END

def IsPrivateShopItemPriceList():
	global g_itemPriceDict
	if g_itemPriceDict:
		return True
	else:
		return False

if app.ENABLE_CHEQUE_SYSTEM:
	def IsPrivateShopItemChequeList():
		global g_itemChequeDict
		if g_itemChequeDict:
			return True
		else:
			return False

def IsBuildingPrivateShop():
	global g_isBuildingPrivateShop
	if player.IsOpenPrivateShop() or g_isBuildingPrivateShop:
		return True
	else:
		return False

def SetPrivateShopItemPrice(itemVNum, itemPrice):
	global g_itemPriceDict
	g_itemPriceDict[int(itemVNum)]=itemPrice

def GetPrivateShopItemPrice(itemVNum):
	try:
		global g_itemPriceDict
		return g_itemPriceDict[itemVNum]
	except KeyError:
		return 0

if app.ENABLE_CHEQUE_SYSTEM:
	def SetPrivateShopItemCheque(itemVNum, itemPrice):
		global g_itemChequeDict
		g_itemChequeDict[int(itemVNum)]=itemPrice
		
	def GetPrivateShopItemCheque(itemVNum):
		try:
			global g_itemChequeDict
			return g_itemChequeDict[itemVNum]
		except KeyError:
			return 0

def UpdateADBoard():
	for key in g_privateShopAdvertisementBoardDict.keys():
		if app.ENABLE_GRAPHIC_ON_OFF:
			if IsShowSalesRange(key):
				g_privateShopAdvertisementBoardDict[key].Show()
			else:
				g_privateShopAdvertisementBoardDict[key].Hide()
		else:
			g_privateShopAdvertisementBoardDict[key].Show()

def DeleteADBoard(vid):
	if not g_privateShopAdvertisementBoardDict.has_key(vid):
		return

	del g_privateShopAdvertisementBoardDict[vid]

if app.ENABLE_GRAPHIC_ON_OFF:
	def IsShowSalesRange(vid):
		if not vid:
			return False

		try:
			VIEW_RANGE = 0

			(xShop, yShop, zShop) = chr.GetPixelPosition(vid)
			(x, y, z) = player.GetMainCharacterPosition()

			if systemSetting.GetPrivateShopLevel() == 1: VIEW_RANGE = 5000
			elif systemSetting.GetPrivateShopLevel() == 2: VIEW_RANGE = 2500
			elif systemSetting.GetPrivateShopLevel() == 3: VIEW_RANGE = 1000
			elif systemSetting.GetPrivateShopLevel() == 4: VIEW_RANGE = 500

			if (abs(x - xShop) < VIEW_RANGE and abs(y - yShop) < VIEW_RANGE) or systemSetting.GetPrivateShopLevel() < 1:
				return True
			else:
				return False
		except:
			return False

if app.ENABLE_OFFSHOP_DECO:
	class PrivateShopAdvertisementBoard(ui.ThinBoardNorm):
		def __init__(self):
			ui.ThinBoardNorm.__init__(self, "UI_BOTTOM")
			#OffShop
			self.shopAdvertismentBoardSeen = []
			#OffShop
			self.vid = None
			self.__MakeTextLine()

		def __del__(self):
			ui.ThinBoardNorm.__del__(self)

		def __MakeTextLine(self):
			self.textLine = ui.TextLine()
			self.textLine.SetParent(self)
			self.textLine.SetWindowHorizontalAlignCenter()
			self.textLine.SetWindowVerticalAlignCenter()
			self.textLine.SetHorizontalAlignCenter()
			self.textLine.SetVerticalAlignCenter()
			self.textLine.SetPosition(0,-2)
			self.textLine.Show()

		def Open(self, vid, text):
			self.vid = vid

			self.textLine.SetText(text)
			#OffShop
			if vid in self.shopAdvertismentBoardSeen:
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
			#OffShop
			self.textLine.UpdateRect()
			self.SetSize(len(text) * 6 + 80 * 2)
			self.Show() 
				
			g_privateShopAdvertisementBoardDict[vid] = self

		def OnMouseLeftButtonUp(self):
			if not self.vid:
				return
			net.SendOnClickPacket(self.vid)
			#OffShop
			if self.vid != player.GetMainCharacterIndex():
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
				self.shopAdvertismentBoardSeen.append(self.vid)
			#OffShop
			return True

		def OnUpdate(self):
			if not self.vid:
				return
	
			if systemSetting.IsShowSalesText():
				if app.ENABLE_GRAPHIC_ON_OFF:
					if IsShowSalesRange(self.vid):
						self.Show()
					else:
						self.Hide()
	
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
				else:
					self.Show()
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
			else:
				for key in g_privateShopAdvertisementBoardDict.keys():
					if player.GetMainCharacterIndex() == key: #????? ???? ??? ????, ???? ??? ?? ??? ???? ?. by ???
						g_privateShopAdvertisementBoardDict[key].Show()
						x, y = chr.GetProjectPosition(player.GetMainCharacterIndex(), 220)
						g_privateShopAdvertisementBoardDict[key].SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
					else:
						g_privateShopAdvertisementBoardDict[key].Hide()

else:
	class PrivateShopAdvertisementBoard(ui.ThinBoard):
		def __init__(self):
			ui.ThinBoard.__init__(self, "UI_BOTTOM")
			#OffShop
			self.shopAdvertismentBoardSeen = []
			#OffShop
			self.vid = None
			self.__MakeTextLine()

		def __del__(self):
			ui.ThinBoard.__del__(self)

		def __MakeTextLine(self):
			self.textLine = ui.TextLine()
			self.textLine.SetParent(self)
			self.textLine.SetWindowHorizontalAlignCenter()
			self.textLine.SetWindowVerticalAlignCenter()
			self.textLine.SetHorizontalAlignCenter()
			self.textLine.SetVerticalAlignCenter()
			self.textLine.Show()

		def Open(self, vid, text):
			self.vid = vid

			self.textLine.SetText(text)
			#OffShop
			if vid in self.shopAdvertismentBoardSeen:
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
			#OffShop
			self.textLine.UpdateRect()
			self.SetSize(len(text)*6 + 10*2, 20)
			self.Show() 
					
			g_privateShopAdvertisementBoardDict[vid] = self

		def OnMouseLeftButtonUp(self):
			if not self.vid:
				return
			net.SendOnClickPacket(self.vid)
			#OffShop
			if self.vid != player.GetMainCharacterIndex():
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
				self.shopAdvertismentBoardSeen.append(self.vid)
			#OffShop
			return True

		def OnUpdate(self):
			if not self.vid:
				return
	
			if systemSetting.IsShowSalesText():
				if app.ENABLE_GRAPHIC_ON_OFF:
					if IsShowSalesRange(self.vid):
						self.Show()
					else:
						self.Hide()
	
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
				else:
					self.Show()
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
			else:
				for key in g_privateShopAdvertisementBoardDict.keys():
					if player.GetMainCharacterIndex() == key: #????? ???? ??? ????, ???? ??? ?? ??? ???? ?. by ???
						g_privateShopAdvertisementBoardDict[key].Show()
						x, y = chr.GetProjectPosition(player.GetMainCharacterIndex(), 220)
						g_privateShopAdvertisementBoardDict[key].SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
					else:
						g_privateShopAdvertisementBoardDict[key].Hide()

if app.ENABLE_OFFSHOP_DECO:
	class PrivateShopAdvertisementBoard0(ui.ThinBoardFire):
		def __init__(self):
			ui.ThinBoardFire.__init__(self, "UI_BOTTOM")
			#OffShop
			self.shopAdvertismentBoardSeen = []
			#OffShop
			self.vid = None
			self.__MakeTextLine()

		def __del__(self):
			ui.ThinBoardFire.__del__(self)

		def __MakeTextLine(self):
			self.textLine = ui.TextLine()
			self.textLine.SetParent(self)
			self.textLine.SetWindowHorizontalAlignCenter()
			self.textLine.SetWindowVerticalAlignCenter()
			self.textLine.SetHorizontalAlignCenter()
			self.textLine.SetVerticalAlignCenter()
			self.textLine.SetPosition(-8,6)
			self.textLine.Show()

		def Open(self, vid, text):
			self.vid = vid

			self.textLine.SetText(text)
			#OffShop
			if vid in self.shopAdvertismentBoardSeen:
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
			#OffShop
			self.textLine.UpdateRect()
			self.SetSize(len(text) * 6 + 80 * 2)
			self.Show() 
				
			g_privateShopAdvertisementBoardDict[vid] = self

		def OnMouseLeftButtonUp(self):
			if not self.vid:
				return
			net.SendOnClickPacket(self.vid)
			#OffShop
			if self.vid != player.GetMainCharacterIndex():
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
				self.shopAdvertismentBoardSeen.append(self.vid)
			#OffShop
			return True

		def OnUpdate(self):
			if not self.vid:
				return
	
			if systemSetting.IsShowSalesText():
				if app.ENABLE_GRAPHIC_ON_OFF:
					if IsShowSalesRange(self.vid):
						self.Show()
					else:
						self.Hide()
	
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
				else:
					self.Show()
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
			else:
				for key in g_privateShopAdvertisementBoardDict.keys():
					if player.GetMainCharacterIndex() == key: #????? ???? ??? ????, ???? ??? ?? ??? ???? ?. by ???
						g_privateShopAdvertisementBoardDict[key].Show()
						x, y = chr.GetProjectPosition(player.GetMainCharacterIndex(), 220)
						g_privateShopAdvertisementBoardDict[key].SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
					else:
						g_privateShopAdvertisementBoardDict[key].Hide()

	class PrivateShopAdvertisementBoard1(ui.ThinBoardIce):
		def __init__(self):
			ui.ThinBoardIce.__init__(self, "UI_BOTTOM")
			#OffShop
			self.shopAdvertismentBoardSeen = []
			#OffShop
			self.vid = None
			self.__MakeTextLine()

		def __del__(self):
			ui.ThinBoardIce.__del__(self)

		def __MakeTextLine(self):
			self.textLine = ui.TextLine()
			self.textLine.SetParent(self)
			self.textLine.SetWindowHorizontalAlignCenter()
			self.textLine.SetWindowVerticalAlignCenter()
			self.textLine.SetHorizontalAlignCenter()
			self.textLine.SetVerticalAlignCenter()
			self.textLine.SetPosition(-8,6)
			self.textLine.Show()

		def Open(self, vid, text):
			self.vid = vid

			self.textLine.SetText(text)
			#OffShop
			if vid in self.shopAdvertismentBoardSeen:
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
			#OffShop
			self.textLine.UpdateRect()
			self.SetSize(len(text) * 6 + 80 * 2)
			self.Show() 
				
			g_privateShopAdvertisementBoardDict[vid] = self

		def OnMouseLeftButtonUp(self):
			if not self.vid:
				return
			net.SendOnClickPacket(self.vid)
			#OffShop
			if self.vid != player.GetMainCharacterIndex():
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
				self.shopAdvertismentBoardSeen.append(self.vid)
			#OffShop
			return True

		def OnUpdate(self):
			if not self.vid:
				return
	
			if systemSetting.IsShowSalesText():
				if app.ENABLE_GRAPHIC_ON_OFF:
					if IsShowSalesRange(self.vid):
						self.Show()
					else:
						self.Hide()
	
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
				else:
					self.Show()
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
			else:
				for key in g_privateShopAdvertisementBoardDict.keys():
					if player.GetMainCharacterIndex() == key: #????? ???? ??? ????, ???? ??? ?? ??? ???? ?. by ???
						g_privateShopAdvertisementBoardDict[key].Show()
						x, y = chr.GetProjectPosition(player.GetMainCharacterIndex(), 220)
						g_privateShopAdvertisementBoardDict[key].SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
					else:
						g_privateShopAdvertisementBoardDict[key].Hide()

	class PrivateShopAdvertisementBoard2(ui.ThinBoardPaper):
		def __init__(self):
			ui.ThinBoardPaper.__init__(self, "UI_BOTTOM")
			#OffShop
			self.shopAdvertismentBoardSeen = []
			#OffShop
			self.vid = None
			self.__MakeTextLine()

		def __del__(self):
			ui.ThinBoardPaper.__del__(self)

		def __MakeTextLine(self):
			self.textLine = ui.TextLine()
			self.textLine.SetParent(self)
			self.textLine.SetWindowHorizontalAlignCenter()
			self.textLine.SetWindowVerticalAlignCenter()
			self.textLine.SetHorizontalAlignCenter()
			self.textLine.SetVerticalAlignCenter()
			self.textLine.SetPosition(-8,6)
			self.textLine.Show()

		def Open(self, vid, text):
			self.vid = vid

			self.textLine.SetText(text)
			#OffShop
			if vid in self.shopAdvertismentBoardSeen:
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
			#OffShop
			self.textLine.UpdateRect()
			self.SetSize(len(text) * 6 + 80 * 2)
			self.Show() 
				
			g_privateShopAdvertisementBoardDict[vid] = self

		def OnMouseLeftButtonUp(self):
			if not self.vid:
				return
			net.SendOnClickPacket(self.vid)
			#OffShop
			if self.vid != player.GetMainCharacterIndex():
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
				self.shopAdvertismentBoardSeen.append(self.vid)
			#OffShop
			return True

		def OnUpdate(self):
			if not self.vid:
				return
	
			if systemSetting.IsShowSalesText():
				if app.ENABLE_GRAPHIC_ON_OFF:
					if IsShowSalesRange(self.vid):
						self.Show()
					else:
						self.Hide()
	
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
				else:
					self.Show()
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
			else:
				for key in g_privateShopAdvertisementBoardDict.keys():
					if player.GetMainCharacterIndex() == key: #????? ???? ??? ????, ???? ??? ?? ??? ???? ?. by ???
						g_privateShopAdvertisementBoardDict[key].Show()
						x, y = chr.GetProjectPosition(player.GetMainCharacterIndex(), 220)
						g_privateShopAdvertisementBoardDict[key].SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
					else:
						g_privateShopAdvertisementBoardDict[key].Hide()

	class PrivateShopAdvertisementBoard3(ui.ThinBoardRibon):
		def __init__(self):
			ui.ThinBoardRibon.__init__(self, "UI_BOTTOM")
			#OffShop
			self.shopAdvertismentBoardSeen = []
			#OffShop
			self.vid = None
			self.__MakeTextLine()

		def __del__(self):
			ui.ThinBoardRibon.__del__(self)

		def __MakeTextLine(self):
			self.textLine = ui.TextLine()
			self.textLine.SetParent(self)
			self.textLine.SetWindowHorizontalAlignCenter()
			self.textLine.SetWindowVerticalAlignCenter()
			self.textLine.SetHorizontalAlignCenter()
			self.textLine.SetVerticalAlignCenter()
			self.textLine.SetPosition(-8,6)
			self.textLine.Show()

		def Open(self, vid, text):
			self.vid = vid

			self.textLine.SetText(text)
			#OffShop
			if vid in self.shopAdvertismentBoardSeen:
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
			#OffShop
			self.textLine.UpdateRect()
			self.SetSize(len(text) * 6 + 80 * 2)
			self.Show() 
				
			g_privateShopAdvertisementBoardDict[vid] = self

		def OnMouseLeftButtonUp(self):
			if not self.vid:
				return
			net.SendOnClickPacket(self.vid)
			#OffShop
			if self.vid != player.GetMainCharacterIndex():
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
				self.shopAdvertismentBoardSeen.append(self.vid)
			#OffShop
			return True

		def OnUpdate(self):
			if not self.vid:
				return
	
			if systemSetting.IsShowSalesText():
				if app.ENABLE_GRAPHIC_ON_OFF:
					if IsShowSalesRange(self.vid):
						self.Show()
					else:
						self.Hide()
	
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
				else:
					self.Show()
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
			else:
				for key in g_privateShopAdvertisementBoardDict.keys():
					if player.GetMainCharacterIndex() == key: #????? ???? ??? ????, ???? ??? ?? ??? ???? ?. by ???
						g_privateShopAdvertisementBoardDict[key].Show()
						x, y = chr.GetProjectPosition(player.GetMainCharacterIndex(), 220)
						g_privateShopAdvertisementBoardDict[key].SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
					else:
						g_privateShopAdvertisementBoardDict[key].Hide()

	class PrivateShopAdvertisementBoard4(ui.ThinBoardWing):
		def __init__(self):
			ui.ThinBoardWing.__init__(self, "UI_BOTTOM")
			#OffShop
			self.shopAdvertismentBoardSeen = []
			#OffShop
			self.vid = None
			self.__MakeTextLine()

		def __del__(self):
			ui.ThinBoardWing.__del__(self)

		def __MakeTextLine(self):
			self.textLine = ui.TextLine()
			self.textLine.SetParent(self)
			self.textLine.SetWindowHorizontalAlignCenter()
			self.textLine.SetWindowVerticalAlignCenter()
			self.textLine.SetHorizontalAlignCenter()
			self.textLine.SetVerticalAlignCenter()
			self.textLine.SetPosition(-8,6)
			self.textLine.Show()

		def Open(self, vid, text):
			self.vid = vid

			self.textLine.SetText(text)
			#OffShop
			if vid in self.shopAdvertismentBoardSeen:
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
			#OffShop
			self.textLine.UpdateRect()
			self.SetSize(len(text) * 6 + 80 * 2)
			self.Show() 
				
			g_privateShopAdvertisementBoardDict[vid] = self

		def OnMouseLeftButtonUp(self):
			if not self.vid:
				return
			net.SendOnClickPacket(self.vid)
			#OffShop
			if self.vid != player.GetMainCharacterIndex():
				self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
				self.shopAdvertismentBoardSeen.append(self.vid)
			#OffShop
			return True

		def OnUpdate(self):
			if not self.vid:
				return
	
			if systemSetting.IsShowSalesText():
				if app.ENABLE_GRAPHIC_ON_OFF:
					if IsShowSalesRange(self.vid):
						self.Show()
					else:
						self.Hide()
	
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
				else:
					self.Show()
					x, y = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
			else:
				for key in g_privateShopAdvertisementBoardDict.keys():
					if player.GetMainCharacterIndex() == key: #????? ???? ??? ????, ???? ??? ?? ??? ???? ?. by ???
						g_privateShopAdvertisementBoardDict[key].Show()
						x, y = chr.GetProjectPosition(player.GetMainCharacterIndex(), 220)
						g_privateShopAdvertisementBoardDict[key].SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
					else:
						g_privateShopAdvertisementBoardDict[key].Hide()

class PrivateShopBuilder(ui.ScriptWindow):

	def __init__(self):
		#print "NEW MAKE_PRIVATE_SHOP_WINDOW ----------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)

		self.__LoadWindow()
		self.itemStock = {}
		self.tooltipItem = None
		self.priceInputBoard = None
		self.days = 0
		self.title = ""
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.interface = None
			self.wndInventory = None
			self.lockedItems = {i:(-1,-1) for i in range(shop.SHOP_SLOT_COUNT)}

	def __del__(self):
		#print "------------------------------------------------------------- DELETE MAKE_PRIVATE_SHOP_WINDOW"
		ui.ScriptWindow.__del__(self)

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PrivateShopBuilder.py")
		except:
			import exception
			exception.Abort("PrivateShopBuilderWindow.LoadWindow.LoadObject")

		try:
			GetObject = self.GetChild
			self.nameLine = GetObject("NameLine")
			self.itemSlot = GetObject("ItemSlot")
			self.btnOk = GetObject("OkButton")
			self.btnClose = GetObject("CloseButton")
			self.titleBar = GetObject("TitleBar")
		except:
			import exception
			exception.Abort("PrivateShopBuilderWindow.LoadWindow.BindObject")

		self.btnOk.SetEvent(ui.__mem_func__(self.OnOk))
		self.btnClose.SetEvent(ui.__mem_func__(self.OnClose))
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.OnClose))

		self.itemSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.itemSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.itemSlot.SetOverInItemEvent(ui.__mem_func__(self.OnOverInItem))
		self.itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OnOverOutItem))

	def Destroy(self):
		self.ClearDictionary()

		self.nameLine = None
		self.itemSlot = None
		self.btnOk = None
		self.btnClose = None
		self.titleBar = None
		self.priceInputBoard = None
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.lockedItems = {i:(-1,-1) for i in range(shop.SHOP_SLOT_COUNT)}
			if self.interface:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_PRIVATE_SHOP)
				self.interface.RefreshMarkInventoryBag()

	def Open(self, title,days):

		self.days = days
		self.title = title

		if len(title) > 25:
			title = title[:22] + "..."

		self.itemStock = {}
		shop.ClearPrivateShopStock()
		self.nameLine.SetText(title)
		self.SetCenterPosition()
		self.Refresh()
		self.Show()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.lockedItems = {i:(-1,-1) for i in range(shop.SHOP_SLOT_COUNT)}
			if self.interface:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_PRIVATE_SHOP)
				self.interface.RefreshMarkInventoryBag()

		global g_isBuildingPrivateShop
		g_isBuildingPrivateShop = True

		if app.ENABLE_CHEQUE_SYSTEM:
			self.IsCashItem = False

	def Close(self):
		global g_isBuildingPrivateShop
		g_isBuildingPrivateShop = False

		self.title = ""
		self.itemStock = {}
		shop.ClearPrivateShopStock()
		self.Hide()
		if self.priceInputBoard:
			self.priceInputBoard.Close()
			self.priceInputBoard = None

		if app.WJ_ENABLE_TRADABLE_ICON:
			for privatePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
				if self.wndInventory and itemInvenPage == self.wndInventory.GetInventoryPageIndex():
					self.wndInventory.wndItem.SetCanMouseEventSlot(itemSlotPos)

			self.lockedItems = {i:(-1,-1) for i in range(shop.SHOP_SLOT_COUNT)}
			if self.interface:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_PRIVATE_SHOP)
				self.interface.RefreshMarkInventoryBag()

		if app.ENABLE_CHEQUE_SYSTEM:
			self.IsCashItem = False

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def Refresh(self):
		getitemVNum=player.GetItemIndex
		getItemCount=player.GetItemCount
		setitemVNum=self.itemSlot.SetItemSlot
		delItem=self.itemSlot.ClearSlot

		for i in xrange(shop.SHOP_SLOT_COUNT):

			if not self.itemStock.has_key(i):
				delItem(i)
				continue

			pos = self.itemStock[i]

			itemCount = getItemCount(*pos)
			if itemCount <= 1:
				itemCount = 0
			setitemVNum(i, getitemVNum(*pos), itemCount)

		self.itemSlot.RefreshSlot()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.RefreshLockedSlot()

	def ReadFilePrice(self,vnum,count):
		d = "shops"
		if not os.path.exists(d):
			os.makedirs(d)
		oldPrice=0
		n=d+"/"+str(vnum)+"_"+str(count)+".txt"
		if os.path.exists(n):
			fd = open( n,'r')
			oldPrice=int(fd.readlines()[0])
			
		return oldPrice
	def SaveFilePrice(self,vnum,count,price):
		d = "shops"
		if not os.path.exists(d):
			os.makedirs(d)
		n=d+"/"+str(vnum)+"_"+str(count)+".txt"
		f = file(n, "w+")
		f.write(str(price))
		f.close()

	def OnSelectEmptySlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			if player.SLOT_TYPE_INVENTORY != attachedSlotType and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedSlotType and\
				(player.SLOT_TYPE_SKILL_BOOK_INVENTORY != attachedSlotType and\
				player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY != attachedSlotType and\
				player.SLOT_TYPE_STONE_INVENTORY != attachedSlotType and\
				player.SLOT_TYPE_GIFT_BOX_INVENTORY != attachedSlotType and app.ENABLE_SPECIAL_INVENTORY_SYSTEM):
				return

			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
			count = player.GetItemCount(attachedInvenType, attachedSlotPos)

			itemVNum = player.GetItemIndex(attachedInvenType, attachedSlotPos)
			item.SelectItem(itemVNum)

			if item.IsAntiFlag(item.ANTIFLAG_GIVE) or item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PRIVATE_SHOP_CANNOT_SELL_ITEM)
				return

			if app.WJ_ENABLE_TRADABLE_ICON and player.SLOT_TYPE_INVENTORY == attachedSlotType:
				self.CantTradableItem(selectedSlotPos, attachedSlotPos)

			priceInputBoard = uiCommon.MoneyInputDialog()
			priceInputBoard.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_PRICE_DIALOG_TITLE)
			priceInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrice))
			priceInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrice))
			#OffShop
			priceInputBoard.SetMaxLength(16)
			#OffShop
			priceInputBoard.Open()

			if app.ENABLE_CHEQUE_SYSTEM:
				if self.IsCashItem:
					itemPrice = GetPrivateShopItemPrice(itemVNum)
					itemCheque = GetPrivateShopItemCheque(itemVNum)

					if itemPrice > 0 or itemCheque > 0:
						priceInputBoard.SetCheque(itemCheque)
						priceInputBoard.SetValue(itemPrice)
			else:
				itemPrice = GetPrivateShopItemPrice(itemVNum)
				if itemPrice > 0:
					priceInputBoard.SetValue(itemPrice)

			self.priceInputBoard = priceInputBoard
			self.priceInputBoard.itemVNum = itemVNum
			self.priceInputBoard.sourceWindowType = attachedInvenType
			self.priceInputBoard.sourceSlotPos = attachedSlotPos
			self.priceInputBoard.targetSlotPos = selectedSlotPos

	def OnSelectItemSlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			snd.PlaySound("sound/ui/loginfail.wav")
			mouseModule.mouseController.DeattachObject()

		else:
			if not selectedSlotPos in self.itemStock:
				return

			invenType, invenPos = self.itemStock[selectedSlotPos]
			shop.DelPrivateShopItemStock(invenType, invenPos)
			snd.PlaySound("sound/ui/drop.wav")
			if app.WJ_ENABLE_TRADABLE_ICON:
				(itemInvenPage, itemSlotPos) = self.lockedItems[selectedSlotPos]
				if self.wndInventory and itemInvenPage == self.wndInventory.GetInventoryPageIndex():
					self.wndInventory.wndItem.SetCanMouseEventSlot(itemSlotPos)

				self.lockedItems[selectedSlotPos] = (-1, -1)

			del self.itemStock[selectedSlotPos]

			self.Refresh()

	def AcceptInputPrice(self):

		if not self.priceInputBoard:
			return True

		if app.ENABLE_CHEQUE_SYSTEM:
			cheque_text = self.priceInputBoard.GetCheque()
			text = self.priceInputBoard.GetText()

			if not text or not cheque_text:
				return True

			if not text.isdigit() or not cheque_text.isdigit():
				return True

			if int(text) <= 0 and int(cheque_text) <= 0:
				return True
		else:
			text = self.priceInputBoard.GetText()

			if not text:
				return True
			if not text.isdigit():
				return True
			if int(text) <= 0:
				return True

		attachedInvenType = self.priceInputBoard.sourceWindowType
		sourceSlotPos = self.priceInputBoard.sourceSlotPos
		targetSlotPos = self.priceInputBoard.targetSlotPos

		for privatePos, (itemWindowType, itemSlotIndex) in self.itemStock.items():
			if itemWindowType == attachedInvenType and itemSlotIndex == sourceSlotPos:
				shop.DelPrivateShopItemStock(itemWindowType, itemSlotIndex)
				del self.itemStock[privatePos]

		if app.ENABLE_CHEQUE_SYSTEM:
			price = int(self.priceInputBoard.GetText())
			cheque = int(self.priceInputBoard.GetCheque())

			# if IsPrivateShopItemPriceList():
				# SetPrivateShopItemPrice(self.priceInputBoard.itemVNum, price, cheque)

			shop.AddPrivateShopItemStock(attachedInvenType, sourceSlotPos, targetSlotPos, price, cheque)
		else:
			price = int(self.priceInputBoard.GetText())

			if IsPrivateShopItemPriceList():
				SetPrivateShopItemPrice(self.priceInputBoard.itemVNum, price)

			shop.AddPrivateShopItemStock(attachedInvenType, sourceSlotPos, targetSlotPos, price)

		count = player.GetItemCount(attachedInvenType, sourceSlotPos)
		vnum = player.GetItemIndex(attachedInvenType, sourceSlotPos)
		self.SaveFilePrice(vnum,count,price)
		self.itemStock[targetSlotPos] = (attachedInvenType, sourceSlotPos)
		snd.PlaySound("sound/ui/drop.wav")
	
		self.Refresh()

		#####

		self.priceInputBoard = None
		return True

	def CancelInputPrice(self):
		if app.WJ_ENABLE_TRADABLE_ICON:
			itemInvenPage = self.priceInputBoard.sourceSlotPos / player.INVENTORY_PAGE_SIZE
			itemSlotPos = self.priceInputBoard.sourceSlotPos - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
			if self.wndInventory and itemInvenPage == self.wndInventory.GetInventoryPageIndex():
				self.wndInventory.wndItem.SetCanMouseEventSlot(itemSlotPos)

			self.lockedItems[self.priceInputBoard.targetSlotPos] = (-1, -1)

		if self.priceInputBoard:
			self.priceInputBoard.Close()
		self.priceInputBoard = None
		return 1

	def OnOk(self):

		if not self.title:
			return

		if 0 == len(self.itemStock):
			return

		shop.BuildPrivateShop(self.title,self.days)
		self.Close()

	def OnClose(self):
		self.Close()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnOverInItem(self, slotIndex):

		if self.tooltipItem:
			if self.itemStock.has_key(slotIndex):
				self.tooltipItem.SetPrivateShopBuilderItem(*self.itemStock[slotIndex] + (slotIndex,))

	def OnOverOutItem(self):

		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	if app.ENABLE_CHEQUE_SYSTEM:
		def SetIsCashItem(self, bCashItem):
			self.IsCashItem = bCashItem

	if app.WJ_ENABLE_TRADABLE_ICON:
		def CantTradableItem(self, destSlotIndex, srcSlotIndex):
			itemInvenPage = srcSlotIndex / player.INVENTORY_PAGE_SIZE
			localSlotPos = srcSlotIndex - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
			self.lockedItems[destSlotIndex] = (itemInvenPage, localSlotPos)
			if self.wndInventory and itemInvenPage == self.wndInventory.GetInventoryPageIndex():
				self.wndInventory.wndItem.SetCantMouseEventSlot(localSlotPos)

		def RefreshLockedSlot(self):
			if self.wndInventory:
				for privatePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
					if self.wndInventory and itemInvenPage == self.wndInventory.GetInventoryPageIndex():
						self.wndInventory.wndItem.SetCantMouseEventSlot(itemSlotPos)

				#self.wndInventory.wndItem.RefreshSlot()

		def BindInterface(self, interface):
			self.interface = interface

		def OnTop(self):
			if self.interface:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_PRIVATE_SHOP)
				self.interface.RefreshMarkInventoryBag()

		def SetInven(self, wndInventory):
			from _weakref import proxy
			self.wndInventory = proxy(wndInventory)
			