import uiScriptLocale

BOARDWIDTH = 771
BOARDHEIGHT = 494

CATEGORYBOXWIDTH = 177
CATEGORYBOXHEIGHT = 412

ONSALEBOXWIDTH = 360
ONSALEBOXHEIGHT = 461

INFOBOXWIDTH = 219
INFOBOXHEIGHT = 461

MAINFOLDER = "locale/es/ui/mall/design_by_lordziege/"

window = {
	"name" : "Itemshop",

	"x" : SCREEN_WIDTH/2-BOARDWIDTH/2,
	"y" : SCREEN_HEIGHT /2-BOARDHEIGHT/2,

	"style" : ("movable", "float",),

	"width" : BOARDWIDTH,
	"height" : BOARDHEIGHT,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : BOARDWIDTH,
			"height" : BOARDHEIGHT,

			"children" :
			(
				 {
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : BOARDWIDTH-15,

					"children" :
					(
						{ "name":"title_name", "type":"text", "x":0, "y":3, "text":'Mall',"horizontal_align" : "center", "text_horizontal_align":"center",},
					),
				},
				{ 
					"name" : "btn_startpage", 
					"type" : "button",
					"text" : "Startpage",
					"text_color" : 0xffF8BF24,
					"x" : 4,
					"y" : 27,
					"default_image" : MAINFOLDER + "startpage_normal.tga",
					"over_image" : MAINFOLDER + "startpage_down.tga",
					"down_image" : MAINFOLDER + "startpage_hover.tga",
				},
				{
					"name" : "info_box",
					"type" : "image",
					"image" :  MAINFOLDER+"bg_preview.tga",
					"style" : ("attach",),

					"x" : 10 + CATEGORYBOXWIDTH + 5 + ONSALEBOXWIDTH + 5 - 9,
					"y" : 27,

					"width" : INFOBOXWIDTH,
					"height" : INFOBOXHEIGHT,

					"children" :
					(
						{ 
							"name" : "btn_info_box_reset_original", 
							"type" : "button",
							"text" : "Original",
							"x" : 14,
							"y" : 257,
							"default_image" : MAINFOLDER + "button_normal.tga",
							"over_image" : MAINFOLDER + "button_hover.tga",
							"down_image" : MAINFOLDER + "button_down.tga",
						},
						{
							"name" : "sb_info_box_name",
							
							"x" : 17,
							"y" : 299,

							"width" : 184,
							"height" : 16,
							
							"children" :
							(
								{
									"name" : "tx_info_box_name",
									"type" : "text",
									"text" : "Name:",
									"x" : 3,
									"y" : 3,
									"text_horizontal_align" : "left",
									"horizontal_align" : "left",
								},
								{
									"name" : "tx_info_box_name_value",
									"type" : "text",
									"text" : "Noname",
									"x" : 3,
									"y" : 3,
									"text_horizontal_align" : "right",
									"horizontal_align" : "right",
								},
							),
						},
						{
							"name" : "sb_info_box_level",

							"x" : 17,
							"y" : 328,

							"width" : 184,
							"height" : 16,
							
							"children" :
							(
								{
									"name" : "tx_info_box_level",
									"type" : "text",
									"text" : "Level:",
									"x" : 3,
									"y" : 3,
									"text_horizontal_align" : "left",
									"horizontal_align" : "left",
								},
								{
									"name" : "tx_info_box_level_value",
									"type" : "text",
									"text" : "100",
									"x" : 3,
									"y" : 3,
									"text_horizontal_align" : "right",
									"horizontal_align" : "right",
								},
							),
						},
						{
							"name" : "sb_info_box_currency",

							"x" : 17,
							"y" : 357,

							"width" : 184,
							"height" : 16,
							"style" : ("attach",),
							"children" :
							(
								{
									"name" : "tx_info_box_currency",
									"type" : "text",
									"text" : "Currency:",
									"x" : 3,
									"y" : 3,
									"text_horizontal_align" : "left",
									"horizontal_align" : "left",
								},
								{
									"name" : "tx_info_box_currency_value",
									"type" : "text",
									"text" : "0 Currency",
									"x" : 3,
									"y" : 3,
									"text_horizontal_align" : "right",
									"horizontal_align" : "right",

								},
							),
						},
						{
							"name" : "btn_info_box_buy_currency", 
							"type" : "button",
							"text" : "Buy currency!",
							"text_color" : 0xffF8BF24,
							"x" : 14,
							"y" : 388,
							"default_image" : MAINFOLDER + "button_normal.tga",
							"over_image" : MAINFOLDER + "button_hover.tga",
							"down_image" : MAINFOLDER + "button_down.tga",
						},
					),
				},
			),
		},
	),
}