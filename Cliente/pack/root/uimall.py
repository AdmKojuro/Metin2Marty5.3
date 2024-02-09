import app
import chrmgr
import ui
import wndMgr
import renderTarget
import exception
import time
import constInfo
import chat
import uiToolTip
import item
import event
import locale
import os
import player
import uiWeb
import chr
import math

'''
|---------------------------------------------------------------------------------------|
|    __________                  _____                                                  |
|    \______   \_______   ____ _/ ____\ ____    ______  ______ ____ _______             |
|     |     ___/\_  __ \ /  _ \\   __\_/ __ \  /  ___/ /  ___//  _ \\_  __ \            |
|     |    |     |  | \/(  <_> )|  |  \  ___/  \___ \  \___ \(  <_> )|  | \/            |
|     |____|     |__|    \____/ |__|   \___  >/____  >/____  >\____/ |__|               |
|                                          \/      \/      \/                           |
|                       ___________         __                                          |
|                       \_   _____/  ____ _/  |_   ____                                 |
|                        |    __)_  /    \\   __\_/ __ \                                |
|                        |        \|   |  \|  |  \  ___/                                |
|                       /_______  /|___|  /|__|   \___  >                               |
|                               \/      \/            \/                                |
|---------------------------------------------------------------------------------------|

Version 1.2.0
                                
'''


IMG_PATH = "locale/es/ui/mall/"
IMG_PATH_MALL_DESIGN_BY_LORDZIEGE = "locale/es/ui/mall/design_by_lordziege/"
UISCRIPT_PATH = "uiscript/mall/"
UISCRIPT_PATH_MALL_DESIGN_BY_LORDZIEGE = "uiscript/mall_design_by_lordziege/"

CHOOSE_MALL_TITLE = "Elige Tu Tienda"

STARTPAGE_TITLE = "Página de inicio"
STARTPAGE_COLUMN_1_TITLE = "Nuevos Items"
STARTPAGE_COLUMN_2_TITLE = "Ofertas"
STARTPAGE_COLUMN_3_TITLE = "Recomendado"

QUESTION_WANT_TO_BUY = "Le gustaría comprar {amount}x {itemName} por {price} {currencyTitle} Comprar?"

ITEMBOX_BUTTON_BUY_TEXT = "Comprar"
ITEMBOX_BUTTON_PREVIEW_TEXT = "Vista Previa"
ITEMBOX_ITEM_AMOUNT_TEXT = "STK."

PLAYER_INFO_NAME_TITLE = "Nombre:"
PLAYER_INFO_LEVEL_TITLE = "Level:"
PLAYER_INFO_CURRENCY_TITLE = "Coins:"

BUTTON_BUY_CURRENCY_TEXT = "Comprar Coins!"
BUTTON_RESET_ORIGINAL = "Original"

ENABLE_WOLFMAN_CHARACTER = False ## True|False
ENABLE_ACCE_SYSTEM = True ## True|False
ENABLE_COSTUME_AURA = False 
ENABLE_COSTUME_SHINING = False 

RENDER_TARGET_INDEX = 2
DISABLE_TOOLTIP_RENDER_TARGET = False ## disable render target tooltip in itemshop. Ask me if u need that extension

ENABLE_MALL_DESIGN_BY_LORD_ZIEGE = True
ITEMBOX_DISCOUNT_TEXT_BY_LORD_ZIEGE = "%d%% Descuento"

class MallWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = False
		if False == self.isLoaded:
			self._LoadScript()
	
	## INIT GUI FUNCTIONS START

	def _LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH_MALL_DESIGN_BY_LORDZIEGE+'mall.py')
			else:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH+'mall.py')
		except:
			exception.Abort('Mall.MallWindow.LoadScriptFile')

		try:
			self._LoadVariables()
		except:
			exception.Abort('Mall.MallWindow.LoadVariables')

		try:
			self._LoadUi()
		except:
			exception.Abort('Mall.MallWindow.LoadUi')

		try:
			self._LoadEvents()
		except:
			exception.Abort('Mall.MallWindow.LoadEvents')
		
		self.isLoaded = True

	def _LoadVariables(self):
		self.cqc = ClientQuestCommunication()
		self.loadingBar = LoadingBar()
		self.startpage = MallStartpage()
		self.startpage.SetOpenMallEvent(self.OpenRequest)
		self.startpage.SetTitleName(CHOOSE_MALL_TITLE)
		self.webWnd = uiWeb.WebWindow()
		self.webWnd.LoadWindow()
		self.webWnd.Hide()
		self.curMallId = None
		self._malls = {}

		## verfuegbare quest kommandos
		commands = {
			'OPEN' : self.Open,
			'OPEN_STARTPAGE' : self.OpenStartpage,
			'ADD_STARTPAGE_MALL' : self.startpage.AddMall, ## param: id, title
			'ADD_STARTPAGE_BANNER' : self.startpage.AddBanner, ## param: id, path
			'ADD_CATEGORY' : self.AddCategory, ## param: id, title, color, icon_vnum, weighting
			'ADD_ITEM' : self.AddItem, ## param: id, vnum, socket0, socket1, socket2, socket3, socket4, socket5, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, mall_id, category_id, price, discount_percent, end_date, buyable_after_run_out, weighting, recommended, new
			'SET_LOADINGBAR_PERCENT' : self.loadingBar.SetPercent, ## param: percent
			'CLEAR_STARTPAGE_MALLS' : self.startpage.ClearMalls,
			'CLEAR_STARTPAGE_BANNERS' : self.startpage.ClearBanners,
			'SET_CURRENCY_VALUE' : self.SetCurrencyValue, ## param: value
			'SET_CURRENCY_NAME' : self.SetCurrencyTitle, ## param: title
			'SET_LAST_UPDATE_MALL_OVERVIEW' : self.startpage.SetLastUpdateMall, ## param: last_edit
			'SET_LAST_UPDATE_MALL' : self.SetLastUpdateMall, ## param: mall_id, last_edit
			'REMOVE_MALL' : self.RemoveMall, ## param: mall_id
			'CREATE_MALL' : self.CreateMall, ## param: mallId, title, buyCurrencyUrl
		}
		
		## chat debug aktivieren
		# self.cqc.EnableChatDebug()
		
		## kommandos hinzufuegen
		self.cqc.AddCommandDict(commands)
		
	def _LoadUi(self):
		self.elements = {
			'board' : self.GetChild('board'),
			'titlebar' : self.GetChild('titlebar'),
			
			'info_box' : self.GetChild('info_box'),
			
			'category_box' : CategoriesOverviewBox(),

			'render_target' : RenderTarget(),
			
			'pages': {
				'items_overview_box' : ItemsOverviewBox(),
				'startpage_box' : ThreeColumnItemsOverviewBox(),
			},

			'player_infos' : {
				'name_value' : self.GetChild('tx_info_box_name_value'), 
				'name' : self.GetChild('tx_info_box_name'), 
				'level_value' : self.GetChild('tx_info_box_level_value'), 
				'level' : self.GetChild('tx_info_box_level'), 
				'currency_value' : self.GetChild('tx_info_box_currency_value'), 
				'currency' : self.GetChild('tx_info_box_currency'), 
			},

			'text' : {
				'title_name' : self.GetChild('title_name'),
			},
			
			'buttons' : {
				'buy_currency' : self.GetChild('btn_info_box_buy_currency'),
				'startpage' : self.GetChild('btn_startpage'),
				'reset_original' : self.GetChild('btn_info_box_reset_original')
			},
		}

		self.elements['category_box'].SetParent(self.elements['board'])
		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			self.elements['category_box'].SetPosition(4, 76)
		else:
			self.elements['category_box'].SetPosition(10, 68)
		self.elements['category_box'].Show()
		self.elements['pages']['items_overview_box'].SetParent(self.elements['board'])
		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			self.elements['pages']['items_overview_box'].SetPosition(185, 27)
		else:
			self.elements['pages']['items_overview_box'].SetPosition(145+15, 35)
		self.elements['pages']['startpage_box'].SetParent(self.elements['board'])
		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			self.elements['pages']['startpage_box'].SetPosition(185, 27)
		else:
			self.elements['pages']['startpage_box'].SetPosition(145+15, 35)

		self.elements['render_target'].SetParent(self.elements['info_box'])
		self.elements['render_target'].SetPosition(2,2)
		self.elements['render_target'].Show()

		self.elements['buttons']['startpage'].SetText(STARTPAGE_TITLE)
		self.elements['player_infos']['name'].SetText(PLAYER_INFO_NAME_TITLE)
		self.elements['player_infos']['level'].SetText(PLAYER_INFO_LEVEL_TITLE)
		self.elements['player_infos']['currency'].SetText(PLAYER_INFO_CURRENCY_TITLE)
		self.elements['buttons']['buy_currency'].SetText(BUTTON_BUY_CURRENCY_TEXT)
		self.elements['buttons']['reset_original'].SetText(BUTTON_RESET_ORIGINAL)
	
	def _LoadEvents(self):
		self.elements['titlebar'].SetCloseEvent(ui.__mem_func__(self._OnClickClose))
		self.elements['buttons']['buy_currency'].SetEvent(self._OnClickBuyCurrency)
		self.elements['buttons']['startpage'].SetEvent(self._OnClickStartpage)
		self.elements['buttons']['reset_original'].SetEvent(self.elements['render_target'].SetOriginalCharacter)

		self.elements['category_box'].SetOnChangeEvent(self.ChangeCategory)
		self.elements['pages']['startpage_box'].SetOnRemoveItemEvent(self._RemoveItem)
		self.elements['pages']['startpage_box'].SetOnBuyItemEvent(self.BuyItem)
		self.elements['pages']['startpage_box'].SetOnPressPreviewEvent(self.elements['render_target'].SetPreview)
		self.elements['pages']['startpage_box'].SetEscapeEvent(self.Close)
		self.elements['pages']['items_overview_box'].SetOnRemoveItemEvent(self._RemoveItem)
		self.elements['pages']['items_overview_box'].SetOnBuyItemEvent(self.BuyItem)
		self.elements['pages']['items_overview_box'].SetOnPressPreviewEvent(self.elements['render_target'].SetPreview)
		self.elements['pages']['items_overview_box'].SetEscapeEvent(self.Close)

	## INIT GUI FUNCTIONS END

	## SEND QUEST COMMAND FUNCTIONS START

	def OpenStartpageRequest(self):
		self.cqc.SendQuestCommand('OPEN_STARTPAGE#%s' %(self.startpage.GetLastUpdateMall()))
	
	def OpenRequest(self, mallId):
		lastUpdate = '0000-00-00 00:00:00'
		if self.GetMall(mallId):
			lastUpdate = self.GetMall(mallId).GetLastUpdateMall()

		self.cqc.SendQuestCommand('OPEN#%d#%s' %(mallId, lastUpdate))

	def BuyItem(self, itemId, amount):
		self.cqc.SendQuestCommand('BUY#%d#%d' %(itemId, amount))

	## SEND QUEST COMMAND FUNCTIONS END

	## CQC COMMAND FUNCTIONS START

	def AddCategory(self, mallId, id, title, color, iconVnum, weighting):
		self.GetMall(mallId).AddCategory(id, title.replace("_"," "), color, iconVnum, weighting)
		
	def AddItem(self, id, vnum, socket0, socket1, socket2, socket3, socket4, socket5, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, mallId, categoryId, price, discountPercent, endDate, buyableAfterRunOut, weighting, recommended, new):
		self.GetMall(mallId).AddItem(id, categoryId, vnum, socket0, socket1, socket2, socket3, socket4, socket5, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, price, discountPercent, endDate, buyableAfterRunOut, weighting, recommended, new)

	def SetCurrencyValue(self, mallId, value):
		self.GetMall(mallId).currency.value = value
		self.RefreshCurrency(value, self.GetMall(mallId).currency.title)
	
	def SetCurrencyTitle(self, mallId, title):
		self.GetMall(mallId).currency.title = title.replace("_"," ")
		self.RefreshCurrency(self.GetMall(mallId).currency.value, self.GetMall(mallId).currency.title)

	def SetLastUpdateMall(self, mallId, lastUpdate):
		self.GetMall(mallId).SetLastUpdateMall(lastUpdate)

	def CreateMall(self, mallId, title, buyCoinsUrl):
		self._malls[int(mallId)] = self.Mall(title.replace("_"," "), buyCoinsUrl)

	def RemoveMall(self, mallId):
		if mallId in self._malls:
			del self._malls[mallId]

	def OpenStartpage(self):
		self.startpage.Open()

	def Open(self, mallId):
		self.curMallId = mallId
		curMall = self.GetMall(self.curMallId)
		self.CreateStartpage()
		self.CreateCategories()
		self.ChangePage('startpage')
		self.CreatePlayerInfos()
		self.elements['render_target'].SetOriginalCharacter()
		self.RefreshCurrency(curMall.currency.value, curMall.currency.title)
		self.RefreshDisplayBuyCurrencyButton()
		self.SetTop()
		self.Show()
	
	## CQC COMMAND FUNCTIONS END
		
	def GetMall(self, mallId):
		if int(mallId) in self._malls:
			return self._malls[int(mallId)]

	def CreateStartpage(self):
		curMall = self.GetMall(self.curMallId)

		itemsColumn1 = curMall.GetNewItems()
		itemsColumn2 = curMall.GetHotItems()
		itemsColumn3 = curMall.GetRecommendedItems()

		self.elements['pages']['startpage_box'].SetData(itemsColumn1, STARTPAGE_COLUMN_1_TITLE, itemsColumn2, STARTPAGE_COLUMN_2_TITLE, itemsColumn3, STARTPAGE_COLUMN_3_TITLE, curMall.currency.title)
		self.elements['pages']['startpage_box'].SetRemovableItemsColumn(2, True)

	def CreateCategories(self):
		self.elements['category_box'].SetData(self.GetMall(self.curMallId).GetCategoriesSortedByWeighting())

	## MAIN GUI FUNCTIONS START
	def CreatePlayerInfos(self):
		self.elements['player_infos']['name_value'].SetText(player.GetName())
		self.elements['player_infos']['level_value'].SetText(str(player.GetStatus(player.LEVEL)))

	def RefreshDisplayBuyCurrencyButton(self):
		url = self.GetMall(self.curMallId).buyCurrencyUrl
		if url != "NULL" and url != "":
			self.elements['buttons']['buy_currency'].Show()
		else:
			self.elements['buttons']['buy_currency'].Hide()

	def ChangePage(self, pageName):
		if pageName == 'startpage':
			self.elements['pages']['items_overview_box'].Hide()
			self.elements['pages']['startpage_box'].Show()
			self.SetTitle(STARTPAGE_TITLE)
		elif pageName == 'category':
			self.elements['pages']['startpage_box'].Hide()
			self.elements['pages']['items_overview_box'].Show()
			self.SetTitle(self.GetMall(self.curMallId).GetCategory(self.elements['category_box'].GetCurCategoryId()).title)

	def SetTitle(self, title):
		self.elements['text']['title_name'].SetText("%s - %s" %(self.GetMall(self.curMallId).title, title))

	def RefreshCurrency(self, value, title):
		self.elements['player_infos']['currency_value'].SetText(str(value)+' '+title)

	## MAIN GUI FUNCTIONS END

	## CATEGORY PAGE GUI FUNCTIONS START

	def ChangeCategory(self, categoryId):
		mall = self.GetMall(self.curMallId)
		self.elements['pages']['items_overview_box'].SetData(mall.GetItemsByCategoryIdSortedByWeighting(categoryId), mall.currency.title)
		self.ChangePage('category')

	## CATEGORY PAGE GUI FUNCTIONS END
	
	## ON CLICK EVENTS START

	def _OnClickClose(self):
		self.Close()

	def _OnClickBuyCurrency(self):
		self.webWnd.Open(self.GetMall(self.curMallId).buyCurrencyUrl)

	def _OnClickStartpage(self):
		self.ChangePage('startpage')

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

	def OnPressExitKey(self):
		self.Close()
		return TRUE

	## ON CLICK EVENTS END

	def _RemoveItem(self, itemId):
		self.GetMall(self.curMallId).RemoveItem(itemId)

	def Close(self):
		self.Hide()
		
	def SendSystemChat(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, "<Mall>: " + str(text))

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.Hide()
		
	class Mall(object):
		def __init__(self, title, buyCurrencyUrl):
			self.lastUpdateMall = '0000-00-00 00:00:00'
			self.title = title
			self.buyCurrencyUrl = buyCurrencyUrl
			self.currency = self.Currency()
			self._categories = []
			self._items = []

		def GetItemsLenByCategoryId(self, categoryId):
			return len([item for item in self._items if item.categoryId == categoryId])

		def AddItem(self, id, categoryId, vnum, socket0, socket1, socket2, socket3, socket4, socket5, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, price, discountPercent, endDate, buyableAfterRunOut, weighting, recommended, new):
			self._items.append(Item(id, categoryId, vnum, socket0, socket1, socket2, socket3, socket4, socket5, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, price, discountPercent, endDate, buyableAfterRunOut, weighting, recommended, new))
		
		def GetItem(self, id):
			for item in self._items:
				if item.id == int(id):
					return item
		
		def GetItems(self):
			return self._items

		def GetItemsByCategoryIdSortedByWeighting(self, categoryId):
			return sorted([item for item in self._items if item.categoryId == categoryId], key=lambda item: item.weighting, reverse=True)

		def GetNewItems(self):
			return sorted([item for item in self._items if item.new > 0], key=lambda item: item.new, reverse=True)

		def GetRecommendedItems(self):
			return sorted([item for item in self._items if item.recommended > 0], key=lambda item: item.recommended, reverse=True)

		def GetHotItems(self):
			return sorted([item for item in self._items if (item.endDate - app.GetGlobalTimeStamp()) > 0 or item.discountPercent > 0], key=lambda item: item.id, reverse=True)

		def GetItemsLen(self):
			return len(self._items)	
		
		def ClearItems(self):
			self._items = []

		def RemoveItem(self, itemId):
			for item in self._items:
				if item.id == itemId:
					self._items.remove(item)
					return
			
		def AddCategory(self, id, title, color, iconVnum, weighting):
			self._categories.append(self.Category(id, title, color, iconVnum, weighting))
		
		def GetCategory(self, id):
			for category in self._categories:
				if category.id == int(id):
					return category

		def GetCategories(self):
			return self._categories

		def GetCategoriesSortedByWeighting(self):
			return sorted(self._categories, key=lambda category: category.weighting, reverse=True)

		def GetCategoriesLen(self):
			return len(self._categories)
				
		def ClearCategories(self):
			self._categories = []

		def SetLastUpdateMall(self, lastUpdate):
			self.lastUpdateMall = lastUpdate

		def GetLastUpdateMall(self):
			return self.lastUpdateMall
			
		class Currency(object):
			def __init__(self):
				self.title = "None"
				self.value = 0

		class Category(object):
			def __init__(self, id, title, color, iconVnum, weighting):
				self.id = int(id)
				self.title = title
				self.color = color
				self.iconVnum = iconVnum
				self.weighting = int(weighting)
				
