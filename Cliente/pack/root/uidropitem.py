import ui
import wndMgr
import app
import grp
import item
import ime
import drop_item
import chat
import localeInfo
import constInfo
import renderTarget

ITEM_LIST=item.GetNames()

class DropItemWindows(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		drop_item.SetDropItemHandler(self)

		self.RENDER_TARGET_INDEX = 4

		self.total_dates = -1
		self.pages_view = drop_item.GetPageView()
		drop_item.SetPageSelect(1)

		self.select_button = []

		self.get_vnum = -1

		self.LoadWindow()

	def __del__(self):
		drop_item.SetDropItemHandler(None)
		ui.ScriptWindow.__init__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/dropitem.py")
		except:
			import exception
			exception.Abort("DropItemWindows.LoadWindow.LoadObject")

		try:
			self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))

			self.ItemNameValue = self.GetChild("ItemNameValue")
			self.ButtonSearch = self.GetChild("ButtonSearch")
			self.ButtonDelete = self.GetChild("ButtonDelete")
			self.ModelRender = self.GetChild("ModelRender")
			self.ModelName1 = self.GetChild("ModelName1")
			self.ModelName2 = self.GetChild("ModelName2")
			self.PrevButton = self.GetChild("PrevButton")
			self.FirstPrevButton = self.GetChild("FirstPrevButton")
			self.NextButton = self.GetChild("NextButton")
			self.LastNextButton = self.GetChild("LastNextButton")
			self.TotalPages = self.GetChild("TotalPages")
			self.TotalResultados = self.GetChild("TotalResultados")
			self.ImgSlotTotalPages = self.GetChild("ImgSlotTotalPages")

			self.ModelName1 = self.GetChild("ModelName1")
			self.ModelName2 = self.GetChild("ModelName2")

			self.ButtonsSelect = {}
			for i in xrange(0,5):
				self.ButtonsSelect["NameMob_%d"%i] 			= self.GetChild("NameMob_%d"%i)
				self.ButtonsSelect["Cantidad_Item_%d"%i] 	= self.GetChild("Cantidad_Item_%d"%i)
				self.ButtonsSelect["Porcentaje_Item_%d"%i] 	= self.GetChild("Porcentaje_Item_%d"%i)

				self.ButtonsSelect["Button_Select_%d"%i] 	= self.GetChild("SelectOption_%d"%i)
				self.ButtonsSelect["Button_Select_%d"%i].Hide()
		except:
			import exception
			exception.Abort("DropItemWindows.Elements.LoadObject")

		self.DropDownList = DropDown(self)
		self.DropDownList.OnChange = self.SelectOption
		self.DropDownList.SetPosition(15,60)
		self.DropDownList.Hide()

		self.ItemNameValue.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdate)
		self.ItemNameValue.SetReturnEvent(ui.__mem_func__(self.__OnHideList))
		self.ItemNameValue.SetEscapeEvent(ui.__mem_func__(self.__OnHideList))


		self.PrevButton.SetEvent(self.__OnClickPage, 'MOSTBOUGHT_RIGHT')
		self.FirstPrevButton.SetEvent(self.__OnClickPage, 'MOSTBOUGHT_RIGHT_LAST')
		self.NextButton.SetEvent(self.__OnClickPage, 'MOSTBOUGHT_LEFT')
		self.LastNextButton.SetEvent(self.__OnClickPage, 'MOSTBOUGHT_LEFT_LAST')

		self.ButtonSearch.SetEvent(self.SearchItem)
		self.ButtonDelete.SetEvent(self.DeleteItem)

		self.PrevButton.Hide()
		self.FirstPrevButton.Hide()
		self.NextButton.Hide()
		self.LastNextButton.Hide()
		self.TotalResultados.Hide()
		self.ImgSlotTotalPages.Hide()

		self.ModelRender.Hide()

	def SearchItem(self):
		if self.get_vnum == -1:
			return

		drop_item.SearchItem(self.get_vnum)

	def DeleteItem(self):
		self.Clear()

	def BINARY_DROP_ITEM_LOADING(self):
		self.select_button = []
		drop_item.SetPageSelect(1)
		self.LoadInfo()
		self.TotalResultados.SetText(localeInfo.SEARCH_ITEM_DROP_TOTAL % (self.total_dates))
		self.TotalResultados.Show()
		self.ImgSlotTotalPages.Show()

	def LoadInfo(self):
		self.total_dates = drop_item.CountDates()
		self.page_actual = drop_item.GetPageActual()

		for i in xrange(0,5):
			self.ButtonsSelect["NameMob_%d"%i].SetText("")
			self.ButtonsSelect["Cantidad_Item_%d"%i].SetText("")
			self.ButtonsSelect["Porcentaje_Item_%d"%i].SetText("")
			self.ButtonsSelect["Button_Select_%d"%i].Hide()
			self.ButtonsSelect["Button_Select_%d"%i].SetUp()

		for i in xrange(min(self.pages_view, self.total_dates - self.page_actual * self.pages_view +self.pages_view)):
			index = i + (self.page_actual - 1)*self.pages_view
			self.ButtonsSelect["NameMob_%d"%i].SetText(self.FReturnInfo("name_mob",index))

			if self.FReturnInfo("count_item",index) == 0:
				self.ButtonsSelect["Cantidad_Item_%d"%i].SetText(localeInfo.SEARCH_ITEM_DROP_COUNT+" +1")
			else:
				self.ButtonsSelect["Cantidad_Item_%d"%i].SetText(localeInfo.SEARCH_ITEM_DROP_COUNT+" x%d"%(self.FReturnInfo("count_item",index)))

			self.ButtonsSelect["Porcentaje_Item_%d"%i].SetText(localeInfo.SEARCH_ITEM_DROP_PERCENT+" %d"%(self.ConvertProb(index))+"%")
			self.ButtonsSelect["Button_Select_%d"%i].SetEvent(self.FuncSelect,index,i)
			self.ButtonsSelect["Button_Select_%d"%i].Show()

			self.CheckRadioButton(index,i)

		Left_Last = self.WFunction(float(self.total_dates)/float(self.pages_view))
		self.TotalPages.SetText("%d/%d"%(self.page_actual,Left_Last))

		if self.page_actual * self.pages_view >= self.total_dates:
			self.NextButton.Hide()
			self.LastNextButton.Hide()
		else:
			self.NextButton.Show()
			self.LastNextButton.Show()

		if self.page_actual > 1:
			self.PrevButton.Show()
			self.FirstPrevButton.Show()
		else:
			self.PrevButton.Hide()
			self.FirstPrevButton.Hide()


	def CheckRadioButton(self, index1, index2):
	 	if len(self.select_button) >0 :	
			if self.select_button[0][0] == index1 and self.select_button[0][1] == index2:
				self.__ClickRadioButton(index2)

	def FuncSelect(self, index1, index2):
		renderTarget.SetBackground(self.RENDER_TARGET_INDEX, "d:/ymir work/ui/game/myshop_deco/model_view_bg.sub")
		renderTarget.SetVisibility(self.RENDER_TARGET_INDEX, True)
		renderTarget.SelectModel(self.RENDER_TARGET_INDEX, self.FReturnInfo("vnum_mob",index1))
		#if app.ENABLE_RENDER_ZOOM:
		#	renderTarget.SetZoom(self.RENDER_TARGET_INDEX, float(1900))
		self.ModelRender.Show()

		self.select_button = []

		self.select_button.append([index1,index2])

		self.ModelName1.SetText(self.FReturnInfo("name_mob",index1))
		self.ModelName2.SetText(self.FReturnInfo("name_mob",index1))

		self.__ClickRadioButton(index2)

	def __ClickRadioButton(self, buttonIndex):
		selButton=self.ButtonsSelect["Button_Select_%d"%buttonIndex]

		for i in xrange(0,5):
			self.ButtonsSelect["Button_Select_%d"%i].SetUp()

		selButton.Down()


	def __OnClickPage(self, func):
		Left_Last = self.WFunction(float(self.total_dates)/float(self.pages_view))

		if func == 'MOSTBOUGHT_LEFT':
			drop_item.SetPageSelect(self.page_actual+1)
			self.LoadInfo()
		elif func == 'MOSTBOUGHT_LEFT_LAST':
			drop_item.SetPageSelect(Left_Last)
			self.LoadInfo()
		elif func == 'MOSTBOUGHT_RIGHT':
			drop_item.SetPageSelect(self.page_actual-1)
			self.LoadInfo()
		elif func == 'MOSTBOUGHT_RIGHT_LAST':
			drop_item.SetPageSelect(1)
			self.LoadInfo()

	def __OnValueUpdate(self):
		ui.EditLine.OnIMEUpdate(self.ItemNameValue)
		get_text=self.ItemNameValue.GetText()
		if len(get_text)>0:
			self.DropDownList.Clear()
			f=0
			n=[]
			for it in ITEM_LIST:
				vnum,name=it["vnum"],it["name"]
				if f==10:
					break
				if len(name)>=len(get_text) and name[:len(get_text)].lower() == get_text.lower():
					self.DropDownList.AppendItem(name,vnum)
					f+=1

			if f>0:
				if self.DropDownList.dropped==0:
					self.DropDownList.Clear()
					self.DropDownList.ExpandMe()
				self.DropDownList.Show()
				return

		self.__OnHideList()

	def __OnHideList(self):
		self.DropDownList.dropped=0
		self.DropDownList.Clear()
		self.DropDownList.Hide()

	def SelectOption(self):
		self.get_vnum=self.DropDownList.DropList.GetSelectedItem().value
		name= ""
		for it in ITEM_LIST:
			if int(it["vnum"]) == self.get_vnum:
				name=it["name"]
				break

		self.ItemNameValue.SetText(str(name))
		self.DropDownList.Clear()
		self.DropDownList.Hide()
		ime.SetCursorPosition(len(name)+1)

	def ConvertProb(self,index):
		if self.FReturnInfo("prob_item",index) <= 0:
			return 1

		if self.FReturnInfo("prob_item",index) >= 100:
			return 100

		return self.FReturnInfo("prob_item",index)

	def FReturnInfo(self,func,index):
		(name_mob,vnum_mob,prob_item,count_item) = drop_item.GetDates(index)
		info = {
			"name_mob" : name_mob,
			"vnum_mob" : vnum_mob,
			"prob_item" : prob_item,
			"count_item": count_item
		}

		return info[func]


	def WFunction(self, num):
		if (num + 1) != int(num+1):
			return int(num+1)
		else:
			return int(num)

	def Clear(self):
		self.total_dates = -1
		self.pages_view = drop_item.GetPageView()
		drop_item.SetPageSelect(1)
		self.select_button = []
		self.get_vnum = -1


		self.PrevButton.Hide()
		self.FirstPrevButton.Hide()
		self.NextButton.Hide()
		self.LastNextButton.Hide()

		self.TotalResultados.Hide()

		self.ImgSlotTotalPages.Hide()

		self.__OnHideList()

		self.ItemNameValue.SetText("")

		self.ModelName1.SetText("")
		self.ModelName2.SetText("")

		for i in xrange(0,5):
			self.ButtonsSelect["NameMob_%d"%i].SetText("")
			self.ButtonsSelect["Cantidad_Item_%d"%i].SetText("")
			self.ButtonsSelect["Porcentaje_Item_%d"%i].SetText("")
			self.ButtonsSelect["Button_Select_%d"%i].Hide()
			self.ButtonsSelect["Button_Select_%d"%i].SetUp()

		renderTarget.SetVisibility(self.RENDER_TARGET_INDEX, False)
		self.ModelRender.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		wndMgr.Hide(self.hWnd)
		return True

	def Close(self):
		self.Clear()
		wndMgr.Hide(self.hWnd)


