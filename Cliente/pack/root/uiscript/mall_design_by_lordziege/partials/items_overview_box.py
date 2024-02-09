import uiScriptLocale

BOXWIDTH = 402
BOXHEIGHT = 448

MAINFOLDER = "locale/es/ui/mall/design_by_lordziege/"

window = {
	"name" : "items_overview_box",

	"x" : SCREEN_WIDTH/2-BOXWIDTH/2,
	"y" : SCREEN_HEIGHT /2-BOXHEIGHT/2,

	"style" : ("float",),

	"width" : BOXWIDTH,
	"height" : BOXHEIGHT,

	"children" :
	(
		{
			"name" : "background",
			"type" : "image",
			"image" : MAINFOLDER+"bg_itembox.tga",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : BOXWIDTH,
			"height" : BOXHEIGHT,

			"children" :
			(
				{
					"name" : "btn_left", 
					"type" : "button",
					"x" : 278,
					"y" : 430,
					"default_image" : MAINFOLDER+"page_button_left_normal.tga",
					"over_image" : MAINFOLDER+"page_button_left_hover.tga",
					"down_image" : MAINFOLDER+"page_button_left_down.tga",
				},
				{
					"name" : "page_nr",
					"type" : "image",
					"x" : 296,
					"y" : 428,
					"image" :  MAINFOLDER+"page_site_bg.tga",
					"style" : ("attach",),

					"width" : 24,
					"height" : 18,

					"children" :
					(	
						{
							"name" : "tx_page_nr",
							"type" : "text",
							"text" : "1/1",
							"x" : 0,
							"y" : 3,
							"horizontal_align" : "center",
							"text_horizontal_align" : "center",
						},
					),
				},
				{
					"name" : "btn_right", 
					"type" : "button",
					"x" : 323,
					"y" : 430,
					"default_image" : MAINFOLDER+"page_button_right_normal.tga",
					"over_image" : MAINFOLDER+"page_button_right_hover.tga",
					"down_image" : MAINFOLDER+"page_button_right_down.tga",
				},
			),
		},
	),
}