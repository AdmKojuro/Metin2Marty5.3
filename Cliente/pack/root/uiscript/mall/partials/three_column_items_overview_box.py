import uiScriptLocale

BOXWIDTH = 402
BOXHEIGHT = 448

MAINFOLDER = "locale/es/ui/mall/"

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
			"image" :  MAINFOLDER+"three_column_box.sub",
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
					"x" : 2,
					"y" : 2,

					"image" :  MAINFOLDER+"red_large_thin_slotbar.sub",
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
					"x" : 5,
					"y" : 107,
					"default_image" : MAINFOLDER+"arrow_left_norm.sub",
					"over_image" : MAINFOLDER+"arrow_left_hover.sub",
					"down_image" : MAINFOLDER+"arrow_left_press.sub",
				},
				{
					"name" : "column1_page_nr",
					"type" : "image",
					"x" : 182,
					"y" : 112,
					"image" :  MAINFOLDER+"page_count_box.sub",
					"style" : ("attach",),

					"width" : 38,
					"height" : 22,

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
					"x" : 360,
					"y" : 107,
					"default_image" : MAINFOLDER+"arrow_right_norm.sub",
					"over_image" : MAINFOLDER+"arrow_right_hover.sub",
					"down_image" : MAINFOLDER+"arrow_right_press.sub",
				},

				{
					"name" : "column2_title",
					"type" : "image",
					"x" : 2,
					"y" : 156,

					"image" :  MAINFOLDER+"red_large_thin_slotbar.sub",
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
					"x" : 5,
					"y" : 261,
					"default_image" : MAINFOLDER+"arrow_left_norm.sub",
					"over_image" : MAINFOLDER+"arrow_left_hover.sub",
					"down_image" : MAINFOLDER+"arrow_left_press.sub",
				},
				{
					"name" : "column2_page_nr",
					"type" : "image",
					"x" : 182,
					"y" : 266,
					"image" :  MAINFOLDER+"page_count_box.sub",
					"style" : ("attach",),

					"width" : 38,
					"height" : 22,

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
					"x" : 360,
					"y" : 261,
					"default_image" : MAINFOLDER+"arrow_right_norm.sub",
					"over_image" : MAINFOLDER+"arrow_right_hover.sub",
					"down_image" : MAINFOLDER+"arrow_right_press.sub",
				},
				
				{
					"name" : "column3_title",
					"type" : "image",
					"x" : 2,
					"y" : 310,

					"image" :  MAINFOLDER+"red_large_thin_slotbar.sub",
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
					"x" : 5,
					"y" : 415,
					"default_image" : MAINFOLDER+"arrow_left_norm.sub",
					"over_image" : MAINFOLDER+"arrow_left_hover.sub",
					"down_image" : MAINFOLDER+"arrow_left_press.sub",
				},
				{
					"name" : "column3_page_nr",
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