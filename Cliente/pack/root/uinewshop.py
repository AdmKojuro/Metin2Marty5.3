import os
import app
import dbg
import item
import chat
import net
import localeInfo
import ime
import time
import ui
import uiCommon
import uiToolTip
import colorInfo
import player
import snd
import grp
import mouseModule
import shop
import wndMgr
import uiPrivateShopBuilder
import constInfo
import uiScriptLocale
from uiUtils import Edit2 as Edit2
c = colorInfo.CHAT_RGB_NOTICE
titleColor = ui.GenerateColor(c[0],c[1],c[2])
NEGATIVE_COLOR = grp.GenerateColor(0.9, 0.4745, 0.4627, 1.0)
POSITIVE_COLOR = grp.GenerateColor(0.6911, 0.8754, 0.7068, 1.0)
##Should be disabled becouse this is only for my client UI ##
POSITION_FIX=False
#############################################################
import uiUtils
class ShopDialogCreate(ui.ScriptWindow):
	UI={}
	def __init__(self, interface):
		ui.ScriptWindow.__init__(self)
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()
		self.privateShopBuilder = uiPrivateShopBuilder.PrivateShopBuilder()
		self.privateShopBuilder.Hide()
		self.privateShopBuilder.SetItemToolTip(self.tooltipItem)
		self.pop=None
		self.__LoadQuestionDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadQuestionDialog(self):
		self.UI["board"] = ui.BoardWithTitleBar()
		self.UI["board"].SetCenterPosition()
		self.UI["board"].AddFlag("movable")
		if POSITION_FIX:
			self.UI["board"].SetSize(240, 155)
		else:
			self.UI["board"].SetSize(240, 165)
		self.UI["board"].SetCenterPosition()
		self.UI["board"].SetTop()
		self.UI["board"].SetTitleName(uiScriptLocale.SHOP_WINDOW_CREATE)
		self.UI["board"].Hide()

		self.UI["nameText"] = ui.MakeText(self.UI["board"], uiScriptLocale.SHOP_NAME, 81, 30, None)
		self.UI["nameEdit"]=Edit2(self.UI["board"],"",10,45,150,25,FALSE,6,30)


		self.UI["selectText"] = ui.TextLine()
		self.UI["selectText"].SetParent(self.UI["board"])
		self.UI["selectText"].SetPosition(15, 72)
		self.UI["selectText"].SetFeather()
		self.UI["selectText"].SetDefaultFontName()
		self.UI["selectText"].SetOutline()
		self.UI["selectText"].SetText(uiScriptLocale.SHOP_TIMEOUT)
		self.UI["selectText"].Show()

		if app.ENABLE_OFFSHOP_DECO:
			self.UI["create"] = ui.MakeButton(self.UI["board"], 20, 125 , "", "d:/ymir work/ui/public/", "acceptbutton00.tga", "acceptbutton01.tga", "acceptbutton02.tga")
			self.UI["create"].SetToolTipText(uiScriptLocale.SHOP_CREATE)
			self.UI["create"].SetEvent(ui.__mem_func__(self.CreateShop))
			self.UI["create"].Show()

			self.UI["decoration"] = ui.MakeButton(self.UI["board"], 87, 125 , "", "d:/ymir work/ui/public/", "decorationbutton00.tga", "decorationbutton01.tga", "decorationbutton02.tga")
			self.UI["decoration"].SetToolTipText("Decorar tienda")
			self.UI["decoration"].SetEvent(ui.__mem_func__(self.OpenDecoration))
			self.UI["decoration"].Show()
			
			self.UI["cancel"] = ui.MakeButton(self.UI["board"], 155, 125 , "", "d:/ymir work/ui/public/", "canclebutton00.tga", "canclebutton01.tga", "canclebutton02.tga")
			self.UI["cancel"].SetToolTipText(uiScriptLocale.CANCEL)
			self.UI["cancel"].SetEvent(ui.__mem_func__(self.Close))
			self.UI["cancel"].Show()
		else:
			self.UI["create"] = ui.MakeButton(self.UI["board"], 30, 110 , "", "d:/ymir work/ui/public/", "xlarge_button_01.sub", "xlarge_button_02.sub", "xlarge_button_03.sub")
			self.UI["create"].SetText(uiScriptLocale.SHOP_CREATE)
			self.UI["create"].SetEvent(ui.__mem_func__(self.CreateShop))
			self.UI["create"].Show()
			
			self.UI["cancel"] = ui.MakeButton(self.UI["board"], 30, 140 , "", "d:/ymir work/ui/public/", "xlarge_button_01.sub", "xlarge_button_02.sub", "xlarge_button_03.sub")
			self.UI["cancel"].SetText(uiScriptLocale.CANCEL)
			self.UI["cancel"].SetEvent(ui.__mem_func__(self.Close))
			self.UI["cancel"].Show()

		self.UI["price"] = ui.MakeText(self.UI["board"], "", 15, 97, None)
		self.UI["price"].SetFeather()
		self.UI["price"].SetDefaultFontName()
		self.UI["price"].SetOutline()
		self.UI["price"].Show()
		self.CreateList()
	def Destroy(self):
		self.UI={}
		self.__OnClosePopupDialog()
		if "board" in self.UI.keys():
			self.UI["board"].Hide()
		return TRUE
		#self.ClearDictionary()
	def CreateList(self):
		if "select" in self.UI.keys():
			self.UI["select"].Hide()
			self.UI["select"].Destroy()
		self.UI["select"] = None
		self.UI["select"] = DropDown(self.UI["board"],"---")
		self.UI["select"].SetPosition(110,70)
		self.UI["select"].SetSize(120,20)
		self.UI["select"].SetTop()
		self.UI["select"].OnChange=self.Load
		self.UI["select"].Show()
	def OnPressEscapeKey(self):
		self.Close()
		return TRUE
	def Close(self):
		self.__OnClosePopupDialog()
		self.UnLoad()
		self.UI["board"].Hide()
		self.Hide()
		self.Hide()
	def CreateShop(self):
		if len(self.UI["nameEdit"].GetText()) <=0:
			self.PopupMessage(uiScriptLocale.SHOP_NAME_EMPTY)
			return
		id=int(self.UI["select"].DropList.GetSelectedItem().value)
		print "Set id "+str(id)
                                                       
         
		item = constInfo.shop_cost[id-1]
		
		if int(item["id"]) <=0:
			self.PopupMessage(uiScriptLocale.SHOP_TIMEOUT_EMPTY)
			return
		self.__OnClosePopupDialog()
		self.privateShopBuilder.Open(self.UI["nameEdit"].GetText(),int(item["id"]))
		self.Close()
	def __OnClosePopupDialog(self):
		if self.pop != None:
			if self.pop.IsShow():
				self.pop.Hide()
		self.pop = None
	def Hide(self):
		if "board" in self.UI.keys():
			self.UI["board"].Hide()
			self.UI["price"].Hide()
			self.UI["select"].dropped = 0
			self.UI["select"].ClearItems()
			self.UI["nameEdit"].SetText("")
			self.Days=0
		ui.ScriptWindow.Hide(self)

	if app.ENABLE_OFFSHOP_DECO:
		#Shop Decoration
		def OpenDecoration(self):
			net.SendChatPacket("/open_decoration")
		#Shop Decoration

	def Show(self):
		self.UI["board"].SetTop()
		self.UI["board"].Show()
		self.UnLoad()
		self.CreateList()
		for i in xrange(len(constInfo.shop_cost)):
			item=constInfo.shop_cost[i]
			name=uiScriptLocale.SHOP_CREATE_NORMAL
			if item["time"]>0:
				name=str(item["time"])+" "
				if item["time"] == 1:
					if item["time_val"]==3600:
						name+=uiScriptLocale.SHOP_CREATE_HOUR
					else:
						name+=uiScriptLocale.SHOP_CREATE_DAY
				else:
					if item["time_val"]==86400:
						name+=uiScriptLocale.SHOP_CREATE_DAYS
					else:
						name+=uiScriptLocale.SHOP_CREATE_HOURS

			self.UI["select"].AppendItem(name,i+1)

		ui.ScriptWindow.Show(self)
		self.UI["board"].Show()
	def Load(self):
		id=self.UI["select"].DropList.GetSelectedItem().value
		if int(id) == 0:
			print "unload"
			self.UnLoad()
			return
		item = constInfo.shop_cost[id-1]
		if item["price"]>0:
			self.UI["price"].SetText(uiScriptLocale.SHOP_CREATE_PRICE+localeInfo.NumberToMoneyString(item["price"]))
			self.UI["price"].Show()
		else:
			self.UI["price"].Show()
	def UnLoad(self):
		self.UI["price"].Hide()
		# self.UI["select"].dropped = 0
		# self.UI["select"].ClearItems()
		self.UI["select"].Hide()

	def Clear(self):
		self.UI["select"].Clear()
	def PopupMessage(self,text):
		pop = uiCommon.PopupDialog()
		pop.SetText(text)
		pop.SetAcceptEvent(self.__OnClosePopupDialog)
		pop.Open()
		self.pop = pop
		self.pop.SetTop()