class Item(object):
	def __init__(self, id, categoryId, vnum, socket0, socket1, socket2, socket3, socket4, socket5, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, price, discountPercent, endDate, buyableAfterRunOut, weighting, recommended, new):
		self.id = int(id)
		self.categoryId = int(categoryId)
		self.vnum = int(vnum)
		self.socket0 = int(socket0)
		self.socket1 = int(socket1)
		self.socket2 = int(socket2)
		self.socket3 = int(socket3)
		self.socket4 = int(socket4)
		self.socket5 = int(socket5)
		self.attrtype0 = int(attrtype0)
		self.attrvalue0 = int(attrvalue0)
		self.attrtype1 = int(attrtype1)
		self.attrvalue1 = int(attrvalue1)
		self.attrtype2 = int(attrtype2)
		self.attrvalue2 = int(attrvalue2)
		self.attrtype3 = int(attrtype3)
		self.attrvalue3 = int(attrvalue3)
		self.attrtype4 = int(attrtype4)
		self.attrvalue4 = int(attrvalue4)
		self.attrtype5 = int(attrtype5)
		self.attrvalue5 = int(attrvalue5)
		self.attrtype6 = int(attrtype6)
		self.attrvalue6 = int(attrvalue6)
		self.price = int(price)
		self.discountPercent = int(discountPercent)
		self.endDate = int(endDate)
		self.buyableAfterRunOut = int(buyableAfterRunOut)
		self.weighting = int(weighting)
		self.recommended = int(recommended)
		self.new = int(new)

