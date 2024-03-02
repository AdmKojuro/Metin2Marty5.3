import ui
import app
import constInfo
import chat
import uiScriptLocale
import net
import player
import item
import mouseModule
import uiPrivateShopBuilder
import cPickle as pickle
import uiCommon
import uiToolTip

PATCH_DESIGN = "fast_equip/design/"

class UiFastEquip(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.toolTip 		= uiToolTip.ItemToolTip()

		self.elements = {}
		self.elements["save_presets"] = {}
		self.elements["max_presets_view"] = 7
		self.elements["max_presets_count"] = player.CHANGE_EQUIP_PAGE_EXTRA
		self.elements["scrollbar_pos"] = 1
		self.elements["select_presets"] = -1

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadPresets(self):
		try:
			with open('fast_equip.wa', 'rb') as f:
				self.elements["save_presets"] = pickle.load(f)
		except IOError:
			self.elements["save_presets"] = {}

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/fastequip_windows.py")
		except:
			import exception
			exception.Abort("UiFastEquip.LoadWindow.LoadObject")

		try:
			self.equipament 		= self.GetChild2("Equipament")
			self.thinboard_presets 	= self.GetChild2("thinboard_presets")
			self.title_bar_s		= self.GetChild2("title_bar_s")
			self.Change_Equipament  = self.GetChild2("Change_Equipament")
		except:
			import exception
			exception.Abort("UiFastEquip.LoadWindow.LoadElements")

		self.LoadPresets()
		self.PresetsElements()

		self.Change_Equipament.SetEvent(ui.__mem_func__(self.ClickChangeEquip))
		self.equipament.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		self.equipament.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		self.equipament.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		self.equipament.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.equipament.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		self.GetChild2("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))

	def PresetsElements(self):
		for i in xrange(0,self.elements["max_presets_view"]):
			self.elements["slot_presets%d"%i] = ui.RadioButton()
			self.elements["slot_presets%d"%i].SetParent(self.thinboard_presets)
			self.elements["slot_presets%d"%i].SetUpVisual(PATCH_DESIGN+"slot_bg.tga")
			self.elements["slot_presets%d"%i].SetOverVisual(PATCH_DESIGN+"slot_bg_h.tga")
			self.elements["slot_presets%d"%i].SetDownVisual(PATCH_DESIGN+"slot_bg_s1.tga")
			self.elements["slot_presets%d"%i].SetPosition(3,24+i*22)
			self.elements["slot_presets%d"%i].Show()

			self.elements["button_rename_%d"%i] = ui.Button()
			self.elements["button_rename_%d"%i].SetParent(self.thinboard_presets)
			self.elements["button_rename_%d"%i].SetUpVisual(PATCH_DESIGN+"button_change_name.tga")
			self.elements["button_rename_%d"%i].SetOverVisual(PATCH_DESIGN+"button_change_name_hover.tga")
			self.elements["button_rename_%d"%i].SetDownVisual(PATCH_DESIGN+"button_change_name.tga")
			self.elements["button_rename_%d"%i].SetPosition(145,24+i*22)
			self.elements["button_rename_%d"%i].Show()

			self.elements["slot_presets_name%d"%i] = ui.TextLine()
			self.elements["slot_presets_name%d"%i].SetParent(self.elements["slot_presets%d"%i])
			self.elements["slot_presets_name%d"%i].SetPosition(5,2)
			#self.elements["slot_presets_name%d"%i].SetHorizontalAlignCenter()
			self.elements["slot_presets_name%d"%i].SetText("{}".format(i+1))
			self.elements["slot_presets_name%d"%i].Show()

		self.elements["scrollbar"] = ScrollBarNew()
		self.elements["scrollbar"].SetParent(self.thinboard_presets)
		self.elements["scrollbar"].SetPosition(173,24)
		self.elements["scrollbar"].SetScrollBarSize(187)
		self.elements["scrollbar"].SetScrollEvent(ui.__mem_func__(self.OnScroll))
		self.elements["scrollbar"].Show()

		self.SetViewPresets()
		self.FuncSelectPresets(0,1)

	def SetViewPresets(self):
		for i in xrange(0,self.elements["max_presets_view"]):
			idx = i + self.elements["scrollbar_pos"]
			self.elements["slot_presets%d"%i].SetEvent(self.FuncSelectPresets,i,idx)
			self.elements["button_rename_%d"%i].SetEvent(self.SavePresets,idx)
			self.ClickRadioButton(i,idx)

			dict = self.elements["save_presets"].get(idx, None)

			if not dict:
				self.elements["slot_presets_name%d"%i].SetText("+  {}. Presets".format(idx))
			else:
				self.elements["slot_presets_name%d"%i].SetText("+  {}".format(dict["name"]))


	def UpdateTitle(self,index_preset):
		if index_preset == self.elements["select_presets"]:
			dict = self.elements["save_presets"].get(index_preset, None)
			if dict:
				self.title_bar_s.SetText("{}".format(dict["name"]).upper())
			else:
				self.title_bar_s.SetText("{}. Presets".format(index_preset).upper())

	def OnScroll(self):
		if self.elements["max_presets_count"] < self.elements["max_presets_view"]+1:
			return

		scrollLineCount = float(self.elements["max_presets_count"]-self.elements["max_presets_view"])

		startIndex = int(scrollLineCount * self.elements["scrollbar"].GetPos()+1)

		if startIndex > (self.elements["max_presets_count"] - self.elements["max_presets_view"])+1:
			startIndex -= 1

		self.elements["scrollbar_pos"] = startIndex

		self.SetViewPresets()

	def FuncSelectPresets(self,index_a,index_b):
		for i in xrange(self.elements["max_presets_view"]):
			self.elements["slot_presets%d"%i].SetUp()

		self.elements["select_presets"] = index_b

		self.ClickRadioButton(index_a,index_b)

		dict = self.elements["save_presets"].get(index_b, None)
		if dict:
			self.title_bar_s.SetText("{}".format(dict["name"]).upper())
		else:
			self.title_bar_s.SetText("{}. Presets".format(index_b).upper())

		self.RefreshEquipSlotWindow()

	def ClickRadioButton(self,index,index1):
		if self.elements["select_presets"] == index1:
			self.elements["slot_presets%d"%index].Down()
		else:
			self.elements["slot_presets%d"%index].SetUp()

	def GetChangeEquipIndex(self):
		return self.elements["select_presets"]


	def SelectEmptySlot(self, selectedSlotPos):
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
			return

		isAttached = mouseModule.mouseController.isAttached()
		index_change_equip = self.GetChangeEquipIndex()

		selectedSlotPos = player.CHANGE_EQUIP_SLOT_COUNT/player.CHANGE_EQUIP_PAGE_EXTRA*(index_change_equip-1) + (selectedSlotPos-player.CHANGE_EQUIP_SLOT_START)

		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			if player.SLOT_TYPE_INVENTORY == attachedSlotType:
				itemIndex = player.GetItemIndex(attachedSlotPos)
				itemCount = player.GetItemCount(attachedSlotPos)

				item.SelectItem(itemIndex)
				itemType = item.GetItemType()

				
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				net.SendItemMovePacket(player.INVENTORY, attachedSlotPos, player.CHANGE_EQUIP, selectedSlotPos, attachedCount)
			
	def SelectItemSlot(self, selectedSlotPos):

		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
			return

		isAttached = mouseModule.mouseController.isAttached()
		index_change_equip = self.GetChangeEquipIndex()
		selectedSlotPos = player.CHANGE_EQUIP_SLOT_COUNT/player.CHANGE_EQUIP_PAGE_EXTRA*(index_change_equip-1) + (selectedSlotPos-player.CHANGE_EQUIP_SLOT_START)
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()
			if player.SLOT_TYPE_INVENTORY == attachedSlotType:
				itemIndex = player.GetItemIndex(attachedSlotPos)
				itemCount = player.GetItemCount(attachedSlotPos)

				item.SelectItem(itemIndex)
				itemType = item.GetItemType()

				
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				net.SendItemMovePacket(player.INVENTORY, attachedSlotPos, player.CHANGE_EQUIP, selectedSlotPos, attachedCount)
		else:
			itemVnum = player.GetItemIndex(player.CHANGE_EQUIP, selectedSlotPos)
			itemCount = player.GetItemCount(player.CHANGE_EQUIP, selectedSlotPos)
			mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_CHANGE_EQUIP, selectedSlotPos, itemVnum, itemCount)

	def UseItemSlot(self, slotIndex):
		curCursorNum = app.GetCursor()
		if app.SELL == curCursorNum:
			return

		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return

		index_change_equip = self.GetChangeEquipIndex()
		slotIndex = player.CHANGE_EQUIP_SLOT_COUNT/player.CHANGE_EQUIP_PAGE_EXTRA*(index_change_equip-1) + (slotIndex-player.CHANGE_EQUIP_SLOT_START)

		net.SendItemUsePacket(player.CHANGE_EQUIP, slotIndex)
		mouseModule.mouseController.DeattachObject()
		self.OverOutItem()


	def RefreshEquipSlotWindow(self):
		index_change_equip = self.GetChangeEquipIndex()
		count = 0

		if index_change_equip != 0:
			if index_change_equip > 1:
				index_old = player.CHANGE_EQUIP_SLOT_COUNT/player.CHANGE_EQUIP_PAGE_EXTRA*(index_change_equip-1)
			else:
				index_old = player.CHANGE_EQUIP_SLOT_COUNT-(player.CHANGE_EQUIP_SLOT_COUNT/index_change_equip)

			for i in xrange(index_old ,player.CHANGE_EQUIP_SLOT_COUNT/player.CHANGE_EQUIP_PAGE_EXTRA*index_change_equip):
				slotNumber = player.CHANGE_EQUIP_SLOT_START + count

				itemCount = player.GetItemCount(player.CHANGE_EQUIP,i)
				if itemCount <= 1:
					itemCount = 0

				self.equipament.SetItemSlot(slotNumber, player.GetItemIndex(player.CHANGE_EQUIP,i), itemCount)
				count += 1
		
		self.equipament.RefreshSlot()


	def OverInItem(self,index):
		index_change_equip = self.GetChangeEquipIndex()
		index = player.CHANGE_EQUIP_SLOT_COUNT/player.CHANGE_EQUIP_PAGE_EXTRA*(index_change_equip-1) + (index-player.CHANGE_EQUIP_SLOT_START)

		if None != self.toolTip:
			self.toolTip.ClearToolTip()
			self.toolTip.SetInventoryItem(index,player.CHANGE_EQUIP)

	def OverOutItem(self):
		self.toolTip.HideToolTip()

	def ClickChangeEquip(self):
		index_change_equip = self.GetChangeEquipIndex()
		if index_change_equip != 0:
			net.SendChatPacket("/change_equip_wa %d"%(index_change_equip))
			
	def SavePresets(self,index):
		name_presets = uiCommon.InputDialog()
		name_presets.SetTitle("Name Presets")
		name_presets.SetAcceptEvent(ui.__mem_func__(self.OnAddPresets))
		name_presets.SetCancelEvent(ui.__mem_func__(self.OnCancelAddPresets))
		name_presets.SetMaxLength(20)
		name_presets.Open()
		self.name_presets = name_presets
		self.index_presets = index

	def OnAddPresets(self):
		text = self.name_presets.GetText()
		if text:
			new_dict = {
				"name" 	: text,
			}

			self.elements["save_presets"][self.index_presets] = new_dict
			self.SaveDictPresets()
			self.UpdateTitle(self.index_presets)

		self.OnCancelAddPresets()

	def SaveDictPresets(self):
		with open('fast_equip.wa', 'wb') as f:
			pickle.dump(self.elements["save_presets"], f)

		self.SetViewPresets()
	def OnCancelAddPresets(self):
		self.name_presets.Close()
		self.name_presets = None
		return True

	def Close(self):
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

class ScrollBarNew(ui.Window):
	MIDDLE_BAR_POS = 4
	SCROLLBAR_BUTTON_HEIGHT = 0
	SCROLLBAR_WIDTH = 4
	SCROLLBAR_MIDDLE_HEIGHT = 33

	class MiddleBar(ui.DragButton):
		def __init__(self):
			ui.DragButton.__init__(self)
			self.AddFlag("movable")

		def MakeImage(self):
			bar = ui.ImageBox()
			bar.SetParent(self)
			bar.LoadImage(PATCH_DESIGN+"middle_bar.tga")
			bar.SetPosition(0, 0)
			bar.AddFlag("not_pick")
			bar.Show()
			self.bar = bar

		def SetSize(self, height):
			height = max(12, height)
			ui.DragButton.SetSize(self, 10, height)
			height -= 4*3

	def __init__(self):
		ui.Window.__init__(self)

		self.pageSize = 1
		self.curPos = 0.0
		self.eventScroll = lambda *arg: None
		self.lockFlag = False
		self.scrollStep = 0.20


		self.CreateScrollBar()

	def __del__(self):
		ui.Window.__del__(self)

	def CreateScrollBar(self):
		barSlot = ui.ImageBox()
		barSlot.SetParent(self)
		barSlot.AddFlag("not_pick")
		barSlot.LoadImage(PATCH_DESIGN+"bar_slot_new.tga")
		barSlot.Show()

		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(ui.__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.MakeImage()
		middleBar.SetSize(33)

		self.middleBar = middleBar
		self.barSlot = barSlot

		self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()


	def Destroy(self):
		self.middleBar = None
		self.eventScroll = lambda *arg: None

	def SetScrollEvent(self, event):
		self.eventScroll = event

	def GetPos(self):
		return self.curPos

	def SetPos(self, pos):
		pos = max(0.0, pos)
		pos = min(1.0, pos)

		newPos = float(self.pageSize) * pos
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, int(newPos))
		self.OnMove()


	def SetScrollBarSize(self, height):
		self.pageSize = height- self.SCROLLBAR_MIDDLE_HEIGHT
		self.SetSize(self.SCROLLBAR_WIDTH, height)
		self.middleBar.SetRestrictMovementArea(self.MIDDLE_BAR_POS, 0, 8, 187)
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, 0)

		self.UpdateBarSlot()

	def UpdateBarSlot(self):
		self.barSlot.SetPosition(0, self.SCROLLBAR_BUTTON_HEIGHT)

	def SetScrollStep(self, step):
		self.scrollStep = step

	def GetScrollStep(self):
		return self.scrollStep

	def OnUp(self):
		self.SetPos(self.curPos-self.scrollStep)

	def OnDown(self):
		self.SetPos(self.curPos+self.scrollStep)

	def OnMove(self):

		if self.lockFlag:
			return

		if 0 == self.pageSize:
			return

		(xLocal, yLocal) = self.middleBar.GetLocalPosition()
		self.curPos = float(yLocal) / float(self.pageSize)

		self.eventScroll()

	def OnMouseLeftButtonDown(self):
		(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
		pickedPos = yMouseLocalPosition  - self.SCROLLBAR_MIDDLE_HEIGHT/2
		newPos = float(pickedPos) / float(self.pageSize)
		self.SetPos(newPos)

	def LockScroll(self):
		self.lockFlag = True

	def UnlockScroll(self):
		self.lockFlag = False
