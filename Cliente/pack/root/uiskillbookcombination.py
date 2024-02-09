import ui
import mouseModule
import player
import item
import net
import uiToolTip
import chat
import localeInfo
import app
import types
import bonus67

SKILLBOOK_SLOT_MAX = player.SKILLBOOK_COMB_SLOT_MAX #10
INVENTORY_PAGE_SIZE = player.INVENTORY_PAGE_SIZE # 45
USE_LIMIT_RANGE = 1000

SKILLBOOK_COMBI_UI_CLOSE = 0
SKILLBOOK_COMBI_UI_OPEN = 1
SKILLBOOK_COMBI_START = 2

class SkillBookCombinationWindow(ui.ScriptWindow):
	
	def __init__(self) :
		ui.ScriptWindow.__init__(self)
		self.SkillBookList = [None] * SKILLBOOK_SLOT_MAX
		self.itemToolTip = uiToolTip.ItemToolTip()
		self.OpenCheck = False
		(self.StartPosX, self.StartPosY, z) = player.GetMainCharacterPosition()
	def __del__(self) :
		ui.ScriptWindow.__del__(self)
		del self.itemToolTip

	def Close(self) :
		bonus67.SendSkillBookCombinationPacket(self.SkillBookList, SKILLBOOK_COMBI_UI_CLOSE)
		self.OpenCheck = False
		for i in xrange(SKILLBOOK_SLOT_MAX):
			self.ClearSlot(i)
		self.Hide()

	def OpenNew(self):
		(self.StartPosX, self.StartPosY, z) = player.GetMainCharacterPosition()
		bonus67.SendSkillBookCombinationPacket(self.SkillBookList, SKILLBOOK_COMBI_UI_OPEN)
		self.OpenCheck = True
	def Open(self) :
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/SkillBookCombinationDialog.py")
		except:
			import exception
			exception.Abort("SkillBookCombinationWindow.Open.SkillBookCombinationDialog.py")
	
		try:
			## Title Bar Close Event
			self.GetChild("SkillBook_TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))

			## Button Event
			self.AcceptButton = self.GetChild("accept_button")
			self.CancelButton = self.GetChild("cancel_button")
			self.AcceptButton.SetEvent(ui.__mem_func__(self.ClickAcceptButton))
			self.CancelButton.SetEvent(ui.__mem_func__(self.Close))

			## SkillBook Slot
			Slot = self.GetChild("SkillBookSlot")
			Slot.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot)) ## [Event] 빈 슬롯 Click 할 때
			Slot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot)) ## [Event] 슬롯 안에 있는 Item Click 할 때
			Slot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			Slot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			self.Slot = Slot

			

			self.SetTop()
		except:
			import exception
			exception.Abort("SkillBookCombinationWindow.Open.Child")		

	def OverInItem(self, slotIndex) :
		if not self.itemToolTip :
			return

		invenPos = self.SkillBookList[slotIndex]

		if -1 < invenPos < INVENTORY_PAGE_SIZE * 4: 
			self.itemToolTip.SetInventoryItem(invenPos, player.INVENTORY)

	def OverOutItem(self) :
		if not self.itemToolTip :
			return

		self.itemToolTip.HideToolTip()


	def SelectEmptySlot(self, slotIndex) :
		## 마우스에 아이템이 붙어 있어야 함
		if not mouseModule.mouseController.isAttached() :
			return

		attachedSlotType	= mouseModule.mouseController.GetAttachedType()
		attachedSlotPos		= mouseModule.mouseController.GetAttachedSlotNumber()
		attachedItemVnum	= player.GetItemIndex(attachedSlotPos)
		item.SelectItem(attachedItemVnum)		
		
		itemType	= item.GetItemType()

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if player.SLOT_TYPE_INVENTORY != attachedSlotType and\
				player.SLOT_TYPE_SKILL_BOOK_INVENTORY != attachedSlotType and\
				player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY != attachedSlotType and\
				player.SLOT_TYPE_STONE_INVENTORY != attachedSlotType and\
				player.SLOT_TYPE_GIFT_BOX_INVENTORY != attachedSlotType:
				return

		if itemType != item.ITEM_TYPE_SKILLBOOK :
			return

		if attachedSlotPos not in self.SkillBookList :
			mouseModule.mouseController.DeattachObject()
			
			self.SkillBookList[slotIndex] = attachedSlotPos
			self.Slot.SetItemSlot(slotIndex, attachedItemVnum)
			self.Slot.RefreshSlot()

	def SelectItemSlot(self, slotIndex) :
		if mouseModule.mouseController.isAttached() :
			return

		## 범위 안
		if -1 < slotIndex < SKILLBOOK_SLOT_MAX :
			self.ClearSlot(slotIndex)
			self.Slot.RefreshSlot()
		else :
			pass

	def ClearSlot(self, idx) :
		self.SkillBookList[idx] = None
		self.Slot.ClearSlot(idx)

	def ClickAcceptButton(self) :
		cnt = 0
		for i in self.SkillBookList :
			if not isinstance(i, types.NoneType) :
				cnt += 1

		if cnt != SKILLBOOK_SLOT_MAX :
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_NOT_FULL_BOOK)
			return
		bonus67.SendSkillBookCombinationPacket(self.SkillBookList, SKILLBOOK_COMBI_START)

		for i in xrange(SKILLBOOK_SLOT_MAX):
			self.ClearSlot(i)

		self.Slot.RefreshSlot()


	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnUpdate(self) :
		if self.OpenCheck:
			(x, y, z) = player.GetMainCharacterPosition()
			if abs(x - self.StartPosX) > USE_LIMIT_RANGE or abs(y - self.StartPosY) > USE_LIMIT_RANGE:
				self.Close()