class CategoriesOverviewBox(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = False
		if False == self.isLoaded:
			self._LoadScript()
		
	def _LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH_MALL_DESIGN_BY_LORDZIEGE + 'partials/categories_box.py')
			else:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH + 'partials/categories_box.py')
		except:
			exception.Abort('Mall.CategoriesOverviewBox.LoadScriptFile')

		try:
			self._LoadVariables()
		except:
			exception.Abort('Mall.CategoriesOverviewBox.LoadVariables')

		try:
			self._LoadUi()
		except:
			exception.Abort('Mall.CategoriesOverviewBox.LoadUi')

		try:
			self._LoadEvents()
		except:
			exception.Abort('Mall.CategoriesOverviewBox.LoadEvents')
		
		self.isLoaded = True

	def _LoadVariables(self):
		self.categoryButtons = []
		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			self.categoryIcons = []
		self.categoryNav = Nav()
		self.curId = 0
		self.categories = []
		self.onChangeEvent = lambda *arg: None
				
	def _LoadUi(self):
		self.elements = {
			'background' : self.GetChild('background'),
		}
		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			maxCategoryButtons = 9
		else:
			maxCategoryButtons = 13
		#load category nav
		self.categoryNav.SetPlusButton(self.GetChild('btn_category_down'))
		self.categoryNav.SetMinusButton(self.GetChild('btn_category_up'))
		self.categoryNav.SetItemMax(maxCategoryButtons)
		#load category buttons
		for i in xrange(maxCategoryButtons):
			button = ui.Button()
			button.SetParent(self.elements['background'])
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				button.SetUpVisual(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"category_button_normal.tga")
				button.SetOverVisual(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"category_button_hover.tga")
				button.SetDownVisual(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"category_button_down.tga")
				button.SetPosition(13, 42+37*i)
			else:
				button.SetUpVisual(IMG_PATH+"btn_category_norm.sub")
				button.SetOverVisual(IMG_PATH+"btn_category_press.sub")
				button.SetDownVisual(IMG_PATH+"btn_category_hover.sub")
				button.SetPosition(6, 49+25*i)
			button.Show()
			self.categoryButtons.append(button)
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				itemIcon = ui.ImageBox()
				itemIcon.SetParent(self)
				itemIcon.AddFlag("not_pick")
				itemIcon.SetPosition(21, 42+37*i)
				itemIcon.LoadImage(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"empty_icon.tga")
				itemIcon.Show()
				self.categoryIcons.append(itemIcon)

	def _RefreshCategoryButtons(self):
		curPos = self.categoryNav.GetPos()
		maxCategories = self.categoryNav.GetItemMax()
		itemLen = self.categoryNav.GetItemLen()

		# hide all buttons if we got less categories than maxCategories
		if itemLen < maxCategories:
			for i in xrange(maxCategories):
				self.categoryButtons[i].Hide()
				if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
					self.categoryIcons[i].Hide()

		# show all buttons that should be displayed and set their name and function
		for i in xrange(min(maxCategories, itemLen)):
			curId = i + curPos
			self.categoryButtons[i].Show()
			self.categoryButtons[i].SetText(self.categories[curId].title)
			self.categoryButtons[i].SetEvent(lambda arg = curId: self._OnClickCategoryBtn(arg))
			if self.categories[curId].color != "NULL" and self.categories[curId].color != "":
				self.categoryButtons[i].SetTextColor(int('0xff%s' %self.categories[curId].color,0))
			else:
				self.categoryButtons[i].SetTextColor(0xffffffff)
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				self.categoryIcons[i].Show()
				if self.categories[curId].iconVnum != "NULL" and self.categories[curId].iconVnum != "":
					item.SelectItem(int(self.categories[curId].iconVnum))
					self.categoryIcons[i].LoadImage(item.GetIconImageFileName())
				else:
					self.categoryIcons[i].LoadImage(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"empty_icon.tga")
				# self.categoryButtons[i].ButtonText.SetPosition((self.categoryButtons[i].GetWidth()/2)+21, self.categoryButtons[i].GetHeight()/2)
				self.categoryButtons[i].ButtonText.SetPosition(50, self.categoryButtons[i].GetHeight()/2)
				self.categoryButtons[i].ButtonText.SetHorizontalAlignLeft()
	
	def _LoadEvents(self):
		self.categoryNav.SetOnChangeEvent(self._RefreshCategoryButtons)

	def SendSystemChat(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, "<Mall>: " + str(text))

	def SetOnChangeEvent(self, event):
		self.onChangeEvent = event
	
	def SetData(self, categories):
		self.categories = categories
		self.curId = 0
		self.categoryNav.SetItemLen(len(self.categories))
		self.categoryNav.SetPos(0)

	def GetCurCategoryId(self):
		return self.categories[self.curId].id

	def _OnClickCategoryBtn(self, id):
		self.curId = id
		if self.onChangeEvent:
			self.onChangeEvent(self.GetCurCategoryId())
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def Destroy(self):
		self.Hide()

class BannerSlider(ui.Window):
	class Banner(object):
		def __init__(self, path):
			self.path = path
		
	def __init__(self):
		ui.Window.__init__(self)
		self.onSwitchBannerEvent = lambda *arg: None
		self.onAddBannerEvent = None
		self.onClearBannersEvent = None
		self.banners = []
		self.bannerOptions = {
			'folder' : '',
			'time' : 5,
			'timeToFade' : 0.04,
			'interval' : 0.05, 
		}
		self._ResetBannerVars()
		self.isLoaded = False
		if False == self.isLoaded:
			self._LoadScript()

	def SetOnSwitchBannerEvent(self, event):
		self.onSwitchBannerEvent = event

	def SetOnAddBannerEvent(self, event):
		self.onAddBannerEvent = event

	def SetOnClearBannersEvent(self, event):
		self.onClearBannersEvent = event

	def SetBannerFolder(self, path):
		self.bannerOptions['folder'] = path

	def GetBannersCount(self):
		return len(self.banners)

	def _ResetBannerVars(self):
		self.bannerVars = {
				'fadeOut' : 0,
				'currentTime' : 0 ,
				'intervallEndTime' : 0,
				'currentAlphaValue' : 0,
				'currentImage' : 0,
				'lastSwitch' : 0,
			}

	def _LoadScript(self):
		## Load gui
		try: 
			self._BindObjects()
		except:
			exception.Abort('Mall.BannerSlider._LoadScript._BindObjects')
			
		self.isLoaded = True
		
	## Bind the objects in _LoadScript
	def _BindObjects(self):
		self.imageBanner = ui.ImageBox()
		self.imageFadeBanner = ui.ImageBox()
		self.imageBanner.SetParent(self)
		self.imageFadeBanner.SetParent(self)
		self.imageBanner.SetPosition(0,0)
		self.imageFadeBanner.SetPosition(0,0)
		self.imageBanner.Show()
		self.imageFadeBanner.Show()		

	def __del__(self):
		ui.Window.__del__(self)
		
	def Destroy(self):
		self.Hide()

	def OnUpdate(self):
		if len(self.banners) > 1:
			if self.bannerVars['lastSwitch'] < time.clock():
				self.SwitchBanner((self.bannerVars['currentImage']+1) % len(self.banners))
			if self.bannerVars['fadeOut'] == 1:
				self.bannerVars['currentTime'] = time.clock()

				if self.bannerVars['currentAlphaValue'] > 0.0:
					if self.bannerVars['currentTime'] >= self.bannerVars['intervallEndTime']:
						newAlphaValue = self.bannerVars['currentAlphaValue'] 
						newAlphaValue -= self.bannerOptions['interval']
						self.SetAlpha(self.imageFadeBanner, newAlphaValue)
						self.bannerVars['intervallEndTime'] = self.bannerVars['currentTime'] + self.bannerOptions['timeToFade']
				else:
					self.bannerVars['fadeOut'] = 0
					self.imageFadeBanner.Hide()

	def SwitchBanner(self, id):
		if len(self.banners) > id:
			self.bannerVars['lastSwitch'] = time.clock() + self.bannerOptions['time'] + self.bannerOptions['timeToFade']/self.bannerOptions['interval']
			self.imageFadeBanner.LoadImage(self.bannerOptions['folder'] + self.banners[self.bannerVars['currentImage']].path)
			self.imageFadeBanner.Show()
			self.imageBanner.LoadImage(self.bannerOptions['folder'] + self.banners[id].path)
			self.bannerVars['currentImage'] = id
			self.SetAlpha(self.imageFadeBanner, 1.0)
			self.bannerVars['fadeOut'] = 1
			self.bannerVars['intervallEndTime'] = self.bannerVars['currentTime'] + self.bannerOptions['timeToFade']
			if self.onSwitchBannerEvent:
				self.onSwitchBannerEvent(id)

	## set the alpha value of an image 'transparent'
	def SetAlpha(self, image, alpha):
		self.bannerVars['currentAlphaValue'] = alpha
		image.SetAlpha(alpha)

	def AddBanner(self, path):
		if os.path.isfile(self.bannerOptions['folder']+path):
			self.banners.append(self.Banner(path))
			self.SwitchBanner(0)
			if self.onAddBannerEvent:
				self.onAddBannerEvent()
		
	def ClearBanners(self):
		self.banners = []
		self._ResetBannerVars()
		if self.onClearBannersEvent:
			self.onClearBannersEvent()

class RenderTarget(ui.Window):

	def __init__(self):
		ui.Window.__init__(self)
		self.isLoaded = False
		self.isCharacter = False
		self.modelVnum = 0
		self.armorVnum = 0
		self.weaponVnum = 0
		self.hairVnum = 0
		self.sashVnum = 0
		if False == self.isLoaded:
			self._LoadScript()

	def _LoadScript(self):
		## Load gui
		try: 
			self._BindObjects()
		except:
			exception.Abort('Mall.PreviewObject._LoadScript._BindObjects')
			
		self.isLoaded = True
		
	## Bind the objects in _LoadScript
	def _BindObjects(self):
		self.renderer =  ui.RenderTarget()
		self.renderer.SetParent(self)
		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			self.renderer.SetSize(190,210)
			self.renderer.SetPosition(12, 10)
		else:
			self.renderer.SetSize(190,279)
			self.renderer.SetPosition(0, 0)
		self.renderer.SetRenderTarget(RENDER_TARGET_INDEX)
		self.renderer.Show()
		renderTarget.SetBackground(RENDER_TARGET_INDEX, IMG_PATH+"char_render_background.sub")
		renderTarget.SetVisibility(RENDER_TARGET_INDEX, True)

	def SendSystemChat(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, "<Mall RenderTarget>: " + str(text))

	def SetModel(self, vnum, isCharacter):
		self.modelVnum = vnum
		self.isCharacter = isCharacter
		renderTarget.SelectModel(RENDER_TARGET_INDEX, vnum)

	def SetModelNew(self, vnum, isCharacter):
		race = self.GetRaceRender()
		self.raceVnum = race
		self.isCharacter = isCharacter
		renderTarget.SelectModel(RENDER_TARGET_INDEX, race)

	def GetModel(self):
		return self.modelVnum

	def GetModelNew(self):
		return self.raceVnum

	def GetRaceRender(self):
		race = 0
		race_fix = net.GetMainActorRace()

#Warrior
		if item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 0:
			race = 9
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 2:
			race = 9
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 5:
			race = 9
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 7:
			race = 9
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 6:
			race = 4
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 4:
			race = 4
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 3:
			race = 4
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 1:
			race = 4
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 8:
			race = 9
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 8:
			race = 4
######################################################
#Assassin
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 7:
			race = 5
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 6:
			race = 1
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 5:
			race = 5
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 4:
			race = 1
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 3:
			race = 1
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 2:
			race = 5
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 1:
			race = 1
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 0:
			race = 5
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 8:
			race = 5
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 8:
			race = 1
######################################################
#Sura
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 7:
			race = 2
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 6:
			race = 6
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 5:
			race = 2
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 4:
			race = 6
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 3:
			race = 6
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 2:
			race = 2
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 1:
			race = 6
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 0:
			race = 2
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 8:
			race = 2
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 8:
			race = 6
######################################################

#Shaman
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 7:
			race = 7
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 6:
			race = 3
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 5:
			race = 7
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 4:
			race = 3
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 3:
			race = 3
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 2:
			race = 7
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 1:
			race = 3
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 0:
			race = 7
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 8:
			race = 7
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 8:
			race = 3
######################################################

#Wolfman
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 1:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 2:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 3:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 4:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 5:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 6:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 7:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 8:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 9:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR) and item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA) and item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 0:
			race = 8
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 3:
			race = 4
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN) and race_fix == 7:
			race = 9
		elif item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN) and race_fix == 8:
			race = 9
