import uiScriptLocale
import app

ROOT = "d:/ymir work/ui/minimap/"

window = {
	"name" : "MiniMap",

	"x" : SCREEN_WIDTH - 148,
	"y" : 0,

	"width" : 148,
	"height" : 160,

	"children" :
	[
		## OpenWindow
		{
			"name" : "OpenWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 148,
			"height" : 160,

			"children" :
			[
				{
					"name" : "OpenWindowBGI",
					"type" : "image",
					"x" : 0,
					"y" : 0,
					"image" : ROOT + "minimap.sub",
				},
				## MiniMapWindow
				{
					"name" : "MiniMapWindow",
					"type" : "window",

					"x" : 4,
					"y" : 5,

					"width" : 128,
					"height" : 128,
				},
				## ScaleUpButton
				{
					"name" : "ScaleUpButton",
					"type" : "button",

					"x" : 101,
					"y" : 116,

					"default_image" : ROOT + "minimap_scaleup_default.sub",
					"over_image" : ROOT + "minimap_scaleup_over.sub",
					"down_image" : ROOT + "minimap_scaleup_down.sub",
				},
				## ScaleDownButton
				{
					"name" : "ScaleDownButton",
					"type" : "button",

					"x" : 115,
					"y" : 103,

					"default_image" : ROOT + "minimap_scaledown_default.sub",
					"over_image" : ROOT + "minimap_scaledown_over.sub",
					"down_image" : ROOT + "minimap_scaledown_down.sub",
				},
				## MiniMapHideButton
				{
					"name" : "MiniMapHideButton",
					"type" : "button",

					"x" : 111,
					"y" : 6,

					"default_image" : ROOT + "minimap_close_default.sub",
					"over_image" : ROOT + "minimap_close_over.sub",
					"down_image" : ROOT + "minimap_close_down.sub",
				},
				## AtlasShowButton
				{
					"name" : "AtlasShowButton",
					"type" : "button",

					"x" : 12,
					"y" : 12,

					"default_image" : ROOT + "atlas_open_default.sub",
					"over_image" : ROOT + "atlas_open_over.sub",
					"down_image" : ROOT + "atlas_open_down.sub",
				},
				{
					"name" : "textInfoBar",
					"type" : "bar",
					"x" : 8,
					"y" : 140,
					"color" : 0x64000000,
					"width" : 132,
					"height" : 28,
					"children" : 
					(
						{
							"name" : "textInfoValue1",
							"type" : "text",
							"x" : 0,
							"y" : 1,
							"text" : "",
							"outline" : 1,
							"horizontal_align" : "center",
							"text_horizontal_align" : "center"
						},
						{
							"name" : "textInfoValue2",
							"type" : "text",
							"x" : 0,
							"y" : 12,
							"text" : "",
							"outline" : 1,
							"horizontal_align" : "center",
							"text_horizontal_align" : "center"
						},
					),
				},
				{
					"name" : "textInfoBar2",
					"type" : "bar",
					"x" : 8,
					"y" : 140 + 26,
					"color" : 0x00000000,
					"width" : 132,
					"height" : 28,
					"children" : 
					(
						{
							"name" : "ServerInfo",
							"type" : "text",
							"x" : 0,
							"y" : 1,
							"text" : "",
							"outline" : 1,
							"horizontal_align" : "center",
							"text_horizontal_align" : "center"
						},
						{
							"name" : "MiniMapClock",
							"type" : "text",
							"x" : 0,
							"y" : 12,
							"text" : "",
							"outline" : 1,
							"horizontal_align" : "center",
							"text_horizontal_align" : "center"
						},
						{
							"name" : "TimeInfo",
							"type" : "text",
							"x" : 0,
							"y" : 12,
							"text" : "",
							"outline" : 1,
							"horizontal_align" : "center",
							"text_horizontal_align" : "center"
						},
					),
				},
				## PositionInfo
				{
					"name" : "PositionInfo",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 160,

					"text" : "",
				},
				## ObserverCount
				{
					"name" : "ObserverCount",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 180,

					"text" : "",
				},
				{
					"name" : "bio",
					"type" : "button",
					"x" : -9,
					"y" : 38,
					#"default_image" : "d:/ymir work/ui/game/minimapa/biolog_0.tga",
					#"over_image" : "d:/ymir work/ui/game/minimapa/biolog_2.tga",
					#"down_image" : "d:/ymir work/ui/game/minimapa/biolog_1.tga",
					##"tooltip_text": localeInfo.BIO_TITLE,
				},
			],
		},
		{
			"name" : "CloseWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 132,
			"height" : 48,

			"children" :
			[
				## ShowButton
				{
					"name" : "MiniMapShowButton",
					"type" : "button",

					"x" : 100,
					"y" : 4,

					"default_image" : ROOT + "minimap_open_default.sub",
					"over_image" : ROOT + "minimap_open_default.sub",
					"down_image" : ROOT + "minimap_open_default.sub",
				},
			],
		},
	],
}

window["children"][0]["children"] = window["children"][0]["children"] + [
		## InGameEventButton
		{
			"name" : "InGameEventButton",
			"type" : "button",

			"x" : 0,
			"y" : 35,

			"tooltip_text" : uiScriptLocale.EVENT_ALARM_TITLE,
			"default_image" : "d:/ymir work/ui/minimap/E_open_default.tga",
			"over_image" : "d:/ymir work/ui/minimap/E_open_over.tga",
			"down_image" : "d:/ymir work/ui/minimap/E_open_down.tga",
		},
	]

if app.ENABLE_DUNGEON_INFO_SYSTEM:
	window["children"][0]["children"] = window["children"][0]["children"] + [
		{
			"name" : "DungeonInfoShowButton",
			"type" : "button",

			"x" : 80,
			"y" : 120,

			#"default_image" : "d:/ymir work/ui/game/dungeon_info/minimap/" + "minimap_dungeon_info_default.tga",
			#"over_image" : "d:/ymir work/ui/game/dungeon_info/minimap/" + "minimap_dungeon_info_over.tga",
			#"down_image" : "d:/ymir work/ui/game/dungeon_info/minimap/" + "minimap_dungeon_info_down.tga",
		},
	]
