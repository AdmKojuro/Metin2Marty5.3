import app
import ui
import localeInfo
import uiScriptLocale
import wndMgr
import net
import calendar
import datetime
import eventMgr
import uiToolTip
import time

SKILL_SLOT_ENABLE	= "d:/ymir work/ui/pet/skill_button/skill_enable_button.sub"
CALENDAR_LENGHT		= 7

SATURDAY			= 6
SUNDAY				= 7

DEFAULT_DAY			= 1
DEFAULT_WEEK		= 1
WEEK_DAY_COUNT		= 7

class KingdomWarScoreBoard(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadScript()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def __LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/KingdomWarScoreBoard.py")

			self.ShinsooScoreText = self.GetChild("ShinsooScoreText")
			self.ChunjoScoreText = self.GetChild("ChunjoScoreText")
			self.JinnoScoreText = self.GetChild("JinnoScoreText")

		except:
			import exception
			exception.Abort("KingdomWarScoreBoard.__LoadScript.UIScript/KingdomWarScoreBoard.py")
			
	def UpdateScore(self, score_shinsoo, score_chunjo, score_jinno):
		self.ShinsooScoreText.SetText(localeInfo.EMPIRE_A + (":\t%d" % score_shinsoo))
		self.ChunjoScoreText.SetText(localeInfo.EMPIRE_B + (":\t%d" % score_chunjo))
		self.JinnoScoreText.SetText(localeInfo.EMPIRE_C + (":\t%d" % score_jinno))
			
class EventRewardList(ui.ScriptWindow):

	def __init__(self, OverviewWindow):
		ui.ScriptWindow.__init__(self)
		self.__LoadScript()
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.eventType = 0
		self.wndOverviewEvent = OverviewWindow
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		self.tooltipItem = None
		self.eventType = 0
		
	def __LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/EventRewardListWindow.py")

			self.eventButton = self.GetChild("EventButton")
			self.rewardItemSlot = self.GetChild("reward_item_slot")
			self.eventDurationText = self.GetChild("EventDuration")
			
			self.eventButton.SetEvent(ui.__mem_func__(self.OpenEventDescription))
			self.rewardItemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			self.rewardItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		except:
			import exception
			exception.Abort("EventRewardList.__LoadScript.UIScript/EventRewardListWindow.py")
	
	def SetEventText(self, text):
		self.eventDurationText.SetText(text)
		
	def SetEventButtonText(self, text):
		self.eventButton.SetText(text)
		
	def SetRewardItemSlot(self, event):
		self.eventType = event
		
		for i in range(eventMgr.REWARD_MAX_NUM):
			(itemVnum, itemCount) = eventMgr.GetEventRewardData(event, i)
			
			if itemVnum > 0 and itemCount > 0:
				self.rewardItemSlot.SetItemSlot(i, itemVnum, itemCount)
			else:
				self.rewardItemSlot.ClearSlot(i)
				self.rewardItemSlot.SetCoverButton(i, SKILL_SLOT_ENABLE, SKILL_SLOT_ENABLE, SKILL_SLOT_ENABLE, SKILL_SLOT_ENABLE, False, False)
				self.rewardItemSlot.SetAlwaysRenderCoverButton(i)
				
	def OpenEventDescription(self):
		type = self.eventType
		
		if type == eventMgr.EVENT_TYPE_BOSS:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("boss_event")
			
		elif type == eventMgr.EVENT_TYPE_FISHING:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("fishing_event")
			
		elif type == eventMgr.EVENT_TYPE_GOLD_FROG:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("golden_frog_event")
			
		elif type == eventMgr.EVENT_TYPE_HEXEGONAL_CHEST:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("hexegonal_drop_event")
			
		elif type == eventMgr.EVENT_TYPE_HIDE_AND_SEEK:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("hide_seek_event")
			
		elif type == eventMgr.EVENT_TYPE_METIN:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("metin_event")
			
		elif type == eventMgr.EVENT_TYPE_MINING:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("mining_event")
			
		elif type == eventMgr.EVENT_TYPE_MOONLIGHT:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("moonlight_drop_event")
			
		elif type == eventMgr.EVENT_TYPE_OX:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("oxevent_manager")
			
		elif type == eventMgr.EVENT_TYPE_TANAKA:
			self.wndOverviewEvent.Close()
			net.SendRequestEventQuest("tanaka_event")

	def OverOutItem(self):
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()
			
	def OverInItem(self, slot):
		if None != self.tooltipItem:
			(itemVnum, itemCount) = eventMgr.GetEventRewardData(self.eventType, slot)
			if itemVnum:
				self.tooltipItem.ClearToolTip()
				self.tooltipItem.AddItemData(itemVnum, 0)

class EventOverview(ui.ScriptWindow):

	DAY_NAME_DICT = {
		0 : localeInfo.TOOLTIP_EVENT_MONDAY,
		1 : localeInfo.TOOLTIP_EVENT_TUESDAY,
		2 : localeInfo.TOOLTIP_EVENT_WEDNESDAY,
		3 : localeInfo.TOOLTIP_EVENT_THURSDAY,
		4 : localeInfo.TOOLTIP_EVENT_FRIDAY,
		5 : localeInfo.TOOLTIP_EVENT_SATURDAY,
		6 : localeInfo.TOOLTIP_EVENT_SUNDAY,
	}
	
	EVENT_NAME_DICT = {
		eventMgr.EVENT_TYPE_EXPERIENCE : localeInfo.TOOLTIP_EVENT_TYPE_EXPERIENCE,
		eventMgr.EVENT_TYPE_ITEM_DROP : localeInfo.TOOLTIP_EVENT_TYPE_ITEM_DROP,
		eventMgr.EVENT_TYPE_BOSS : localeInfo.TOOLTIP_EVENT_TYPE_BOSS,
		eventMgr.EVENT_TYPE_METIN : localeInfo.TOOLTIP_EVENT_TYPE_METIN,
		eventMgr.EVENT_TYPE_MINING : localeInfo.TOOLTIP_EVENT_TYPE_MINING,
		eventMgr.EVENT_TYPE_GOLD_FROG : localeInfo.TOOLTIP_EVENT_TYPE_GOLD_FROG,
		eventMgr.EVENT_TYPE_MOONLIGHT : localeInfo.TOOLTIP_EVENT_TYPE_MOONLIGHT,
		eventMgr.EVENT_TYPE_HEXEGONAL_CHEST : localeInfo.TOOLTIP_EVENT_TYPE_HEXEGONAL_CHEST,
		eventMgr.EVENT_TYPE_FISHING : localeInfo.TOOLTIP_EVENT_TYPE_FISHING,
		eventMgr.EVENT_TYPE_HIDE_AND_SEEK : localeInfo.TOOLTIP_EVENT_TYPE_HIDE_AND_SEEK,
		eventMgr.EVENT_TYPE_OX : localeInfo.TOOLTIP_EVENT_TYPE_OX,
		eventMgr.EVENT_TYPE_TANAKA : localeInfo.TOOLTIP_EVENT_TYPE_TANAKA,
		eventMgr.EVENT_TYPE_SIEGE_WAR : localeInfo.TOOLTIP_EVENT_TYPE_SIEGE_WAR,
		eventMgr.EVENT_TYPE_KINGDOM_WAR : localeInfo.TOOLTIP_EVENT_TYPE_KINGDOM_WAR,
	}
	
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		
		self.toolTip = uiToolTip.ToolTip()
		
		self.bannerDict = {}
		self.weekendDict = {}
		self.weekDict = {}
		self.activeEventDict = {}
		self.updateTime = 0
		
		self.__BuildCalendar()
		self.calendarWeek = self.__GetCurrentMonthWeek()
		
		self.__LoadScript()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def __LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/EventOverviewWindow.py")
			
			self.board = self.GetChild("board")
			self.calendarEffect = self.GetChild("calendar_effect")
			self.eventRewardBar = self.GetChild("EventRewardBarWindow")
			
			self.bannerDict = {
				1	:	self.GetChild("calendar_01"),
				2	:	self.GetChild("calendar_02"),
				3	:	self.GetChild("calendar_03"),
				4	:	self.GetChild("calendar_04"),
				5	:	self.GetChild("calendar_05"),
				6	:	self.GetChild("calendar_06"),
				7	:	self.GetChild("calendar_07"),
			}
			
			self.GetChild("Event_Title_Bar").SetCloseEvent(self.Close)
			self.GetChild("prev_button").SetEvent(ui.__mem_func__(self.__PrevCalendarPage))
			self.GetChild("next_button").SetEvent(ui.__mem_func__(self.__NextCalendarPage))
			
			self.eventRewardBar.Hide()
			
			for (key, banner) in self.bannerDict.items():
				banner.OnMouseOverIn = lambda selfArg = self, index = key: selfArg.ShowEventTooltip(index)
				banner.OnMouseOverOut = lambda selfArg = self: selfArg.HideEventTooltip()
			
			self.__SetCalendarWeek(self.calendarWeek)
			
			self.wndKingdomWarScoreBoard = KingdomWarScoreBoard()
			self.wndKingdomWarScoreBoard.Hide()

		except:
			import exception
			exception.Abort("EventOverview.__LoadScript.UIScript/EventOverviewWindow.py")
			
	def KingdomWarScore(self, score_shinsoo, score_chunjo, score_jinno):
		if not self.wndKingdomWarScoreBoard:
			self.wndKingdomWarScoreBoard = KingdomWarScoreBoard()
			
		if not self.wndKingdomWarScoreBoard.IsShow():
			self.wndKingdomWarScoreBoard.Show()
			
		self.wndKingdomWarScoreBoard.UpdateScore(score_shinsoo, score_chunjo, score_jinno)
			
	def __BuildCalendar(self):
		monthDayCount = self.__GetCurrentMonthDayCount()
		month = self.__GetCurrentMonth()
		year = self.__GetCurrentYear()
		
		weekDays = []
		weekCount = 1
		
		#Weekends Dict
		for day in range(1, monthDayCount + 1):
			weekDay = datetime.date(year, month, day).isoweekday() 
			
			isWeekend = False
			if weekDay == SATURDAY or weekDay == SUNDAY:
				isWeekend = True
			
			self.weekendDict[day] = isWeekend
			
			weekDays.append(day)

			#Add week to the dict if it is the last day of week/month
			if (day % 7 == 0) or (day == monthDayCount):
				self.weekDict[weekCount] = weekDays

				#Prepare week variable/week list for the next week
				weekCount += 1
				weekDays = []
		
	def Destroy(self):
		self.ClearDictionary()
		
		self.calendarEffect = 0
		self.toolTip = None

		del self.wndKingdomWarScoreBoard
	
	def Open(self):
		if not eventMgr.GetRequestEventData():
			self.ClearActiveEvents()
			
			# Events on server have a range of 0-11
			month = self.__GetCurrentMonth() - 1
			net.SendRequestEventData(month)
			return

		self.RefreshActiveEvent()
		#self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def ClearActiveEvents(self):
		for (key, event) in self.activeEventDict.items():
			event.Hide()
			del self.activeEventDict[key]
						
	def Close(self):
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def SetEventTooltip(self, day, eventCount):
		self.toolTip.SetParent(self)
		self.toolTip.SetThinBoardSize(275)
				
		#Append month & date
		weekDay = datetime.datetime(self.__GetCurrentYear(), self.__GetCurrentMonth(), day).weekday()
		dayName = self.DAY_NAME_DICT[weekDay]
		dateString = "%d/%d/%d - %s" % (day, self.__GetCurrentMonth(), self.__GetCurrentYear(), dayName)
		self.toolTip.AppendTextLine(dateString, self.toolTip.TITLE_COLOR)
		self.toolTip.AppendSpace(3)
		
		for i in range(eventCount):
		#For each event data append a new line on tooltip
			(id, type, start, end, value0, value1, completed) = eventMgr.GetEventData(i, day)
			
			eventName = self.EVENT_NAME_DICT[type]
			timeStart = datetime.datetime.fromtimestamp(start).strftime("%H:%M")
			timeEnd = datetime.datetime.fromtimestamp(end).strftime("%d/%m/%Y - %H:%M")

			if type == eventMgr.EVENT_TYPE_EXPERIENCE or type == eventMgr.EVENT_TYPE_ITEM_DROP:
				self.toolTip.AppendTextLine("%s[%d%%]: %s | %s" % (eventName, value0, timeStart, timeEnd))
			else:
				self.toolTip.AppendTextLine("%s: %s | %s" % (eventName, timeStart, timeEnd))
				
			self.toolTip.AppendSpace(1)
			
		self.toolTip.SetToolTipPosition(0, self.GetHeight() + self.toolTip.GetHeight() - 10)
		self.toolTip.SetWindowHorizontalAlignCenter()
		self.toolTip.AlignHorizonalCenter()
		
	def ShowEventTooltip(self, index):
		day = self.__FindDayByIndex(index)
		eventCount = eventMgr.GetDayEventCount(day)
		
		if eventCount:
			self.SetEventTooltip(day, eventCount)
			self.toolTip.Show()
		
	def HideEventTooltip(self):
		self.toolTip.ClearToolTip()
		self.toolTip.Hide()
		
	def __FindDayByIndex(self, bannerIndex):
		#Subtract one as banners' starting index is 1 while list starts with 0
		dayPos = bannerIndex - 1
		if len(self.weekDict[self.calendarWeek]) < bannerIndex:
			return 0
		
		return self.weekDict[self.calendarWeek][dayPos]
		
	def __FindWeekDayByMonthDay(self, day):
		pos = 1
		
		for i in range(len(self.weekDict[self.calendarWeek])):
			if self.weekDict[self.calendarWeek][i] == day:
				return pos
			pos += 1
			
		return 0
		
	def __PrevCalendarPage(self):
		self.__SetCalendarWeek(self.calendarWeek - 1)
		
	def __NextCalendarPage(self):
		self.__SetCalendarWeek(self.calendarWeek + 1)
		
	def __SetCalendarWeek(self, week):
		if week > len(self.weekDict):
			return
			
		if week < DEFAULT_WEEK:
			return

		#Build calendar with date banners
		ShownBanner = []
		bannerIndex = 1
		for day in self.weekDict[week]:
		
			if self.weekendDict[day] == True:
				imagePath = "d:/ymir work/ui/calendar/calendar_weekend_day%d.sub"
			else:
				imagePath = "d:/ymir work/ui/calendar/calendar_work_day%d.sub"
				
			self.bannerDict[bannerIndex].LoadImage(imagePath % day)
			self.bannerDict[bannerIndex].SetScale(0.6, 0.6)
			ShownBanner.append(bannerIndex)
			bannerIndex += 1
		
		#Hide banners which were not assigned a date
		for (key, banner) in self.bannerDict.items():
			if key not in ShownBanner:
				banner.Hide()
			else:
				banner.Show()
				
		#Move banner effect to current day banner
		currentWeek = self.__GetCurrentMonthWeek()
		if week == currentWeek:
			currentDay = self.__GetCurrentDay()
			pos = self.__GetBannerPosition(currentWeek, currentDay)
			if pos != -1:
				self.calendarEffect.SetPosition(3 + 45 * pos, 5)
				self.calendarEffect.Show()
		else:
			self.calendarEffect.Hide()

		self.calendarWeek = week
		
	def __GetBannerPosition(self, week, day):
		for i in range(len(self.weekDict[week])):
			if self.weekDict[week][i] == day:
				return i

		return -1
		
	def RefreshActiveEvent(self):
		activeEventCount = 0
		for day in range(1, eventMgr.DAY_MAX_NUM + 1):
			eventCount = eventMgr.GetDayEventCount(day)
			for eventIdx in range(eventCount):
				(id, type, start, end, value0, value1, completed) = eventMgr.GetEventData(eventIdx, day)
				currentTime = int(time.time())
				
				if completed:
					if id in self.activeEventDict:
						self.activeEventDict[id].Hide()
						del self.activeEventDict[id]
						
						self.ArrangeEventRewardPosition()
					continue
						
				if (start < currentTime) and (end < currentTime):
					if id in self.activeEventDict:
						self.activeEventDict[id].Hide()
						del self.activeEventDict[id]

						self.ArrangeEventRewardPosition()
						continue
					
				if (start <= currentTime) and (end > currentTime):
					if id not in self.activeEventDict:
						eventName = self.EVENT_NAME_DICT[type]
						eventReward = EventRewardList(self)
						eventReward.SetParent(self)
						eventReward.SetPosition(0, 100 + 20 + (50 * activeEventCount))
						eventReward.SetEventButtonText(eventName)
						eventReward.SetEventText(localeInfo.SecondToDHM(end - currentTime))
						eventReward.SetRewardItemSlot(type)
						eventReward.Show()
						
						self.SetSize(self.GetWidth(), 174 + 50 * activeEventCount)
						self.board.SetSize(self.board.GetWidth(), 174 + 50 * activeEventCount)
						
						self.activeEventDict[id] = eventReward
						
					else:
						self.activeEventDict[id].SetEventText(localeInfo.SecondToDHM(end - currentTime))
						
					activeEventCount += 1
					
		if activeEventCount:
			if not self.eventRewardBar.IsShow():
				self.eventRewardBar.Show()
		else:
			if self.eventRewardBar.IsShow():
				self.eventRewardBar.Hide()

				self.SetSize(self.GetWidth(), 100)
				self.board.SetSize(self.board.GetWidth(), 100)
				
					
	def ArrangeEventRewardPosition(self):
		eventCount = 0
		for event in self.activeEventDict.itervalues(): 
			event.SetPosition(0, 100 + 20 + (50 * eventCount))
			
			self.SetSize(self.GetWidth(), 174 + 50 * eventCount)
			self.board.SetSize(self.board.GetWidth(), 174 + 50 * eventCount)
			
			eventCount += 1
		
	def OnUpdate(self):
		if self.updateTime < app.GetTime():
			if self.IsShow():
				self.RefreshActiveEvent()
				
				#Refresh active event list every 1 sec
				self.updateTime = 1 + app.GetTime()

	def __GetCurrentDay(self):
		return datetime.date.today().day
		
	def __GetCurrentMonthWeek(self):
		currentDay = datetime.date.today().day
		for (key, weekList) in self.weekDict.items():
			if currentDay in weekList:
				return key
				
		return DEFAULT_WEEK
		
	def __GetCurrentMonthDayCount(self):
		time = datetime.date.today()
		return calendar.monthrange(time.year, time.month)[1]
		
	def __GetCurrentMonth(self):
		return datetime.date.today().month
		
	def __GetCurrentYear(self):
		return datetime.date.today().year