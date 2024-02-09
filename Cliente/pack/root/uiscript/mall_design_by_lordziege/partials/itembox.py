import uiScriptLocale

ITEMBOXWIDTH = 161+2
ITEMBOXHEIGHT = 102+14+2

MAINFOLDER = "locale/es/ui/mall/"
MAINFOLDER = "locale/es/ui/mall/design_by_lordziege/"

window = {
	"name" : "item_box",

	"x" : SCREEN_WIDTH/2-ITEMBOXWIDTH/2,
	"y" : SCREEN_HEIGHT /2-ITEMBOXHEIGHT/2,

	"style" : ("float",),

	"width" : ITEMBOXWIDTH,
	"height" : ITEMBOXHEIGHT,

	"children" :
	(
		{
			"name" : "red_sale",
			"type" : "image",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"image" : MAINFOLDER + "sale_button.tga",
		},
		{
			"name" : "time_box",
			"x" : 81+1,
			"y" : 102+1,

			"width":65,
			"height":14,

			"children" :
			(
				## Countdown
				{
					"name" : "tx_countdown",
					"type" : "text",
					"text" : "Xh Xm Xs",
					"text_horizontal_align" : "right",
					"horizontal_align" : "right",
					"x" : 3,
					"y" : 0,
				},
			),
		},
		{
			"name" : "percent_box",

			"x" : 17+1,
			"y" : 102+1,
			"width":64,
			"height":14,

			"children" : 
			(
				{
					"name" : "tx_percent",
					"type" : "text",
					"text" : "10%",
					"text_horizontal_align" : "left",
					"horizontal_align" : "left",
					"x" : 3,
					"y" : 0,
				},
			),
		},
		{
			"name" : "background",
			"type" : "image",
			"style" : ("attach",),

			"x" : 0+1,
			"y" : 0+1,

			"image" : MAINFOLDER + "item_box.tga",

			"children" :
			(
				## Itemname
				{
					"name" : "tx_item_name",
					"type" : "text",
					"text" : "XXX",
					"x" : 6,
					"y" : 3,
				},
				## Item icon
				{
					"name" : "item_icon",
					"type" : "image",
					"image" : MAINFOLDER + "empty_icon.tga",
					"x" : 3,
					"y" : 3,
				},
				{
					"name" : "item_price_box",

					"x" : 68,
					"y" : 3,
					"width":90,
					"height":18,

					"children" : 
					(
						{
							"name" : "tx_item_price",
							"type" : "text",
							"text" : "XXX",
							"x" : 3,
							"y" : 3,
							"text_horizontal_align" : "left",
							"horizontal_align" : "left",
						},
						{
							"name" : "tx_item_currency",
							"type" : "text",
							"text" : "COINS",
							"x" : 3,
							"y" : 3,
							"text_horizontal_align" : "right",
							"horizontal_align" : "right",
						},
					),
				},
				
				## preview button
				{
					"name" : "btn_preview", 
					"type" : "button",
					"x" : 37,
					"y" : 51,
					"text" : "Preview",
					"text_color" : 0xffF8BF24,
					"default_image" : MAINFOLDER+"buy_preview_button_normal.tga",
					"over_image" : MAINFOLDER+"buy_preview_button_hover.tga",
					"down_image" : MAINFOLDER+"buy_preview_button_down.tga",
				},
				## buy button
				{
					"name" : "btn_buy", 
					"type" : "button",
					"x" : 37,
					"y" : 76,
					"text" : "Buy",
					"default_image" : MAINFOLDER+"buy_preview_button_normal.tga",
					"over_image" : MAINFOLDER+"buy_preview_button_hover.tga",
					"down_image" : MAINFOLDER+"buy_preview_button_down.tga",
				},
				## amount
				{
					"name" : "amount_box",
					"type" : "image",
					"image": MAINFOLDER + "item_box_amount.tga",
					"x" : 37,
					"y" : 23,
					"width" : 90,	
					"height" : 18,

					"children" :
					(
						{
							"name" : "ed_amount",
							"type" : "editline",
									
							"width" : 90,
							"height" : 18,
							"input_limit" : 3,
							"enable_codepage" : 0,
									
							"x" : 32+2,
							"y" : 3,
						},
						{
							"name" : "tx_item_amount_text",
							"type" : "text",
							"text" : "pc(s).",
							"x" : 32+65,
							"y" : 3,
						},
					),
				},
				
			),
		},
	),
}