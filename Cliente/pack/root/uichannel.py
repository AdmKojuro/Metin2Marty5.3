import ui
import net
import app
import chat
import math
import wndMgr
import serverInfo
import background
import ServerStateChecker
import localeInfo

class ChannelChanger(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__MakeWindow()
		self.__MakeBoard()
		self.__Fill_Up_ChannelList()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.Hide()
		return True

	def __MakeWindow(self):
		ServerStateChecker.Create(self)
		self.SetSize(150, 195)
		self.SetPosition((wndMgr.GetScreenWidth() / 2) - int(math.floor(self.GetWidth() / 2.)), (wndMgr.GetScreenHeight() / 2) - int(math.floor(self.GetHeight() / 2.)))
		self.AddFlag("float")
		self.Show()

	def __MakeBoard(self):
		self.Board = ui.Board()
		self.Board.SetParent(self)
		self.Board.SetSize(self.GetWidth(), self.GetHeight())
		self.Board.SetPosition(0, 0)
		self.Board.AddFlag("float")
		self.Board.Show()
		
		self.TitleBar = ui.TitleBar()
		self.TitleBar.SetParent(self.Board)
		self.TitleBar.SetPosition(7, 7)
		self.TitleBar.MakeTitleBar(self.GetWidth() - 2 * 7, "red")
		self.TitleBar.SetCloseEvent(self.Close)
		self.TitleBar.Show()
		
		self.TitleText = ui.TextLine()
		self.TitleText.SetParent(self.TitleBar)
		self.TitleText.SetPosition(0, 4)
		self.TitleText.SetText(localeInfo.UI_CHANNEL_CHOOSE)
		self.TitleText.SetWindowHorizontalAlignCenter()
		self.TitleText.SetHorizontalAlignCenter()
		self.TitleText.Show()
		
		self.RefreshButton = ui.Button()
		self.RefreshButton.SetParent(self.Board)
		self.RefreshButton.SetPosition(self.Board.GetWidth() / 2 - 8, self.TitleBar.GetHeight() + 9)
		self.RefreshButton.SetUpVisual("d:/ymir work/ui/game/guild/refresh_button_01.sub")
		self.RefreshButton.SetOverVisual("d:/ymir work/ui/game/guild/refresh_button_02.sub")
		self.RefreshButton.SetDownVisual("d:/ymir work/ui/game/guild/refresh_button_03.sub")
		self.RefreshButton.SetToolTipText(localeInfo.UI_CHANNEL_REFRESH, 0, - 23)
		self.RefreshButton.SetEvent(lambda : self.__Fill_Up_ChannelList())
		self.RefreshButton.Show()
		
		self.ChannelListBase = ui.SlotBar()
		self.ChannelListBase.SetParent(self.Board)
		self.ChannelListBase.SetSize(self.Board.GetWidth() - 2 * 16, 5 * 18 - 4)
		self.ChannelListBase.SetPosition(16 , 7 + self.TitleBar.GetHeight() + 6 + 10 + 6)
		self.ChannelListBase.Show()
		self.ChannelList = ui.ListBox()
		self.ChannelList.SetParent(self.ChannelListBase)
		self.ChannelList.SetSize(self.ChannelListBase.GetWidth()- 20, self.ChannelListBase.GetHeight())
		self.ChannelList.SetPosition(0, 0)
		self.ChannelList.SetEvent(ui.__mem_func__(self.__OnSelectChannel))
		self.ChannelList.Show()
		
		self.ChangeButton = ui.Button()
		self.ChangeButton.SetParent(self.Board)
		self.ChangeButton.SetPosition(self.Board.GetWidth() / 2 - 44, self.Board.GetHeight() - 52)
		self.ChangeButton.SetUpVisual("d:/ymir work/ui/public/Large_button_01.sub")
		self.ChangeButton.SetOverVisual("d:/ymir work/ui/public/Large_button_02.sub")
		self.ChangeButton.SetDownVisual("d:/ymir work/ui/public/Large_button_03.sub")
		self.ChangeButton.SetEvent(lambda : self.__OnClickConnectButton())
		self.ChangeButton.SetText(localeInfo.UI_CHANNEL_GO)
		self.ChangeButton.Show()
		
		self.ClsButton = ui.Button()
		self.ClsButton.SetParent(self.Board)
		self.ClsButton.SetPosition(self.Board.GetWidth() / 2 - 44, self.Board.GetHeight() - 29)
		self.ClsButton.SetUpVisual("d:/ymir work/ui/public/Large_button_01.sub")
		self.ClsButton.SetOverVisual("d:/ymir work/ui/public/Large_button_02.sub")
		self.ClsButton.SetDownVisual("d:/ymir work/ui/public/Large_button_03.sub")
		self.ClsButton.SetEvent(lambda : self.Close())
		self.ClsButton.SetText(localeInfo.UI_CHANNEL_CLOSE)
		self.ClsButton.ButtonText.SetFontColor(1, 1, 1)
		self.ClsButton.Show()
		
		self.DisableChangeButton()
		
		self.ChannelListScrollBar = ui.ScrollBar()
		self.ChannelListScrollBar.SetParent(self.ChannelListBase)
		self.ChannelListScrollBar.SetPosition(18, 3)
		self.ChannelListScrollBar.SetScrollBarSize(83)
		self.ChannelListScrollBar.SetWindowHorizontalAlignRight()
		self.ChannelListScrollBar.SetScrollEvent(ui.__mem_func__(self.__OnScrollChannelList))
		self.ChannelListScrollBar.Show()

	def DisableChangeButton(self):
		self.ChangeButton.Disable()
		self.ChangeButton.Down()
		self.ChangeButton.ButtonText.SetFontColor(0.4, 0.4, 0.4)

	def EnableChangeButton(self):
		self.ChangeButton.Enable()
		self.ChangeButton.SetUp()
		self.ChangeButton.ButtonText.SetFontColor(1, 1, 1)

	def __GetRegionID(self):
		return 0

	def __GetServerID(self):
		serverID = 1
		regionID = self.__GetRegionID()
		for i in serverInfo.REGION_DICT[regionID].keys():
			if serverInfo.REGION_DICT[regionID][i]["name"] == net.GetServerInfo().split(",")[0]:
				serverID = int(i)
				break
		
		return serverID

	def __Fill_Up_ChannelList(self):
		self.__RequestServerStateList()
		self.__RefreshServerStateList()

	def __RequestServerStateList(self):
		regionID = self.__GetRegionID()
		serverID = self.__GetServerID()
		try:
			channelDict = serverInfo.REGION_DICT[regionID][serverID]["channel"]
		except:
			return
		
		ServerStateChecker.Initialize(self)
		for id, channelDataDict in channelDict.items():
			key = channelDataDict["key"]
			ip = channelDataDict["ip"]
			udp_port = channelDataDict["udp_port"]
			ServerStateChecker.AddChannel(key, ip, udp_port)
		ServerStateChecker.Request()

	def __RefreshServerStateList(self):
		regionID = self.__GetRegionID()
		serverID = self.__GetServerID()
		bakChannelID = self.ChannelList.GetSelectedItem()
		self.ChannelList.ClearItem()
		try:
			channelDict = serverInfo.REGION_DICT[regionID][serverID]["channel"]
		except:
			return
		
		for channelID, channelDataDict in channelDict.items():
			channelName = channelDataDict["name"]
			channelState = channelDataDict["state"]
			self.ChannelList.InsertItem(channelID, "%s %s" % (channelName, channelState))
		
		self.ChannelList.SelectItem(bakChannelID-1)

	def NotifyChannelState(self, addrKey, state):
		try:
			stateName = serverInfo.STATE_DICT[state]
		except:
			stateName = serverInfo.STATE_NONE
		
		regionID  = int(addrKey / 1000)
		serverID  = int(addrKey / 10) % 100
		channelID = addrKey % 10
		try:
			serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["state"] = stateName
			self.__RefreshChannelStateList()
		except:
			pass

	def __OnSelectChannel(self):
		if self.ChangeButton.IsDown():
			self.EnableChangeButton()

	def __OnScrollChannelList(self):
		viewItemCount = self.ChannelList.GetViewItemCount()
		itemCount = self.ChannelList.GetItemCount()
		pos = self.ChannelListScrollBar.GetPos() * (itemCount - viewItemCount)
		self.ChannelList.SetBasePos(int(pos))

	def __OnClickConnectButton(self):
		ServerStateChecker.Update()
		channelID = self.ChannelList.GetSelectedItem()
		if not channelID:
			return
		
		if net.GetServerInfo().strip().split(", ")[1] == self.ChannelList.textDict[self.ChannelList.selectedLine].strip().split(" ")[0]:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.UI_CHANNEL_YET_ON_THIS_CH)
			return
		
		self.Close()
		net.SendChatPacket("/channel "+ str(channelID))

	def DirectConnect(self, ChannelIP, ChannelPort, AuthServerIP, AuthServerPort):
		net.SetLoginInfo(decode_string(net.ACCOUNT_ID), decode_string(net.ACCOUNT_PW))
		net.ConnectToAccountServer(ChannelIP, ChannelPort, AuthServerIP, AuthServerPort)
		net.DirectEnter(0)
		net.SendSelectCharacterPacket(0)
		net.SendEnterGamePacket()

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		ServerStateChecker.Initialize(self)
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnUpdate(self):
		ServerStateChecker.Update()

	def OnRunMouseWheel(self, nLen):
		if not self.ChannelListScrollBar or self.ChannelListScrollBar and not self.ChannelListScrollBar.IsShow():
			return False
		
		if self.IsInPosition():
			if nLen > 0:
				self.ChannelListScrollBar.OnUp()
			else:
				self.ChannelListScrollBar.OnDown()
			
			return True
		else:
			return False

