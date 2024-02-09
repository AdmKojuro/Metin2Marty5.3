import uiScriptLocale

BOARDWIDTH = 610
BOARDHEIGHT = 230

SHOPBOARDWIDTH = 150
SHOPBOARDHEIGHT = 170

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
						{ "name":"title_name", "type":"text", "x":0, "y":3, "text":'Choose Mall',"horizontal_align" : "center", "text_horizontal_align":"center",},
					),
				},
				{
					"name": "banner",
					"type" : "thinboard",
					"x" : 20,
					"y" : 40,
			
					"width" : 420,
					"height" : 170,

					"style" : ("movable", "attach",),

					"children": 
					(
						# {
						# 	"name" : "image_banner",
						# 	"type" : "image",
						# 	"style" : ("movable", "attach",),
						# 	"x" : 10,
						# 	"y" : 10,
						# 	"image" : MAINFOLDER+"banner_empty.tga",
						# },
						# {
						# 	"name" : "image_fade_banner",
						# 	"type" : "image",
						# 	"style" : ("movable", "attach",),
						# 	"x" : 10,
						# 	"y" : 10,
						# 	"image" : MAINFOLDER+"banner_empty.tga",
						# },
					),
				},
				{
					"name" : "board_mall",
					"type" : "thinboard",
					"style" : ("attach",),

					"x" : 400 + 50,
					"y" : 40,

					"width" : SHOPBOARDWIDTH,
					"height" : SHOPBOARDHEIGHT,

					"children" :
					(
						{
							"name" : "malls",
							"style" : ("attach",),

							"x" : 3,
							"y" : 2,

							"width" : SHOPBOARDWIDTH,
							"height" : SHOPBOARDHEIGHT,

							"children" :
							(
								{ 
									"name" : "btn_mall_arrow_up", 
									"type" : "button",
									"x" : 70,
									"y" : 2,
									"default_image" : MAINFOLDER + "small_arrow_up_norm.sub",
									"over_image" : MAINFOLDER + "small_arrow_up_press.sub",
									"down_image" : MAINFOLDER + "small_arrow_up_hover.sub",
								},
								{ 
									"name" : "btn_mall_arrow_down", 
									"type" : "button",
									"x" : 70,
									"y" : 159,
									"default_image" : MAINFOLDER + "small_arrow_down_norm.sub",
									"over_image" : MAINFOLDER + "small_arrow_down_press.sub",
									"down_image" : MAINFOLDER + "small_arrow_down_hover.sub",
								},
							),
						},
					),
				},
			),
		},
	),
}