######################################################

		sex = chr.RaceToSex(player.GetRace())
		MALE = 1
		FEMALE = 0

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE) and sex == MALE:
			race = player.GetRace() + 4

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE) and sex == FEMALE:
			race = player.GetRace()

		if race == 0:
			race = player.GetRace()

		if race == 9:
			race = 0

		if race == 11: #Para atuendos de chamana mujer visualizados desde hombre
			race = 3

		return race

	def GetModel(self):
		return self.modelVnum

	def SetArmor(self, vnum):
		self.armorVnum = vnum
		renderTarget.ChangeArmor(RENDER_TARGET_INDEX, vnum)

	def GetArmor(self):
		return self.armorVnum

	def SetWeapon(self, vnum):
		self.weaponVnum = vnum
		renderTarget.ChangeWeapon(RENDER_TARGET_INDEX, vnum)

	def GetWeapon(self):
		return self.weaponVnum

	def SetHair(self, vnum):
		self.hairVnum = vnum
		renderTarget.ChangeHair(RENDER_TARGET_INDEX, vnum)

	def GetHair(self):
		return self.hairVnum
		
	def SetAcce(self, vnum):
		self.acceVnum = vnum
		renderTarget.ChangeAcce(RENDER_TARGET_INDEX, vnum)

	def SetAura(self, vnum):
		self.auraVnum = vnum
		renderTarget.ChangeAura(RENDER_TARGET_INDEX, vnum)

	if ENABLE_COSTUME_SHINING:
		def SetShiningWeapon(self, vnum):
			self.shiningweaponVnum = vnum
			renderTarget.ChangeWeaponShining(RENDER_TARGET_INDEX, vnum)

		def SetShiningArmor(self, vnum):
			self.shiningarmorVnum = vnum
			renderTarget.ChangeArmorShining(RENDER_TARGET_INDEX, vnum)

		def SetShiningSpecial(self, vnum):
			self.shiningspecialVnum = vnum
			renderTarget.ChangeSpecialShining(RENDER_TARGET_INDEX, vnum)

		def GetShiningWeapon(self):
			return self.shiningweaponVnum

		def GetShiningArmor(self):
			return self.shiningarmorVnum

		def GetShiningSpecial(self):
			return self.shiningspecialVnum

	def GetAura(self):
		return self.auraVnum

	def GetAcce(self):
		return self.acceVnum
		
	def SetOriginalCharacter(self):
		self.SetCharacter(player.GetRace(), player.GetArmor(), player.GetWeapon(), player.GetHair())
		if ENABLE_ACCE_SYSTEM:
			self.SetAcce(player.GetAcce())
		if ENABLE_COSTUME_AURA:
			self.SetAura(player.GetAura())
		
		#self.SetShiningWeapon(vnum)
		

	def SetCharacter(self, race, armor, weapon, hair):
		self.SetModel(race, True)
		self.SetArmor(armor)
		self.SetWeapon(weapon)
		self.SetHair(hair)
		self.isCharacter = True

	def SetCharacterNew(self, race, armor, weapon, hair):
		self.SetModelNew(race, True)
		self.SetArmor(armor)
		self.SetWeapon(weapon)
		self.SetHair(hair)
		self.isCharacter = True

	def SetCharacterWithoutWeapon(self, race, armor, hair):
		self.SetModelNew(race, True)
		self.SetArmor(armor)
		self.SetHair(hair)
		self.isCharacter = True

	def SetPreview(self, vnum, isMob = False):
		item.SelectItem(vnum)
		
		if not isMob:
			# if item.GetItemType() == item.ITEM_TYPE_WEAPON:
			if item.GetItemType() == item.ITEM_TYPE_WEAPON or item.GetItemSubType() == item.COSTUME_TYPE_WEAPON:
				if not self.isCharacter:
					self.SetOriginalCharacter()
				self.SetCharacterNew(self.GetModel(), self.GetArmor(), vnum, self.GetHair())
				if ENABLE_ACCE_SYSTEM:
					self.SetAcce(self.GetAcce())
				#if ENABLE_COSTUME_AURA:
					#self.SetAura(self.GetAura())
			elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
				if not self.isCharacter:
					self.SetOriginalCharacter()
				self.SetCharacterWithoutWeapon(self.GetModel(), vnum, self.GetHair())
				if ENABLE_ACCE_SYSTEM:
					self.SetAcce(self.GetAcce())
				if ENABLE_COSTUME_AURA:
					self.SetAura(self.GetAura())

			# if ENABLE_COSTUME_SHINING:
				# elif item.GetItemType() == item.ITEM_TYPE_SHINING:
					# if item.GetItemSubType() == item.SHINING_WEAPON:
						# self.SetOriginalCharacter()
						# self.SetCharacter(self.GetModel(), self.GetArmor(), self.GetWeapon(), self.GetHair())
						# if ENABLE_ACCE_SYSTEM:
							# self.SetAcce(self.GetAcce())
						# if ENABLE_COSTUME_AURA:
							# self.SetAura(self.GetAura())
						# self.SetShiningWeapon(vnum)
					# elif item.GetItemSubType() == item.SHINING_ARMOR:
						# self.SetOriginalCharacter()
						# self.SetCharacter(self.GetModel(), self.GetArmor(), self.GetWeapon(), self.GetHair())
						# if ENABLE_ACCE_SYSTEM:
							# self.SetAcce(self.GetAcce())
						# if ENABLE_COSTUME_AURA:
							# self.SetAura(self.GetAura())
						# self.SetShiningArmor(vnum)
					# elif item.GetItemSubType() == item.SHINING_SPECIAL:
						# self.SetOriginalCharacter()
						# self.SetCharacter(self.GetModel(), self.GetArmor(), self.GetWeapon(), self.GetHair())
						# if ENABLE_ACCE_SYSTEM:
							# self.SetAcce(self.GetAcce())
						# if ENABLE_COSTUME_AURA:
							# self.SetAura(self.GetAura())
						# self.SetShiningSpecial(vnum)

			elif item.GetItemType() == item.ITEM_TYPE_COSTUME:
				if item.GetItemSubType() == item.COSTUME_TYPE_HAIR:
					if not self.isCharacter:
						self.SetOriginalCharacter()
					self.SetCharacterNew(self.GetModel(), self.GetArmor(), self.GetWeapon(), item.GetValue(3))
					if ENABLE_ACCE_SYSTEM:
						self.SetAcce(self.GetAcce())
					if ENABLE_COSTUME_AURA:
						self.SetAura(self.GetAura())
				elif item.GetItemSubType() == item.COSTUME_TYPE_BODY:
					if not self.isCharacter:
						self.SetOriginalCharacter()
					self.SetCharacterWithoutWeapon(self.GetModel(), vnum, self.GetHair())
					if ENABLE_ACCE_SYSTEM:
						self.SetAcce(self.GetAcce())
					if ENABLE_COSTUME_AURA:
						self.SetAura(self.GetAura())
				elif ENABLE_ACCE_SYSTEM and item.GetItemSubType() == item.COSTUME_TYPE_SASH:
					if not self.isCharacter:
						self.SetOriginalCharacter()
					self.SetCharacterWithoutWeapon(self.GetModel(), self.GetArmor(), self.GetHair())
					self.SetAcce(vnum)
				elif ENABLE_COSTUME_AURA and item.GetItemSubType() == item.COSTUME_TYPE_AURA:
					if not self.isCharacter:
						self.SetOriginalCharacter()
					self.SetCharacterWithoutWeapon(self.GetModel(), self.GetArmor(), self.GetHair())
					self.SetAura(vnum)
		else:
			self.SetModel(vnum, False)

	def __del__(self):
		ui.Window.__del__(self)

	def Destroy(self):
		self.Hide()
		
class MallStartpage(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = False
		if False == self.isLoaded:
			self._LoadScript()
		
	def _LoadScript(self):
		try:
			self._LoadVariables()
		except:
			exception.Abort('MallStartpage._LoadScript.LoadVariables')
		## Load script
		try:
			pyScrLoader = ui.PythonScriptLoader()
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH_MALL_DESIGN_BY_LORDZIEGE+'mall_startpage.py')
			else:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH+'mall_startpage.py')
		except:
			exception.Abort('MallStartpage._LoadScript.LoadScriptFile')
		## Load gui
		try: 
			self._LoadUi()
		except:
			exception.Abort('MallStartpage._LoadScript._LoadUi')
		## Load events
		try: 
			self._LoadEvents()
		except:
			exception.Abort('MallStartpage._LoadScript._LoadEvents')
		
		self.isLoaded = True

	def _LoadVariables(self):
		self.lastUpdateMall = '0000-00-00 00:00:00'
		self.malls = []
		self.mallButtons = []
		self.mallNav = Nav()
		self.bannerSlider = BannerSlider()
		self.bannerSlider.SetBannerFolder('banner/')
		self.maxBannerButtons = 10 # No one will use more than 10 banners. Or?
		self.bannerButtons = []
		self.openMallEvent = None
		
	def _LoadUi(self):
		self.elements = {
			'board' : self.GetChild('board'),
			'titlebar' : self.GetChild('titlebar'),
			'malls' : self.GetChild('malls'),
			'banner' : self.GetChild('banner'),
			'text' : {
				'title_name' : self.GetChild('title_name'),
			},
		}
		
		
		maxMallButtons = 6
		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			maxMallButtons = 4
		#load mall nav
		self.mallNav.SetPlusButton(self.GetChild('btn_mall_arrow_down'))
		self.mallNav.SetMinusButton(self.GetChild('btn_mall_arrow_up'))
		self.mallNav.SetItemMax(maxMallButtons)
		#load mall buttons
		for i in xrange(maxMallButtons):
			button = ui.Button()
			button.SetParent(self.elements['malls'])
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				button.SetUpVisual(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"shop_button_normal.tga")
				button.SetOverVisual(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"shop_button_down.tga")
				button.SetDownVisual(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"shop_button_hover.tga")
				button.SetPosition(5, 24+30*i)
			else:
				button.SetUpVisual(IMG_PATH+"btn_category_norm.sub")
				button.SetOverVisual(IMG_PATH+"btn_category_press.sub")
				button.SetDownVisual(IMG_PATH+"btn_category_hover.sub")
				button.SetPosition(5, 10+25*i)
			self.mallButtons.append(button)
		
		#load banner slider
		self.bannerSlider.SetParent(self.elements['banner'])
		self.bannerSlider.SetSize(400, 150)
		self.bannerSlider.SetPosition(10, 10)
		self.bannerSlider.Show()

		#load banner buttons
		for i in xrange(self.maxBannerButtons):
			toggleButton = ui.ToggleButton()
			toggleButton.SetParent(self.elements['banner'])
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				toggleButton.SetUpVisual(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"dot_norm.tga")
				toggleButton.SetOverVisual(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"dot_hover.tga")
				toggleButton.SetDownVisual(IMG_PATH_MALL_DESIGN_BY_LORDZIEGE+"dot_press.tga")
				toggleButton.SetPosition((400-20*self.maxBannerButtons)+20*i, 160)
			else:
				toggleButton.SetUpVisual(IMG_PATH+"dot_norm.sub")
				toggleButton.SetOverVisual(IMG_PATH+"dot_hover.sub")
				toggleButton.SetDownVisual(IMG_PATH+"dot_press.sub")
				toggleButton.SetPosition((400-20*self.maxBannerButtons)+20*i, 140)
			self.bannerButtons.append(toggleButton)

	def _LoadEvents(self):
		self.elements['titlebar'].SetCloseEvent(ui.__mem_func__(self._OnClickClose))
		self.mallNav.SetOnChangeEvent(self.RefreshMallButtons)
		self.bannerSlider.SetOnSwitchBannerEvent(self.ChangeBannerButton)
		self.bannerSlider.SetOnAddBannerEvent(self.RefreshBannerButtons)
		self.bannerSlider.SetOnClearBannersEvent(self.RefreshBannerButtons)

	def SetTitleName(self, text):
		self.elements['text']['title_name'].SetText(text)

	def SetLastUpdateMall(self, lastUpdate):
		self.lastUpdateMall = lastUpdate

	def GetLastUpdateMall(self):
		return self.lastUpdateMall

	def SetOpenMallEvent(self, event):
		self.openMallEvent = event
		
	def _OnClickClose(self):
		self.Close()
	
	def _OnClickMallBtn(self, mallId):
		if self.openMallEvent:
			self.Close()
			self.openMallEvent(mallId)

	def _OnClickBannerBtn(self, id):
		self.bannerSlider.SwitchBanner(id)
	
	def AddMall(self, id, title):
		self.malls.append(self.Mall(id,title.replace("_"," ")))
		self.mallNav.SetItemLen(len(self.malls))
		self.mallNav.SetPos(0)
		
	def AddBanner(self, path):
		self.bannerSlider.AddBanner(path)
		
	def ClearMalls(self):
		self.malls = []
		self.mallNav.SetItemLen(0)
		self.mallNav.SetPos(0)
		
	def ClearBanners(self):
		self.bannerSlider.ClearBanners()
	
	def ChangeBannerButton(self, id):
		for bannerButton in self.bannerButtons:
			bannerButton.SetUp()
		self.bannerButtons[self.maxBannerButtons - self.bannerSlider.GetBannersCount() + id].Down()
		
	def RefreshBannerButtons(self):
		# hide all buttons
		for i in xrange(self.maxBannerButtons):
			self.bannerButtons[i].Hide()

		# show all buttons that should be displayed and set their name and function
		maxBannersLen = min(self.maxBannerButtons, self.bannerSlider.GetBannersCount())
		for i in xrange(maxBannersLen):
			realIndex = (self.maxBannerButtons-1) - i
			self.bannerButtons[realIndex].Show()
			self.bannerButtons[realIndex].SetToggleUpEvent(lambda arg = ((maxBannersLen-1) - i): self._OnClickBannerBtn(arg))
			self.bannerButtons[realIndex].SetToggleDownEvent(lambda arg = ((maxBannersLen-1) - i): self._OnClickBannerBtn(arg))
		
	def RefreshMallButtons(self):
		curPos = self.mallNav.GetPos()
		maxMalls = self.mallNav.GetItemMax()
		itemLen = self.mallNav.GetItemLen()

		# hide all buttons if we got less malls than maxMalls
		if itemLen < maxMalls:
			for i in xrange(maxMalls):
				self.mallButtons[i].Hide()

		# show all buttons that should be displayed and set their name and function
		for i in xrange(min(maxMalls, itemLen)):
			curId = i + curPos
			self.mallButtons[i].Show()
			self.mallButtons[i].SetText(self.malls[curId].title)
			self.mallButtons[i].SetEvent(lambda arg = self.malls[curId].id: self._OnClickMallBtn(arg))
		
	def Open(self):
		self.mallNav.SetPos(0)
		self.bannerSlider.SwitchBanner(0)
		self.SetTop()
		self.Show()
		
	def Close(self):
		self.Hide()

	def SendSystemChat(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, "<Mall Startpage>: " + str(text))

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

	def OnPressExitKey(self):
		self.Close()
		return TRUE

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.Hide()

	class Mall(object):
		def __init__(self, id, title):
			self.id = int(id)
			self.title = title

