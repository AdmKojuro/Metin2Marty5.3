import uiScriptLocale

BOARDWIDTH = 668-40
BOARDHEIGHT = 230

SHOPBOARDWIDTH = 208
SHOPBOARDHEIGHT = 170

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
						{ "name":"title_name", "type":"text", "x":0, "y":3, "text":'Choose Mall',"horizontal_align" : "center", "text_horizontal_align":"center",},
					),
				},
				{
					"name": "banner",
					"type" : "image",
					"x" : 10,
					"y" : 30+3,
			
					"width" : 420,
					"height" : 170,

					"style" : ("movable", "attach",),
					
					"image" : MAINFOLDER+"bg_banner.tga",

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
					"type" : "image",
					"style" : ("attach",),

					"x" : 400 + 50 -20,
					"y" : 30,

					"width" : SHOPBOARDWIDTH,
					"height" : SHOPBOARDHEIGHT,
					
					"image" : MAINFOLDER+"bg_shop_buttons.tga",

					"children" :
					(
						{
							"name" : "malls",
							"style" : ("attach",),

							"x" : 3+7,
							"y" : 2+12,

							"width" : SHOPBOARDWIDTH,
							"height" : SHOPBOARDHEIGHT,

							"children" :
							(
								{ 
									"name" : "btn_mall_arrow_up", 
									"type" : "button",
									"x" : 34,
									"y" : 2,
									"default_image" : MAINFOLDER + "category_up_normal.tga",
									"over_image" : MAINFOLDER + "category_up_down.tga",
									"down_image" : MAINFOLDER + "category_up_hover.tga",
								},
								{ 
									"name" : "btn_mall_arrow_down", 
									"type" : "button",
									"x" : 34,
									"y" : 145,
									"default_image" : MAINFOLDER + "category_down_normal.tga",
									"over_image" : MAINFOLDER + "category_down_down.tga",
									"down_image" : MAINFOLDER + "category_down_hover.tga",
								},
							),
						},
					),
				},
			),
		},
	),
}