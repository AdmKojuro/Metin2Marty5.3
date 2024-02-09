import uiScriptLocale

BOARDWIDTH = 771
BOARDHEIGHT = 494

CATEGORYBOXWIDTH = 145
CATEGORYBOXHEIGHT = 448

ONSALEBOXWIDTH = 402
ONSALEBOXHEIGHT = 448

INFOBOXWIDTH = 194
INFOBOXHEIGHT = 448

MAINFOLDER = "locale/es/ui/mall/"

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
					"x" : 16,
					"y" : 35,
					"default_image" : MAINFOLDER + "btn_middle_norm.sub",
					"over_image" : MAINFOLDER + "btn_middle_press.sub",
					"down_image" : MAINFOLDER + "btn_middle_hover.sub",
				},
				{
					"name" : "info_box",
					"type" : "image",
					"image" :  MAINFOLDER+"info_box.sub",
					"style" : ("attach",),

					"x" : 10 + CATEGORYBOXWIDTH + 5 + ONSALEBOXWIDTH + 5,
					"y" : 35,

					"width" : INFOBOXWIDTH,
					"height" : INFOBOXHEIGHT,

					"children" :
					(
						{ 
							"name" : "btn_info_box_reset_original", 
							"type" : "button",
							"text" : "Original",
							"x" : 31,
							"y" : 287,
							"default_image" : MAINFOLDER + "btn_category_norm.sub",
							"over_image" : MAINFOLDER + "btn_category_press.sub",
							"down_image" : MAINFOLDER + "btn_category_hover.sub",
						},
						{
							"name" : "sb_info_box_name",
							"type" : "image",
							"x" : 4,
							"y" : 328+24*0,
							"image" : MAINFOLDER+"black_large_slotbar.sub",
							"width" : 186,
							"height" : 22,
							
							"children" :
							(
								{
									"name" : "tx_info_box_name",
									"type" : "text",
									"text" : "Name:",
									"x" : 5,
									"y" : 3,
									"text_horizontal_align" : "left",

								},
								{
									"name" : "tx_info_box_name_value",
									"type" : "text",
									"text" : "Noname",
									"x" : 5,
									"y" : 3,
									"text_horizontal_align" : "right",
									"horizontal_align" : "right",

								},
							),
						},
						{
							"name" : "sb_info_box_level",
							"type" : "image",
							"x" : 4,
							"y" : 328+24*1,
							"image" : MAINFOLDER+"black_large_slotbar.sub",
							"width" : 186,
							"height" : 22,
							
							"children" :
							(
								{
									"name" : "tx_info_box_level",
									"type" : "text",
									"text" : "Level:",
									"x" : 5,
									"y" : 3,
									"text_horizontal_align" : "left",

								},
								{
									"name" : "tx_info_box_level_value",
									"type" : "text",
									"text" : "100",
									"x" : 5,
									"y" : 3,
									"text_horizontal_align" : "right",
									"horizontal_align" : "right",

								},
							),
						},
						{
							"name" : "sb_info_box_currency",
							"type" : "image",
							"x" : 4,
							"y" : 328+24*2,
							"image" : MAINFOLDER+"black_large_slotbar.sub",
							"width" : 186,
							"height" : 22,
							
							"children" :
							(
								{
									"name" : "tx_info_box_currency",
									"type" : "text",
									"text" : "Currency:",
									"x" : 5,
									"y" : 3,
									"text_horizontal_align" : "left",

								},
								{
									"name" : "tx_info_box_currency_value",
									"type" : "text",
									"text" : "0 Currency",
									"x" : 5,
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
							"x" : 16,
							"y" : 410,
							"default_image" : MAINFOLDER+"btn_big_norm.sub",
							"over_image" : MAINFOLDER+"btn_big_hover.sub",
							"down_image" : MAINFOLDER+"btn_big_press.sub",
						},
					),
				},
			),
		},
	),
}