class ThreeColumnItemsOverviewBox(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = False
		if False == self.isLoaded:
			self._LoadScript()
		
	def _LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH_MALL_DESIGN_BY_LORDZIEGE + 'partials/three_column_items_overview_box.py')
			else:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH + 'partials/three_column_items_overview_box.py')
		except:
			exception.Abort('Mall.ThreeColumnItemsOverviewBox.LoadScriptFile')

		try:
			self._LoadVariables()
		except:
			exception.Abort('Mall.ThreeColumnItemsOverviewBox.LoadVariables')

		try:
			self._LoadUi()
		except:
			exception.Abort('Mall.ThreeColumnItemsOverviewBox.LoadUi')

		try:
			self._LoadEvents()
		except:
			exception.Abort('Mall.ThreeColumnItemsOverviewBox.LoadEvents')
		
		self.isLoaded = True

	def _LoadVariables(self):
		self._items = {
			'column1' : [],
			'column2' : [],
			'column3' : [],
		}
		self._columnTitles = {
			'column1' : 'None',
			'column2' : 'None',
			'column3' : 'None',
		}
		self._navs = {
			'column1' : Nav(),
			'column2' : Nav(),
			'column3' : Nav(),
		}
		self._itemBoxes = {
			'column1' : [],
			'column2' : [],
			'column3' : [],
		}
		self._currencyTitle = "None"
		self.onRemoveItemEvent = lambda *arg: None
		self.onBuyItemEvent = lambda *arg: None
		self.onPressPreviewEvent = lambda *arg: None
		self.escapeEvent = None

		self._removableItemsColumn = {
			'column1' : False,
			'column2' : False,
			'column3' : False,
		}
				
	def _LoadUi(self):
		self.elements = {
			'background' : self.GetChild('background'),
			'column1' : {
				'page_nr' : self.GetChild('column1_page_nr'),
				'text' : {
					'page_nr' : self.GetChild('tx_column1_page_nr'),
					'title' : self.GetChild('tx_column1_title'),
				},
			},
			'column2' : {
				'page_nr' : self.GetChild('column2_page_nr'),
				'text' : {
					'page_nr' : self.GetChild('tx_column2_page_nr'),
					'title' : self.GetChild('tx_column2_title'),
				},
			},
			'column3' : {
				'page_nr' : self.GetChild('column3_page_nr'),
				'text' : {
					'page_nr' : self.GetChild('tx_column3_page_nr'),
					'title' : self.GetChild('tx_column3_title'),
				},
			},
		}

		maxItemboxes = 3
		maxItemboxesCountX = 3
		itemboxesYCoords = {'column1':37,'column2':191,'column3':345}

		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			maxItemboxes = 2
			maxItemboxesCountX = 2
			itemboxesYCoords = {'column1':34,'column2':183,'column3':331}

		for columnId in [1,2,3]:
			#load nav for column
			self._navs['column%d' %columnId].SetPlusButton(self.GetChild('btn_column%d_right' %columnId))
			self._navs['column%d' %columnId].SetMinusButton(self.GetChild('btn_column%d_left' %columnId))
			self._navs['column%d' %columnId].SetItemMax(maxItemboxes)
			self._navs['column%d' %columnId].SetScrollStep(maxItemboxes)

			#load itemboxes for column
			for i in xrange(maxItemboxes):
				itemBox = ItemBox()
				itemBox.SetParent(self.elements['background'])	
				if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
					itemBox.SetPosition(14 + 170*(i%(maxItemboxesCountX)), itemboxesYCoords['column%d' %columnId])
				else:
					itemBox.SetPosition(5 + 133*(i%(maxItemboxesCountX)), itemboxesYCoords['column%d' %columnId])
				itemBox.SetWantToBuyText(QUESTION_WANT_TO_BUY)
				itemBox.SetButtonPreviewText(ITEMBOX_BUTTON_PREVIEW_TEXT)
				itemBox.SetButtonBuyText(ITEMBOX_BUTTON_BUY_TEXT)
				itemBox.SetItemAmountText(ITEMBOX_ITEM_AMOUNT_TEXT)
				itemBox.SetOnBuyItemEvent(self._BuyItem)
				itemBox.SetOnPressPreviewEvent(self._PressPreview)
				itemBox.SetEscapeEvent(self._EscapeEvent)
				itemBox.Show()
				self._itemBoxes['column%d' %columnId].append(itemBox)
		
	def _LoadEvents(self):
		for i in [1,2,3]:
			self._navs['column%d' %i].SetOnChangeEvent(lambda arg = i: self._RefreshItems(arg))

	def _BuyItem(self, itemId, amount):
		if self.onBuyItemEvent:
			self.onBuyItemEvent(itemId, amount)

	def _PressPreview(self, vnum, isMob = False):
		if self.onPressPreviewEvent:
			self.onPressPreviewEvent(vnum, isMob)

	def _EscapeEvent(self):
		if self.escapeEvent:
			self.escapeEvent()

	def _RefreshItems(self, columnId):
		curPos = self._navs['column%d' %columnId].GetPos()
		maxItems = self._navs['column%d' %columnId].GetItemMax()
		itemLen = self._navs['column%d' %columnId].GetItemLen() - curPos

		# hide all itemboxes if we got less items than maxItems to show
		if itemLen < maxItems:
			for i in xrange(maxItems):
				self._itemBoxes['column%d' %columnId][i].Hide()

		# show all itemboxes that should be displayed and set their data
		for i in xrange(min(maxItems, itemLen)):
			curId = i + curPos
			self._itemBoxes['column%d' %columnId][i].SetData(self._items['column%d'%columnId][curId], self._currencyTitle)
			self._itemBoxes['column%d' %columnId][i].Show()
		self._RefreshPageNr(columnId)

	def SendSystemChat(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, "<Mall>: " + str(text))

	def _RefreshPageNr(self, columnId):
		if self._navs['column%d' %columnId].GetMaxPage() > 1:
			self.elements['column%d' %columnId]['page_nr'].Show()
			self.elements['column%d' %columnId]['text']['page_nr'].SetText("%d/%d" %(self._navs['column%d' %columnId].GetCurPage(), self._navs['column%d' %columnId].GetMaxPage()))
		else:
			self.elements['column%d' %columnId]['page_nr'].Hide()

	def _GetItemIsBuyableAfterRunOut(self, itemId, columnId):
		for item in self._items['column%d'%columnId]:
			if item.id == itemId:
				return item.buyableAfterRunOut

	def _RemoveItemFromColumn(self, itemId, columnId):
		for item in self._items['column%d'%columnId]:
			if item.id == itemId:
				self._items['column%d'%columnId].remove(item)
				return True
		return False
	
	def _RemoveItembox(self, itemId):
		runOnRemoveItemEvent = False

		for columnId in [1,2,3]:
			refreshItems = False
			nav = self._navs['column%d' %columnId]
			if self._GetItemIsBuyableAfterRunOut(itemId, columnId):
				# should we remove this item from this column if its still buyable?
				if self._IsRemovableItemsColumn(columnId):
					if self._RemoveItemFromColumn(itemId, columnId):
						# set new nav length
						nav.SetItemLen(len(self._items['column%d' %columnId]))
						refreshItems = True
						
			else:
				if self._RemoveItemFromColumn(itemId, columnId):
					nav.SetItemLen(len(self._items['column%d' %columnId]))
					refreshItems = True
					runOnRemoveItemEvent = True

			if refreshItems:
				# are we still on the max page? or did we just made an empty page? Go back to max page!
				if nav.GetMaxPage() < nav.GetCurPage():
					nav.SetCurPage(nav.GetMaxPage())
				else:
					nav.SetCurPage(nav.GetCurPage())

		if runOnRemoveItemEvent:
			if self.onRemoveItemEvent:
				self.onRemoveItemEvent(itemId)

	def OnUpdate(self):
		for columnId in [1,2,3]: 
			for item in self._items['column%d' %columnId]:
				if item.endDate != 25200 and item.endDate != 0 and (item.endDate - app.GetGlobalTimeStamp()) <= 0 and not item.buyableAfterRunOut:
					self._RemoveItembox(item.id)

	def SetOnRemoveItemEvent(self, event):
		self.onRemoveItemEvent = event

	def SetOnBuyItemEvent(self, event):
		self.onBuyItemEvent = event

	def SetOnPressPreviewEvent(self, event):
		self.onPressPreviewEvent = event

	def SetEscapeEvent(self, event):
		self.escapeEvent = event

	def SetRemovableItemsColumn(self, columnId, removable):
		self._removableItemsColumn['column%d'%columnId] = removable

	def _IsRemovableItemsColumn(self, columnId):
		return self._removableItemsColumn['column%d'%columnId]

	def SetData(self, itemsColumn1, column1Title, itemsColumn2, column2Title,  itemsColumn3, column3Title, currencyTitle):
		self._items['column1'] = itemsColumn1
		self._items['column2'] = itemsColumn2
		self._items['column3'] = itemsColumn3

		self.elements['column1']['text']['title'].SetText(column1Title)
		self.elements['column2']['text']['title'].SetText(column2Title)
		self.elements['column3']['text']['title'].SetText(column3Title)

		self._currencyTitle = currencyTitle

		for i in [1,2,3]:
			self._navs['column%d' %i].SetItemLen(len(self._items['column%d' %i]))
			self._navs['column%d' %i].SetPos(0)
	
	def SetColumn1Title(self, title):
		self._columnTitles['column1'] = title
	
	def SetColumn2Title(self, title):
		self._columnTitles['column2'] = title

	def SetColumn3Title(self, title):
		self._columnTitles['column3'] = title

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.Hide()

