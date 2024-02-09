import uiScriptLocale

ITEMBOXWIDTH = 126
ITEMBOXHEIGHT = 70

MAINFOLDER = "locale/es/ui/mall/"

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
			"name" : "background",
			"type" : "image",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"image" : MAINFOLDER + "item_main_box_norm.sub",

			"children" :
			(
				{
					"name" : "time_box",
					"type" : "image",
					"x" : 0,
					"y" : -19,
					"image" : MAINFOLDER+"grey_medium_slotbar.sub",

					"children" :
					(
						## Countdown
						{
							"name" : "tx_countdown",
							"type" : "text",
							"text" : "Xh Xm Xs",
							"x" : 20,
							"y" : 3,
						},
					),
				},
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
					"name" : "icon_box",
					"type" : "image",
					"x" : 4,
					"y" : 17,
					"image" : MAINFOLDER+"item_icon_box.sub",

					"children" :
					(
						{
							"name" : "item_icon",
							"type" : "expanded_image",
							"x" : 7,
							"y" : 2,
							"image" : MAINFOLDER+"item_icon_box.sub",
						},
						{
							"name" : "tx_item_price",
							"type" : "text",
							"text" : "XXX COINS",
							"x" : 7,
							"y" : 35,
						},
					),
				},
				
				## preview button
				{
					"name" : "btn_preview", 
					"type" : "button",
					"x" : 67,
					"y" : 38,
					"text" : "Preview",
					"text_color" : 0xffF8BF24,
					"default_image" : MAINFOLDER+"btn_buy_norm.sub",
					"over_image" : MAINFOLDER+"btn_buy_hover.sub",
					"down_image" : MAINFOLDER+"btn_buy_press.sub",
				},
				## buy button
				{
					"name" : "btn_buy", 
					"type" : "button",
					"x" : 67,
					"y" : 53,
					"text" : "Buy",
					"default_image" : MAINFOLDER+"btn_buy_norm.sub",
					"over_image" : MAINFOLDER+"btn_buy_hover.sub",
					"down_image" : MAINFOLDER+"btn_buy_press.sub",
				},
				## amount
				{
					"name" : "amount_box",
					"type" : "image",
					"x" : 66,
					"y" : 23,
					"image": MAINFOLDER + "black_small_slotbar.sub",
							
					"children" :
					(
						{
							"name" : "ed_amount",
							"type" : "editline",
									
							"width" : 57,
							"height" : 18,
							"input_limit" : 3,
							"enable_codepage" : 0,
									
							"x" : 3,
							"y" : 1,
						},
						{
							"name" : "tx_item_amount_text",
							"type" : "text",
							"text" : "pc(s).",
							"x" : 35,
							"y" : 1,
						},
					),
				},
				{
					"name" : "percent_box",
					"type" : "image",
					"x" : 90,
					"y" : -15,
					"image" : MAINFOLDER+"discount_percent_box.sub",
					
					"children" : 
					(
						{
							"name" : "tx_percent",
							"type" : "text",
							"text" : "10%",
							"fontsize" : "LARGE",
							"outline" : "true",
							"r": 1.000,
							"g": 0.549,
							"b": 0.000,
							"x" : 5,
							"y" : 5,
						},
					),
				},
			),
		},
	),
}