import ui
import item
import wndMgr
import player 
import uiToolTip
import net
import constInfo
import event 
import chat
import uiCommon
import localeInfo
import app
import chat

class SelectGems(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()

		self.slot_inventario,self.slot_gui,self.slotPos,self.slot_select = [],[],0,0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/selectitemwindow.py")
		except:
			import exception
			exception.Abort("ItemSelectWindow.LoadDialog.LoadObject")

		try:
			GetObject = self.GetChild
			self.board,self.titleBar,self.itemSlot,self.btnExit = GetObject("board"),GetObject("TitleBar"),GetObject("ItemSlot"),GetObject("ExitButton")
		except:
			import exception
			exception.Abort("ItemSelectWindow.LoadDialog.BindObject")

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.btnExit.SetEvent(ui.__mem_func__(self.Close))
		self.itemSlot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		self.itemSlot.SAFE_SetButtonEvent("LEFT", "EXIST", self.SelectItemSlot)
		self.itemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

	def Close(self):
		wndMgr.OnceIgnoreMouseLeftButtonUpEvent()
		self.Hide()

	def Open(self):
		self.Clear()
		self.Show()
		self.RefreshSlot(False)

	def Clear(self):
		self.slot_inventario,self.slot_gui,self.slotPos,self.slot_select = [],[],0,0

	def SelectItemSlot(self, slotPos):
		wndMgr.OnceIgnoreMouseLeftButtonUpEvent()
		SlotInventario,SlotGui,self.slot_select = self.slot_inventario[slotPos],slotPos,slotPos
		net.GayaCraft(SlotInventario)

	def SetTableSize(self, size):
		SLOT_X_COUNT = 5
		self.itemSlot.ArrangeSlot(0, SLOT_X_COUNT, size, 32, 32, 0, 0)
		self.itemSlot.RefreshSlot()
		self.itemSlot.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
		self.board.SetSize(self.board.GetWidth(), 76 + 32*size)
		self.SetSize(self.board.GetWidth(), 76 + 32*size)
		self.UpdateRect()

	def RefreshSlot(self,refresh = False):
		getItemVNum,getItemCount,setItemVNum=player.GetItemIndex,player.GetItemCount,self.itemSlot.SetItemSlot

		if not refresh:
			for i in xrange(player.STONE_INVENTORY_SLOT_COUNT*5):
				slotNumber = item.STONE_INVENTORY_SLOT_START + i
				itemVNum = getItemVNum(slotNumber)

				if 0 == itemVNum or not item.IsMetin(itemVNum):
					continue

				if not item.IsMetin(itemVNum):
					continue

				itemGrade = player.GetItemGrade(slotNumber)
				if itemGrade > 3:
					continue

				self.slot_inventario.append(slotNumber)
				self.slot_gui.append(self.slotPos)

				self.slotPos += 1
				if self.slotPos > 54:
					break

		itemCount = len(self.slot_inventario)
		if itemCount < 15:
			self.SetTableSize(3)

		else:
			lineCount = 3
			lineCount += (itemCount - 15) / 5
			if itemCount % 5:
				lineCount += 1
			self.SetTableSize(lineCount)

		count = 0
		for inventoryPos in self.slot_inventario:
			itemVNum = getItemVNum(inventoryPos)
			itemCount = getItemCount(inventoryPos)

			if itemCount <= 1:
				itemCount = 0

			setItemVNum(count, itemVNum, itemCount)
			count += 1
	
		self.itemSlot.RefreshSlot()
		if refresh:
			self.tooltipItem.Hide()

	def OverOutItem(self):
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OverInItem(self, slotIndex):
		if None != self.tooltipItem:
			inventorySlotPos = self.slot_inventario[slotIndex]
			self.tooltipItem.SetInventoryItem(inventorySlotPos)

	def SucceedGaya(self):
		del self.slot_inventario[self.slot_select]
		del self.slot_gui[self.slot_select]
		self.RefreshSlot(True)

	def OnPressEscapeKey(self):
		self.Close()
		return True

class SelectGemsShop(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()

		self.MessageBuy = uiCommon.QuestionDialog()
		self.pLeftTime = 0

		self.items = []
		self.slots_block = []


	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "Modulo/gemshopwindow.py")
		except:
			import exception
			exception.Abort("SelectGemsShop.LoadDialog.LoadObject")

		try:
			GetObject = self.GetChild
			self.bg = GetObject("bg_slots")
			self.refresh_items = GetObject("refresh_button")
			self.btnExit = GetObject("TitleBar")
			self.time_gaya = GetObject("time_gaya")
		except:
			import exception
			exception.Abort("SelectGems.LoadDialog.BindObject")

		self.btnExit.SetCloseEvent(ui.__mem_func__(self.Close))
		self.refresh_items.SetEvent(ui.__mem_func__(self.RefreshItems))

		self.ItemSlotResult = ui.GridSlotWindow()
		self.ItemSlotResult.SetParent(self.bg)
		self.ItemSlotResult.SetPosition(8, 28)
		self.ItemSlotResult.ArrangeSlot(0, 3, 3, 32, 32, 13, 26)
		self.ItemSlotResult.SAFE_SetButtonEvent("LEFT", "EXIST", self.SelectItemSlot)
		self.ItemSlotResult.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.ItemSlotResult.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		self.ItemSlotResult.Show()

		positions = [
			[8,28],[8+45,28],[8+(45*2),28],
			[8,28+58],[8+45,28+58],[8+(45*2),28+58],
			[8,28+(58*2)],[8+45,28+(58*2)],[8+(45*2),28+(58*2)]
		]

		self.name,self.icongaya = {},{}
		count = 0

		for i in positions:
			self.icongaya[count] = ui.ImageBox()
			self.icongaya[count].SetParent(self.bg)
			self.icongaya[count].SetPosition(positions[count][0]-2,positions[count][1]+40)
			self.icongaya[count].LoadImage("d:/ymir work/ui/gemshop/gemshop_gemicon.sub")
			self.icongaya[count].Show()

			self.name[count] = ui.TextLine()
			self.name[count].SetParent(self.bg)
			self.name[count].SetPosition(positions[count][0]+14,positions[count][1]+39)
			self.name[count].Show()
			count +=1

	def Open(self):
		self.Show()

	def Time(self, time):
		self.pLeftTime =  int(time) + app.GetGlobalTimeStamp()

	def SetTime(self, time):
		time_collect = time - app.GetGlobalTimeStamp()

		if time_collect < 0:
			time_collect = 0
			#net.SendChatPacket("/w_gaya refresh_time")

		self.time_gaya.SetText(self.__FormatTime(time_collect))

	def __FormatTime(self, time):
		m, s = divmod(time, 60)
		h, m = divmod(m, 60)
		return "%1d:%02d:%02d" % (h, m, s)	

	def OnUpdate(self):
		self.SetTime(self.pLeftTime)

	def Clear(self):
		self.items = []
		self.slots_block = []

	def Close(self):
		self.Hide()
		self.IsOpen = False
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS():
			self.OnCloseQuestionDialog()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def Destroy(self):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS():
			self.OnCloseQuestionDialog()

	def Information(self,vnums,precios,count):
		self.items.append([int(vnums),int(precios),int(count)])

	def SlotsDesblock(self,slot0,slot1,slot2,slot3,slot4,slot5):
		self.slots_block.append([slot0,slot1,slot2,slot3,slot4,slot5])
		count = 0
		for i in xrange(0,6):
			if(self.slots_block[0][i] == "0"):
				self.ItemSlotResult.DisableSlot(count+3)
			count +=1
	
	def LoadInformation(self):
		for i in xrange(0,len(self.items)):
			self.name[i].SetText(str(self.items[i][1]))
			self.ItemSlotResult.SetItemSlot(i, self.items[i][0], self.items[i][2])

	def SelectItemSlot(self, slotPos):
		if(slotPos >= 3):
			if(self.slots_block[0][slotPos-3] == "0"):
				self.MessageBuy.SetText("You don't have Gaya Market Expansion.")
			else:
				self.MessageBuy.SetText("Do you want to purchase this item.")
		else:
			self.MessageBuy.SetText("Do you want to purchase this item.")
		self.MessageBuy.SetAcceptEvent(lambda arg=TRUE, positions= slotPos: self.BuyFunction(arg,positions))
		self.MessageBuy.SetCancelEvent(lambda arg=FALSE, positions= slotPos: self.BuyFunction(arg,positions))
		self.MessageBuy.Open()		

	def OverOutItem(self):
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OverInItem(self, slotIndex):
		if None != self.tooltipItem:
			self.tooltipItem.ClearToolTip()
			self.tooltipItem.SetItemToolTip(int(self.items[slotIndex][0]))

	def RefreshItems(self):
		self.MessageBuy.SetText("Do you want to refresh the shop?.")
		self.MessageBuy.SetAcceptEvent(lambda arg=TRUE: self.RefreshFunction(arg))
		self.MessageBuy.SetCancelEvent(lambda arg=FALSE: self.RefreshFunction(arg))
		self.MessageBuy.Open()	

	def BuyFunction(self,arg,slot=-1):
		self.MessageBuy.Close()

		if arg:
			net.GayaMarket(slot)

	def RefreshFunction(self,arg):
		self.MessageBuy.Close()

		if arg:
			net.GayaRefresh()