class ItemsOverviewBox(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = False
		if False == self.isLoaded:
			self._LoadScript()
		
	def _LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH_MALL_DESIGN_BY_LORDZIEGE + 'partials/items_overview_box.py')
			else:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH + 'partials/items_overview_box.py')
		except:
			exception.Abort('Mall.ItemsOverviewBox.LoadScriptFile')

		try:
			self._LoadVariables()
		except:
			exception.Abort('Mall.ItemsOverviewBox.LoadVariables')

		try:
			self._LoadUi()
		except:
			exception.Abort('Mall.ItemsOverviewBox.LoadUi')

		try:
			self._LoadEvents()
		except:
			exception.Abort('Mall.ItemsOverviewBox.LoadEvents')
		
		self.isLoaded = True

	def _LoadVariables(self):
		self._items = []
		self._currencyTitle = "None"
		self._itemBoxes = []
		self._nav = Nav()
		self.onRemoveItemEvent = lambda *arg: None
		self.onBuyItemEvent = lambda *arg: None
		self.onPressPreviewEvent = lambda *arg: None
		self.escapeEvent = None
				
	def _LoadUi(self):
		self.elements = {
			'background' : self.GetChild('background'),
			'page_nr' : self.GetChild('page_nr'),
			'text' : {
				'page_nr' : self.GetChild('tx_page_nr'),
			},
		}

		maxItemboxes = 9
		maxItemboxesCountX = 3

		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			maxItemboxes = 6
			maxItemboxesCountX = 2
		
		#load for sale nav
		self._nav.SetPlusButton(self.GetChild('btn_right'))
		self._nav.SetMinusButton(self.GetChild('btn_left'))
		self._nav.SetItemMax(maxItemboxes)
		self._nav.SetScrollStep(maxItemboxes)

		#load itemboxes
		for i in xrange(maxItemboxes):
			itemBox = ItemBox()
			itemBox.SetParent(self.elements['background'])	
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				itemBox.SetPosition(14 + 170*(i%(maxItemboxesCountX)), 34+123*(i/maxItemboxesCountX))
			else:
				itemBox.SetPosition(5 + 133*(i%(maxItemboxesCountX)), 71+120*(i/maxItemboxesCountX))
			itemBox.SetWantToBuyText(QUESTION_WANT_TO_BUY)
			itemBox.SetButtonPreviewText(ITEMBOX_BUTTON_PREVIEW_TEXT)
			itemBox.SetButtonBuyText(ITEMBOX_BUTTON_BUY_TEXT)
			itemBox.SetItemAmountText(ITEMBOX_ITEM_AMOUNT_TEXT)
			itemBox.SetOnBuyItemEvent(self._BuyItem)
			itemBox.SetOnPressPreviewEvent(self._PressPreview)
			itemBox.SetEscapeEvent(self._EscapeEvent)
			itemBox.Show()
			self._itemBoxes.append(itemBox)

	def _LoadEvents(self):
		self._nav.SetOnChangeEvent(self._RefreshItems)

	def _BuyItem(self, itemId, amount):
		if self.onBuyItemEvent:
			self.onBuyItemEvent(itemId, amount)

	def _PressPreview(self, vnum, isMob = False):
		if self.onPressPreviewEvent:
			self.onPressPreviewEvent(vnum, isMob)

	def _EscapeEvent(self):
		if self.escapeEvent:
			self.escapeEvent()

	def _RefreshItems(self):
		curPos = self._nav.GetPos()
		maxItems = self._nav.GetItemMax()
		itemLen = self._nav.GetItemLen() - curPos

		# hide all itemboxes if we got less items than maxItems to show
		if itemLen < maxItems:
			for i in xrange(maxItems):
				self._itemBoxes[i].Hide()

		# show all itemboxes that should be displayed and set their data
		for i in xrange(min(maxItems, itemLen)):
			curId = i + curPos
			self._itemBoxes[i].SetData(self._items[curId], self._currencyTitle)
			self._itemBoxes[i].Show()
		self._RefreshPageNr()

	def SendSystemChat(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, "<Mall>: " + str(text))
	
	def _RefreshPageNr(self):
		if self._nav.GetMaxPage() > 1:
			self.elements['page_nr'].Show()
			self.elements['text']['page_nr'].SetText("%d/%d" %(self._nav.GetCurPage(), self._nav.GetMaxPage()))
		else:
			self.elements['page_nr'].Hide()

	def _RemoveItem(self, itemId):
		for item in self._items:
			if item.id == itemId:
				if not item.buyableAfterRunOut:
					self._items.remove(item)
					return True
				else:
					return False
		return False

	def _GetItemIsBuyableAfterRunOut(self, itemId):
		for item in self._items:
			if item.id == itemId:
				return item.buyableAfterRunOut

	def _RemoveItembox(self, itemId):
		if self._RemoveItem(itemId):
			self._nav.SetItemLen(len(self._items))

			# are we still on the max page? or did we just made an empty page? Go back to max page!
			if self._nav.GetMaxPage() < self._nav.GetCurPage():
				self._nav.SetCurPage(self._nav.GetMaxPage())
			else:
				self._nav.SetCurPage(self._nav.GetCurPage())
		
			if self.onRemoveItemEvent:
				self.onRemoveItemEvent(itemId)

	def OnUpdate(self):
		for item in self._items:
			if item.endDate != 25200 and item.endDate != 0 and (item.endDate - app.GetGlobalTimeStamp()) <= 0 and not item.buyableAfterRunOut:
				self._RemoveItembox(item.id)

	def SetOnRemoveItemEvent(self, event):
		self.onRemoveItemEvent = event

	def SetOnBuyItemEvent(self, event):
		self.onBuyItemEvent = event

	def SetOnPressPreviewEvent(self, event):
		self.onPressPreviewEvent = event

	def SetEscapeEvent(self, event):
		self.escapeEvent = event

	def SetData(self, items, currencyTitle):
		self._items = items
		self._currencyTitle = currencyTitle
		self._nav.SetItemLen(len(self._items))
		self._nav.SetPos(0)
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def Destroy(self):
		self.Hide()