class DropDown(ui.Window):
	dropped  = 0
	dropstat = 0
	last = 0
	lastS = 0
	maxh = 95
	tt = ""
	OnChange = None
	class Item(ui.ListBoxEx.Item):
		def __init__(self,parent, text,value=0):
			ui.ListBoxEx.Item.__init__(self)

			self.textBox=ui.TextLine()
			self.textBox.SetParent(self)
			self.textBox.SetText(text)
			# self.textBox.SetLimitWidth(parent.GetWidth()-132)
			self.textBox.Show()
			self.value = value
		def GetValue(self):
			return self.value
		def __del__(self):
			ui.ListBoxEx.Item.__del__(self)

	def __init__(self,parent,tt = "",down=1):
		ui.Window.__init__(self,"TOP_MOST")
		self.tt=tt
		self.down = down
		self.SetParentProxy(parent)
		self.bg = ui.Bar("TOP_MOST")
		self.bg.SetParent(self)
		self.bg.SetPosition(0,0)
		self.bg.SetColor(0xc0000000)
		self.bg.OnMouseOverIn = self.bgMouseIn
		self.bg.OnMouseOverOut = self.bgMouseOut
		self.bg.OnMouseLeftButtonDown = self.ExpandMe
		self.bg.Show()
		self.act = ui.TextLine()
		self.act.SetParent(self.bg)
		self.act.SetPosition(4,2)
		self.act.SetText(self.tt)
		self.act.Show()
		self.GetText = self.act.GetText

		self.Drop = ui.Bar("TOP_MOST")
		self.Drop.SetParent(self.GetParentProxy())
		self.Drop.SetPosition(0,21)
		# self.Drop.SetSize(150,95)
		self.Drop.SetSize(150,0)
		# self.Drop.SetColor(0xc00a0a0a)
		self.Drop.SetColor(0xff0a0a0a)


		self.ScrollBar = ui.ThinScrollBar()
		self.ScrollBar.SetParent(self.Drop)
		self.ScrollBar.SetPosition(132,0)
		# self.ScrollBar.SetScrollBarSize(95)
		self.ScrollBar.SetScrollBarSize(0)
		# self.ScrollBar.Show()

		self.DropList = ui.ListBoxEx()
		self.DropList.SetParent(self.Drop)
		self.DropList.itemHeight = 12
		self.DropList.itemStep = 13
		self.DropList.SetPosition(0,0)
		# self.DropList.SetSize(132,self.maxh)
		self.DropList.SetSize(132,13)
		self.DropList.SetScrollBar(self.ScrollBar)
		self.DropList.SetSelectEvent(self.SetTitle)
		self.DropList.SetViewItemCount(0)
		self.DropList.Show()
		if self.tt != "":
			self.AppendItemAndSelect(self.tt)
		self.selected = self.DropList.GetSelectedItem()


		self.SetSize(120,20)
	def __del__(self):
		ui.Window.__del__(self)
	c = 1
	def AppendItem(self,text,value=0):
		self.c+=1
		self.DropList.AppendItem(self.Item(self,text,value))
		self.maxh = min(95,13*self.c)
		if self.c > 7:
			self.ScrollBar.Show()


	def AppendItemAndSelect(self,text,value=0):
		self.DropList.AppendItem(self.Item(self,text,value))
		self.DropList.SelectIndex(len(self.DropList.itemList)-1)


	def ClearItems(self):
		self.DropList.RemoveAllItems()
		self.AppendItemAndSelect(self.tt)
		self.act.SetText(self.tt)
	def Clear(self):
		self.DropList.SelectIndex(0)
	def SelectByAffectId(self,id):
		for x in self.DropList.itemList:
			if x.value == id:
				self.DropList.SelectItem(x)
				break

	def SetTitle(self,item):
		self.act.SetText(str(item.textBox.GetText()))
		self.last = self.DropList.basePos
		self.lastS = self.ScrollBar.GetPos()
		self.dropped = 0
		self.selected = item
		if self.OnChange:
			self.OnChange()
		# self.Drop.Hide()

	def SetPosition(self,w,h):
		ui.Window.SetPosition(self,w,h)
		if self.down == 1:
			self.Drop.SetPosition(w,h+21)
		else:
			self.Drop.SetPosition(w,h-self.Drop.GetHeight())

	def SetSize(self,w,h):
		ui.Window.SetSize(self,w,h)
		self.bg.SetSize(w,h)
		self.Drop.SetSize(w,0)
		self.DropList.SetSize(w-18,self.maxh)
		for x in self.DropList.itemList:
			x.SetSize(w-18,12)
		self.ScrollBar.SetPosition(w-18,0)


	def ExpandMe(self):
		if self.dropped == 1:
			# self.Drop.Hide()
			self.dropped = 0
		else:
			# self.Drop.Show()
			self.dropped = 1

	def OnUpdate(self):
		iter = 6
		if self.Drop.GetHeight() < 50:
			self.ScrollBar.Hide()
		else:
			self.ScrollBar.Show()

		if self.dropped == 0 and self.dropstat == 1:
			if self.Drop.GetHeight() <=0:
				self.dropstat = 0
				self.Drop.SetSize(self.Drop.GetWidth(),0)
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight())
				self.Drop.Hide()
			else:
				if self.Drop.GetHeight()-iter < 0:
					self.Drop.SetSize(self.Drop.GetWidth(),0)
				else:
					self.Drop.SetSize(self.Drop.GetWidth(),self.Drop.GetHeight()-iter)
					(w,h) = self.GetLocalPosition()
					self.SetPosition(w,h)


				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight())
			self.DropList.SetViewItemCount(int(self.Drop.GetHeight()/13))
			self.DropList.SetBasePos(self.last+1)
			self.DropList.SetBasePos(self.last)
		elif self.dropped == 1 and self.dropstat == 0:
			self.Drop.Show()
			self.SetTop()
			if self.Drop.GetHeight() >=self.maxh:
				self.Drop.SetSize(self.Drop.GetWidth(),self.maxh)
				self.ScrollBar.SetScrollBarSize(self.maxh)
				self.dropstat = 1
				self.DropList.SetViewItemCount(7)
				self.ScrollBar.SetPos(self.lastS)
			else:
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight()+iter)
				self.Drop.SetSize(self.Drop.GetWidth(),self.Drop.GetHeight()+iter)
				(w,h) = self.GetLocalPosition()
				self.SetPosition(w,h)
			self.DropList.SetViewItemCount(int(self.Drop.GetHeight()/13))
			self.DropList.SetBasePos(self.last+1)
			self.DropList.SetBasePos(self.last)

	## BG Hover
	def bgMouseIn(self):
		self.bg.SetColor(0xc00a0a0a)
	def bgMouseOut(self):
		self.bg.SetColor(0xc0000000)

