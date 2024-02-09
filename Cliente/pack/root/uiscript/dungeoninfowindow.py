#
# Title: Dungeon Information System
# Author: Owsap
# Description: List of all available dungeons.
# Date: 2021.01.09
# Last Update: 2021.06.03
# Version 2.0.0.2
#
# Skype: owsap.
# Discord: Owsap#0905
#
# 0x0
#
# Web: https://owsap-productions.com/
# GitHub: https://github.com/Owsap
#

import app
import uiScriptLocale

BOARD_WIDTH = 465
BOARD_HEIGHT = 590

INNER_RIGHT_BOARD_WIDTH = 190
INNER_RIGHR_BOARD_HEIGH = 265

INNER_RIGHT_BOARD_X = INNER_RIGHT_BOARD_WIDTH + 4
INNER_RIGHR_BOARD_Y = INNER_RIGHR_BOARD_HEIGH

ROOT = "d:/ymir work/ui/game/dungeon_info/"

window = {
	"name" : "DungeonInfoWindow",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGHT,

	"children" :
	(
		## Main Window Board
		{
			"name" : "Board",
			"type" : "board",
			"style" : ("attach", "ltr"),

			"x" : 0,
			"y" : 0,

			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGHT,

			"children" :
			(
				## Main Window Title Bar
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 7,

					"width" : BOARD_WIDTH - 13,

					"children" :
					(
						{
							"name" : "TitleText",
							"type" : "text",

							"x" : 0,
							"y" : -2,

							"text" : uiScriptLocale.DUNGEON_INFO_TITLE,
							"all_align" : "center"
						},
					),
				},

				{
					## Board Container
					"name" : "BoardContainer",
					"type" : "image",
					"style" : ("attach",),

					"x" : 0,
					"y" : 32,

					"image" : ROOT + "inner_bg.jpg",
					"horizontal_align" : "center",

					"children" :
					(
						## Dungeon Title Name ( Background Image )
						{
							"name" : "TitleBackgroundImage",
							"type" : "image",
							"style" : ("attach",),

							"x" : 0,
							"y" : 3,
							"horizontal_align" : "center",

							"image" : ROOT + "title_bg.png",

							"children" : (
								## Dungeon Title Name ( Text )
								{
									"name" : "TitleNameText",
									"type" : "text",

									"x" : 10,
									"y" : 2,
									"text_horizontal_align" : "left",

									"text" : "",
									"fontname" : "Verdana:17",
									"color" : 0xFFC1C1C1,
									"outline" : 1,
								},


							),
						},

						## Dungeon Preview / Render Image ( Render / Image )
						{
							"name" : "PreviewBackgroundImg",
							"type" : "image",
							"style" : ("attach",),

							"x" : INNER_RIGHT_BOARD_X,
							"y" : 28,
							"horizontal_align" : "right",

							"image" : ROOT + "preview_bg.jpg",

							"children" : (
								## Dungeon Preview Name ( Text )
								{
									"name" : "PreviewNameText",
									"type" : "text",

									"x" : INNER_RIGHT_BOARD_X / 2,
									"y" : 2,
									"text_horizontal_align" : "center",

									"text" : "",
									"fontname" : "Verdana:17",
									"color" : 0xFFC1C1C1,
									"outline" : 1,
								},
							),
						},
						## Dungeon Preview ( Render )
						{
							"name" : "PreviewRender",
							"type" : "render_target",

							"x" : INNER_RIGHT_BOARD_X,
							"y" : 50,
							"horizontal_align" : "right",

							"width" : INNER_RIGHT_BOARD_WIDTH,
							"height" : INNER_RIGHR_BOARD_HEIGH - 55,

							"index" : 2,

							"children" : (
								{
									"name" : "PreviewRenderZoomOutButton",
									"type" : "toggle_button",

									"x" : 18,
									"y" : 18,
									"horizontal_align" : "right",
									"vertical_align" : "bottom",

									"tooltip_text" : uiScriptLocale.DUNGEON_INFO_PREVIEW_ZOOM_IN,
									"tooltip_x" : 0,
									"tooltip_y" : -20,

									"default_image" : "d:/ymir work/ui/game/monster_card/button/zoomin/zoomin_rotation_button_default.sub",
									"over_image" : "d:/ymir work/ui/game/monster_card/button/zoomin/zoomin_rotation_button_over.sub",
									"down_image" : "d:/ymir work/ui/game/monster_card/button/zoomin/zoomin_rotation_button_down.sub",
								},
								{
									"name" : "PreviewRenderZoomInButton",
									"type" : "toggle_button",

									"x" : 18 + 18,
									"y" : 18,
									"horizontal_align" : "right",
									"vertical_align" : "bottom",

									"tooltip_text" : uiScriptLocale.DUNGEON_INFO_PREVIEW_ZOOM_OUT,
									"tooltip_x" : 0,
									"tooltip_y" : -20,

									"default_image" : "d:/ymir work/ui/game/monster_card/button/zoomout/zoomin_rotation_button_default.sub",
									"over_image" : "d:/ymir work/ui/game/monster_card/button/zoomout/zoomin_rotation_button_over.sub",
									"down_image" : "d:/ymir work/ui/game/monster_card/button/zoomout/zoomin_rotation_button_down.sub",
								},
							),
						},

						{
							"name" : "ScrollButton",
							"type" : "expanded_image",

							"x" : 130,
							"y" : 26,

							"width" : 32,
							"height" : 128,

							"vertical_align" : "bottom",

							"image" : ROOT + "scroll_down.png",
						},

						{
							"name" : "ScrollBar",
							"type" : "scrollbar",

							"x" : 240 - 11,
							"y" : 30,

							"size" : 485,
						},

						## Dungeon Button List ( ThinBoard )
						{
							"name" : "ButtonListThinBoard",
							"type" : "thinboard_circle",

							"x" : 3,
							"y" : 28,

							"width" : 240,
							"height" : 485,

							"children" : (
								## ...
							),
						},

						## Dungeon Information Window
						{
							"name" : "InformationWindow",
							"type" : "window",

							"x" : INNER_RIGHT_BOARD_X,
							"y" : INNER_RIGHR_BOARD_HEIGH,
							"horizontal_align" : "right",

							"width" : INNER_RIGHT_BOARD_WIDTH,
							"height" : INNER_RIGHR_BOARD_HEIGH + 15,

							"children" : (
								## Dungeon Rank Score Button
								# {
									# "name" : "RankScoreButton",
									# "type" : "button",

									# "x" : INNER_RIGHT_BOARD_WIDTH / 2 - 96,
									# "y" : 0,

									# "tooltip_text" : uiScriptLocale.DUNGEON_RANKING_TYPE_TOOL_TIP_01,
									# "tooltip_x" : 0,
									# "tooltip_y" : -20,

									# "default_image" : ROOT + "button/rank_button00.sub",
									# "over_image" : ROOT + "button/rank_button01.sub",
									# "down_image" : ROOT + "button/rank_button02.sub",
								# },
								# ## Dungeon Rank Time Button
								# {
									# "name" : "RankTimeButton",
									# "type" : "button",

									# "x" : INNER_RIGHT_BOARD_WIDTH / 2 - 96 + 65,
									# "y" : 0,

									# "tooltip_text" : uiScriptLocale.DUNGEON_RANKING_TYPE_TOOL_TIP_02,
									# "tooltip_x" : 0,
									# "tooltip_y" : -20,

									# "default_image" : ROOT + "button/rank_button10.sub",
									# "over_image" : ROOT + "button/rank_button11.sub",
									# "down_image" : ROOT + "button/rank_button12.sub",
								# },
								# ## Dungeon Rank Damage Button
								# {
									# "name" : "RankDamageButton",
									# "type" : "button",

									# "x" : INNER_RIGHT_BOARD_WIDTH / 2 - 96 + 65 + 65,
									# "y" : 0,

									# "tooltip_text" : uiScriptLocale.DUNGEON_RANKING_TYPE_TOOL_TIP_03,
									# "tooltip_x" : 0,
									# "tooltip_y" : -20,

									# "default_image" : ROOT + "button/rank_button20.sub",
									# "over_image" : ROOT + "button/rank_button21.sub",
									# "down_image" : ROOT + "button/rank_button22.sub",
								# },

								## Dungeon Required Items
								{
									"name" : "RequiredItemBackgroundImg",
									"type" : "image",
									"style" : ("attach",),

									"x" : 191,
									"y" : 25,

									"image" : ROOT + "item_slots_bg.jpg",
									"horizontal_align" : "right",

									"children" : (
										## Dungeon Required Items ( Text )
										{
											"name" : "RequiredItemText",
											"type" : "text",

											"x" : 12,
											"y" : 18,

											"text" : uiScriptLocale.DUNGEON_INFO_ITEMS,
										},
										## Dungeon Required Items ( Slots )
										{
											"name" : "RequiredItemSlot",
											"type" : "slot",

											"x" : 57,
											"y" : 10,

											"width" : 130,
											"height" : 32,

											"image" : "d:/ymir work/ui/pet/skill_button/skill_enable_button.sub",

											"slot" : (
												{ "index" : 0, "x" : 0, "y" : 0, "width" : 32, "height" : 32 },
												{ "index" : 1, "x" : 32 + 12, "y" : 0, "width" : 32, "height" : 32 },
												{ "index" : 2, "x" : 64 + 23, "y" : 0, "width" : 32, "height" : 32 },
											),
										},
									)
								},

								{
									"name" : "MyPointsWindow",
									"type" : "window",

									"x" : INNER_RIGHT_BOARD_X - 3,
									"y" : -85 + 30,
									"horizontal_align" : "right",

									"width" : INNER_RIGHT_BOARD_WIDTH,
									"height" : 0,

									"children" : (
										{
											"name" : "DungeonInfoPersonal",
											"type" : "image",

											"x" : 0,
											"y" : 85,
											"horizontal_align" : "center",

											"image" : "d:/ymir work/ui/pattern/seperator.tga",

											"children" :
											(
												# Personal Stats
												{
													"name" : "DungeonInfoPersonalStats",
													"type" : "text",

													"x" : 0,
													"y" : -5,
													"all_align" : "center",

													"text" : uiScriptLocale.DUNGEON_INFO_MY_RESULTS,

													"fontname" : "Verdana:16",
													"color" : 0xFFFEE3AE,
													"outline" : 1,
												},
											),
										},
										{
											"name" : "MyPointsThinBoard",
											"type" : "thinboard",

											"x" : INNER_RIGHT_BOARD_WIDTH,
											"y" : 85 + 30,

											"width" : INNER_RIGHT_BOARD_WIDTH,
											"height" : 125,

											"horizontal_align" : "right",

											"children" : (
												## Dungeon Total Finished
												{
													"name" : "TotalFinishedText", "type" : "text", "x" : 10, "y" : 10,
													"text" : "",
												},
												## Dungeon Fastest Time
												{
													"name" : "FastestTimeText", "type" : "text", "x" : 10, "y" : 10 + 15 * 1,
													"text" : "",
												},
												## Dungeon Highest Damage
												{
													"name" : "HighestDamageText", "type" : "text", "x" : 10, "y" : 10 + 15 * 2,
													"text" : "",
												},
											)
										},
									),
								},
								####################################################
								####################################################

								{
									"name" : "InformationThinBoard",
									"type" : "thinboard",

									"x" : INNER_RIGHT_BOARD_WIDTH,
									"y" : 85 + 30,

									"width" : INNER_RIGHT_BOARD_WIDTH,
									"height" : 125,

									"horizontal_align" : "right",

									"children" : (
										## Dungeon Type
										{
											"name" : "TypeText", "type" : "text", "x" : 10, "y" : 10,
											"text" : "",
										},
										## Dungeon Level Limit
										{
											"name" : "LevelLimitText", "type" : "text", "x" : 10, "y" : 10 + 15 * 1,
											"text" : "",
										},
										## Dungeon Party Limit ( Members )
										{
											"name" : "MemberLimitText", "type" : "text", "x" : 10, "y" : 10 + 15 * 2,
											"text" : "",
										},
										## Dungeon Duration
										{
											"name" : "DurationText", "type" : "text", "x" : 10, "y" : 10 + 15 * 3,
											"text" : "",
										},
										## Dungeon Cooldown
										{
											"name" : "CooldownText", "type" : "text", "x" : 10, "y" : 10 + 15 * 4,
											"text" : "",
										},
										## Dungeon Location Map Name
										{
											"name" : "LocationText", "type" : "text", "x" : 10, "y" : 10 + 15 * 5,
											"text" : "",
										},
										## Dungeon Entrace Map Name
										{
											"name" : "EntraceText", "type" : "text", "x" : 10, "y" : 10 + 15 * 6,
											"text" : "",
										},
									)
								},

								## Teleport (Warp) ( Button )
								{
									"name" : "WarpButton",
									"type" : "button",

									"x" : 5,
									"y" : 85,
									"horizontal_align" : "left",

									"tooltip_text" : uiScriptLocale.DUNGEON_INFO_GO_TO_ENTRANCE_TOOL_TIP,
									"tooltip_x" : 0,
									"tooltip_y" : -20,

									"default_image" : "d:/ymir work/ui/minigame/miniboss/btn_enter1.sub",
									"over_image" : "d:/ymir work/ui/minigame/miniboss/btn_enter2.sub",
									"down_image" : "d:/ymir work/ui/minigame/miniboss/btn_enter3.sub",
								},

								## Dungeon Box Drop ( Button )
								{
									"name" : "BoxButton",
									"type" : "toggle_button",

									"x" : 60,
									"y" : 90,
									"horizontal_align" : "center",

									"tooltip_text" : uiScriptLocale.DUNGEON_INFO_DROPS_TOOL_TIP,
									"tooltip_x" : 0,
									"tooltip_y" : -20,

									"default_image" : "d:/ymir work/ui/game/windows/Mall_Button_01.sub",
									"over_image" : "d:/ymir work/ui/game/windows/Mall_Button_02.sub",
									"down_image" : "d:/ymir work/ui/game/windows/Mall_Button_03.sub",
								},

								## Dungeon Box Drop ( Window )
								{
									"name" : "BoxWindow",
									"type" : "window",

									"x" : 5,
									"y" : 105,
									"horizontal_align" : "left",

									"width" : 180,
									"height" : 140,

									"children" : (
										{
											"name" : "BoxBackgroundImg",
											"type" : "expanded_image",

											"x" : 0,
											"y" : 0,

											"width" : 150,
											"height" : 140,

											"image" : "d:/ymir work/ui/game/belt_inventory/bg.tga",

											"children" :
											(
												{
													"name" : "BoxItemSlot",
													"type" : "grid_table",

													"x" : 7,
													"y" : 6,

													"start_index" : 0,
													"x_count" : 4,
													"y_count" : 4,
													"x_step" : 32,
													"y_step" : 32,

													"image" : "d:/ymir work/ui/pet/skill_button/skill_enable_button.sub",
												},
											),
										},
									)
								},

								## Dungeon Element Icon ( Image )
								{
									"name" : "ElementalImage",
									"type" : "image",
									"style" : ("attach",),

									"x" : 25, #0,
									"y" : 85,
									"horizontal_align" : "center",

									"image" : "d:/ymir work/ui/game/12zi/element/dark.sub",
								},

								## Tab Buttons ( Dungeon Information | Personal Dungeon Points )
								{
									"name" : "TabButtonImage",
									"type" : "image",
									"style" : ("attach",),

									"x" : 191,
									"y" : 31,
									"horizontal_align" : "right",
									"vertical_align" : "bottom",

									"image" : ROOT + "tab1.png",

									"children" : (
										{
											"name" : "TabButton1",
											"type" : "button",

											"x" : 5,
											"y" : 2,

											"width" : 80,
											"height" : 25,
										},
										{
											"name" : "TabButton2",
											"type" : "button",

											"x" : 87,
											"y" : 2,

											"width" : 80,
											"height" : 25,
										},
									)
								},
							),
						},

						## Help ToolTip Button
						{
							"name" : "HelpToolTipButton",
							"type" : "button",

							"x" : BOARD_WIDTH - 50,
							"y" : 5,

							"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
							"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
							"down_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
						},
					),
				},
			)
		},
	)
}
