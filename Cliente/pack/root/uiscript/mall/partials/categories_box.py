import uiScriptLocale

BOXWIDTH = 145
BOXHEIGHT = 415

MAINFOLDER = "locale/es/ui/mall/"

window = {
	"name" : "category_box",

	"x" : SCREEN_WIDTH/2-BOXWIDTH/2,
	"y" : SCREEN_HEIGHT /2-BOXHEIGHT/2,

	"style" : ("float",),

	"width" : BOXWIDTH,
	"height" : BOXHEIGHT,

	"children" :
	(
		{
			"name" : "background",
			"style" : ("attach",),

			"type" : "image",
			"image" :  MAINFOLDER+"category_box.sub",

			"x" : 0,
			"y" : 0,

			"width" : BOXWIDTH,
			"height" : BOXHEIGHT,

			"children" :
			(
				{
					"name" : "btn_category_up", 
					"type" : "button",
					"x" : 58,
					"y" : 5,
					"default_image" : MAINFOLDER+"arrow_up_norm.sub",
					"over_image" : MAINFOLDER+"arrow_up_hover.sub",
					"down_image" : MAINFOLDER+"arrow_up_press.sub",
				},
				{
					"name" : "btn_category_down", 
					"type" : "button",
					"x" : 58,
					"y" : 375,
					"default_image" : MAINFOLDER+"arrow_down_norm.sub",
					"over_image" : MAINFOLDER+"arrow_down_hover.sub",
					"down_image" : MAINFOLDER+"arrow_down_press.sub",
				},
			),
		},
	),
}