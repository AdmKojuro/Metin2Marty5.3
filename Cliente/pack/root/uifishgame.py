import ui, exception, chat, app, net

EVENT_TIME = 20
MISS_OR_HIT_TIME = 1
FISH_CHANGE_ROTATION_SPEED = 6
FISH_CLICK_CHANGE_ROTATION = 3 # don't touch.
FISH_LOGIC_SPEED = 3 # main speed don't touch.


class FishGame(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.Rotation = 0
		self.FullX = 0
		self.XFullX = 0
		self.GoalValue=0
		self.OldLeftTime = 0
		self.startTime = app.GetTime()+EVENT_TIME
		self.MissOrHit = 1
		self.MissOrHitLeftTime = 0
		self.IsOn = True
		self.IsCircle = False
		self.__LoadWindow()
	
	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/fishinggamewindow.py")
			self.Click = self.GetChild("fishing_background_water")
			self.Fish = self.GetChild("Fish")
			self.Circle = self.GetChild("fishing_goal_circle")
			self.GoalText = self.GetChild("fishing_goal_count_text")
			self.Miss = self.GetChild("miss")
			self.Hit = self.GetChild("hit")
		except:
			exception.Abort("FishGame.__LoadWindow.UIScript/fishinggamewindow.py")

		self.Click.SAFE_SetStringEvent("MOUSE_OVER_IN",self.SetFishCursor)
		self.Click.SAFE_SetStringEvent("MOUSE_OVER_OUT",self.SetCurrentCursor)
		self.Click.SAFE_SetMouseClickEvent(self.LeftClick,"")
		# self.Click.SetEvent(ui.__mem_func__(self.LeftClick),"mouse_click")

		self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.Close))

		self.Miss.Hide()
		self.Hit.Hide()

	def Show(self):
		ui.ScriptWindow.Show(self)
		#self.Show()

	def LeftClick(self, arg):
		rotation = [90,280,530,720]
		self.FullX = app.GetRandom(0,FISH_CLICK_CHANGE_ROTATION)
		self.Fish.SetRotation(rotation[self.FullX])

		(x,y) = self.Fish.GetLocalPosition()
		if self.IsCircle:
			self.Hit.SetPosition(x+10,y-10)
			self.MissOrHitLeftTime = app.GetTime()+MISS_OR_HIT_TIME
			self.Hit.Show()
			self.GoalValue+=1
			self.GoalText.SetText("%d/3"%self.GoalValue)
			net.SendChatPacket("/cmd_fish_event 58236")
			if self.GoalValue == 3:
				#chat.AppendChat(chat.CHAT_TYPE_MISSION, "Success <> ")
				self.IsOn = False
				net.SendChatPacket("/cmd_fish_event 78531")
				self.Destroy()
		else:
			self.Miss.SetPosition(x+10,y-10)
			self.MissOrHitLeftTime = app.GetTime()+MISS_OR_HIT_TIME
			self.Miss.Show()

	def SetFishCursor(self):
		if app.GetCursor() != app.DOOR:
			app.SetCursor(app.DOOR)

	def SetCurrentCursor(self):
		if app.GetCursor() != app.NORMAL:
			app.SetCursor(app.NORMAL)

	def Destroy(self):
		self.ClearDictionary()
		self.SetCurrentCursor()
		self.Rotation = 0
		self.FullX = 0
		self.XFullX = 0
		self.GoalValue=0
		self.OldLeftTime = 0
		self.startTime = 0
		self.MissOrHit = 0
		self.MissOrHitLeftTime = 0
		self.IsOn = 0
		self.IsCircle = 0
		self.Click = 0
		self.Fish = 0
		self.Circle = 0
		self.GoalText =0
		self.Miss = 0
		self.Hit = 0
		self.Hide()

	def OnRender(self):
		(x,y) = self.Fish.GetLocalPosition()
		if self.IsOn:
			if self.Miss.IsShow():
				if app.GetTime() > self.MissOrHitLeftTime:
					self.Miss.Hide()
			if self.Hit.IsShow():
				if app.GetTime() > self.MissOrHitLeftTime:
					self.Hit.Hide()

			leftTime = max(0,self.startTime-app.GetTime())
			if self.OldLeftTime < leftTime:
				self.GetChild("fishing_timer_gauge").SetPercentage(leftTime,EVENT_TIME)

			if leftTime <= 0:
				#if self.GoalValue == 3:
				#	chat.AppendChat(chat.CHAT_TYPE_MISSION, "Success <> ")
				#else:
				#	chat.AppendChat(chat.CHAT_TYPE_MISSION, "Unsuccess <> ")
				self.IsOn = False
				net.SendChatPacket("/cmd_fish_event 90295")
				self.Destroy()

			if x <= 24:
				self.Fish.SetRotation(90)
				self.FullX=0
				self.XFullX=app.GetRandom(1,2)
			elif x >= 252:
				self.FullX=1
				self.Fish.SetRotation(280)
				self.XFullX=app.GetRandom(1,2)
			elif y <= 40:
				self.FullX=2
				self.Fish.SetRotation(530)
				self.XFullX=app.GetRandom(1,2)
			elif y >= 222:
				self.FullX=3
				self.Fish.SetRotation(720)
				self.XFullX=app.GetRandom(1,2)

			if self.FullX==0:
				if self.XFullX == 1:
					self.Fish.SetPosition(x+FISH_CHANGE_ROTATION_SPEED+app.GetRandom(1,3),y+app.GetRandom(1,FISH_LOGIC_SPEED))
				else:
					self.Fish.SetPosition(x+FISH_CHANGE_ROTATION_SPEED+app.GetRandom(1,3),y-app.GetRandom(1,FISH_LOGIC_SPEED))
			elif self.FullX==1:
				if self.XFullX == 1:
					self.Fish.SetPosition(x-FISH_CHANGE_ROTATION_SPEED+app.GetRandom(1,5),y+app.GetRandom(1,FISH_LOGIC_SPEED))
				else:
					self.Fish.SetPosition(x-FISH_CHANGE_ROTATION_SPEED+app.GetRandom(1,5),y-app.GetRandom(1,FISH_LOGIC_SPEED))
			elif self.FullX==2:
				if self.XFullX == 1:
					self.Fish.SetPosition(x+app.GetRandom(1,3),y+FISH_CHANGE_ROTATION_SPEED+app.GetRandom(1,FISH_LOGIC_SPEED))
				else:
					self.Fish.SetPosition(x-app.GetRandom(1,3),y+FISH_CHANGE_ROTATION_SPEED+app.GetRandom(1,FISH_LOGIC_SPEED))
			elif self.FullX==3:
				if self.XFullX == 1:
					self.Fish.SetPosition(x-app.GetRandom(1,3),y-FISH_CHANGE_ROTATION_SPEED+app.GetRandom(1,FISH_LOGIC_SPEED))
				else:
					self.Fish.SetPosition(x+app.GetRandom(1,3),y-FISH_CHANGE_ROTATION_SPEED+app.GetRandom(1,FISH_LOGIC_SPEED))

			(directionx,directiony)=self.Circle.GetLocalPosition()
			if x >= directionx and x <= directionx+self.Circle.GetWidth() and y >= directiony and y <= directiony+self.Circle.GetHeight():
				self.Circle.SetDiffuseColor(1,0,0,1)
				self.IsCircle = True
			else:
				self.Circle.SetDiffuseColor(1,1,1,1)
				self.IsCircle = False

			#chat.AppendChat(chat.CHAT_TYPE_INFO, "X=%d Y=%d"%(x,y))

	def Close(self):
		net.SendChatPacket("/cmd_fish_event 90295")
		self.Destroy()

