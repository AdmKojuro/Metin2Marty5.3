import uiScriptLocale

window = {
	"name" : "QuestionDropDialog",
	"style" : ("movable", "float",),

	"x" : SCREEN_WIDTH/2 - 125,
	"y" : SCREEN_HEIGHT/2 - 52,

	"width" : 250,
	"height" : 160,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 250,
			"height" : 160,

			"children" :
			(
				{
					"name" : "thinboard",
					"type" : "thinboard",

					"x" : 4,
					"y" : 4,

					"width" : 250 - 8,
					"height" : 160 - 8,
				},
				{
					"name" : "message",
					"type" : "text",

					"x" : 0,
					"y" : 25,

					"horizontal_align" : "center",
					"text" : uiScriptLocale.MESSAGE,

					"text_horizontal_align" : "center",
					"text_vertical_align" : "center",
				},
				{
					"name" : "ItemSlot",
					"type" : "slot",

					"x" : 25,
					"y" : 45,

					"width" : 32,
					"height" : 200,

					"horizontal_align" : "center",

					"image" : "d:/ymir work/ui/public/Slot_Base.sub",

					"slot" : (
						{"index":0, "x":0, "y":0, "width":32, "height":32},
						{"index":1, "x":0, "y":33, "width":32, "height":32},
						{"index":2, "x":0, "y":66, "width":32, "height":32},
					),
				},
				{
					"name" : "button_holder",
					"type" : "thinboard_circle",

					"x" : 20,
					"y" : 45,

					"width" : 160,
					"height" : 100 - 2,

					"horizontal_align" : "center",

					"children" :
					(
						{
							"name" : "accept",
							"type" : "button",

							"x" : 0,
							"y" : 8,

							"horizontal_align" : "center",
							"text" : uiScriptLocale.DROP_ITEM,

							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",
						},
						{
							"name" : "destroy",
							"type" : "button",

							"x" : 0,
							"y" : 8 + 30,

							"horizontal_align" : "center",
							"text" : uiScriptLocale.DESTROY,

							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",
						},
						{
							"name" : "cancel",
							"type" : "button",

							"x" : 0,
							"y" : 8 + 30 + 30,

							"horizontal_align" : "center",
							"text" : uiScriptLocale.NO,

							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",
						},
					),
				},
			),
		},
	),
}