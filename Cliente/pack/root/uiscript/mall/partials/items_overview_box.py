import uiScriptLocale

BOXWIDTH = 402
BOXHEIGHT = 448

MAINFOLDER = "locale/es/ui/mall/"

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
			"image" :  MAINFOLDER+"for_sale_box.sub",
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
					"x" : 5,
					"y" : 415,
					"default_image" : MAINFOLDER+"arrow_left_norm.sub",
					"over_image" : MAINFOLDER+"arrow_left_hover.sub",
					"down_image" : MAINFOLDER+"arrow_left_press.sub",
				},
				{
					"name" : "page_nr",
					"type" : "image",
					"x" : 182,
					"y" : 420,
					"image" :  MAINFOLDER+"page_count_box.sub",
					"style" : ("attach",),

					"width" : 38,
					"height" : 22,

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
					"x" : 360,
					"y" : 415,
					"default_image" : MAINFOLDER+"arrow_right_norm.sub",
					"over_image" : MAINFOLDER+"arrow_right_hover.sub",
					"down_image" : MAINFOLDER+"arrow_right_press.sub",
				},
			),
		},
	),
}