class ItemBox(ui.ScriptWindow):
	rideVnums = {
		## thanks to Sehnsucht
		## Vnum:Mobvnum
		52001:20201,
		52002:20201,
		52003:20201,
		52004:20201,
		52005:20201,
		52006:20205,
		52007:20205,
		52008:20205,
		52009:20205,
		52010:20205,
		52011:20209,
		52012:20209,
		52013:20209,
		52014:20209,
		52015:20209,
		52016:20202,
		52017:20202,
		52018:20202,
		52019:20202,
		52020:20202,
		52021:20206,
		52022:20206,
		52023:20206,
		52024:20206,
		52025:20206,
		52026:20210,
		52027:20210,
		52028:20210,
		52029:20210,
		52030:20210,
		52031:20204,
		52032:20204,
		52033:20204,
		52034:20204,
		52035:20204,
		52036:20208,
		52037:20208,
		52038:20208,
		52039:20208,
		52040:20208,
		52041:20212,
		52042:20212,
		52043:20212,
		52044:20212,
		52045:20212,
		52046:20203,
		52047:20203,
		52048:20203,
		52049:20203,
		52050:20203,
		52051:20207,
		52052:20207,
		52053:20207,
		52054:20207,
		52055:20207,
		52056:20211,
		52057:20211,
		52058:20211,
		52059:20211,
		52060:20211,
		52061:20213,
		52062:20213,
		52063:20213,
		52064:20213,
		52065:20213,
		52066:20214,
		52067:20214,
		52068:20214,
		52069:20214,
		52070:20214,
		52071:20215,
		52072:20215,
		52073:20215,
		52074:20215,
		52075:20215,
		52076:20216,
		52077:20216,
		52078:20216,
		52079:20216,
		52080:20216,
		52081:20217,
		52082:20217,
		52083:20217,
		52084:20217,
		52085:20217,
		52086:20218,
		52087:20218,
		52088:20218,
		52089:20218,
		52090:20218,
		52091:20223,
		52092:20223,
		52093:20223,
		52094:20223,
		52095:20223,
		52096:20224,
		52097:20224,
		52098:20224,
		52099:20224,
		52100:20224,
		52101:20225,
		52102:20225,
		52103:20225,
		52104:20225,
		52105:20225,
		52107:20228,
		52106:20228,
		52108:20228,
		52109:20228,
		52110:20228,
		52111:20229,
		52112:20229,
		52113:20229,
		52114:20229,
		52115:20229,
		52116:20230,
		52117:20230,
		52118:20230,
		52119:20230,
		52120:20230,
		71114:20110,
		71116:20111,
		71118:20112,
		71120:20113,
		71115:20110,
		71117:20111,
		71119:20112,
		71121:20113,
		71124:20114,
		71125:20115,
		71126:20116,
		71127:20117,
		71128:20118,
		71131:20119,
		71132:20119,
		71133:20119,
		71134:20119,
		71137:20120,
		71138:20121,
		71139:20122,
		71140:20123,
		71141:20124,
		71142:20125,
		71161:20219,
		71164:20220,
		71165:20221,
		71166:20222,
		71171:20227,
		71172:20226,
		71176:20231,
		71177:20232,
		71182:20233,
		71183:20234,
		71184:20235,
		71185:20236,
		71186:20237,
		71187:20238,
		71192:20240,
		71193:20239,
		71197:20241,
		71198:20242,
		71220:20243,
		
		98002:20810,
		98003:20811,
		71291:20299
		
		
		
	}

	petVnums = {
		## thanks to Sehnsucht
		## Vnum:Mobvnum
		53001:34001,
		53002:34002,
		53003:34003,
		53005:34004,
		53006:34009,
		53007:34010,
		53008:34011,
		53009:34012,
		53010:34008,
		53011:34007,
		53012:34005,
		53013:34006,
		53014:34013,
		53015:34014,
		53016:34015,
		53017:34016,
		53018:34020,
		53019:34019,
		53020:34017,
		53021:34018,
		53022:34021,
		53023:34022,
		53024:34023,
		53025:34024,
		53218:34023,
		53219:34023,
		53220:34024,
		53221:34024,
		53222:34026,
		53223:34027,
		53224:34028,
		53225:34029,
		53226:34030,
		53227:34031,
		53228:34033,
		53229:34032,
		53230:34034,
		53231:34035,
		53232:34039,
		53233:34055,
		53234:34056,
		53235:34057,
		53236:34060,
		53237:34061,
		53238:34058,
		53239:34059,
		53240:34063,
		53241:34062,
		53256:34066,
		53242:34066,
		53243:34066,
		53244:34067,
		53245:34068,
		53246:34069,
		53247:34070,
		53248:34071,
		53249:34072,
		53250:34084,
		53251:34085,
		38200:34006,
		38201:34006,
	}

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = False
		if False == self.isLoaded:
			self._LoadScript()
		
	def _LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH_MALL_DESIGN_BY_LORDZIEGE + 'partials/itembox.py')
			else:
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH + 'partials/itembox.py')
		except:
			exception.Abort('Mall.ItemBox.LoadScriptFile')

		try:
			self._LoadVariables()
		except:
			exception.Abort('Mall.ItemBox.LoadVariables')

		try:
			self._LoadUi()
		except:
			exception.Abort('Mall.ItemBox.LoadUi')

		try:
			self._LoadEvents()
		except:
			exception.Abort('Mall.ItemBox.LoadEvents')
		
		self.isLoaded = True

	def _LoadVariables(self):
		self.item = None
		self.currencyTitle = None
		self.time = None
		self.lastTime = None
		self.wantToBuyText = "Would you like to buy {amount}x {itemName} for {price} {currencyTitle}?"
		self.toolTip = uiToolTip.ItemToolTip()
		self.itemBuyQuestionDialog = self.ItemBuyDialog()
		self.onBuyItemEvent = lambda *arg: None
		self.onPressPreviewEvent = lambda *arg: None

	def _LoadUi(self):
		self.elements = {
			'time_box' : self.GetChild('time_box'),
			'percent_box' : self.GetChild('percent_box'),
			'amount_box' : self.GetChild('amount_box'),
			'item_icon' : self.GetChild('item_icon'),

			'text' : {
				'percent' : self.GetChild('tx_percent'),
				'item_name' : self.GetChild('tx_item_name'),
				'item_price' : self.GetChild('tx_item_price'),
				'countdown' : self.GetChild('tx_countdown'),
				'item_amount' : self.GetChild('tx_item_amount_text'),
			},

			'edit_box' : {
				'amount' : self.GetChild('ed_amount'),
			},

			'buttons' : {
				'buy' : self.GetChild('btn_buy'),
				'preview' : self.GetChild('btn_preview'),
			}
		}
		
		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			self.elements['red_sale'] = self.GetChild('red_sale')
			self.elements['text']['item_currency'] = self.GetChild('tx_item_currency')
			self.elements['text']['item_name'].Hide()

	def _LoadEvents(self):
		self.elements['buttons']['buy'].SetEvent(self._OnClickBuy)
		self.elements['buttons']['preview'].SetEvent(self._OnClickPreview)
		self.elements['edit_box']['amount'].SetNumberMode()
		
		self.itemBuyQuestionDialog.SetAcceptEvent(lambda arg=TRUE: self.AnswerBuyItem(arg))
		self.itemBuyQuestionDialog.SetCancelEvent(lambda arg=FALSE: self.AnswerBuyItem(arg))
		self.elements['item_icon'].SAFE_SetStringEvent("MOUSE_OVER_IN",self.IconMouseOverIn)
		self.elements['item_icon'].SAFE_SetStringEvent("MOUSE_OVER_OUT",self.IconMouseOverOut)

	def SetOnBuyItemEvent(self, event):
		self.onBuyItemEvent = event

	def SetEscapeEvent(self, event):
		self.elements['edit_box']['amount'].SetEscapeEvent(event)

	def SetOnPressPreviewEvent(self, event):
		self.onPressPreviewEvent = event

	def SetData(self, itemData, currencyTitle):
		self.item = itemData
		self.currencyTitle = currencyTitle

		self.elements['time_box'].Hide()
		self.elements['buttons']['preview'].Hide()
		self.elements['percent_box'].Hide()
		self.elements['amount_box'].Hide()
		if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			self.elements['red_sale'].Hide()

		item.SelectItem(self.item.vnum)
		
		## item infos
		self.elements['text']['item_name'].SetText(item.GetItemName())
		try:
			self.elements['item_icon'].LoadImage(str(item.GetIconImageFileName()))
		except:
			import dbg
			dbg.TraceError("Mall.ItemBox.LoadImage")
		
		if not ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
			self.elements['item_icon'].SetScale(1, ([1, 0.6, 0.4])[item.GetItemSize()[1]-1])

		## amount
		if item.IsFlag(4) == 1:
			self.elements['amount_box'].Show()
		self.elements['edit_box']['amount'].SetText('1')
		self.elements['edit_box']['amount'].KillFocus()

		## percent
		if self.item.discountPercent == 0:
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				self.elements['text']['item_price'].SetText("%d" %self.item.price)
				self.elements['text']['item_currency'].SetText("%s" %self.currencyTitle)
			else:
				self.elements['text']['item_price'].SetText("%d %s" %(self.item.price, self.currencyTitle))
		else:
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				self.elements['text']['item_price'].SetText("%d" %(self.item.price-(self.item.price/100.00)*self.item.discountPercent))
				self.elements['text']['item_currency'].SetText("%s" %self.currencyTitle)
			else:		
				self.elements['text']['item_price'].SetText('%d %s' % (self.item.price-(self.item.price/100.00)*self.item.discountPercent, currencyTitle))

			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				self.elements['text']['percent'].SetText(ITEMBOX_DISCOUNT_TEXT_BY_LORD_ZIEGE %(self.item.discountPercent))
			else:
				self.elements['text']['percent'].SetText('%s%%'%self.item.discountPercent)
			self.elements['percent_box'].Show()
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				self.elements['red_sale'].Show()

		## time
		if (self.item.endDate - app.GetGlobalTimeStamp()) < 0:
			self.time = None
		else:
			self.elements['time_box'].Show()
			if ENABLE_MALL_DESIGN_BY_LORD_ZIEGE:
				self.elements['red_sale'].Show()
			self.time = self.item.endDate
			self.lastTime = 0
		
		if item.GetItemType() == item.ITEM_TYPE_WEAPON or (item.GetItemType() == item.ITEM_TYPE_ARMOR and item.GetItemSubType() == 0) or item.GetItemType() == item.ITEM_TYPE_COSTUME: ## fix by Poccix 
			showPreviewButton = True
			races = [
				[item.ITEM_ANTIFLAG_MALE, item.ITEM_ANTIFLAG_WARRIOR], ## WARRIOR_M
				[item.ITEM_ANTIFLAG_FEMALE, item.ITEM_ANTIFLAG_ASSASSIN], ## ASSASSIN_W
				[item.ITEM_ANTIFLAG_MALE, item.ITEM_ANTIFLAG_SURA], ## SURA_M
				[item.ITEM_ANTIFLAG_FEMALE, item.ITEM_ANTIFLAG_SHAMAN], ## SHAMAN_W
				[item.ITEM_ANTIFLAG_FEMALE, item.ITEM_ANTIFLAG_WARRIOR], ## WARRIOR_W
				[item.ITEM_ANTIFLAG_MALE, item.ITEM_ANTIFLAG_ASSASSIN], ## ASSASSIN_M
				[item.ITEM_ANTIFLAG_FEMALE, item.ITEM_ANTIFLAG_SURA], ## SURA_W
				[item.ITEM_ANTIFLAG_MALE, item.ITEM_ANTIFLAG_SHAMAN], ## SHAMAN_M
			]

			if ENABLE_WOLFMAN_CHARACTER:
				races.append([item.ITEM_ANTIFLAG_WOLFMAN, item.ITEM_ANTIFLAG_WOLFMAN]) ## WOLFMAN

			#for antiflag in races[player.GetRace()]:
				#if item.IsAntiFlag(antiflag):
					#showPreviewButton = False
			if showPreviewButton:
				self.elements['buttons']['preview'].Show()
		else:
			if self.item.vnum in self.rideVnums or self.item.vnum in self.petVnums:
				self.elements['buttons']['preview'].Show()

	def SetWantToBuyText(self, text):
		self.wantToBuyText = text

	def SetButtonPreviewText(self, text):
		self.elements['buttons']['preview'].SetText(text)

	def SetButtonBuyText(self, text):
		self.elements['buttons']['buy'].SetText(text)
		
	def SetItemAmountText(self, text):
		self.elements['text']['item_amount'].SetText(text)

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def Destroy(self):
		self.Hide()

	def OnUpdate(self):
		self.RunAmountCorrection()
		self.RunItemBoxCounter()

	def IconMouseOverIn(self):
		self.toolTip.ClearToolTip()
		item.SelectItem(self.item.vnum)
		attrSlot = [(self.item.attrtype0, self.item.attrvalue0),(self.item.attrtype1, self.item.attrvalue1),(self.item.attrtype2, self.item.attrvalue2),(self.item.attrtype3, self.item.attrvalue3),(self.item.attrtype4, self.item.attrvalue4),(self.item.attrtype5, self.item.attrvalue5),(self.item.attrtype6, self.item.attrvalue6)]
		attrSlot += [(0,0) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM-len(attrSlot))]
		
		## if item is real time (limit type) then calculate the time
		if item.GetLimit(0)[0] == 7:
			socket0 = self.item.socket0 + app.GetGlobalTimeStamp()
		else:
			socket0 = self.item.socket0
		if DISABLE_TOOLTIP_RENDER_TARGET:
			self.toolTip.AddItemData(itemVnum = self.item.vnum, metinSlot = [socket0, self.item.socket1, self.item.socket2, self.item.socket3, self.item.socket4, self.item.socket5], attrSlot = attrSlot, enableRenderTarget = False)
		else:
			self.toolTip.AddItemData(self.item.vnum, [socket0, self.item.socket1, self.item.socket2, self.item.socket3, self.item.socket4, self.item.socket5], attrSlot)
		
	def IconMouseOverOut(self):
		self.toolTip.Hide()

	def _OnClickBuy(self):
		if not self.elements['edit_box']['amount'].GetText():
			self.elements['edit_box']['amount'].SetText('1')

		self.elements['edit_box']['amount'].KillFocus()
		amount = self.elements['edit_box']['amount'].GetText()
		price = int(amount) * int(self.elements['text']['item_price'].GetText().split(' ')[0])

		buy = {
			'amount': amount, 
			'itemName': self.elements['text']['item_name'].GetText(), 
			'price' : price, 
			'currencyTitle': self.currencyTitle
		}

		self.itemBuyQuestionDialog.SetText(self.wantToBuyText.format(**buy))
		self.itemBuyQuestionDialog.Open()

	def _OnClickPreview(self):
		if self.onPressPreviewEvent:
			if self.item.vnum in self.petVnums:
				self.onPressPreviewEvent(self.petVnums[self.item.vnum], True)
				#chat.AppendChat(chat.CHAT_TYPE_INFO, "Estoy visualizando un pet")
			elif self.item.vnum in self.rideVnums:
				self.onPressPreviewEvent(self.rideVnums[self.item.vnum], True)
				#chat.AppendChat(chat.CHAT_TYPE_INFO, "Estoy visualizando una montura")
			else:
				self.onPressPreviewEvent(self.item.vnum)

	def AnswerBuyItem(self, arg):
		self.itemBuyQuestionDialog.Close()
		if arg == 1:
			if not self.elements['edit_box']['amount'].GetText():
				self.elements['edit_box']['amount'].SetText('1')
			amount = self.elements['edit_box']['amount'].GetText()

			if self.onBuyItemEvent:
				self.onBuyItemEvent(self.item.id, int(amount))
		
	def SendChat(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, '<Mall Itembox>: '+str(text))

	def RunAmountCorrection(self):
		## if the first character of amount is a 0 or its empty and not focused, then set the amount to 1
		amount = self.elements['edit_box']['amount'].GetText()
		if amount:
			if amount[0] == '0':
				print('settext1')
				self.elements['edit_box']['amount'].SetText('1')
			if int(amount) > 200:
				print('settext200')
				self.elements['edit_box']['amount'].SetText('200')
		else:
			if not self.elements['edit_box']['amount'].IsFocus():
				self.elements['edit_box']['amount'].SetText('1')

	def RunItemBoxCounter(self):
		if self.time:
			remaining = self.time - app.GetGlobalTimeStamp()
			if self.lastTime < time.clock():
				if remaining <= 0:
					self.time = None

					if not self.item.buyableAfterRunOut:
						self.elements['text']['countdown'].SetText('0h 0m 0s')
						self.elements['buttons']['buy'].Disable()
					else:
						self.elements['time_box'].Hide()
						self.elements['percent_box'].Hide()
						self.elements['text']['item_price'].SetText("%d %s" %(self.item.price, self.currencyTitle))
					return
					
				self.lastTime = time.clock() + 1
				hoursRemaining = int(remaining) / 3600
				minutesRemaining = int(remaining % 3600) / 60
				secondsRemaining = int(remaining % 60)
				self.elements['text']['countdown'].SetText('%dh %dm %ds' % (hoursRemaining, minutesRemaining, secondsRemaining))

	class ItemBuyDialog(ui.ScriptWindow):
		def __init__(self):
			ui.ScriptWindow.__init__(self)
			self.isLoaded = False
			if False == self.isLoaded:
				self._LoadScript()

		def __del__(self):
			ui.ScriptWindow.__del__(self)

		def _LoadScript(self):
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, UISCRIPT_PATH+'partials/questiondialog.py')
			except:
				exception.Abort('Mall.ItemBuyDialog.LoadScriptFile')

			try:
				self._LoadUi()
			except:
				exception.Abort('Mall.ItemBuyDialog.LoadUi')

			self.isLoaded = True
			
		def _LoadUi(self):
			self.elements = {
				'board' : self.GetChild('board'),
				'text_line' : self.GetChild('message'),
				'accept_button' : self.GetChild('accept'),
				'cancel_button' : self.GetChild('cancel')
			}

		def Open(self):
			self.SetCenterPosition()
			self.SetTop()
			self.Show()

		def Close(self):
			self.Hide()

		def SetWidth(self, width):
			height = self.GetHeight()
			self.SetSize(width, height)
			self.elements['board'].SetSize(width, height)
			self.SetCenterPosition()
			self.UpdateRect()

		def SAFE_SetAcceptEvent(self, event):
			self.elements['accept_button'].SAFE_SetEvent(event)

		def SAFE_SetCancelEvent(self, event):
			self.elements['cancel_button'].SAFE_SetEvent(event)

		def SetAcceptEvent(self, event):
			self.elements['accept_button'].SetEvent(event)

		def SetCancelEvent(self, event):
			self.elements['cancel_button'].SetEvent(event)

		def SetText(self, text):
			self.elements['text_line'].SetText(text)

		def SetAcceptText(self, text):
			self.elements['accept_button'].SetText(text)

		def SetCancelText(self, text):
			self.elements['cancel_button'].SetText(text)

		def OnPressEscapeKey(self):
			self.Close()
			return TRUE

