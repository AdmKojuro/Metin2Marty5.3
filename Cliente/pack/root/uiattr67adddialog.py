import app
import bonus67
import uiToolTip
import ui
import mouseModule
import player
import item
import localeInfo
import chat
import uiCommon

MAX_FRAGMENTS 		= 10
MAX_SUPPORT			= 5
PERCENT_FRAGMENT 	= 2
ATTR_COMBI_UI_CLOSE = 0
ATTR_COMBI_UI_OPEN  = 1

class Attr67AddWindow(ui.ScriptWindow):
	def __init__(self) :
		ui.ScriptWindow.__init__(self)
		self.itemToolTip = uiToolTip.ItemToolTip()
		self.vnum_fragment = 0
		self.vnum_support = 0
		self.slot_add = -1
		self.slot_support = -1
		self.count_fragment = 0
		self.count_support = 0
		self.questiondialog = None
		self.toolTip = uiToolTip.ItemToolTip()

	def __del__(self) :
		ui.ScriptWindow.__del__(self)
		del self.itemToolTip

	def OpenNew(self):
		bonus67.SendAddAttrCombPacket(ATTR_COMBI_UI_OPEN)

	def Open(self) :
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/attr67adddialog.py")
		except:
			import exception
			exception.Abort("Attr67AddWindow.Open.attr67adddialog.py")

		try:
			self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.Close))
			self.regist_slot = self.GetChild("regist_slot")
			self.material_slot = self.GetChild("material_slot")
			self.material_slot_count_text = self.GetChild("material_slot_count_text")
			self.material_slot_arrow_up_button = self.GetChild("material_slot_arrow_up_button")
			self.material_slot_arrow_down_button = self.GetChild("material_slot_arrow_down_button")
			self.support_slot = self.GetChild("support_slot")
			self.support_slot_count_text = self.GetChild("support_slot_count_text")
			self.support_slot_arrow_up_button = self.GetChild("support_slot_arrow_up_button")
			self.support_slot_arrow_down_button = self.GetChild("support_slot_arrow_down_button")
			self.TotalSuccessText = self.GetChild("TotalSuccessText")
			self.attr_add_button = self.GetChild("attr_add_button")

		except:
			import exception
			exception.Abort("Attr67AddWindow.Open.Child")

		self.TotalPorcent()

		self.support_slot_arrow_up_button.SetEvent(lambda arg='ARROW_UP': self.SupportSlotArrow(arg))
		self.support_slot_arrow_down_button.SetEvent(lambda arg='ARROW_DOWN': self.SupportSlotArrow(arg))

		self.material_slot_arrow_up_button.SetEvent(lambda arg='ARROW_UP': self.MaterialSlotArrow(arg))
		self.material_slot_arrow_down_button.SetEvent(lambda arg='ARROW_DOWN': self.MaterialSlotArrow(arg))

		self.support_slot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySupportSlot))
		self.support_slot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSupportSlot))

		self.support_slot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.support_slot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		self.regist_slot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
		self.regist_slot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))

		self.regist_slot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.regist_slot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		self.material_slot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.material_slot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		self.attr_add_button.SetEvent(ui.__mem_func__(self.ClickAddButton))

	def ClearFunc(self):
		self.regist_slot.SetItemSlot(0,0,0)
		self.material_slot.SetItemSlot(1,0,0)
		self.vnum_fragment 	= 0
		self.slot_add 		= -1
		self.count_fragment = 0
		self.ClearSupportFunc()
		self.TotalPorcent()
		

	def ClearSupportFunc(self):
		self.support_slot.SetItemSlot(2,0,0)
		self.slot_support = -1
		self.vnum_support = 0
		self.count_support = 0
		self.TotalPorcent()

	def AddMaterialSlot(self,vnum):
		self.material_slot.SetItemSlot(1,vnum,0)
		self.vnum_fragment = vnum

	def SupportSlotArrow(self,func):

		if self.GetSlotPos() <= -1:
			return

		if self.vnum_fragment == 0:
			return

		if self.GetSupportSlotPos() <= -1:
			return

		if player.GetItemCount(self.GetSupportSlotPos()) <= 0:
			return

		if func == 'ARROW_UP' and self.count_support < MAX_SUPPORT and player.GetItemCount(self.GetSupportSlotPos()) >= self.count_support+1:
			self.count_support += 1

		elif func == 'ARROW_DOWN' and self.count_support > 1:
			self.count_support -= 1

		self.TotalPorcent()

	def MaterialSlotArrow(self,func):

		if self.GetSlotPos() <= -1:
			return

		if self.vnum_fragment == 0:
			return

		if player.GetItemCountByVnum(self.vnum_fragment) <= 0:
			return

		if func == 'ARROW_UP' and self.count_fragment < MAX_FRAGMENTS and player.GetItemCountByVnum(self.vnum_fragment) >= self.count_fragment+1:
			self.count_fragment += 1

		elif func == 'ARROW_DOWN' and self.count_fragment > 0:
			self.count_fragment -= 1

		self.TotalPorcent()

	def GetCountSupportPercent(self):
		if self.count_support == 0 or self.GetSupportSlotPos() <= -1:
			return 0
		item.SelectItem(self.vnum_support)
		return self.count_support*(item.GetValue(1)/MAX_SUPPORT)

	def TotalPorcent(self):
		self.support_slot_count_text.SetText(str(self.count_support))
		self.material_slot_count_text.SetText(str(self.count_fragment))
		self.TotalSuccessText.SetText(localeInfo.ATTR_6TH_7TH_TOTAL_SUCCESS_PERCENT%((self.count_fragment*PERCENT_FRAGMENT)+self.GetCountSupportPercent()))
		

	def __OnSelectEmptySupportSlot(self,selectedSlotPos):
		if self.GetSlotPos() <= -1:
			return

		isAttached = mouseModule.mouseController.isAttached()

		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			itemVNum = player.GetItemIndex(attachedSlotPos)

			if itemVNum >= 72064 and itemVNum <= 72067:
				self.slot_support = attachedSlotPos
				self.vnum_support = itemVNum
				self.count_support = 1
				self.support_slot.SetItemSlot(selectedSlotPos,itemVNum,0)
				self.TotalPorcent()

	def __OnSelectItemSupportSlot(self,selectedSlotPos):
		if self.GetSlotPos() <= -1:
			return

		isAttached = mouseModule.mouseController.isAttached()

		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			itemVNum = player.GetItemIndex(attachedSlotPos)	

			item.SelectItem(itemVNum)

			if itemVNum >= 72064 and itemVNum <= 72067:
				self.slot_support = attachedSlotPos
				self.vnum_support = itemVNum
				self.count_support = 1
				self.support_slot.SetItemSlot(selectedSlotPos,itemVNum,0)
				self.TotalPorcent()
		else:
			self.ClearSupportFunc()	

	def __OnSelectEmptySlot(self,selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()

		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			itemVNum = player.GetItemIndex(attachedSlotPos)	
			itemCount = player.GetItemCount(attachedSlotPos)

			item.SelectItem(itemVNum)

			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()

			if player.IsEquipmentSlot(attachedSlotPos):
				return

			if (itemType == item.ITEM_TYPE_ARMOR or (itemType == item.ITEM_TYPE_WEAPON and itemSubType != item.WEAPON_ARROW))  and (self.GetCountAttribute(attachedSlotPos) >= 5 and self.GetCountAttribute(attachedSlotPos) < 7):
				self.ClearFunc()
				self.slot_add = attachedSlotPos
				self.regist_slot.SetItemSlot(selectedSlotPos,itemVNum,itemCount)
				bonus67.SendRegistFragmentPacket(attachedSlotPos)

	def __OnSelectItemSlot(self,selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()

		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			itemVNum = player.GetItemIndex(attachedSlotPos)	
			itemCount = player.GetItemCount(attachedSlotPos)

			item.SelectItem(itemVNum)

			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()

			if player.IsEquipmentSlot(attachedSlotPos):
				return

			if (itemType == item.ITEM_TYPE_ARMOR or (itemType == item.ITEM_TYPE_WEAPON and itemSubType != item.WEAPON_ARROW)) and (self.GetCountAttribute(attachedSlotPos) >= 5 and self.GetCountAttribute(attachedSlotPos) < 7):
				self.ClearFunc()
				self.slot_add = attachedSlotPos
				self.regist_slot.SetItemSlot(selectedSlotPos,itemVNum,itemCount)
				bonus67.SendRegistFragmentPacket(attachedSlotPos)
		else:
			if self.vnum_fragment != 0:
				self.ClearFunc()

	def GetSupportSlotPos(self):
		return self.slot_support

	def GetCountSupport(self):
		return self.count_support

	def GetSlotPos(self):
		return self.slot_add

	def GetCountFragment(self):
		return self.count_fragment

	def GetRestriccion(self,pos,count_fragment):
		itemVNum = player.GetItemIndex(pos)	
		item.SelectItem(itemVNum)

		itemType = item.GetItemType()
		itemSubType = item.GetItemSubType()

		if pos < 0:
			return False

		if count_fragment != 0:
			if (count_fragment > MAX_FRAGMENTS or player.GetItemCountByVnum(self.vnum_fragment) <= 0 or player.GetItemCountByVnum(self.vnum_fragment) < count_fragment):
				return False

		if player.IsEquipmentSlot(pos):
			return False

		if (itemType == item.ITEM_TYPE_ARMOR or (itemType == item.ITEM_TYPE_WEAPON and itemSubType != item.WEAPON_ARROW)):
			return True

		return False

	def GetRestriccionSupport(self,pos_support,count_support):
		itemVNum = player.GetItemIndex(pos_support)	

		if pos_support != -1:
			if (pos_support < 0 and count_support > 0):
				return False

			if pos_support >= 0 and player.GetItemCount(pos_support) <= 0:
				return False

			if pos_support >= 0 and count_support > MAX_SUPPORT:
				return False

			if player.GetItemCount(pos_support) < count_support:
				return False

			if itemVNum >= 72064 and itemVNum <= 72067:
				return True

			return False

		else:
			return True

	def GetCountAttribute(self,pos):
		count = 0

		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			(type, value) = player.GetItemAttribute(pos, i)

			if type != 0:
				count += 1
		
		return count

	def ClickAddButton(self):
		questiondialog = uiCommon.QuestionDialog2()
		questiondialog.SetText1(localeInfo.ATTR_6TH_7TH_ADD_QUESTION1)
		questiondialog.SetText2(localeInfo.ATTR_6TH_7TH_ADD_QUESTION2)
		questiondialog.SetAcceptEvent(ui.__mem_func__(self.ClickAddAcept))
		questiondialog.SetCancelEvent(ui.__mem_func__(self.ClickAddCancel))
		questiondialog.Open()

		self.questiondialog = questiondialog

	def ClickAddAcept(self):
		pos = self.GetSlotPos()
		count_fragment = self.GetCountFragment()
		pos_support = self.GetSupportSlotPos()
		count_support = self.GetCountSupport()

		self.ClickAddCancel()
		
		if self.GetRestriccion(pos,count_fragment) == False:
			return

		if self.GetRestriccionSupport(pos_support,count_support) == False:
			return

		if count_fragment <= 0 and count_support <= 0:
			return

		if self.GetCountAttribute(pos) < 5 or self.GetCountAttribute(pos) > 6:
			return

		bonus67.SendAddAttrPacket(pos,count_fragment,pos_support,count_support)
		self.Close()

	def ClickAddCancel(self):
		if self.questiondialog:
			self.questiondialog.Close()
		self.questiondialog = None


	def GetIndexOver(self,index):
		if index == 0:
			return player.GetItemIndex(self.GetSlotPos())
		elif index == 1:
			return self.vnum_fragment
		else:
			return self.vnum_support

	def OverInItem(self,index):
		self.toolTip.ClearToolTip()


		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if index == 0:
				metinSlot.append(player.GetItemMetinSocket(self.GetSlotPos(),i))
			else:
				metinSlot.append(0)

		AttrList = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			if index == 0:
				AttrList.append(player.GetItemAttribute(self.GetSlotPos(),i))
			else:
				AttrList.append((0,0))


		itemVNum = 	self.GetIndexOver(index)

		self.toolTip.AddItemData(itemVNum,metinSlot,AttrList)

	def OverOutItem(self):
		self.toolTip.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

	def Close(self):
		self.ClickAddCancel()
		self.ClearFunc()
		bonus67.SendAddAttrCombPacket(ATTR_COMBI_UI_CLOSE)
		self.Hide()

