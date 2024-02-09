import app
import net
import player
import item

import ui
import uiToolTip

import localeInfo
import uiScriptLocale

import chrmgr
import wndMgr

class RefineElementDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.Initialize()

	def Initialize(self):
		self.titleBar = None
		self.titleText = None
		self.board = None
		self.itemToolTip = None
		self.costText = None
		self.itemSlot = None
		self.acceptButton = None
		self.cancelButton = None

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/RefineElementDialog.py")
		except:
			import exception
			exception.Abort("RefineElementDialog.LoadWindow.LoadObject")

		try:
			self.titleBar = self.GetChild("TitleBar")
			self.titleText = self.GetChild("TitleName")
			self.board = self.GetChild("Board")
			self.costText = self.GetChild("Cost")
			self.itemSlot = self.GetChild("ItemSlot")
			self.acceptButton = self.GetChild("AcceptButton")
			self.cancelButton = self.GetChild("CancelButton")	
		except:
			import exception
			exception.Abort("RefineElementDialog.LoadWindow.BindObject")
			
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.CancelRefine))
		self.itemSlot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		
		self.acceptButton.SetEvent(ui.__mem_func__(self.AcceptRefine))
		self.cancelButton.SetEvent(ui.__mem_func__(self.CancelRefine))

		itemToolTip = uiToolTip.ItemToolTip()
		itemToolTip.SetParent(self)
		itemToolTip.SetFollow(False)
		itemToolTip.SetPosition(15, 38)
		itemToolTip.Show()
		self.itemToolTip = itemToolTip

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.Initialize()
		
	def OpenRefine(self, srcCell, dstCell, refineType):
		if refineType == item.REFINE_ELEMENT_TYPE_UPGRADE:
			self.titleText.SetText(uiScriptLocale.REFINE_ELEMENT_UPGRADE_TITLE)
		elif refineType == item.REFINE_ELEMENT_TYPE_DOWNGRADE:
			self.titleText.SetText(uiScriptLocale.REFINE_ELEMENT_DOWNGRADE_TITLE)
		
		srcVnum = player.GetItemIndex(srcCell)
		dstVnum = player.GetItemIndex(dstCell)
		refineCost = item.REFINE_ELEMENT_UPGRADE_YANG if refineType == item.REFINE_ELEMENT_TYPE_UPGRADE else item.REFINE_ELEMENT_DOWNGRADE_YANG
		self.costText.SetText(localeInfo.NumberToMoneyString1(refineCost))
		if player.GetMoney() < refineCost:
			self.costText.SetPackedFontColor(0xffff1c49)
		else:
			self.costText.SetPackedFontColor(0xffdddddd)
			
		self.itemToolTip.ClearToolTip()
		
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(dstCell, i))

		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(dstCell, i))

		item.SelectItem(srcVnum)
		self.itemToolTip.isRefineElement = refineType
		self.itemToolTip.bRefineElementType = item.GetValue(0)
		self.itemToolTip.AddRefineItemData(dstVnum, metinSlot, attrSlot, player.GetItemRefineElement(dstCell))
		
		item.SelectItem(dstVnum)
		xSlotCount, ySlotCount = item.GetItemSize()
		for i in xrange(self.itemSlot.GetSlotCount()):
			if i < ySlotCount:
				self.itemSlot.ShowSlotBaseImage(i)
			else:
				self.itemSlot.HideSlotBaseImage(i)

		self.itemSlot.SetItemSlot(0, dstVnum)
		self.itemSlot.SetPosition(12, 38 + ((self.itemToolTip.GetHeight() - (32 * ySlotCount)) / 2))
		
		newWidth = self.itemToolTip.GetWidth() + 60
		newHeight = self.itemToolTip.GetHeight() + 100

		self.itemToolTip.SetPosition(50, 38)
		self.titleBar.SetWidth(newWidth - 15)
		self.SetSize(newWidth, newHeight)
		self.board.SetSize(newWidth, newHeight)
		
		# Workaround to solve children position
		(x, y) = self.GetLocalPosition()
		self.SetPosition(x, y)	
	
		self.SetTop()
		self.SetFocus()
		self.Show()
		
	def OnIMEReturn(self):
		self.AcceptRefine()
		return True
		
	def AcceptRefine(self):
		net.SendRefineElementPacket(0)
		self.Close()

	def CancelRefine(self):
		net.SendRefineElementPacket(255)
		self.Close()

	def Close(self):		
		self.Hide()

	def OnPressEscapeKey(self):
		self.CancelRefine()
		return True

class RefineElementChangeDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.Initialize()

	def Initialize(self):
		self.board = None
		self.acceptButton = None
		self.cancelButton = None
		self.costText = None
		
		self.elementButtonDict = {}
		self.changeElementType = 255
		self.currentElementType = -1

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/RefineElementChange.py")
		except:
			import exception
			exception.Abort("RefineElementDialog.LoadWindow.LoadObject")

		try:
			self.board = self.GetChild("board")
			self.acceptButton = self.GetChild("AcceptButton")
			self.cancelButton = self.GetChild("CancelButton")
			self.costText = self.GetChild("Cost")
			
			self.elementButtonDict[chrmgr.REFINE_ELEMENT_CATEGORY_ELECT] = self.GetChild("ElectButton")
			self.elementButtonDict[chrmgr.REFINE_ELEMENT_CATEGORY_FIRE] = self.GetChild("FireButton")
			self.elementButtonDict[chrmgr.REFINE_ELEMENT_CATEGORY_ICE] = self.GetChild("IceButton")
			self.elementButtonDict[chrmgr.REFINE_ELEMENT_CATEGORY_WIND] = self.GetChild("WindButton")
			self.elementButtonDict[chrmgr.REFINE_ELEMENT_CATEGORY_EARTH] = self.GetChild("EarthButton")
			self.elementButtonDict[chrmgr.REFINE_ELEMENT_CATEGORY_DARK] = self.GetChild("DarkButton")
		except:
			import exception
			exception.Abort("RefineElementDialog.LoadWindow.BindObject")
			
		self.board.SetCloseEvent(ui.__mem_func__(self.CancelRefine))

		self.acceptButton.SetEvent(ui.__mem_func__(self.AcceptRefine))
		self.cancelButton.SetEvent(ui.__mem_func__(self.CancelRefine))
		
		for key, value in self.elementButtonDict.iteritems():
			value.SetEvent(ui.__mem_func__(self.SelectElementType), key)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.Initialize()
		
	def Open(self, srcCell, dstCell):
		for key, value in self.elementButtonDict.iteritems():
			value.Enable()
			value.SetUp()
			
		self.costText.SetText(localeInfo.NumberToMoneyString(item.REFINE_ELEMENT_CHANGE_YANG))
		
		currentElementRefine = player.GetItemRefineElement(dstCell)
		if currentElementRefine:
			elementType = int(currentElementRefine / 100000000) - 1
			self.currentElementType = elementType
			if self.elementButtonDict.has_key(elementType):
				self.elementButtonDict[elementType].Disable()
		
		self.SetTop()
		self.SetFocus()
		self.Show()
		
	def SelectElementType(self, elementType):
		self.changeElementType = elementType + 1
		for key, value in self.elementButtonDict.iteritems():
			if key == elementType:
				value.Down()
			else:
				if key != self.currentElementType:
					value.SetUp()
	
	def AcceptRefine(self):
		net.SendRefineElementPacket(self.changeElementType)
		self.Close()

	def CancelRefine(self):
		net.SendRefineElementPacket(255)
		self.Close()

	def Close(self):		
		self.Hide()

	def OnPressEscapeKey(self):
		self.CancelRefine()
		return True