class Nav(object):
	def __init__(self):
		self.curPos = 0
		self.itemMax = 0
		self.itemLen = 0
		self.scrollStep = 1
		self.onChangeEvent = None

	def SetPlusButton(self, button):
		self.plusButton = button
		self.plusButton.SetEvent(self._OnPressPlus)

	def SetMinusButton(self, button):
		self.minusButton = button
		self.minusButton.SetEvent(self._OnPressMinus)

	def SetScrollStep(self, step):
		self.scrollStep = int(step)

	def SetOnChangeEvent(self, event):
		self.onChangeEvent = event

	def SetItemMax(self, value):
		self.itemMax = value
		self._RefreshNavButtonsVisibility()

	def GetItemMax(self):
		return self.itemMax

	def SetItemLen(self, value):
		self.itemLen = value
		self._RefreshNavButtonsVisibility()

	def GetItemLen(self):
		return self.itemLen

	def GetMaxPage(self):
		return int(math.ceil(float(self.GetItemLen())/float(self.GetItemMax())))

	def GetCurPage(self):
		return int(math.ceil(float(self.GetPos())/float(self.GetItemMax())))+1

	def SetCurPage(self, page):
		self.SetPos(self.GetItemMax()*(page-1))

	def SendSystemChat(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, "<Mall>: " + str(text))
	
	def _OnPressPlus(self):
		self.SetPos(self.curPos+self.scrollStep)

	def _OnPressMinus(self):
		self.SetPos(self.curPos-self.scrollStep)

	def GetPos(self):
		return self.curPos

	def SetPos(self, pos):
		self.curPos = pos
		self._OnPress()
	
	def _RefreshNavButtonsVisibility(self):
		if self.minusButton and self.plusButton:
			if self.itemLen > self.itemMax:
				if (self.curPos <= 0):
					self.plusButton.Show()
					self.minusButton.Hide()
				elif (self.curPos+self.itemMax < self.itemLen):
					self.plusButton.Show()
					self.minusButton.Show()
				elif (self.curPos+self.itemMax >= self.itemLen):
					self.plusButton.Hide()
					self.minusButton.Show()
			else:
				self.minusButton.Hide()
				self.plusButton.Hide()

	def _OnPress(self):
		self._RefreshNavButtonsVisibility()
		if self.onChangeEvent:
			self.onChangeEvent()

class LoadingBar(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = False
		if False == self.isLoaded:
			self._LoadScript()

	def _LoadScript(self):
		## Load gui
		try: 
			self.BindObjects()
		except:
			exception.Abort('Mall.LoadingBar._LoadScript.BindObjects')
			
		self.isLoaded = True
		
	## Bind the objects in _LoadScript
	def BindObjects(self):
		self.board = ui.ThinBoard()
		self.board.SetSize(260,30)
		self.board.SetCenterPosition()
		self.board.Hide()

		self.progressBarActualFile = ui.AniImageBox()
		self.progressBarActualFile.SetParent(self.board)
		self.progressBarActualFile.AppendImage(IMG_PATH + 'loading_bar.tga')
		self.progressBarActualFile.SetPosition(5, 8)
		self.progressBarActualFile.SetDelay(90)
		self.progressBarActualFile.SetPercentage(0, 100)
		self.progressBarActualFile.Show()

		self.lb_percent = ui.TextLine()
		self.lb_percent.SetParent(self.board)
		self.lb_percent.SetPosition(225,9)
		self.lb_percent.SetText('0%')
		self.lb_percent.Show()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	## Hide the Gui
	def Destroy(self):
		self.Hide()

	def SetPercent(self, percent):
		percent = int(percent)
		if percent >= 100:
			self.board.Hide()
		else:
			self.board.SetTop()
			self.board.Show()
			self.progressBarActualFile.SetPercentage(percent, 100)
			self.lb_percent.SetText(str(percent) +'%')

	## Close the gui
	def Close(self):
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

	def OnPressExitKey(self):
		self.Close()
		return TRUE

import net
import event
import chat

class ClientQuestCommunication(object):
	def __init__(self):
		self.__chat_debug = 0
		self.__arg_seperator = ','
		self.__space_replacer = '#'
		
		self.__tmp_command = ''
		
		self.__quest_cmd = ''
		self.__quest_id = 0
		
		self.__commands = {
			'SET_QUEST_ID' : self.__SetQuestId,
			'GET_QUEST_CMD' : self.__GetQuestCmd,
		}
		
	def ClearTmpCommand(self):
		self.__tmp_command = ''
		
	def EnableChatDebug(self):
		self.__chat_debug = 1
		
	def SetArgSeperator(self, seperator):
		self.__arg_seperator = seperator
		
	def SetSpaceReplace(self, replacer):
		self.__space_replacer = replacer
		
	def AddCommand(self, cmd, event):
		self.__commands[cmd] = event
			
	def AddCommandDict(self, cmds):
		self.__commands.update(cmds)
		
	def __SetQuestId(self, quest_id):
		self.__quest_id = int(quest_id)
		
	def __GetQuestCmd(self):
		if self.__chat_debug:
			chat.AppendChat(chat.CHAT_TYPE_INFO, 'DEBUG: SEND COMMAND TO QUEST %d -> %s'%(self.__quest_id, self.__quest_cmd))
			
		net.SendQuestInputStringPacket(self.__quest_cmd)
		self.__quest_cmd = ''
		
	def SendQuestCommand(self, cmd):
		if self.__chat_debug:
			chat.AppendChat(chat.CHAT_TYPE_INFO, 'DEBUG: SEND CLICK TO QUEST %d'%self.__quest_id)
			
		self.__quest_cmd = cmd
		event.QuestButtonClick(self.__quest_id)
		
	def ReceiveQuestCommand(self, quest_command):
		if self.__chat_debug:
			chat.AppendChat(chat.CHAT_TYPE_INFO, 'DEBUG: RECEIVE COMMAND %s'%quest_command)
			
		self.__tmp_command += quest_command

		close_pos = self.__tmp_command.find(')')

		if close_pos != -1:
			open_pos = self.__tmp_command.find('(')
			
			command = self.__tmp_command[:open_pos]
			args = self.__tmp_command[open_pos+1:close_pos].replace(self.__space_replacer,' ').split(self.__arg_seperator)
			
			if self.__chat_debug:
				chat.AppendChat(chat.CHAT_TYPE_INFO, 'DEBUG: TRY TO RUN %s'%self.__tmp_command)
				
			self.__tmp_command = ''
			
			if command in self.__commands:
				if args[0]:
					self.__commands[command](*args)
				else:
					self.__commands[command]()
			else:
				if self.__chat_debug:
					chat.AppendChat(chat.CHAT_TYPE_INFO, 'DEBUG: COMMAND %s IS NOT FOUND IN COMMAND LIST'%command)
				print('ClientQuestCommunication() command %s is not found in command list'%command)

wnd = MallWindow()