class DropDown(ui.Window):
	dropped  = 0
	dropstat = 0
	width = 0
	height = 0
	maxh = 30
	OnChange = None
	class Item(ui.Window):
		TEMPORARY_PLACE = 0
		width = 0
		height = 0
		def __init__(self,parent, text,value=0,skill=0):
			ui.Window.__init__(self)
			self.textBox=ui.MakeTextLine(self)
			self.textBox.SetText(text)
			self.value = int(value)
			self.skill = int(skill)

		def __del__(self):
			ui.Window.__del__(self)

		def SetParent(self, parent):
			ui.Window.SetParent(self, parent)
			self.parent=parent

		def OnMouseLeftButtonDown(self):
			self.parent.SelectItem(self)

		def SetSize(self,w,h):
			w = w-110
			ui.Window.SetSize(self,w,h)
			self.width = w
			self.height = h
		def OnUpdate(self):	
			if self.IsIn():
				self.isOver = True
			else:
				self.isOver = False
		def OnRender(self):
			xRender, yRender = self.GetGlobalPosition()
			yRender -= self.TEMPORARY_PLACE
			widthRender = self.width
			heightRender = self.height + self.TEMPORARY_PLACE*2
			grp.SetColor(ui.BACKGROUND_COLOR)
			grp.RenderBar(xRender, yRender, widthRender, heightRender)
			grp.SetColor(ui.DARK_COLOR)
			grp.RenderLine(xRender, yRender, widthRender, 0)
			grp.RenderLine(xRender, yRender, 0, heightRender)
			grp.SetColor(ui.BRIGHT_COLOR)
			grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
			grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

			if self.isOver:
				grp.SetColor(ui.HALF_WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + 3, self.width - 3, heightRender - 5)

	def __init__(self,parent):
		ui.Window.__init__(self,"TOP_MOST")
		self.down = 1
		self.parent=parent

		self.DropList = ui.ListBoxEx()
		self.DropList.SetParent(self)
		self.DropList.itemHeight = 20
		self.DropList.itemWidth = 220
		self.DropList.itemStep = 18
		self.DropList.SetPosition(0,0)
		self.DropList.SetSize(200,2) 
		self.DropList.SetSelectEvent(self.SetTitle)
		self.DropList.SetViewItemCount(0)
		self.DropList.Show()
		self.selected = self.DropList.GetSelectedItem()
		
		self.SetSize(220-110,95)

	def __del__(self): 
		ui.Window.__del__(self)

	def AppendItem(self,text,value=0,skill=0):
		self.DropList.AppendItem(self.Item(self,text,value,skill))

	def OnPressEscapeKey(self):
		self.Hide()
		self.Clear()

	def SetTitle(self,item):
		self.dropped = 0
		self.selected = item
		if self.OnChange:
			self.OnChange()
		self.Clear()

	def SetSize(self,w,h):
		ui.Window.SetSize(self,w,h+10)
		self.width = w
		self.height = h
		self.DropList.SetSize(w,h)

	def Clear(self):
		for x in self.DropList.itemList:
			x.Hide()
		self.DropList.RemoveAllItems()

	def ExpandMe(self):
		if self.dropped == 1:
			self.dropped = 0
		else:
			self.dropped = 1

	def OnUpdate(self):
		(w,h) = self.parent.GetLocalPosition()
		self.maxh =self.DropList.itemStep*len(self.DropList.itemList)
		self.SetPosition(w+15,h+75)
		if self.dropped == 0 or not self.parent.IsShow() or len(self.DropList.itemList)==0:
			self.SetSize(self.GetWidth(),0)
			self.DropList.SetViewItemCount(0)
			self.Hide()
		elif self.dropped == 1:
			self.Show()
			self.SetTop()
			height = self.maxh+5 if int(self.maxh/self.DropList.itemStep) <2 else self.maxh
			self.SetSize(self.GetWidth(),height)
			self.DropList.SetViewItemCount(self.maxh/self.DropList.itemStep)
