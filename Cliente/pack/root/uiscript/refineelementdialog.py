import uiScriptLocale

window = {
	"name" : "RefineElementDialog",
	"style" : ("movable", "float",),

	"x" : SCREEN_WIDTH - 400,
	"y" : 70 * 800 / SCREEN_HEIGHT,

	"width" : 0,
	"height" : 0,

	"children" :
	(
		{
			"name" : "Board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 0,
			"height" : 0,

			"children" :
			(
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 0,
					"color" : "red",

					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",
							
							"x" : 0,
							"y" : 3,
							
							"text" : uiScriptLocale.REFINE_ELEMENT_UPGRADE_TITLE,
							
							"horizontal_align" : "center",
							"text_horizontal_align" : "center",
						},
					),
				},
				{
					"name" : "ItemSlot",
					"type" : "slot",
				
					"x" : 0,
					"y" : 0,
				
					"width" : 32,
					"height" : 32,
					
					"vertical_align" : "center",
					
					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
				
					"slot": (
						{ "index":0, "x":0, "y":0, "width":32, "height":32 },
						{ "index":1, "x":0, "y":32, "width":32, "height":32 },
						{ "index":2, "x":0, "y":64, "width":32, "height":32 },
					),
				},
				{
					"name" : "Cost",
					"type" : "text",
					
					"x" : 0,
					"y" : 54,
					
					"text" : uiScriptLocale.REFINE_COST,
					
					"horizontal_align" : "center",
					"vertical_align" : "bottom",
					"text_horizontal_align" : "center",
				},
				{
					"name" : "AcceptButton",
					"type" : "button",

					"x" : -35,
					"y" : 35,

					"text" : uiScriptLocale.OK,
					"horizontal_align" : "center",
					"vertical_align" : "bottom",

					"default_image" : "d:/ymir work/ui/public/Middle_Button_01.sub",
					"over_image" : "d:/ymir work/ui/public/Middle_Button_02.sub",
					"down_image" : "d:/ymir work/ui/public/Middle_Button_03.sub",
				},
				{
					"name" : "CancelButton",
					"type" : "button",

					"x" : 35,
					"y" : 35,

					"text" : uiScriptLocale.CANCEL,
					"horizontal_align" : "center",
					"vertical_align" : "bottom",

					"default_image" : "d:/ymir work/ui/public/Middle_Button_01.sub",
					"over_image" : "d:/ymir work/ui/public/Middle_Button_02.sub",
					"down_image" : "d:/ymir work/ui/public/Middle_Button_03.sub",
				},
			),
		},
	),
}