import app
import uiScriptLocale
import localeInfo

MIDDLE_WIDTH = SCREEN_WIDTH / 2
MIDLLE_HEIGHT = SCREEN_HEIGHT / 2

window = {
	"name" : "LoginWindow",
	"sytle" : ("movable",),

	"x" : 0, 
	"y" : 0,

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" : 
	(
		# Background
		{
			"name" : "background", 
			"type" : "expanded_image",

			"x" : 0, 
			"y" : 0,

			"x_scale" : float(SCREEN_WIDTH) / 1920.0,
			"y_scale" : float(SCREEN_HEIGHT) / 1080.0,

			"image" : "israel/loginwindow/backgroutest.tga",
			
			"children" :
			(
				# {
					# "name" : "discord_btn",
					# "type" : "button",

					# "x" : -240-70, 
					# "y" : -70,

					# "horizontal_align" : "center",
					# "vertical_align" : "center",

					# "default_image" : "israel/loginwindow/btn/discord_00.tga", 
					# "over_image" : "israel/loginwindow/btn/discord_01.tga",
					# "down_image" : "israel/loginwindow/btn/discord_02.tga",
				# },
				# {
					# "name" : "web_btn",
					# "type" : "button",

					# "x" : -180-70, 
					# "y" : -70,

					# "horizontal_align" : "center",
					# "vertical_align" : "center",

					# "default_image" : "israel/loginwindow/btn/web_00.tga", 
					# "over_image" : "israel/loginwindow/btn/web_01.tga",
					# "down_image" : "israel/loginwindow/btn/web_02.tga",
				# },
				# {
					# "name" : "board_btn",
					# "type" : "button",

					# "x" : -120-70, 
					# "y" : -70,

					# "horizontal_align" : "center",
					# "vertical_align" : "center",

					# "default_image" : "israel/loginwindow/btn/board_00.tga", 
					# "over_image" : "israel/loginwindow/btn/board_01.tga",
					# "down_image" : "israel/loginwindow/btn/board_02.tga",
				# },
				# {
					# "name" : "board",
					# "type" : "image",
							
					# "x" : 120, 
					# "y" : -100,
					
					# "vertical_align" : "center",
					# "horizontal_align" : "center",
					# "image" : "israel/loginwindow/news.tga",
					
					# "children":
					# (
						# {
							# "name": "info1",
							# "type": "text",
							
							# "x": 110,
							# "y": 187,
							
							# "text": "Andra Global",
						# },
						# {
							# "name": "info2",
							# "type": "text",
							
							# "x": 30,
							# "y": 210,
							
							# "text": "(!) New server with everything in detail.",
						# },
						# {
							# "name": "info3",
							# "type": "text",
							
							# "x": 30,
							# "y": 230,
							
							# "text": "(!) Try our beta server and you won't regret it.",
						# },
					# ),
				# },
			),
		},
		{
			"name" : "changechannel",
			"type" : "expanded_image",
			
			"x" : 0,
			"y" : -70,

			"image": "israel/loginwindow/select_panel.tga",
			
			"horizontal_align" : "center",
			"vertical_align" : "center",

			"children" :
			(
				{
					"name" : "ch1",
					"type" : "radio_button",
					
					"x" : 0, 
					"y" : -60,
					
					"horizontal_align" : "center",
					"vertical_align" : "center",
					
					"default_image" : "israel/loginwindow/channel/button_0.tga",
					"over_image" : "israel/loginwindow/channel/button_1.tga",
					"down_image" : "israel/loginwindow/channel/button_2.tga",
					
					"children" : 
					(
						{
							"name" : "ch1_text",
							"type" : "text",
									
							"x" : 18, 
							"y" : 10,
									
							"text" : "Channel 1",
							"color" : 0xffc4ad57,
						},
						{
							"name" : "ch1_status",
							"type" : "text",
							"x" : 100, 
							"y" : 10,
							"text" : "...",
							"color" : 0xffc4ad57,
						},
					),
				},
				{
					"name" : "ch2",
					"type" : "radio_button",
					
					"x" : 0, 
					"y" : -27,
					
					"horizontal_align" : "center",
					"vertical_align" : "center",
					
					"default_image" : "israel/loginwindow/channel/button_0.tga",
					"over_image" : "israel/loginwindow/channel/button_1.tga",
					"down_image" : "israel/loginwindow/channel/button_2.tga",

					"children" : 
					(
						{
							"name" : "ch2_text",
							"type" : "text",
									
							"x" : 18, 
							"y" : 10,
									
							"text" : "Channel 2",
							"color" : 0xffc4ad57,
						},
						{
							"name" : "ch2_status",
							"type" : "text",
							"x" : 100, 
							"y" : 10,
							"text" : "...",
							"color" : 0xffc4ad57,
						},
					),
				},
				{
					"name" : "ch3",
					"type" : "radio_button",
					
					"x" : 0, 
					"y" : 3,
					
					"horizontal_align" : "center",
					"vertical_align" : "center",
					
					"default_image" : "israel/loginwindow/channel/button_0.tga",
					"over_image" : "israel/loginwindow/channel/button_1.tga",
					"down_image" : "israel/loginwindow/channel/button_2.tga",
					
					"children" : 
					(
						{
							"name" : "ch3_text",
							"type" : "text",
									
							"x" : 18, 
							"y" : 10,
									
							"text" : "Channel 3",
							"color" : 0xffc4ad57,
						},
						{
							"name" : "ch3_status",
							"type" : "text",
							"x" : 100, 
							"y" : 10,
							"text" : "...",
							"color" : 0xffc4ad57,
						},
					),
				},
				{
					"name" : "ch4",
					"type" : "radio_button",
					
					"x" : 0, 
					"y" : 36,
					
					"horizontal_align" : "center",
					"vertical_align" : "center",
					
					"default_image" : "israel/loginwindow/channel/button_0.tga",
					"over_image" : "israel/loginwindow/channel/button_1.tga",
					"down_image" : "israel/loginwindow/channel/button_2.tga",
					
					"children" : 
					(
						{
							"name" : "ch4_text",
							"type" : "text",
									
							"x" : 18, 
							"y" : 10,
									
							"text" : "Channel 4",
							"color" : 0xffc4ad57,
						},
						{
							"name" : "ch4_status",
							"type" : "text",
							"x" : 100, 
							"y" : 10,
							"text" : "...",
							"color" : 0xffc4ad57,
						},
					),
				},
			),
		},	
		# Board
		{
			"name" : "board_main",
			"type" : "window",
					
			"x" : -270, 
			"y" : 150,
					
			"width" : 352, 
			"height" : 500,
					
			"vertical_align" : "center",
			"horizontal_align" : "center",
					
			"children" :
			(
				{
					"name" : "board",
					"type" : "image",
							
					"x" : -170,
					"y" : -200,
					
					"vertical_align" : "center",
					"horizontal_align" : "center",
					"image" : "israel/loginwindow/board.tga",
							
					"children" : 
					(
						{
							"name" : "id_slotbar",
							"type" : "radio_button",
									
							"x" : 0, 
							"y" : -88,
									
							"width" : 220,
							"height" : 40,
							
							"horizontal_align" : "center",
							"vertical_align" : "center",
									
							
							"default_image" : "israel/loginwindow/slotbar.tga",
							"over_image" : "israel/loginwindow/slotbar.tga",
							"down_image" : "israel/loginwindow/slotbar.tga",
									
							"children" : 
							(
								{
									"name" : "id",
									"type" : "editline",
											
									"x" : 12, 
									"y" : 12,
											
									"width" : 200, 
									"height" : 20,
											
									"color" : 0xffc4ad57,
									"input_limit": 16,
								},
							),
						},
						{
							"name" : "pwd_slotbar",
							"type" : "radio_button",
									
							"x" : 0, 
							"y" : -43,
							
							"width" : 220,
							"height" : 40,
							
							"horizontal_align" : "center",
							"vertical_align" : "center",
									
							"default_image" : "israel/loginwindow/slotbar.tga",
							"over_image" : "israel/loginwindow/slotbar.tga",
							"down_image" : "israel/loginwindow/slotbar.tga",
									
							"children" : 
							(
								{
									"name" : "pwd",
									"type" : "editline",
											
									"x" : 12, 
									"y" : 12,
											
									"width" : 200, 
									"height" : 20,

									"color" : 0xffc4ad57,
									"input_limit": 16,
									"secret_flag": 1,
									"children":
									(
										{
											"name": "HideText",
											"type": "button",
											"x": 160,
											"y": -5,
											"default_image": "israel/loginwindow/hide_password_default.tga",
											"over_image": "israel/loginwindow/hide_password_down.tga",
											"down_image": "israel/loginwindow/hide_password_over.tga",
										},
									),
								},
							),
						},
						{
							"name" : "login_button",
							"type" : "button",

							"x" : -2, 
							"y" : 63,

							"horizontal_align" : "center",
							"vertical_align" : "center",

							"default_image" : "israel/loginwindow/button_0.tga", 
							"over_image" : "israel/loginwindow/button_1.tga",
							"down_image" : "israel/loginwindow/button_2.tga",

							# "children" : 
							# (
								# {
									# "name" : "login_text",
									# "type" : "text",
											
									# "x" : 31, 
									# "y" : 7,
											
									# "text" : uiScriptLocale.LOGIN_CONNECT,
									# "color" : 0xffc4ad57,
								# },
							# ),
						},
						{
							# "name" : "exit_button1",
							# "type" : "button",

							# "x" : 120, 
							# "y" : 63,

							# "horizontal_align" : "center",
							# "vertical_align" : "center",

							# "default_image" : "israel/loginwindow/button_0.tga", 
							# "over_image" : "israel/loginwindow/button_1.tga",
							# "down_image" : "israel/loginwindow/button_2.tga",

							# "children" : 
							# (
								# {
									# "name" : "login_text",
									# "type" : "text",
											
									# "x" : 31, 
									# "y" : 7,
											
									# "text" : uiScriptLocale.LOGIN_CONNECT,
									# "color" : 0xffc4ad57,
								# },
							# ),
						},
					),
				},
			),
		},
		{
			"name" : "dog",
			"type" : "button",
							
			"x" :0, 
			"y" : 300,
							
			"horizontal_align" : "center",
			"vertical_align" : "center",
							
			"default_image" : "israel/loginwindow/dog1.tga", 
			"over_image" : "israel/loginwindow/dog2.tga",
			"down_image" : "israel/loginwindow/dog2.tga",
		},
		{
			"name" : "account_board",
			"type" : "image",
			
			"x" : 230,
			"y" : -70,

			"horizontal_align" : "center",
			"vertical_align" : "center",

			"image" : "israel/loginwindow/account/board.tga",

			"children" :
			(
				{
					"name" : "account_0_image",
					"type" : "image",
					
					"x" : 60,
					"y" : 110,
					
					"image" : "israel/loginwindow/channel/button_0.tga",
					
					"children" : 
					(
						{
							"name" : "account_0_Key",
							"type" : "text",
							"x" : 10, 
							"y" : 10,
							"color" : 0xffc4ad57,
							"text":"F1",
						},
						{
							"name" : "account_0_text",
							"type" : "text",

							"x" : 0, 
							"y" : -1,

							"color" : 0xffc4ad57,
							"all_align" : True,
						},
					),
				},
				{
					"name" : "load_button_0",
					"type" : "button",
					
					"x" : 60, 
					"y" : 110,
					
					"default_image" : "israel/loginwindow/channel/button_3.tga",
					"over_image" :  "israel/loginwindow/channel/button_3.tga",
					"down_image" : "israel/loginwindow/channel/button_3.tga",
					
					"children" : 
					(
						{
							"name" : "load_text",
							"type" : "text",

							"x" : 0, 
							"y" : 7,

							# "text" : uiScriptLocale.LOGIN_ACCOUNT_LOAD,
							"color" : 0xffc4ad57,
						},
					),
				},
				{
					"name" : "account_1_image",
					"type" : "image",
					"x" : 60, 
					"y" : 143,
					"image" : "israel/loginwindow/channel/button_0.tga",
					"children" : 
					(
						{
							"name" : "account_1_Key",
							"type" : "text",
							"x" : 10, 
							"y" : 10,
							"color" : 0xffc4ad57,
							"text":"F2",
						},
						{
							"name" : "account_1_text",
							"type" : "text",
							
							"x" : 0, 
							"y" : -1,
							
							"color" : 0xffc4ad57,
							"all_align" : True,
						},
					),
				},
				{
					"name" : "load_button_1",
					"type" : "button",
					
					"x" : 60, 
					"y" : 143,
					
					"default_image" : "israel/loginwindow/channel/button_3.tga",
					"over_image" :  "israel/loginwindow/channel/button_3.tga",
					"down_image" : "israel/loginwindow/channel/button_3.tga",
					
					"children" : 
					(
						{
							"name" : "load_text",
							"type" : "text",

							"x" : 36, 
							"y" : 7,

							# "text" : uiScriptLocale.LOGIN_ACCOUNT_LOAD,
							"color" : 0xffc4ad57,
						},
					),
				},
				{
					"name" : "account_2_image",
					"type" : "image",
					"x" : 60, 
					"y" : 176,
					"image" : "israel/loginwindow/channel/button_0.tga",
					"children" : 
					(
						{
							"name" : "account_2_Key",
							"type" : "text",
							"x" : 10, 
							"y" : 10,
							"color" : 0xffc4ad57,
							"text":"F3",
						},
						{
							"name" : "account_2_text",
							"type" : "text",
							
							"x" : 0, 
							"y" : -1,
							
							"color" : 0xffc4ad57,
							"all_align" : True,
						},
					),
				},
				{
					"name" : "load_button_2",
					"type" : "button",
					
					"x" : 60, 
					"y" : 176,
					
					"default_image" : "israel/loginwindow/channel/button_3.tga",
					"over_image" :  "israel/loginwindow/channel/button_3.tga",
					"down_image" : "israel/loginwindow/channel/button_3.tga",
					
					"children" : 
					(
						{
							"name" : "load_text",
							"type" : "text",

							"x" : 36, 
							"y" : 7,

							# "text" : uiScriptLocale.LOGIN_ACCOUNT_LOAD,
							"color" : 0xffc4ad57,
						},
					),
				},
				{
					"name" : "account_3_image",
					"type" : "image",
					"x" : 60, 
					"y" : 209,
					"image" : "israel/loginwindow/channel/button_0.tga",
					"children" : 
					(
						{
							"name" : "account_3_Key",
							"type" : "text",
							"x" : 10, 
							"y" : 10,
							"color" : 0xffc4ad57,
							"text":"F4",
						},
						{
							"name" : "account_3_text",
							"type" : "text",
							
							"x" : 0, 
							"y" : -1,
							
							"color" : 0xffc4ad57,
							"all_align" : True,
						},
					),
				},
				{
					"name" : "load_button_3",
					"type" : "button",
					
					"x" : 60, 
					"y" : 209,
					
					"default_image" : "israel/loginwindow/channel/button_3.tga",
					"over_image" :  "israel/loginwindow/channel/button_3.tga",
					"down_image" : "israel/loginwindow/channel/button_3.tga",
					
					"children" : 
					(
						{
							"name" : "load_text",
							"type" : "text",

							"x" : 0, 
							"y" : 7,

							# "text" : uiScriptLocale.LOGIN_ACCOUNT_LOAD,
							"color" : 0xffc4ad57,
						},
					),
				},
				
				
				{
					"name" : "delete_button_0",
					"type" : "button",
					
					"x" : 265, 
					"y" : 110+10,
					
					"default_image" : "israel/loginwindow/account/delete_0.tga",
					"over_image" :  "israel/loginwindow/account/delete_1.tga",
					"down_image" : "israel/loginwindow/account/delete_2.tga",
				},
				{
					"name" : "save_button_0",
					"type" : "button",
					
					"x" : 265,  
					"y" : 110+10,
					
					"default_image" : "israel/loginwindow/account/save_0.tga",
					"over_image" :  "israel/loginwindow/account/save_1.tga",
					"down_image" : "israel/loginwindow/account/save_2.tga",
				},
				{
					"name" : "delete_button_1",
					"type" : "button",
					
					"x" : 265, 
					"y" : 143+10,
					
					"default_image" : "israel/loginwindow/account/delete_0.tga",
					"over_image" :  "israel/loginwindow/account/delete_1.tga",
					"down_image" : "israel/loginwindow/account/delete_2.tga",
				},
				{
					"name" : "save_button_1",
					"type" : "button",
					
					"x" : 265, 
					"y" : 143+10,
					
					"default_image" : "israel/loginwindow/account/save_0.tga",
					"over_image" :  "israel/loginwindow/account/save_1.tga",
					"down_image" : "israel/loginwindow/account/save_2.tga",
				},
				{
					"name" : "delete_button_2",
					"type" : "button",
					
					"x" : 265, 
					"y" : 176+10,
					
					"default_image" : "israel/loginwindow/account/delete_0.tga",
					"over_image" :  "israel/loginwindow/account/delete_1.tga",
					"down_image" : "israel/loginwindow/account/delete_2.tga",
				},
				{
					"name" : "save_button_2",
					"type" : "button",
					
					"x" : 265, 
					"y" : 176+10,
					
					"default_image" : "israel/loginwindow/account/save_0.tga",
					"over_image" :  "israel/loginwindow/account/save_1.tga",
					"down_image" : "israel/loginwindow/account/save_2.tga",
				},
				{
					"name" : "delete_button_3",
					"type" : "button",
					
					"x" : 265, 
					"y" : 209+10,
					
					"default_image" : "israel/loginwindow/account/delete_0.tga",
					"over_image" :  "israel/loginwindow/account/delete_1.tga",
					"down_image" : "israel/loginwindow/account/delete_2.tga",
				},
				{
					"name" : "save_button_3",
					"type" : "button",
					
					"x" : 265, 
					"y" : 209+10,
					
					"default_image" : "israel/loginwindow/account/save_0.tga",
					"over_image" :  "israel/loginwindow/account/save_1.tga",
					"down_image" : "israel/loginwindow/account/save_2.tga",
				},

				{
					"name" : "exit_button",
					"type" : "button",
						
					"x" : -10,
					"y" : 310,
							
					"default_image" : "israel/loginwindow/close0.tga",
					"over_image" :  "israel/loginwindow/close1.tga",
					"down_image" : "israel/loginwindow/close2.tga",

					"children" : 
					(
						{
							"name" : "exit_text",
							"type" : "text",

							"x" : 45,
							"y" : 5,

							"text" : uiScriptLocale.LOGIN_EXIT,
							"color" : 0xffc4ad57,
						},
					),
				},
			),
		},
		{
			"name" : "LanguageBoard",
			"type" : "thinboard",
			"x" : 13000,
			"y" : -37000,
			"width" : 420,
			"height" : 50,
			"horizontal_align" : "center",
			"vertical_align" : "center",
			"children" : 
			(
				{
					"name" : "btn_lang_en", 
					"type" : "radio_button",
		
					"x" : 20 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/en_default.tga",
					"over_image" : "locale/general/ui/login/flags/en_over.tga",
					"down_image" : "locale/general/ui/login/flags/en_down.tga",
				},
				{
					"name" : "btn_lang_ro", 
					"type" : "radio_button",
		
					"x" : 60 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/ro_default.tga",
					"over_image" : "locale/general/ui/login/flags/ro_over.tga",
					"down_image" : "locale/general/ui/login/flags/ro_down.tga",
				},
				{
					"name" : "btn_lang_it", 
					"type" : "radio_button",
		
					"x" : 100 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/it_default.tga",
					"over_image" : "locale/general/ui/login/flags/it_over.tga",
					"down_image" : "locale/general/ui/login/flags/it_down.tga",
				},
				{
					"name" : "btn_lang_tr", 
					"type" : "radio_button",
		
					"x" : 140 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/tr_default.tga",
					"over_image" : "locale/general/ui/login/flags/tr_over.tga",
					"down_image" : "locale/general/ui/login/flags/tr_down.tga",
				},
				{
					"name" : "btn_lang_de", 
					"type" : "radio_button",
		
					"x" : 180 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/de_default.tga",
					"over_image" : "locale/general/ui/login/flags/de_over.tga",
					"down_image" : "locale/general/ui/login/flags/de_down.tga",
				},
				{
					"name" : "btn_lang_pl", 
					"type" : "radio_button",
		
					"x" : 220 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/pl_default.tga",
					"over_image" : "locale/general/ui/login/flags/pl_over.tga",
					"down_image" : "locale/general/ui/login/flags/pl_down.tga",
				},
				{
					"name" : "btn_lang_pt", 
					"type" : "radio_button",
		
					"x" : 260 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/pt_default.tga",
					"over_image" : "locale/general/ui/login/flags/pt_over.tga",
					"down_image" : "locale/general/ui/login/flags/pt_down.tga",
				},
				{
					"name" : "btn_lang_es", 
					"type" : "radio_button",
		
					"x" : 300 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/es_default.tga",
					"over_image" : "locale/general/ui/login/flags/es_over.tga",
					"down_image" : "locale/general/ui/login/flags/es_down.tga",
				},
				{
					"name" : "btn_lang_cz", 
					"type" : "radio_button",
		
					"x" : 340 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/cz_default.tga",
					"over_image" : "locale/general/ui/login/flags/cz_over.tga",
					"down_image" : "locale/general/ui/login/flags/cz_down.tga",
				},
				{
					"name" : "btn_lang_hu", 
					"type" : "radio_button",
		
					"x" : 380 - 10,
					"y" : 20,
		
					"default_image" : "locale/general/ui/login/flags/hu_default.tga",
					"over_image" : "locale/general/ui/login/flags/hu_over.tga",
					"down_image" : "locale/general/ui/login/flags/hu_down.tga",
				},
			),
		},
	),
}