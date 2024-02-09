import uiScriptLocale

BOXWIDTH = 145
BOXHEIGHT = 415

MAINFOLDER = "locale/es/ui/mall/"
NEWFOLDER = "locale/es/ui/mall/design_by_lordziege/"

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
			"image" :  NEWFOLDER+"bg_category.tga",

			"x" : 0,
			"y" : 0,

			"width" : BOXWIDTH,
			"height" : BOXHEIGHT,

			"children" :
			(
				{
					"name" : "btn_category_up", 
					"type" : "button",
					"x" : 33,
					"y" : 12,
					"default_image" : NEWFOLDER + "category_up_normal.tga",
					"over_image" : NEWFOLDER + "category_up_down.tga",
					"down_image" : NEWFOLDER + "category_up_hover.tga",
				},
				{
					"name" : "btn_category_down", 
					"type" : "button",
					"x" : 34,
					"y" : 383,
					"default_image" : NEWFOLDER + "category_down_normal.tga",
					"over_image" : NEWFOLDER + "category_down_down.tga",
					"down_image" : NEWFOLDER + "category_down_hover.tga",
				},
			),
		},
	),
}