# (c) 2019 Owsap Productions

import uiScriptLocale

BOARD_WIDTH = 325
BOARD_HEIGHT = 340 + 14

THINBOARD_WIDTH = 310
THINBOARD_HEIGHT = 280 + 14

BUTTON_GAP = 12.5

COLOR_PICKER_WIDTH = 256
COLOR_PICKER_HEIGHT = 256

window = {
	"name" : "SkillColorWindow",
	"style" : ("movable", "float",),

	"x" : (SCREEN_WIDTH - BOARD_WIDTH) / 2,
	"y" : (SCREEN_HEIGHT - BOARD_HEIGHT) / 2,

	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGHT,

	"children" :
	(
		## Main Board
		{
			"name" : "Board",
			"type" : "board_with_titlebar",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGHT,

			"title" : uiScriptLocale.SKILL_COLOR_TITLE,

			"children" :
			(
				## Thinboard (container)
				{
					"name" : "ThinBoard",
					"type" : "thinboard_circle",

					"x" : 13,
					"y" : 35,

					"width" : THINBOARD_WIDTH - 15,
					"height" : THINBOARD_HEIGHT - 15,

					"children" :
					(
						## Background Color Bar
						{
							"name" : "BGColorBar",
							"type" : "bar",

							"x" : 0,
							"y" : 0,

							"width" : THINBOARD_WIDTH - 15,
							"height" : THINBOARD_HEIGHT - 15,

							"color" : 0xff303030,
						},

						## Background Image
						{
							"name" : "BGImage",
							"type" : "image",

							"x" : 2.5,
							"y" : 2.5,

							"image" : "d:/ymir work/ui/skillcolor/background_expanded.tga",
						},

						## Background Color Picker Image (p1)
						{
							"name" : "BGColorPickerImage",
							"type" : "image",

							"x" : 35,
							"y" : 20,

							"width" : COLOR_PICKER_WIDTH,
							"height" : COLOR_PICKER_HEIGHT,

							"image" : "d:/ymir work/ui/skillcolor/color_picker_background.tga",

							"children" :
							(
								## Color Picker Button
								{
									"name" : "BGColorPickerButton",
									"type" : "button",

									"x" : 0,
									"y" : 0,

									"width" : COLOR_PICKER_WIDTH,
									"height" : COLOR_PICKER_HEIGHT,
								},
								## Color Picker Dot Image
								{
									"name" : "BGColorPickerDotImage",
									"type" : "image",

									"x" : 0,
									"y" : 0,

									"width" : 12,
									"height" : 12,

									"image" : "d:/ymir work/ui/skillcolor/color_picker_dot.tga",
								},
							),
						},

						## Background2 Image (p2)
						{
							"name" : "BG2Image",
							"type" : "image",

							"x" : 2.5,
							"y" : 2.5,

							"image" : "d:/ymir work/ui/skillcolor/background_expanded.tga",

							"children" :
							(
								## Background2 Window (container)
								{
									"name" : "BG2Window",
									"type" : "window",

									"x" : 70 + 1,
									"y" : 30,

									"width" : 150,
									"height" : 172.5,

									"children" :
									(
										## Preset Selection
										{
											"name" : "BG2ColorPresetSlotImage",
											"type" : "expanded_image",
											"style" : ("attach",),

											"x" : 0,
											"y" : 0,

											"width" : COLOR_PICKER_WIDTH,
											"height" : COLOR_PICKER_HEIGHT,

											"image" : "d:/ymir work/ui/pet/Pet_Incu_001.tga",

											"children" :
											(
												{
													"name" : "BG2ColorPresetTitle",
													"type" : "text",

													"x" : 0,
													"y" : -10,

													"text" : uiScriptLocale.SKILL_COLOR_PRESET_TITLE,
													"all_align" : "center"
												},
												{
													"name" : "BG2ColorPresetEditLine",
													"type" : "button",

													"x" : 13,
													"y" : 30 - 2,

													"width" : 136,
													"height" : 16,

													"text" : uiScriptLocale.SKILL_COLOR_PRESET_SELECT,
												},
												{
													"name" : "BG2ColorPresetButton",
													"type" : "button",

													"x" : 124.5,
													"y" : 29,

													"default_image" : "d:/ymir work/ui/game/mailbox/friend_select_arrow_button_default.sub",
													"over_image" : "d:/ymir work/ui/game/mailbox/friend_select_arrow_button_over.sub",
													"down_image" : "d:/ymir work/ui/game/mailbox/friend_select_arrow_button_down.sub",
												},
											),
										},

										## Preset Buttons
										{
											"name" : "BG2ColorPresetSaveButton",
											"type" : "button",

											"x" : 0,
											"y" : 60,

											"text" : uiScriptLocale.SKILL_COLOR_PRESET_SAVE,
											"text_height" : 6,

											"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
											"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
											"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",
										},
										{
											"name" : "BG2ColorPresetClearButton",
											"type" : "button",

											"x" : 0,
											"y" : 60 + 30,

											"text" : uiScriptLocale.SKILL_COLOR_PRESET_CLEAR,
											"text_height" : 6,

											"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
											"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
											"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",
										},

										## Custom Hex Color
										{
											"name" : "BG2CustomColorInputSlotImage",
											"type" : "expanded_image",
											"style" : ("attach",),

											"x" : 0,
											"y" : 120,

											"width" : COLOR_PICKER_WIDTH,
											"height" : COLOR_PICKER_HEIGHT,

											"image" : "d:/ymir work/ui/pet/Pet_Incu_001.tga",

											"children" :
											(
												{
													"name" : "BG2CustomColorTitle",
													"type" : "text",

													"x" : 0,
													"y" : -10,

													"text" : uiScriptLocale.SKILL_COLOR_CUSTOM_COLOR_CODE_TITLE,
													"all_align" : "center"
												},
												{
													"name" : "BG2CustomColorEditLine",
													"type" : "editline",

													"x" : 13,
													"y" : 30,

													"width" : 136,
													"height" : 15,

													"text" : "",
													"input_limit" : 7,
												},
											),
										},
									),
								},

								## Default Color Button
								{
									"name" : "DefaultButton",
									"type" : "button",

									"x" : 70 + 1.5,
									"y" : THINBOARD_HEIGHT - 85,

									"text" : uiScriptLocale.SKILL_COLOR_DEFAULT,
									"text_height" : 7,

									"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
									"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
									"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",
								},

								## ToolTips
								{
									"name" : "BG2CustomColorToolTip",
									"type" : "button",

									"x" : 200,
									"y" : 35 + 120,

									"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
									"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
									"down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
								},
								{
									"name" : "BG2ColorPresetToolTip",
									"type" : "button",

									"x" : 200,
									"y" : 35,

									"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
									"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
									"down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
								},
							),
						},
					),
				},

				## Color Layer Buttons
				{
					"name" : "ColorLayer1Button",
					"type" : "button",

					"x" : BUTTON_GAP + 62.5 + 35 * 0,
					"y" : BOARD_HEIGHT - 65,

					"text" : "1",
					"text_height" : 4,

					"default_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_00.sub",
					"over_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_01.sub",
					"down_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
					"disable_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
				},

				{
					"name" : "ColorLayer2Button",
					"type" : "button",

					"x" : BUTTON_GAP + 62.5 + 35 * 1,
					"y" : BOARD_HEIGHT - 65,

					"text" : "2",
					"text_height" : 4,

					"default_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_00.sub",
					"over_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_01.sub",
					"down_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
					"disable_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
				},

				{
					"name" : "ColorLayer3Button",
					"type" : "button",

					"x" : BUTTON_GAP + 62.5 + 35 * 2,
					"y" : BOARD_HEIGHT - 65,

					"text" : "3",
					"text_height" : 4,

					"default_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_00.sub",
					"over_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_01.sub",
					"down_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
					"disable_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
				},
				{
					"name" : "ColorLayer4Button",
					"type" : "button",

					"x" : BUTTON_GAP + 62.5 + 35 * 3,
					"y" : BOARD_HEIGHT - 65,

					"text" : "4",
					"text_height" : 4,

					"default_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_00.sub",
					"over_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_01.sub",
					"down_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
					"disable_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
				},
				{
					"name" : "ColorLayer5Button",
					"type" : "button",

					"x" : BUTTON_GAP + 62.5 + 35 * 4,
					"y" : BOARD_HEIGHT - 65,

					"text" : "5",
					"text_height" : 4,

					"default_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_00.sub",
					"over_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_01.sub",
					"down_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
					"disable_image" : "d:/ymir work/ui/privatesearch/private_pagenumber_02.sub",
				},

				## Pagination Buttons
				{
					"name" : "PrevPageButton",
					"type" : "button",

					"x" : 0 + 30,
					"y" : (COLOR_PICKER_HEIGHT / 2) + 40,

					"default_image" : "d:/ymir work/ui/privatesearch/private_prev_btn_01.sub",
					"over_image" : "d:/ymir work/ui/privatesearch/private_prev_btn_02.sub",
					"down_image" : "d:/ymir work/ui/privatesearch/private_prev_btn_01.sub",
				},
				{
					"name" : "NextPageButton",
					"type" : "button",

					"x" : COLOR_PICKER_HEIGHT + 25,
					"y" : (COLOR_PICKER_HEIGHT / 2) + 40,

					"default_image" : "d:/ymir work/ui/privatesearch/private_next_btn_01.sub",
					"over_image" : "d:/ymir work/ui/privatesearch/private_next_btn_02.sub",
					"down_image" : "d:/ymir work/ui/privatesearch/private_next_btn_01.sub",
				},

				## Confirmation Button
				{
					"name" : "ConfirmButton",
					"type" : "button",

					"x" : BUTTON_GAP,
					"y" : BOARD_HEIGHT - 35.5,

					"text" : uiScriptLocale.SKILL_COLOR_CHANGE,
					"text_height" : 6,

					"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
					"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
					"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",
				},

				## Cancel Button
				{
					"name" : "CancelButton",
					"type" : "button",

					"x" : BUTTON_GAP + 150,
					"y" : BOARD_HEIGHT - 35.5,

					"text" : uiScriptLocale.SKILL_COLOR_CANCEL,
					"text_height" : 6,

					"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
					"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
					"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",
				},

				## Background2 Color Preset Window (internal)
				{
					"name" : "BG2ColorPresetWindow",
					"type" : "window",

					"x" : 94.5,
					"y" : 115,

					"width" : 131,
					"height" : 0,
				},

				## Background2 Mouse Over Image (internal)
				{
					"name" : "BG2MouseOverImage",
					"type" : "expanded_image",
					"style" : ("not_pick",),

					"x" : 0 + 10,
					"y" : 0 + 32,

					"image" : "d:/ymir work/ui/game/mailbox/friend_list_mouse_over_img.sub",
				},
			)
		},
	),
}