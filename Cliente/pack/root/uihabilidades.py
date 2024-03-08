import ui, constInfo, event

class Habilidades(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.Load()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def Load(self):
	
		self.board = ui.Board()
		self.board.SetSize(385, 280)
		self.board.SetCenterPosition()
		self.board.Show()
		
		self.grupo1_btn = ui.Button()
		self.grupo1_btn.SetParent(self.board)
		self.grupo1_btn.SetUpVisual('d:/ymir work/ui/public/Large_Button_01.sub')
		self.grupo1_btn.SetOverVisual('d:/ymir work/ui/public/Large_Button_02.sub')
		self.grupo1_btn.SetDownVisual('d:/ymir work/ui/public/Large_Button_03.sub')
		self.grupo1_btn.SetPosition(60, 236)
		self.grupo1_btn.SetText("ELEGIR")
		self.grupo1_btn.SetEvent(ui.__mem_func__(self.__grupo1_func))
		self.grupo1_btn.Show()
		
		self.grupo2_btn = ui.Button()
		self.grupo2_btn.SetParent(self.board)
		self.grupo2_btn.SetUpVisual('d:/ymir work/ui/public/Large_Button_01.sub')
		self.grupo2_btn.SetOverVisual('d:/ymir work/ui/public/Large_Button_02.sub')
		self.grupo2_btn.SetDownVisual('d:/ymir work/ui/public/Large_Button_03.sub')
		self.grupo2_btn.SetPosition(240, 236)
		self.grupo2_btn.SetText("ELEGIR")
		self.grupo2_btn.SetEvent(ui.__mem_func__(self.__grupo2_func))
		self.grupo2_btn.Show()
		
		self.corporal = ui.ImageBox()
		self.corporal.SetParent(self.board)
		self.corporal.SetPosition(10, 20)
		self.corporal.LoadImage('locale/es/ui/habilidades/corporal.tga')
		self.corporal.Hide()
		
		self.mental = ui.ImageBox()
		self.mental.SetParent(self.board)
		self.mental.SetPosition(190, 20)
		self.mental.LoadImage('locale/es/ui/habilidades/mental.tga')
		self.mental.Hide()
		
		self.daguero = ui.ImageBox()
		self.daguero.SetParent(self.board)
		self.daguero.SetPosition(10, 20)
		self.daguero.LoadImage('locale/es/ui/habilidades/daguero.tga')
		self.daguero.Hide()
		
		self.arquero = ui.ImageBox()
		self.arquero.SetParent(self.board)
		self.arquero.SetPosition(190, 20)
		self.arquero.LoadImage('locale/es/ui/habilidades/arquero.tga')
		self.arquero.Hide()
		
		self.espejo = ui.ImageBox()
		self.espejo.SetParent(self.board)
		self.espejo.SetPosition(10, 20)
		self.espejo.LoadImage('locale/es/ui/habilidades/espejo.tga')
		self.espejo.Hide()
		
		self.magia = ui.ImageBox()
		self.magia.SetParent(self.board)
		self.magia.SetPosition(190, 20)
		self.magia.LoadImage('locale/es/ui/habilidades/magia.tga')
		self.magia.Hide()
		
		self.dragon = ui.ImageBox()
		self.dragon.SetParent(self.board)
		self.dragon.SetPosition(10, 20)
		self.dragon.LoadImage('locale/es/ui/habilidades/dragon.tga')
		self.dragon.Hide()
		
		self.curacion = ui.ImageBox()
		self.curacion.SetParent(self.board)
		self.curacion.SetPosition(190, 20)
		self.curacion.LoadImage('locale/es/ui/habilidades/curacion.tga')
		self.curacion.Hide()

		self.titulo = ui.TextLine()
		self.titulo.SetParent(self.board)
		self.titulo.SetPosition(155, 12)
		self.titulo.SetText("Elige una Doctrina:")
		self.titulo.Show()
		
	def Close(self):
		self.board.Hide()
		
	def Open(self, raza):
		self.Load()
		if raza == 1:
			self.Guerrero()
		elif raza == 2:
			self.Ninja()
		elif raza == 3:
			self.Sura()
		elif raza == 4:
			self.Chaman()
			
	def __grupo1_func(self):
		self.QuestCMD("GRUPO1")
		self.Close()
		
	def __grupo2_func(self):
		self.QuestCMD("GRUPO2")
		self.Close()
		
	def QuestCMD(self, cmd):
		constInfo.Habilidades["HABSTR"] = cmd
		event.QuestButtonClick(int(constInfo.Habilidades["qid"]))

	def Guerrero(self):
		self.corporal.Show()
		self.mental.Show()

	def Ninja(self):
		self.daguero.Show()
		self.arquero.Show()

	def Sura(self):
		self.espejo.Show()
		self.magia.Show()

	def Chaman(self):
		self.dragon.Show()
		self.curacion.Show()

#TheBestMein()		