class ShopEditWindow(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.items={}
		self.Edit={}
		self.pop=None
		self.id=0
		self.lastUpdate=0
		self.priceInputBoard=None
		self.__Load()

		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		self.Close()

	def __Load_LoadScript(self, fileName):
		try:
			pyScriptLoader = ui.PythonScriptLoader()
			pyScriptLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("ShopEditWindow.__Load_LoadScript")

	def __Load_BindObject(self):
		try:
			self.titleBar = self.GetChild("TitleBar")
			self.titleName = self.GetChild("TitleName")
			self.ItemSlot = self.GetChild("ItemSlot")
			try:
				for key in ["BuyButton","SellButton","MiddleTab1","MiddleTab2","MiddleTab2","SmallTab1","SmallTab2","SmallTab3"]:
					self.GetChild(key).Hide()
			except Exception:
				pass
			self.CloseButton = self.GetChild("CloseButton")
		except:
			import exception
			exception.Abort("StoneDialog.__Load_BindObject")

		self.CloseButton.SetText(uiScriptLocale.SHOP_EDIT_SHOP_CANCEL)
		self.CloseButton.SetEvent(ui.__mem_func__(self.Close))
		self.titleName.SetText(uiScriptLocale.SHOP_EDIT_SHOP_WINDOW)
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.ItemSlot.SAFE_SetButtonEvent("LEFT", "EMPTY", self.OnSelectEmptySlot)
		self.ItemSlot.SAFE_SetButtonEvent("LEFT", "EXIST", self.OnSelectItemSlot)
		self.ItemSlot.SAFE_SetButtonEvent("RIGHT", "EXIST", self.UnselectItemSlot)
		self.ItemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.ItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		self.Edit["Board"] = ui.BoardWithTitleBar()
		self.Edit["Board"].SetSize(150, 100)
		self.Edit["Board"].AddFlag("movable")
		self.Edit["Board"].SetPosition(0,0)
		self.Edit["Board"].SetTitleName(uiScriptLocale.SHOP_SELECT)
		self.Edit["Board"].SetCloseEvent(self.CloseEdit)
		self.Edit["Board"].Show()




		self.Edit["ChangePrice"] = ui.Button()
		self.Edit["ChangePrice"].SetParent(self.Edit["Board"])
		self.Edit["ChangePrice"].SetPosition(30,35)
		self.Edit["ChangePrice"].SetUpVisual('d:/ymir work/ui/public/Large_button_01.sub')
		self.Edit["ChangePrice"].SetOverVisual('d:/ymir work/ui/public/Large_button_02.sub')
		self.Edit["ChangePrice"].SetDownVisual('d:/ymir work/ui/public/Large_button_03.sub')
		self.Edit["ChangePrice"].SetText(uiScriptLocale.SHOP_CHANGE_PRICE)
		self.Edit["ChangePrice"].Show()

		self.Edit["Remove"] = ui.Button()
		self.Edit["Remove"].SetParent(self.Edit["Board"])
		self.Edit["Remove"].SetPosition(30,65)
		self.Edit["Remove"].SetUpVisual('d:/ymir work/ui/public/Large_button_01.sub')
		self.Edit["Remove"].SetOverVisual('d:/ymir work/ui/public/Large_button_02.sub')
		self.Edit["Remove"].SetDownVisual('d:/ymir work/ui/public/Large_button_03.sub')
		self.Edit["Remove"].SetText(uiScriptLocale.SHOP_REMOVE_ITEM)
		self.Edit["Remove"].Show()
	def __Load(self):
		self.__Load_LoadScript("UIScript/ShopDialog.py")
		self.__Load_BindObject()
	def CloseEdit(self):
		self.Edit["Board"].Hide()
	def Show(self,id):
		if self.IsShow():
			self.Close()
		else:
			ui.ScriptWindow.Show(self)
			net.SendChatPacket("/refresh_shop_items "+id)
			self.id=id
		self.Refresh()

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE
	def __OnClosePopupDialog(self):
		if self.pop != None:
			if self.pop.IsShow():
				self.pop.Hide()
		self.pop = None
	def __GetRealIndex(self, i):
		return shop.SHOP_SLOT_COUNT + i

	def Close(self):
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()
		self.CancelInputPrice()
		self.__OnClosePopupDialog()
		self.Edit["Board"].Hide()
		self.Hide()
		return TRUE

	def Clear(self):
		self.items={}
		self.CancelInputPrice()
		self.__OnClosePopupDialog()
		self.Edit["Board"].Hide()
		self.Refresh()

	def GetItemCount(self,slot):
		try:
			return int(self.items[int(slot)]["count"])
		except KeyError:
			return 0

	def GetItemID(self,slot):
		try:
			return int(self.items[int(slot)]["vnum"])
		except KeyError:
			return 0

	def AddItem(self,slot,data):
		self.items[int(slot)]=data
		self.Refresh()
	def Refresh(self):
		self.CancelInputPrice()
		self.__OnClosePopupDialog()
		self.Edit["Board"].Hide()
		setItemID=self.ItemSlot.SetItemSlot
		for i in xrange(shop.SHOP_SLOT_COUNT):
			vnum=self.GetItemID(i)
			itemCount = self.GetItemCount(i)

			setItemID(i, vnum, itemCount)
		wndMgr.RefreshSlot(self.ItemSlot.GetWindowHandle())
		self.ItemSlot.RefreshSlot()

	def __ShowToolTip(self, slot):
		if self.tooltipItem:
			self.tooltipItem.ClearToolTip()
			if int(slot) in self.items.keys():
				it=self.items[int(slot)]
				if it.get("sourceSlot",-1)!=-1:
					self.tooltipItem.SetEditPrivateShopItem(int(it["sourceWindow"]),int(it["sourceSlot"]),it["price"])
				else:
					# if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
						# self.tooltipItem.AppendSellingCheque2(it["cheque"])
					self.tooltipItem.AddItemData(int(it["vnum"]),it["sockets"],it["attrs"], refineElement = int(it["refine_element"]))
					#self.tooltipItem.AddItemData(int(it["vnum"]),it["sockets"],it["attrs"],0, 0, player.INVENTORY, -1, refineElement = it["refine_element"])
					self.tooltipItem.AppendSellingPrice(it["price"])
					self.tooltipItem.AppendSellingCheque2(it["cheque"])
			else:
				self.tooltipItem.HideToolTip()

	def OverInItem(self, slotIndex):
		#slotIndex = self.__GetRealIndex(slotIndex)
		self.ItemSlot.SetUsableItem(FALSE)
		self.__ShowToolTip(slotIndex)

	def OverOutItem(self):
		self.ItemSlot.SetUsableItem(FALSE)
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
	def OnSelectEmptySlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			if player.SLOT_TYPE_INVENTORY != attachedSlotType and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedSlotType:
				return
			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
			count = player.GetItemCount(attachedInvenType, attachedSlotPos)

			itemVNum = player.GetItemIndex(attachedInvenType, attachedSlotPos)
			item.SelectItem(itemVNum)



			if item.IsAntiFlag(item.ANTIFLAG_GIVE) or item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PRIVATE_SHOP_CANNOT_SELL_ITEM)
				return

			priceInputBoard = uiCommon.MoneyInputDialog()
			priceInputBoard.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_PRICE_DIALOG_TITLE)
			priceInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrice))
			priceInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrice))
			priceInputBoard.SetMaxLength(16)
			priceInputBoard.Open()

			#self.ItemSlot.ActivateSlot(int(selectedSlotPos))

			self.priceInputBoard = priceInputBoard
			self.priceInputBoard.itemVNum = itemVNum
			self.priceInputBoard.sourceWindowType = attachedInvenType
			self.priceInputBoard.sourceSlotPos = attachedSlotPos
			self.priceInputBoard.targetSlotPos = selectedSlotPos
	def UnselectItemSlot(self,selectedSlotPos):
		self.Edit["Board"].Hide()
		#self.ItemSlot.DeactivateSlot(int(selectedSlotPos))
		self.CancelInputPrice()
		self.__OnClosePopupDialog()
	def OnSelectItemSlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		#selectedSlotPos = self.__GetRealIndex(selectedSlotPos)
		if isAttached:
			snd.PlaySound("sound/ui/loginfail.wav")
			#self.ItemSlot.DeactivateSlot(int(selectedSlotPos))
			mouseModule.mouseController.DeattachObject()
			self.Edit["Board"].Hide()
		else:
			if not int(selectedSlotPos) in self.items.keys():
				self.Edit["Board"].Hide()
				return

			snd.PlaySound("sound/ui/drop.wav")
			#self.ItemSlot.ActivateSlot(selectedSlotPos)
			self.EditItem(selectedSlotPos)

	def AcceptInputPrice(self):

		if not self.priceInputBoard:
			return True

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
		price = int(self.priceInputBoard.GetText())
		if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
			cheque = int(self.priceInputBoard.GetCheque())
		count = player.GetItemCount(attachedInvenType, sourceSlotPos)
		vnum = player.GetItemIndex(attachedInvenType, sourceSlotPos)
		if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
			self.items[int(targetSlotPos)]={
				"vnum":int(vnum),
				"count":int(count),
				"price":int(price),
				"cheque":int(cheque),
				"sourceSlot":sourceSlotPos,
				"sourceWindow":attachedInvenType
			}

		else:
			self.items[int(targetSlotPos)]={
				"vnum":int(vnum),
				"count":int(count),
				"price":int(price),
				"sourceSlot":sourceSlotPos,
				"sourceWindow":attachedInvenType
			}
		snd.PlaySound("sound/ui/pick.wav")
		#self.ItemSlot.DeactivateSlot(targetSlotPos)
		if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
			net.SendChatPacket("/update_shop_item add|%d|%d|%d|%d|%s|%s"%(int(self.id),int(targetSlotPos),int(sourceSlotPos),int(attachedInvenType),str(price), str(cheque)))
		else:
			net.SendChatPacket("/update_shop_item add|%d|%d|%d|%d|%s"%(int(self.id),int(targetSlotPos),int(sourceSlotPos),int(attachedInvenType),str(price)))
		self.Refresh()		

		#####

		self.priceInputBoard = None
		return True

	def CancelInputPrice(self):
		self.priceInputBoard = None
		return True
	def EditItem(self,slot):
		self.Edit["ChangePrice"].SetEvent(ui.__mem_func__(self.EditPrice),int(slot))
		self.Edit["Remove"].SetEvent(ui.__mem_func__(self.RemoveItem),int(slot))
		(w,h)=(170,328)
		(x,y)=self.GetLocalPosition()
		self.Edit["Board"].SetPosition((x-w),(y+(h/2)))
		self.Edit["Board"].Show()
	def EditPrice(self,slot):
		self.priceInputBoard = uiCommon.MoneyInputDialog()
		self.priceInputBoard.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_PRICE_DIALOG_TITLE)
		self.priceInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptEditPrice))
		self.priceInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrice))
		self.priceInputBoard.SetMaxLength(16)
		self.priceInputBoard.Open()
		self.priceInputBoard.targetSlotPos = int(slot)
	def RemoveItem(self,slot):
		self.pop = uiCommon.QuestionDialog()
		self.pop.SetText(uiScriptLocale.SHOP_REMOVE_ITEM_QUEST)
		self.pop.SetAcceptEvent(lambda arg1=str(slot): self.AcceptRemoveItem(arg1))
		self.pop.SetCancelEvent(ui.__mem_func__(self.__OnClosePopupDialog))
		self.pop.Open()
	def AcceptRemoveItem(self,slot):
		if int(slot) in self.items.keys():
			snd.PlaySound("sound/ui/drop.wav")
			net.SendChatPacket("/update_shop_item remove|"+str(self.id)+"|"+str(self.items[int(slot)]["id"]))
			#self.ItemSlot.DeactivateSlot(int(slot))
			del self.items[int(slot)]
		self.Refresh()

		self.__OnClosePopupDialog()
	def AcceptEditPrice(self):

		if not self.priceInputBoard:
			return True

		text = self.priceInputBoard.GetText()

		if not text:
			return True

		if not text.isdigit():
			return True

		if int(text) <= 0:
			return True

		targetSlotPos = self.priceInputBoard.targetSlotPos
		price = int(self.priceInputBoard.GetText())
		if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
			cheque = int(self.priceInputBoard.GetCheque())
		#self.ItemSlot.DeactivateSlot(int(targetSlotPos))
		self.items[int(targetSlotPos)]["price"]=price
		if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
			self.items[int(targetSlotPos)]["cheque"]=cheque
		snd.PlaySound("sound/ui/drop.wav")
		if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
			net.SendChatPacket("/update_shop_item price|"+str(self.id)+"|"+str(self.items[int(targetSlotPos)]["id"])+"|"+str(price)+"|"+str(cheque))
		else:
			net.SendChatPacket("/update_shop_item price|"+str(self.id)+"|"+str(self.items[int(targetSlotPos)]["id"])+"|"+str(price))
		self.Refresh()	
		self.priceInputBoard = None
		return True
	# def OnUpdate(self):
		# if self.IsShow() and self.lastUpdate < app.GetGlobalTime():
			# self.lastUpdate=app.GetGlobalTime()+1000
			# if int(self.id)>0:				
				# net.SendChatPacket("/update_shop_item edit_tick|"+self.id)
