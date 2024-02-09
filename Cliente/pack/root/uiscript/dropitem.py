import uiScriptLocale

SIZE_WIDTH = 522
SIZE_HEIGHT = 300

RIGHTBOARD_WIDTH = 196
RIGHTBOARD_HEIGHT = 254
RIGHTBOARD_X = 315
RIGHTBOARD_Y = 36


RIGHTBOARD_WIDTH_1 = 300
RIGHTBOARD_HEIGHT_1 = 217
RIGHTBOARD_X_1 = 10
RIGHTBOARD_Y_1 = 73


RIGHTBOARD_WIDTH_2 = 300
RIGHTBOARD_HEIGHT_2 = 33
RIGHTBOARD_X_2 = 10
RIGHTBOARD_Y_2 = 36

RENDER_TARGET_INDEX = 4

window = {
	"name" : "drop_item_windows",

	"x" : (SCREEN_WIDTH -SIZE_WIDTH) / 2,
	"y" : (SCREEN_HEIGHT - SIZE_HEIGHT) / 2,

	"style" : ("movable", "float",),

	"width" : SIZE_WIDTH,
	"height" : SIZE_HEIGHT,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"x" : 0,
			"y" : 0,

			"width" : SIZE_WIDTH,
			"height" :SIZE_HEIGHT,
			
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",

					"x" : 6,
					"y" : 6,

					"width" : SIZE_WIDTH-12,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":SIZE_WIDTH/2, "y":3, "text" : uiScriptLocale.SEARCH_ITEM_DROP_TITLE, "text_horizontal_align":"center" },
					),
				},

				{
					"name" : "ThinBoardName",
					"type" : "thinboard_circle",
					"x" : RIGHTBOARD_X_2, "y" : RIGHTBOARD_Y_2, "width" : RIGHTBOARD_WIDTH_2, "height" : RIGHTBOARD_HEIGHT_2,
					"children":
					(

						{
							"name" : "ImgSlotName",
							"type" : "image",

							"x" : 7, "y" : 7,
							"image" : "d:/ymir work/ui/public/parameter_slot_05.sub",

							"children" :
							(
								{
									"name" : "ItemNameValue",
									"type" : "editline",
									"x" : 5,
									"y" : 3,
									"width" : 120,
									"height" : 15,
									"input_limit" : 25,
									"text" : "",
								},
							),
						},

						{
							"name" : "ButtonSearch",
							"type" : "button",

							"x" : 150+2,
							"y" : 7,

							"tooltip_text": uiScriptLocale.SEARCH_ITEM_DROP_SEARCH,

							"default_image" : "d:/ymir work/ui/public/acceptbutton00.sub",
							"over_image" : "d:/ymir work/ui/public/acceptbutton01.sub",
							"down_image" : "d:/ymir work/ui/public/acceptbutton02.sub",
						},

						{
							"name" : "ButtonDelete",
							"type" : "button",

							"x" : 150+70,
							"y" : 7,

							"tooltip_text": uiScriptLocale.SEARCH_ITEM_DROP_DELETE,

							"default_image" :  "d:/ymir work/ui/public/canclebutton00.sub",
							"over_image" :  "d:/ymir work/ui/public/canclebutton01.sub",
							"down_image" :  "d:/ymir work/ui/public/canclebutton02.sub",
						},
					),
				},

				{
					"name" : "ThinBoardRender",
					"type" : "thinboard_circle",
					"x" : RIGHTBOARD_X, "y" : RIGHTBOARD_Y, "width" : RIGHTBOARD_WIDTH, "height" : RIGHTBOARD_HEIGHT,
					"children" :
					(

						{
							"name": "ModelRender",
							"type": "render_target",
							"x" : 2, "y" : 26,
							"width":190,
							"height": 210,
							"index": RENDER_TARGET_INDEX,
							#"image":  "d:/ymir work/ui/game/myshop_deco/model_view_bg.sub",
						},

						{
							"name" : "ModelTitle",
							"type" : "image",
							
							"x" : 3, "y" : 2,
							"image" : "d:/ymir work/ui/game/myshop_deco/model_view_title.sub",
							
							"children" :
							(
								{ "name" : "ModelName1", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align":"center" },
							),
						},

						{
							"name": "ModelSubTitle",
							"type": "image",
							
							"x" : 3, "y" : 230,
							"image" : "d:/ymir work/ui/game/myshop_deco/model_view_title.sub",
							"children" :
							(
								{
									"name" : "ImgSlotSubTitle",
									"type" : "image",
									
									"x" : 30, "y" : 1,
									"image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
									
									"children" :
									(
										{ "name" : "ModelName2", "type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align":"center" },
									),
								},
							),
						},
					),
				},

				{
					"name" : "ThinBoardSelect",
					"type" : "thinboard_circle",
					"x" : RIGHTBOARD_X_1, "y" : RIGHTBOARD_Y_1, "width" : RIGHTBOARD_WIDTH_1, "height" : RIGHTBOARD_HEIGHT_1,
					"children":
					(
						{
							"name" : "SelectOption_0",
							"type" : "radio_button",

							"x" : 5+3,
							"y" : 5,

							"default_image" : "d:/ymir work/ui/game/mailbox/post_default.sub",
							"over_image" : "d:/ymir work/ui/game/mailbox/post_over.sub",
							"down_image" : "d:/ymir work/ui/game/mailbox/post_select.sub",

							"children":
							(
								{ "name" : "NameMob_0", "type" : "text", "x" : 19, "y" : 9, "text" : "", "text_horizontal_align":"left"},
								{ "name" : "Cantidad_Item_0", "type" : "text", "x" : 230, "y" : 3, "text" : "", "text_horizontal_align":"center" },
								{ "name" : "Porcentaje_Item_0", "type" : "text", "x" : 230, "y" : 16, "text" : "", "text_horizontal_align":"center" },
							),
						},

						{
							"name" : "SelectOption_1",
							"type" : "radio_button",

							"x" : 5+3,
							"y" : 5+38,

							"default_image" : "d:/ymir work/ui/game/mailbox/post_default.sub",
							"over_image" : "d:/ymir work/ui/game/mailbox/post_over.sub",
							"down_image" : "d:/ymir work/ui/game/mailbox/post_select.sub",

							"children":
							(
								{ "name" : "NameMob_1", "type" : "text", "x" : 19, "y" : 9, "text" : "", "text_horizontal_align":"left"},
								{ "name" : "Cantidad_Item_1", "type" : "text", "x" : 230, "y" : 3, "text" : "", "text_horizontal_align":"center" },
								{ "name" : "Porcentaje_Item_1", "type" : "text", "x" : 230, "y" : 16, "text" : "", "text_horizontal_align":"center" },

							),

						},

						{
							"name" : "SelectOption_2",
							"type" : "radio_button",

							"x" : 5+3,
							"y" : 5+(38*2),

							"default_image" : "d:/ymir work/ui/game/mailbox/post_default.sub",
							"over_image" : "d:/ymir work/ui/game/mailbox/post_over.sub",
							"down_image" : "d:/ymir work/ui/game/mailbox/post_select.sub",

							"children":
							(
								{ "name" : "NameMob_2", "type" : "text", "x" : 19, "y" : 9, "text" : "", "text_horizontal_align":"left"},
								{ "name" : "Cantidad_Item_2", "type" : "text", "x" : 230, "y" : 3, "text" : "", "text_horizontal_align":"center" },
								{ "name" : "Porcentaje_Item_2", "type" : "text", "x" : 230, "y" : 16, "text" : "", "text_horizontal_align":"center" },

							),

						},

						{
							"name" : "SelectOption_3",
							"type" : "radio_button",

							"x" : 5+3,
							"y" : 5+(38*3),

							"default_image" : "d:/ymir work/ui/game/mailbox/post_default.sub",
							"over_image" : "d:/ymir work/ui/game/mailbox/post_over.sub",
							"down_image" : "d:/ymir work/ui/game/mailbox/post_select.sub",

							"children":
							(
								{ "name" : "NameMob_3", "type" : "text", "x" : 19, "y" : 9, "text" : "", "text_horizontal_align":"left"},
								{ "name" : "Cantidad_Item_3", "type" : "text", "x" : 230, "y" : 3, "text" : "", "text_horizontal_align":"center" },
								{ "name" : "Porcentaje_Item_3", "type" : "text", "x" : 230, "y" : 16, "text" : "", "text_horizontal_align":"center" },

							),

						},

						{
							"name" : "SelectOption_4",
							"type" : "radio_button",

							"x" : 5+3,
							"y" : 5+(38*4),

							"default_image" : "d:/ymir work/ui/game/mailbox/post_default.sub",
							"over_image" : "d:/ymir work/ui/game/mailbox/post_over.sub",
							"down_image" : "d:/ymir work/ui/game/mailbox/post_select.sub",

							"children":
							(
								{ "name" : "NameMob_4", "type" : "text", "x" : 19, "y" : 9, "text" : "", "text_horizontal_align":"left"},
								{ "name" : "Cantidad_Item_4", "type" : "text", "x" : 230, "y" : 3, "text" : "", "text_horizontal_align":"center" },
								{ "name" : "Porcentaje_Item_4", "type" : "text", "x" : 230, "y" : 16, "text" : "", "text_horizontal_align":"center" },

							),
							
						},

						{
							"name" : "PrevButton",
							"type" : "button",

							"x" : 5+20,
							"y" : 5+(39*5),

							"default_image" : "d:/ymir work/ui/privatesearch/private_prev_btn_01.sub",
							"over_image" : "d:/ymir work/ui/privatesearch/private_prev_btn_02.sub",
							"down_image" : "d:/ymir work/ui/privatesearch/private_prev_btn_01.sub",
						},

						{
							"name" : "FirstPrevButton",
							"type" : "button",

							"x" : 5+3,
							"y" : 5+(39*5),

							"default_image" : "d:/ymir work/ui/privatesearch/private_first_prev_btn_01.sub",
							"over_image" : "d:/ymir work/ui/privatesearch/private_first_prev_btn_02.sub",
							"down_image" : "d:/ymir work/ui/privatesearch/private_first_prev_btn_01.sub",
						},


						{
							"name" : "NextButton",
							"type" : "button",

							"x" : 5+3+70,
							"y" : 5+(39*5),

							"default_image" : "d:/ymir work/ui/privatesearch/private_next_btn_01.sub",
							"over_image" : "d:/ymir work/ui/privatesearch/private_next_btn_02.sub",
							"down_image" : "d:/ymir work/ui/privatesearch/private_next_btn_01.sub",
						},

						{
							"name" : "LastNextButton",
							"type" : "button",

							"x" : 5+20+70,
							"y" : 5+(39*5),

							"default_image" : "d:/ymir work/ui/privatesearch/private_last_next_btn_01.sub",
							"over_image" : "d:/ymir work/ui/privatesearch/private_last_next_btn_02.sub",
							"down_image" : "d:/ymir work/ui/privatesearch/private_last_next_btn_01.sub",
						},



						{
							"name": "ImgSlotTotalPages",
							"type": "image",
							
							"x" : 40, "y" : 197,
							"image" : "d:/ymir work/ui/privatesearch/private_pagenumber_00.sub",
							"children":
							(
								{ "name" : "TotalPages", "type" : "text", "x" : 0, "y" : -1, "text" : "0/0", "all_align":"center" },

							),
						},

						{ "name" : "TotalResultados", "type" : "text", "x" : 120, "y" : 95, "text" : "Total: 0", "all_align":"center" },

					),
				},
			),
		},
	),
}