import uiScriptLocale

import app

if app.ENABLE_SORT_INVENTORY:
	SORT_X_ADJUST = 38
	SORT_TITLE_X_ADJUST = 20
else:
	SORT_X_ADJUST = 0
	SORT_TITLE_X_ADJUST = 0

window = {
	"name" : "SpecialInventoryWindow",

	# "x" : 100,
	# "y" : 20,

	"x" : SCREEN_WIDTH - 176 - 287 - 10,
	"y" : SCREEN_HEIGHT - 37 - 525,

	"style" : ("movable", "float",),

	"width" : 176,
	"height" : 395 + 10,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 176,
			"height" : 395 + 10,

			"children" :
			(
				# Separate
				{
					"name" : "SeparateBaseImage",
					"type" : "image",
					"style" : ("attach",),
				
					"x" : 8,
					"y" : 7,
				
					"image" : "d:/ymir work/ui/pattern/titlebar_inv_refresh_baseframe.tga",
				
					"children" :
					(
						## Separate Button (38x24)
						{
							"name" : "SortInventoryButton",
							"type" : "button",
				
							"x" : 11,
							"y" : 3,
				
							"tooltip_text" : uiScriptLocale.INVENTORY_ITEM_SORT_TOOLTIP,
							"tooltip_x" : 0,
							"tooltip_y" : -23,
				
							"default_image" : "d:/ymir work/ui/game/inventory/refresh_small_button_01.sub",
							"over_image" : "d:/ymir work/ui/game/inventory/refresh_small_button_02.sub",
							"down_image" : "d:/ymir work/ui/game/inventory/refresh_small_button_03.sub",
							"disable_image" : "d:/ymir work/ui/game/inventory/refresh_small_button_04.sub",
						},
					),
				},

				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8 + SORT_X_ADJUST,
					"y" : 7,

					"width" : 161 - SORT_X_ADJUST,

					"color" : "yellow",

					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",

							"x" : 77 - SORT_TITLE_X_ADJUST,
							"y" : 3,

							"text" : uiScriptLocale.SPECIAL_INVENTORY_TITLE,
							"text_horizontal_align" : "center"
						},
					),
				},

				## Item Slot
				{
					"name" : "ItemSlot",
					"type" : "grid_table",

					"x" : 8,
					"y" : 35,

					"start_index" : 0,
					"x_count" : 5,
					"y_count" : 9,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub"
				},

				{
					"name" : "Inventory_Tab_01",
					"type" : "radio_button",

					"x" : 6.5,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_1,

					"children" :
					(
						{
							"name" : "Inventory_Tab_01_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "I",
						},
					),
				},
				{
					"name" : "Inventory_Tab_02",
					"type" : "radio_button",

					"x" : 6.5 + 32.5,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_2,

					"children" :
					(
						{
							"name" : "Inventory_Tab_02_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "II",
						},
					),
				},
				{
					"name" : "Inventory_Tab_03",
					"type" : "radio_button",

					"x" : 6.5 + 32.5 + 32.5,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_3,

					"children" :
					(
						{
							"name" : "Inventory_Tab_03_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "III",
						},
					),
				},
				{
					"name" : "Inventory_Tab_04",
					"type" : "radio_button",

					"x" : 6.5 + 32.5 + 32.5 + 32.5,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_4,

					"children" :
					(
						{
							"name" : "Inventory_Tab_04_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "IV",
						},
					),
				},
				{
					"name" : "Inventory_Tab_05",
					"type" : "radio_button",

					"x" : 6.5 + 32.5 + 32.5 + 32.5 + 32.5,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_5,

					"children" :
					(
						{
							"name" : "Inventory_Tab_05_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "V",
						},
					),
				},

				## Button
				{
					"name" : "SkillBookButton",
					"type" : "radio_button",

					"x" : 10,
					"y" : 47,

					"tooltip_text" : uiScriptLocale.SPECIAL_INVENTORY_SKILL_BOOK_TOOLTIP,
					"vertical_align" : "bottom",

					"default_image" : "d:/ymir work/ui/game/special_inventory/book1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/book2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/book3.tga",
				},

				{
					"name" : "UpgradeItemsButton",
					"type" : "radio_button",

					"x" : 20 + 32.5,
					"y" : 47,

					"tooltip_text" : uiScriptLocale.SPECIAL_INVENTORY_MATERIAL_TOOLTIP,
					"vertical_align" : "bottom",

					"default_image" : "d:/ymir work/ui/game/special_inventory/refine1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/refine2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/refine3.tga",
				},

				{
					"name" : "StoneButton",
					"type" : "radio_button",

					"x" : 20 + 70,
					"y" : 47,

					"tooltip_text" : uiScriptLocale.SPECIAL_INVENTORY_STONE_TOOLTIP,
					"vertical_align" : "bottom",

					"default_image" : "d:/ymir work/ui/game/special_inventory/stone1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/stone2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/stone3.tga",
				},

				{
					"name" : "GiftBoxButton",
					"type" : "radio_button",

					"x" : 20 + 110,
					"y" : 47,

					"tooltip_text" : uiScriptLocale.SPECIAL_INVENTORY_CHEST_TOOLTIP,
					"vertical_align" : "bottom",

					"default_image" : "d:/ymir work/ui/game/special_inventory/sandik1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/sandik2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/sandik3.tga",
				},
			),
		},
	),
}