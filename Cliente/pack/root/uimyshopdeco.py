import ui
import chat
import wndMgr
import net
import constInfo

class UiShopDeco(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		self.shop = 0
		self.decoration = 0
		self.wa=[30000,30001,30002,30003,30004,30005,30006,30007],
		self.modelotitulo = ["Est\xe1ndar","Infierno","Edad de hielo","Pergamino","Estandarte de guerra","Alas"],
		self.modelotienda=["Est\xe1ndar","Mil y una noches","Maestro asador","Cazatesoros","Llama baja","Comerciante de armas","Comercio de armaduras","Alquimista"],
		self.ModuleEx()
		self.__Procces(0,"Est\xe1ndar")

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "d:/ymir work/ui/myshopdecorationwindow.py")

		except:
			import exception
			exception.Abort("UiShopDeco.LoadWindow")

		try:
			
			self.name = self.GetChild("ModelName")
			self.next = self.GetChild("NextButton")
			self.cancel = self.GetChild("CancelButton")
			self.prev = self.GetChild("PrevButton")
			self.ok = self.GetChild("CompleteButton")
			self.GetChild("MyShopTitleBar").SetCloseEvent(ui.__mem_func__(self.Close))

			self.ok.Hide()
			self.prev.Hide()

			self.next.SetEvent(self.__NextProcces)
			self.prev.SetEvent(self.__PrevProcces)
			self.ok.SetEvent(self.__OkProcces)
			self.cancel.SetEvent(self.Close)

		except:
			import exception
			exception.Abort("UiShopDecoModules.LoadWindow")

	def ModuleEx(self):
		count_0 = 0
		count_1 = 0
		self.buttons = {}
		self.buttons1 = {}
		self.button_x = [
		[[25,40,"Est\xe1ndar"]],
		[[25,40+27,"Mil y una noches"]],
		[[25,40+27+27,"Maestro asador"]],
		[[25,40+27+27+27,"Cazatesoros"]],
		[[25,40+27+27+27+27,"Llama baja"]],
		[[25,40+27+27+27+27+27,"Comerciante de armas"]],
		[[25,40+27+27+27+27+27+27,"Comercio de armaduras"]],
		[[25,40+27+27+27+27+27+27+27,"Alquimista"]]
		]
		self.button_x1 = [
		[[25,40,"Est\xe1ndar"]],
		[[25,40+27,"Infierno"]],
		[[25,40+27+27,"Edad de hielo"]],
		[[25,40+27+27+27,"Pergamino"]],
		[[25,40+27+27+27+27,"Estandarte de guerra"]],
		[[25,40+27+27+27+27+27,"Alas"]]
		]


		self.bg = ui.ExpandedImageBox()
		self.bg.SetParent(self)
		self.bg.SetPosition(194,65)
		self.bg.LoadImage("d:/ymir work/ui/game/myshop_deco/model_view_bg.sub")
		self.bg.Show()
			
		self.iconshop = ui.ImageBox()
		self.iconshop.SetParent(self.bg)
		self.iconshop.SetPosition(5,30)
		self.iconshop.LoadImage("d:/ymir work/image/30001.tga")
		self.iconshop.SetWindowHorizontalAlignCenter()
		self.iconshop.SetWindowVerticalAlignCenter()
		self.iconshop.Show()




		for a in self.button_x:
			self.buttons[count_0] = ui.RadioButton()
			self.buttons[count_0].SetParent(self)
			self.buttons[count_0].SetPosition(a[0][0],a[0][1])
			self.buttons[count_0].SetUpVisual("d:/ymir work/ui/game/myshop_deco/select_btn_01.sub")
			self.buttons[count_0].SetOverVisual("d:/ymir work/ui/game/myshop_deco/select_btn_02.sub")
			self.buttons[count_0].SetDownVisual("d:/ymir work/ui/game/myshop_deco/select_btn_03.sub")
			self.buttons[count_0].SetText(a[0][2])
			self.buttons[count_0].SetEvent(self.__Procces,count_0,a[0][2])
			self.buttons[count_0].Show()
			count_0 +=1

		for b in self.button_x1:
			self.buttons1[count_1] = ui.RadioButton()
			self.buttons1[count_1].SetParent(self)
			self.buttons1[count_1].SetPosition(b[0][0],b[0][1])
			self.buttons1[count_1].SetUpVisual("d:/ymir work/ui/game/myshop_deco/select_btn_01.sub")
			self.buttons1[count_1].SetOverVisual("d:/ymir work/ui/game/myshop_deco/select_btn_02.sub")
			self.buttons1[count_1].SetDownVisual("d:/ymir work/ui/game/myshop_deco/select_btn_03.sub")
			self.buttons1[count_1].SetText(b[0][2])
			self.buttons1[count_1].SetEvent(self.__Procces1,count_1,b[0][2])
			self.buttons1[count_1].Hide()
			count_1 +=1

		self.button_s = (
			self.buttons[0],
			self.buttons[1],
			self.buttons[2],
			self.buttons[3],
			self.buttons[4],
			self.buttons[5],
			self.buttons[6],
			self.buttons[7],
		)

		self.button_s1 = (
			self.buttons1[0],
			self.buttons1[1],
			self.buttons1[2],
			self.buttons1[3],
			self.buttons1[4],
			self.buttons1[5],
		)		

	def __Procces(self,index,name):
		for btn in self.button_s:
			btn.SetUp()
		self.button_s[index].Down()
		self.name.SetText(name)
		self.shop = index
		i_position = [[1,30],[5,30],[4,30],[2,30],[0,30],[-3,28],[2,30],[-1,30]]
		self.iconshop.LoadImage("d:/ymir work/image/"+str(self.wa[0][index])+".tga")
		self.iconshop.SetPosition(i_position[index][0],i_position[index][1])

	def __Procces1(self,index,name):
		for btn in self.button_s1:
			btn.SetUp()
		self.button_s1[index].Down()
		self.name.SetText(name)
		self.decoration = index
		self.__ThinDecoration(index)

	def __ThinDecoration(self,index):
		t_board = [ui.ThinBoardNorm(),ui.ThinBoardFire(),ui.ThinBoardIce(),ui.ThinBoardPaper(),ui.ThinBoardRibon(),ui.ThinBoardWing()]
		t_position = [[34,23],[19,10],[19,10],[19,10],[19,10],[19,10]]
		self.thinboard = t_board[index]
		self.thinboard.SetParent(self.bg)
		self.thinboard.SetPosition(t_position[index][0],t_position[index][1])
		self.thinboard.SetSize(170)
		self.thinboard.Show()

	def __NextProcces(self):
		for close in self.button_s:
			close.Hide()
		for open in self.button_s1:
			open.Show()
		self.prev.Show()
		self.next.Hide()
		self.ok.Show()
		self.cancel.Hide()
		self.__Procces1(0, "Est\xe1ndar")

	def __PrevProcces(self):
		for close in self.button_s1:
			close.Hide()
		for open in self.button_s:
			open.Show()
		self.next.Show()
		self.prev.Hide()
		self.ok.Hide()
		self.thinboard.Hide()
		self.cancel.Show()

	def __OkProcces(self):
		self.deconpc=["0","1","2","3","4","5"]
		# chat.AppendChat(chat.CHAT_TYPE_INFO, "Modelo de la tienda: %s, Modelo del t\xedtulo de la tienda: %s"%(self.modelotienda[0][self.shop],self.modelotitulo[0][self.decoration]))
		chat.AppendChat(chat.CHAT_TYPE_INFO, "Modelo de la tienda: %s."%(self.modelotienda[0][self.shop]))
		chat.AppendChat(chat.CHAT_TYPE_INFO, "Modelo del t\xedtulo de la tienda: %s."%(self.modelotitulo[0][self.decoration]))
		net.SendChatPacket("/shop_decoration %d %d " % (self.wa[0][self.shop],self.decoration))
		wndMgr.Hide(self.hWnd)

	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def OnPressExitKey(self):
		self.Close()
		return True

	def Close(self):
		wndMgr.Hide(self.hWnd)
