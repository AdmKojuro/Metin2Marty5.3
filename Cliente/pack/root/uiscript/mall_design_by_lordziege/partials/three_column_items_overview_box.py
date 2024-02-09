import uiScriptLocale

BOXWIDTH = 402
BOXHEIGHT = 448

MAINFOLDER = "locale/es/ui/mall/design_by_lordziege/"

window = {
	"name" : "three_column_items_overview_box",

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
					"name" : "column1_title",
					"type" : "image",
					"x" : 14,
					"y" : 13,

					"image" :  MAINFOLDER+"new_items_button.tga",
					"style" : ("attach",),

					"children" :
					(	
						{
							"name" : "tx_column1_title",
							"type" : "text",
							"text" : "Hot Offers",
							"x" : 4,
							"y" : 0,
						},
					),
				},

				{
					"name" : "btn_column1_left", 
					"type" : "button",
					"x" : 278,
					"y" : 15,
					"default_image" : MAINFOLDER+"page_button_left_normal.tga",
					"over_image" : MAINFOLDER+"page_button_left_hover.tga",
					"down_image" : MAINFOLDER+"page_button_left_down.tga",
				},
				{
					"name" : "column1_page_nr",
					"type" : "image",
					"x" : 296,
					"y" : 13,
					"image" :  MAINFOLDER+"page_site_bg.tga",
					"style" : ("attach",),

					"width" : 24,
					"height" : 18,

					"children" :
					(	
						{
							"name" : "tx_column1_page_nr",
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
					"name" : "btn_column1_right", 
					"type" : "button",
					"x" : 323,
					"y" : 15,
					"default_image" : MAINFOLDER+"page_button_right_normal.tga",
					"over_image" : MAINFOLDER+"page_button_right_hover.tga",
					"down_image" : MAINFOLDER+"page_button_right_down.tga",
				},

				{
					"name" : "column2_title",
					"type" : "image",
					"x" : 14,
					"y" : 160,

					"image" :  MAINFOLDER+"new_items_button.tga",
					"style" : ("attach",),

					"children" :
					(	
						{
							"name" : "tx_column2_title",
							"type" : "text",
							"text" : "New Items",
							"x" : 4,
							"y" : 0,
						},
					),
				},

				{
					"name" : "btn_column2_left", 
					"type" : "button",
					"x" : 278,
					"y" : 162,
					"default_image" : MAINFOLDER+"page_button_left_normal.tga",
					"over_image" : MAINFOLDER+"page_button_left_hover.tga",
					"down_image" : MAINFOLDER+"page_button_left_down.tga",
				},
				{
					"name" : "column2_page_nr",
					"type" : "image",
					"x" : 296,
					"y" : 160,
					"image" :  MAINFOLDER+"page_site_bg.tga",
					"style" : ("attach",),

					"width" : 24,
					"height" : 18,

					"children" :
					(	
						{
							"name" : "tx_column2_page_nr",
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
					"name" : "btn_column2_right", 
					"type" : "button",
					"x" : 323,
					"y" : 162,
					"default_image" : MAINFOLDER+"page_button_right_normal.tga",
					"over_image" : MAINFOLDER+"page_button_right_hover.tga",
					"down_image" : MAINFOLDER+"page_button_right_down.tga",
				},
				
				{
					"name" : "column3_title",
					"type" : "image",
					"x" : 14,
					"y" : 308,

					"image" :  MAINFOLDER+"new_items_button.tga",
					"style" : ("attach",),
					"children" :
					(	
						{
							"name" : "tx_column3_title",
							"type" : "text",
							"text" : "Most bought",
							"x" : 4,
							"y" : 0,
						},
					),
				},

				{
					"name" : "btn_column3_left", 
					"type" : "button",
					"x" : 278,
					"y" : 310,
					"default_image" : MAINFOLDER+"page_button_left_normal.tga",
					"over_image" : MAINFOLDER+"page_button_left_hover.tga",
					"down_image" : MAINFOLDER+"page_button_left_down.tga",
				},
				{
					"name" : "column3_page_nr",
					"type" : "image",
					"x" : 296,
					"y" : 308,
					"image" :  MAINFOLDER+"page_site_bg.tga",
					"style" : ("attach",),

					"width" : 24,
					"height" : 18,

					"children" :
					(	
						{
							"name" : "tx_column3_page_nr",
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
					"name" : "btn_column3_right", 
					"type" : "button",
					"x" : 323,
					"y" : 310,
					"default_image" : MAINFOLDER+"page_button_right_normal.tga",
					"over_image" : MAINFOLDER+"page_button_right_hover.tga",
					"down_image" : MAINFOLDER+"page_button_right_down.tga",
				},
			),
		},
	),
}