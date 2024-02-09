import uiScriptLocale
import item
import app

COSTUME_START_INDEX = item.COSTUME_SLOT_START

window = {
	"name" : "CostumeWindow",

	"x" : SCREEN_WIDTH - 175 - 140,
	"y" : SCREEN_HEIGHT - 37 - 565,

	"style" : ("float",),

	"width" : 140,
	"height" : (180 + 47), #±âÁ¸º¸´Ù 47 ±æ¾îÁü

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			# "style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 140,
			"height" : (180 + 47),
		
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : 130,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":60, "y":3, "text":uiScriptLocale.COSTUME_WINDOW_TITLE, "text_horizontal_align":"center" },
					),
				},

				## Equipment Slot
				{
					"name" : "Costume_Base",
					"type" : "image",

					"x" : 13,
					"y" : 38,
				
					"image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "costume/new_costume_bg.jpg",					

					"children" :
					(

						{
							"name" : "CostumeSlot",
							"type" : "slot",

							"x" : 3,
							"y" : 3,

							"width" : 127,
							"height" : 160,

							"slot" : (
										{"index":item.COSTUME_SLOT_BODY, "x":62, "y":45, "width":32, "height":64},
										{"index":item.COSTUME_SLOT_HAIR, "x":62, "y": 8, "width":32, "height":32},
										{"index":item.COSTUME_SLOT_WEAPON, "x":12, "y":14, "width":32, "height":96},
										{"index":item.COSTUME_SLOT_MOUNT, "x":6, "y":127, "width":32, "height":32},
										{"index":item.COSTUME_SLOT_ACCE, "x":70, "y":127, "width":32, "height":32},
										{"index":item.COSTUME_SLOT_AURA, "x":38, "y":127, "width":32, "height":32},
									),
						},
					),
				},
			),
		},
	),
}
