import uiScriptLocale
import player

WINDOW_X = 486
WINDOW_Y = 259

CHANGE_EQUIP_START_INDEX = player.CHANGE_EQUIP_SLOT_START
PATCH_DESIGN = "fast_equip/design/"

window = {
	"name" : "FastEquipWindows",

	"x" : SCREEN_WIDTH/2-(WINDOW_X/2),
	"y" : SCREEN_HEIGHT/2-(WINDOW_Y/2),

	"style" : ("movable", "float"),

	"width" : WINDOW_X,
	"height" : WINDOW_Y,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" :WINDOW_X,
			"height" : WINDOW_Y,

			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",

					"x" : 8,
					"y" : 7,

					"width" : WINDOW_X-15,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":WINDOW_X/2, "y":3, "text":"Fast Equip", "text_horizontal_align":"center" },
					),
				},

				{
					"name" : "thinboard_presets",
					"type" : "thinboard_circle",

					"x" : 11,
					"y" : 33,

					"width" :184,
					"height" : 214,

					"children":
					(	
						{"name": "bar_p","type": "image","x" : 2,"y" : 2,"image" : PATCH_DESIGN+"bar_board_p.tga",
						"children":
						(	
							{ "name":"title_bar1", "type":"text", "x":90, "y":4, "text":"PRESETS", "text_horizontal_align":"center" },
						),
						},

						{"name": "bg_button","type": "image","x" : 2,"y" : 178,"image" : PATCH_DESIGN+"bg_button_change.tga"},

						{
							"name" : "Change_Equipament",
							"type" : "button",

							"x" : 32,
							"y" : 186,

							"text": "Change Equip",

							"default_image" : PATCH_DESIGN+"button_change.tga",
							"over_image" :  PATCH_DESIGN+"button_change_h.tga",
							"down_image" : PATCH_DESIGN+"button_change.tga",

						},

					),
				},


				{
					"name" : "thinboard_slots",
					"type" : "thinboard_circle",

					"x" : 197,
					"y" : 33,

					"width" :277,
					"height" : 214,

					"children":
					(	
						{"name": "bar_s","type": "image","x" : 2,"y" : 2,"image" : PATCH_DESIGN+"bar_board_s.tga",
						"children":
						(	
							{ "name":"title_bar_s", "type":"text", "x":130, "y":4, "text":"PRESETS", "text_horizontal_align":"center" },
						),
						},

						{"name": "item_slot_img","type": "image","x" : 3,"y" : 24,"image" : PATCH_DESIGN+"bg_slots.tga",
						"children":
						(
							{
								"name": "Equipament",
								"type": "slot",
								"x" : 3,
								"y" : 2,
								"slot" : (
											{"index":CHANGE_EQUIP_START_INDEX+0, "x":39, "y":37, "width":32, "height":64},
											{"index":CHANGE_EQUIP_START_INDEX+1, "x":39, "y":2, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+2, "x":39, "y":145, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+3, "x":75, "y":67, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+4, "x":3, "y":3, "width":32, "height":96},
											{"index":CHANGE_EQUIP_START_INDEX+5, "x":114, "y":67, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+6, "x":114, "y":35, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+7, "x":2, "y":145, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+8, "x":75, "y":145, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+9, "x":114, "y":2, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+10, "x":75, "y":35, "width":32, "height":32},

											{"index":CHANGE_EQUIP_START_INDEX+19, "x":220, "y":46, "width":32, "height":64},
											{"index":CHANGE_EQUIP_START_INDEX+20, "x":220, "y":9, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+21, "x":171, "y":14, "width":32, "height":96},
											{"index":CHANGE_EQUIP_START_INDEX+22, "x":5, "y":126, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+23, "x":70, "y":127, "width":32, "height":32},
											{"index":CHANGE_EQUIP_START_INDEX+27, "x":39, "y":106, "width":32, "height":32},


								),

								"width": 271,
								"height": 187,
							},
						),

						},
					),
				},
			),
		},
	),
}