class ShopDialog(ui.ScriptWindow):

	Edit={}
	UI={}
	pop=None
	uiNewShopEdit = ShopEditWindow()
	uiNewShopEdit.Close()
	tooltip = uiToolTip.ToolTip(220)
	tooltip.Hide()
	BoardHeight=40
	CurrentEdit=0
	EditBoardY=35
	lastUpdate=0
	def __init__(self, interface):
		self.interface = interface
		self.uiNewShopCreate = ShopDialogCreate(interface)
		self.uiNewShopCreate.Hide()
		ui.ScriptWindow.__init__(self)
		self.LoadDialog()
	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		self.UI["board"]=ui.BoardWithTitleBar()
		self.UI["board"].SetParent(self)
		self.UI["board"].SetSize(200, self.BoardHeight)
		self.UI["board"].SetTitleName(uiScriptLocale.SHOP_WINDOW_NAME)
		self.UI["board"].SetCloseEvent(self.Close)
		self.UI["board"].Show()
		self.UI["shops"]={}

		self.Edit["Board"] = ui.BoardWithTitleBar()
		self.Edit["Board"].SetParent(self)
		self.Edit["Board"].SetSize(150, 100)
		if POSITION_FIX:
			self.Edit["Board"].SetPosition(250,0)
		else:
			self.Edit["Board"].SetPosition(220,0)
		self.Edit["Board"].SetTitleName(uiScriptLocale.SHOP_SELECT)
		self.Edit["Board"].SetCloseEvent(self.CloseEdit)
		self.Edit["Board"].Hide()

		self.SetCenterPosition()
		self.AddFlag("movable")
		self.UpdateSize()
	def AddEditOption(self,name,text,func):
		self.Edit[name] = ui.Button()
		self.Edit[name].SetParent(self.Edit["Board"])
		self.Edit[name].SetPosition(30,self.EditBoardY)
		self.Edit[name].SetUpVisual('d:/ymir work/ui/public/Large_button_01.sub')
		self.Edit[name].SetOverVisual('d:/ymir work/ui/public/Large_button_02.sub')
		self.Edit[name].SetDownVisual('d:/ymir work/ui/public/Large_button_03.sub')
		self.Edit[name].SetText(text)
		self.Edit[name].Show()
		self.EditBoardY+=30
		self.Edit["Board"].SetSize(150, self.EditBoardY)
		self.Edit[name].SetEvent(func)
		self.UpdateSize()
	def CloseEdit(self):
		if "Board" in self.Edit.keys():
			self.Edit["Board"].Hide()
		self.CurrentEdit=0

	def Load(self,data):
		title=data["name"]
		if len(title) > 22:
			title = title[:19] + "..."
		gui={}
		gui["nameTextImage"] = ui.MakeImageBox(self.UI["board"], "d:/ymir work/ui/public/Parameter_Slot_04.sub", 10, self.BoardHeight)
		gui["nameTextImage"].SAFE_SetStringEvent("MOUSE_OVER_IN", lambda arg=data: self.__ShowToolTip(arg),TRUE)
		gui["nameTextImage"].SAFE_SetStringEvent("MOUSE_OVER_OUT", self.__HideItemToolTip)
		gui["name"] = ui.MakeTextLine(gui["nameTextImage"])
		gui["name"].SetWindowHorizontalAlignCenter()
		gui["name"].SetHorizontalAlignCenter()
		gui["name"].SetText(title)

		gui["manage"] = ui.MakeButton(self.UI["board"],130, self.BoardHeight,"","d:/ymir work/ui/public/","middle_Button_01.sub","middle_Button_02.sub","middle_Button_03.sub")
		gui["manage"].SetText(uiScriptLocale.SHOP_MANAGE)
		gui["manage"].SetEvent(ui.__mem_func__(self.ManageShop),data)
		gui["manage"].Show()
		if self.CurrentEdit==data["id"]:
			self.ManageShop(data,1)
		gui["data"]=data
		self.UI["shops"][str(data["id"])]=gui
		self.BoardHeight+=30
		self.UpdateSize()

	def __OnClosePopupDialog(self):
		if self.pop != None:
			if self.pop.IsShow():
				self.pop.Hide()
		self.pop = None

	def ManageShop(self,data,force=0):
		self.EditBoardY=35
		self.AddEditOption("change_name",uiScriptLocale.SHOP_CHANGE_NAME,lambda arg1=data: self.OnChangeButtonClick(arg1))
		self.AddEditOption("get_yang",uiScriptLocale.SHOP_GET_YANG,lambda arg1=data: self.GetShopYang(arg1))
		if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
			self.AddEditOption("get_cheque",uiScriptLocale.SHOP_GET_CHEQUE,lambda arg1=data: self.GetShopCheque(arg1))
		self.AddEditOption("edit",uiScriptLocale.SHOP_EDIT,lambda arg1=data["id"]: self.OnEditButtonClick(arg1))
		self.AddEditOption("close",uiScriptLocale.SHOP_CLOSE,lambda arg1=data: self.CloseShop(arg1))
		if self.Edit["Board"].IsShow() and self.CurrentEdit==data["id"] and force==0:
			self.Edit["Board"].Hide()
			self.CurrentEdit=0
		else:
			title=data["name"]
			if len(title) > 22:
				title = title[:19] + "..."
			self.Edit["Board"].SetTitleName(title)
			self.Edit["Board"].Show()
			self.CurrentEdit=data["id"]

	def UpdateSize(self):
		Y=25
		if POSITION_FIX:
			Y=45
		self.UI["create_button"] = ui.MakeButton(self.UI["board"],10, self.UI["board"].GetHeight()-Y,"","d:/ymir work/ui/public/","Xlarge_Button_01.sub","Xlarge_Button_02.sub","Xlarge_Button_03.sub")
		self.UI["create_button"].SetText(uiScriptLocale.SHOP_OPEN)
		self.UI["create_button"].SetEvent(lambda : self.CreateShop())
		self.UI["create_button"].Show()
		if POSITION_FIX:
			self.UI["board"].SetSize(200, self.BoardHeight+55)
		else:
			self.UI["board"].SetSize(200, self.BoardHeight+35)
		if self.BoardHeight < self.EditBoardY:
			self.SetSize(400, self.EditBoardY+50)
		else:
			self.SetSize(400, self.BoardHeight+60)

	def CreateShop(self):
		self.Hide()
		self.uiNewShopCreate.Hide()
		self.uiNewShopCreate.Destroy()
		self.uiNewShopCreate = ShopDialogCreate(self.interface)
		self.uiNewShopCreate.Show()

	def CloseShop(self,shop):
		self.pop = uiCommon.QuestionDialog()
		self.pop.SetText(uiScriptLocale.SHOP_CLOSE_QUEST%(shop["name"]))
		self.pop.SetAcceptEvent(lambda arg1=str(shop["id"]): self.OnCloseShop(arg1))
		self.pop.SetCancelEvent(ui.__mem_func__(self.__OnClosePopupDialog))
		self.pop.Open()

	def OnCloseShop(self,id):
		self.__OnClosePopupDialog()
		net.SendChatPacket("/close_shop %d" %(int(id)))
		if len(self.UI["shops"].keys())==1:
			self.Close()

	if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
		def GetShopCheque(self,shop):
			if int(shop["cheque"]) <=0:
				self.PopupMessage(uiScriptLocale.SHOP_NOT_EARNED_CHEQUE)
				return
			self.pop = uiCommon.QuestionDialog()
			self.pop.SetText(uiScriptLocale.SHOP_CHEQUE_QUEST%(localeInfo.NumberToMoneyString(shop["cheque"]),shop["name"]))
			self.pop.SetAcceptEvent(lambda arg1=str(shop["id"]): self.OnGetCheque(arg1))
			self.pop.SetCancelEvent(ui.__mem_func__(self.__OnClosePopupDialog))
			self.pop.Open()
		def OnGetCheque(self,id):
			self.__OnClosePopupDialog()
			net.SendChatPacket("/shop_cheque %d" %(int(id)))

	def GetShopYang(self,shop):
		if int(shop["gold"]) <=0:
			self.PopupMessage(uiScriptLocale.SHOP_NOT_EARNED_YANG)
			return
		self.pop = uiCommon.QuestionDialog()
		self.pop.SetText(uiScriptLocale.SHOP_YANG_QUEST%(localeInfo.NumberToMoneyString(shop["gold"]),shop["name"]))
		self.pop.SetAcceptEvent(lambda arg1=str(shop["id"]): self.OnGetYang(arg1))
		self.pop.SetCancelEvent(ui.__mem_func__(self.__OnClosePopupDialog))
		self.pop.Open()
	def AddItem(self,slot,data):
		self.uiNewShopEdit.AddItem(slot,data)
	def ClearItems(self):
		self.uiNewShopEdit.Clear()
	def OnGetYang(self,id):
		self.__OnClosePopupDialog()
		net.SendChatPacket("/shop_yang %d" %(int(id)))

	def OnChangeButtonClick(self,shop):
		inputDialog = uiCommon.InputDialog()
		inputDialog.SetTitle(uiScriptLocale.SHOP_ENTER_NEW_NAME)
		inputDialog.SetMaxLength(32)
		inputDialog.SetAcceptEvent(lambda arg1=str(shop["id"]): self.OnChangeName(arg1))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.CloseInputNameDialog))
		inputDialog.Open()

		self.inputDialog = inputDialog

	def CloseInputNameDialog(self):
		self.inputDialog = None

	def OnChangeName(self,id):
		if not self.inputDialog:
			return
		name=self.inputDialog.GetText()
		if not len(name):
			return
		name=name.replace(" ","\\")
		net.SendChatPacket("/shop_name %d %s" %(int(id),name))
		self.CloseInputNameDialog()

	def Destroy(self):
		self.Hide()
		self.__OnClosePopupDialog()
		self.UI={}
		self.wndPopupDialog = None
		self.inputDialog = None
		if self.uiNewShopEdit:
			self.uiNewShopEdit.Close()
		self.uiNewShopEdit = None
	def OnPressEscapeKey(self):
		self.Hide()
	def Hide(self):
		self.CloseEdit()
		self.__OnClosePopupDialog()
		if self.uiNewShopEdit:
			self.uiNewShopEdit.Close()
		self.__HideItemToolTip()
		ui.ScriptWindow.Hide(self)
	def Close(self):
		self.Hide()

	def __ShowToolTip(self,shop):
		if self.tooltip:
			self.tooltip.ClearToolTip()
			self.tooltip.AutoAppendTextLine(uiScriptLocale.SHOP_INFORMATION+str(shop["name"]), titleColor)
			self.tooltip.AppendSpace(5)
			self.tooltip.AutoAppendTextLine(uiScriptLocale.SHOP_EARNED_YANG+str(localeInfo.NumberToMoneyString((shop["gold"]))), POSITIVE_COLOR)
			self.tooltip.AppendSpace(5)
			if app.__ENABLE_CHEQUE_OFFLINE_SHOP__:
				self.tooltip.AutoAppendTextLine(uiScriptLocale.SHOP_EARNED_CHEQUE+str(localeInfo.NumberToMoneyString((shop["cheque"]))), POSITIVE_COLOR)	
				self.tooltip.AppendSpace(5)
			self.tooltip.AutoAppendTextLine(uiScriptLocale.SHOP_REST_ITEMS+str(int(shop["sold"])), POSITIVE_COLOR)
			self.tooltip.AppendSpace(5)
			self.tooltip.AutoAppendTextLine(uiScriptLocale.SHOP_SOLD_ITEMS+str(shop["items"]), POSITIVE_COLOR)
			self.tooltip.AppendSpace(5)
			self.tooltip.AutoAppendTextLine(uiScriptLocale.SHOP_TIME_TO_CLOSE+str(localeInfo.SecondToDHM(int(shop["time"])-int(time.time()))), NEGATIVE_COLOR)
			self.tooltip.AppendSpace(5)
			self.tooltip.ShowToolTip()


	def __HideItemToolTip(self):
		if self.tooltip:
			self.tooltip.HideToolTip()

	def OnEditButtonClick(self,id):
		if self.uiNewShopEdit.IsShow():
			self.uiNewShopEdit.Close()
		else:
			self.uiNewShopEdit.Show(str(id))
	def Show(self):
		ui.ScriptWindow.Show(self)
		if len(self.UI["shops"].keys())==0:
			self.CreateShop()
		else:
			ui.ScriptWindow.Show(self)
	def HideAll(self):
		self.BoardHeight=40
		for key,item in self.UI["shops"].iteritems():
			for k,v in item.iteritems():
				if k !="data":
					v.Hide()
			self.UI["shops"][key]["data"]={}
		self.UI["shops"]={}
	def PopupMessage(self, msg):
		self.wndPopupDialog = uiCommon.PopupDialog()
		self.wndPopupDialog.SetText(msg)
		self.wndPopupDialog.Open()
	def OnUpdate(self):
		if self.lastUpdate < app.GetGlobalTime():
			self.lastUpdate=app.GetGlobalTime()+900
			if "shops" in self.UI.keys():
				if str(self.CurrentEdit) not in self.UI["shops"].keys():
					self.CloseEdit()
		if "create_button" in self.UI.keys():
			if POSITION_FIX:
				self.UI["create_button"].SetPosition(10, self.UI["board"].GetHeight()-45)
			else:
				self.UI["create_button"].SetPosition(10, self.UI["board"].GetHeight()-40)
