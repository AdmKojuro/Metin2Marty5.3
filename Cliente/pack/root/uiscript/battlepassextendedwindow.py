import uiScriptLocale
import constInfo

WINDOW_WIDTH = 555
WINDOW_HEIGHT = 345

ROOT_PATH = "d:/ymir work/ui/public/battlepass/"
GAUGE_PATH = "d:/ymir work/ui/public/battlepass/gauges/"
RANKLIST_PATH = "d:/ymir work/ui/public/battlepass/ranklist/"

NORMAL_PATH = "d:/ymir work/ui/public/battlepass/normal/"
PREMIUM_PATH = "d:/ymir work/ui/public/battlepass/premium/"
EVENT_PATH = "d:/ymir work/ui/public/battlepass/event/"

window = {
	"name" : "BattlePassWindow",
	"style" : ("movable", "float",),
	"x" : (SCREEN_WIDTH / 2) - (WINDOW_WIDTH / 2),
	"y" : (SCREEN_HEIGHT / 2) - (WINDOW_HEIGHT / 2) ,
	"width" : WINDOW_WIDTH, "height" : WINDOW_HEIGHT,
	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",
			"x" : 0, "y" : 0, "width" : WINDOW_WIDTH, 	"height" : WINDOW_HEIGHT,
			"title" : uiScriptLocale.EXTENDET_BATTLE_PASS_TITLE,
			"children" :
			(
				## Background Area
				{
					"name" : "area_normal",
					"type" : "border_a",
					"x" : 0, "y" : 32 + 30, "width" : WINDOW_WIDTH - 20, "height" : WINDOW_HEIGHT - 32 - 10 - 30, "horizontal_align" : "center",
				},
				{
					"name" : "area_premium",
					"type" : "border_a",
					"x" : 0, "y" : 32 + 30, "width" : WINDOW_WIDTH - 20, "height" : WINDOW_HEIGHT - 32 - 10 - 30, "horizontal_align" : "center",
					"children" :
					(
						## Color-Lines
						{ "name" : "Line", "type" : "line", "x" : 1, "y" : 1, "width" : WINDOW_WIDTH - 20 - 2, "height" : 0, "color" : 0xffd49800, },
						{ "name" : "Line", "type" : "line", "x" : 1, "y" : WINDOW_HEIGHT - 32 - 10 - 30 - 2, "width" : WINDOW_WIDTH - 20 - 2, "height" : 0, "color" : 0xffd49800, },
						
						{ "name" : "Line", "type" : "line", "x" : 1, "y" : 1, "width" : 0, "height" : WINDOW_HEIGHT - 32 - 10 - 30 - 2, "color" : 0xffd49800, },
						{ "name" : "Line", "type" : "line", "x" : WINDOW_WIDTH - 20 - 2, "y" : 1, "width" : 0, "height" : WINDOW_HEIGHT - 32 - 10 - 30 - 2, "color" : 0xffd49800, },
					),
				},
				{
					"name" : "area_event",
					"type" : "border_a",
					"x" : 0, "y" : 32 + 30, "width" : WINDOW_WIDTH - 20, "height" : WINDOW_HEIGHT - 32 - 10 - 30, "horizontal_align" : "center",
					"children" :
					(
						## Color-Lines
						{ "name" : "Line", "type" : "line", "x" : 1, "y" : 1, "width" : WINDOW_WIDTH - 20 - 2, "height" : 0, "color" : 0xffa32a94, },
						{ "name" : "Line", "type" : "line", "x" : 1, "y" : WINDOW_HEIGHT - 32 - 10 - 30 - 2, "width" : WINDOW_WIDTH - 20 - 2, "height" : 0, "color" : 0xffa32a94, },
						
						{ "name" : "Line", "type" : "line", "x" : 1, "y" : 1, "width" : 0, "height" : WINDOW_HEIGHT - 32 - 10 - 30 - 2, "color" : 0xffa32a94, },
						{ "name" : "Line", "type" : "line", "x" : WINDOW_WIDTH - 20 - 2, "y" : 1, "width" : 0, "height" : WINDOW_HEIGHT - 32 - 10 - 30 - 2, "color" : 0xffa32a94, },
					),
				},
				
				## Normal-Info
				{
					"name" : "NormalInfoBoard",
					"type" : "bar",
					"x" : 0, "y" : 32 + 30 + 3, "width" : WINDOW_WIDTH - 20 - 6, "height" : WINDOW_HEIGHT - 32 - 10 - 30 - 6, "horizontal_align" : "center",
					"color" : 0x90191919,
					"children" :
					(
						{
							"name" : "InfoThinBoard", "type" : "thinboard_circle",
							"x" : 0, "y" : 0, "horizontal_align" : "center", "vertical_align" : "center",
							"width" : 307, "height" : 180,
							"children":
							(
								{
									"name" : "Titlebar", "type" : "image",
									"x" : 3, "y" : 3, "image" : NORMAL_PATH + "ranklist_titlebar.sub",
									"children" :
									( { "name" : "NormalInfoText2", "type" : "text", "x" : 0, "y" : -1, "all_align" : "center", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1, "text" : "Info-Board", }, ),
								},
								{
									"name" : "NormalTextInfo",
									"x" : 0, "y" : 8, "width" : 303, "height" : 100, "horizontal_align" : "center", "vertical_align" : "center",
									"children" :
									(
										{
											"name" : "NormalInfoText", "type" : "text",
											"x" : 0, "y" : -2, "all_align" : "center",
											"text" : "", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1,
										},
										{
											"name" : "NormalInfoTextTime", "type" : "text",
											"x" : 0, "y" : 10, "all_align" : "center",
											"text" : "", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1,
										},
									),
								},
							),
						},
					),
				},
				## Input Premium-Ticket
				{
					"name" : "PremiumInfoBoard",
					"type" : "bar",
					"x" : 0, "y" : 32 + 30 + 3, "width" : WINDOW_WIDTH - 20 - 6, "height" : WINDOW_HEIGHT - 32 - 10 - 30 - 6, "horizontal_align" : "center",
					"color" : 0x70d49800,
					"children" :
					(
						{
							"name" : "InfoThinBoard", "type" : "thinboard_circle",
							"x" : 0, "y" : 0, "horizontal_align" : "center", "vertical_align" : "center",
							"width" : 307, "height" : 180,
							"children":
							(
								{
									"name" : "Titlebar", "type" : "image",
									"x" : 3, "y" : 3, "image" : PREMIUM_PATH + "ranklist_titlebar.sub",
									"children" :
									( { "name" : "NormalInfoText2", "type" : "text", "x" : 0, "y" : -1, "all_align" : "center", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1, "text" : "Info-Board", }, ),
								},
								{
									"name" : "InputTicketBoard", "type" : "bar",
									"x" : 0, "y" : 25, "width" : 301, "height" : 152, "horizontal_align" : "center",
									"color" : 0xff321d00,
									"children" :
									(
										{
											"name" : "TimeBackgroundColor", "type" : "bar",
											"x" : 0, "y" : 5, "width" : 301, "height" : 37,
											"color" : 0xff512f00,
										},
										{
											"name" : "InputTicketInfoTimeText1", "type" : "text",
											"x" : 0, "y" : -64, "all_align" : "center",
											"text" : "Aktuelle Season:", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1,
										},
										{
											"name" : "InputTicketInfoTimeText2", "type" : "text",
											"x" : 0, "y" : -46, "all_align" : "center",
											"text" : "10.11.2021 12:00:00 - 12.11.2021 16:00:00", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1,
										},
										{
											"name" : "InputTicketInfoText", "type" : "text",
											"x" : 0, "y" : -15, "all_align" : "center",
											"text" : "Aktiviere deinen Premium Battle-Pass!", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1,
										},
										## ItemSlot
										{
											"name" : "ticket_slot_icon", "type" : "image",
											
											"x" : 0, "y" : 18, "horizontal_align" : "center", "vertical_align" : "center",
											
											"image" : PREMIUM_PATH + "slot_input_ticket.sub",
											
											"children" :
											(
												## regist slot
												{
													"name" : "ticket_slot", "type" : "slot", "x" : 7, "y" : 7, "width" : 46, "height" : 46,
													"slot" : ( {"index":0, "x":0, "y":0, "width":32, "height":32}, )
												},
											),
										},
										{
											"name" : "ActivateTicketButton", "type" : "button",
											"x" : 0, "y" : 56, "horizontal_align" : "center", "vertical_align" : "center",
											
											"text_y" : -1, "text_outline" : 1, "text_color" : 0xFFFFFFFF,  
											"text" : "Premium-Ticket aktivieren!",
											
											"default_image" : PREMIUM_PATH + "button_activate_ticket_normal.sub",
											"over_image" : PREMIUM_PATH + "button_activate_ticket_hover.sub",
											"down_image" : PREMIUM_PATH + "button_activate_ticket_down.sub",
										},
									),
								},
							),
						},
						{
							"name" : "PremiumTextInfo",
							"x" : 0, "y" : 8, "width" : 303, "height" : 100, "horizontal_align" : "center", "vertical_align" : "center",
							"children" :
							(
								{
									"name" : "PremiumInfoText", "type" : "text",
									"x" : 0, "y" : -2, "all_align" : "center",
									"text" : "%s", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1,
								},
								{
									"name" : "PremiumInfoTextTime", "type" : "text",
									"x" : 0, "y" : 10, "all_align" : "center",
									"text" : "%s", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1,
								},
							),
						},
					),
				},
				
				## No-Event-Info
				{
					"name" : "EventInfoBoard",
					"type" : "bar",
					"x" : 0, "y" : 32 + 30 + 3, "width" : WINDOW_WIDTH - 20 - 6, "height" : WINDOW_HEIGHT - 32 - 10 - 30 - 6, "horizontal_align" : "center",
					"color" : 0x70a32a94,
					"children" :
					(
						{
							"name" : "InfoThinBoard", "type" : "thinboard_circle",
							"x" : 0, "y" : 0, "horizontal_align" : "center", "vertical_align" : "center",
							"width" : 307, "height" : 180,
							"children":
							(
								{
									"name" : "Titlebar", "type" : "image",
									"x" : 3, "y" : 3, "image" : EVENT_PATH + "ranklist_titlebar.sub",
									"children" :
									( { "name" : "NormalInfoText2", "type" : "text", "x" : 0, "y" : -1, "all_align" : "center", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1, "text" : "Info-Board", }, ),
								},
								{
									"name" : "BackgroundColor", "type" : "bar",
									"x" : 3, "y" : 25, "width" : 301, "height" : 152,
									"color" : 0xff2a0026,
								},
								{
									"name" : "EventTextInfo",
									"x" : 0, "y" : 8, "width" : 400, "height" : 100, "horizontal_align" : "center", "vertical_align" : "center",
									"children" :
									(
										{
											"name" : "EventInfoText", "type" : "text",
											"x" : 0, "y" : -2, "all_align" : "center",
											"text" : "", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1,
										},
										{
											"name" : "EventInfoTextTime", "type" : "text",
											"x" : 0, "y" : 10, "all_align" : "center",
											"text" : "", "fontsize" : "LARGE", "color" : 0xFFFFFFFF, "outline" : 1,
										},
									),
								},
							),
						},
					),
				},
				
				## UI-Area
				{
					"name" : "UIArea",
					"x" : 0, "y" : 32 + 30 + 10, "width" : WINDOW_WIDTH - 20, "height" : WINDOW_HEIGHT - 32 - 10 - 30, "horizontal_align" : "center",
					"children" :
					(
						## Scroll Border
						{
							"name" : "BorderScroll", "type" : "thinboard_circle",
							"x" : 295, "y" : 0, "width" : 20, "height" : 255,
						},
						## Missions Border
						{
							"name" : "BorderMissions", "type" : "thinboard_circle",
							"x" : 8, "y" : 0,
							"width" : 298, "height" : 255,
						},
						## Ranking Boreder
						{
							"name" : "BorderRanking", "type" : "thinboard_circle",
							"x" : 8, "y" : 0,
							"width" : 307, "height" : 255,
							"children":
							(
								## Ranking Title
								{
									"name" : "RankingTitle", "type" : "image",
									"x" : 3, "y" : 3,
									"image" : NORMAL_PATH + "ranklist_titlebar.sub",
									"children" :
									(
										{ "name" : "ranking_title_icon_rank", "type" : "image", "x" : 6, "y" : 1, "image" : RANKLIST_PATH + "icon_rank.sub", },
										{ "name" : "ranking_title_icon_name", "type" : "image", "x" : 93,"y" : 3, "image" : RANKLIST_PATH + "icon_name.sub", },
										{ "name" : "ranking_title_icon_time", "type" : "image", "x" : 221,"y" : 2, "image" : RANKLIST_PATH + "icon_time.sub", },
									),
								},
								## Pagination Window
								{
									"name" : "PaginationWindow", "type" : "window",
									"x" : 25, "y" : 27,
									"vertical_align" : "bottom",
									"width" : 307, "height" : 22,
									"children" :
									(
										{
											"name" : "first_page_button", "type" : "button",
											"x" : 0,"y" : 2,
											"default_image" : RANKLIST_PATH + "button_first_normal.sub",
											"over_image" : RANKLIST_PATH + "button_first_hover.sub",
											"down_image" : RANKLIST_PATH + "button_first_down.sub",
										},
										{
											"name" : "prev_page_button", "type" : "button",
											"x" : 23, "y" : 2,
											"default_image" : RANKLIST_PATH + "button_prev_normal.sub",
											"over_image" : RANKLIST_PATH + "button_prev_hover.sub",
											"down_image" : RANKLIST_PATH + "button_prev_down.sub",
										},
										{
											"name" : "page1_button", "type" : "radio_button",
											"x" : 45, "y" : 0,
											"text" : "1",
											"default_image" : RANKLIST_PATH + "button_page_normal.sub",
											"over_image" : RANKLIST_PATH + "button_page_hover.sub",
											"down_image" : RANKLIST_PATH + "button_page_down.sub",
										},
										{
											"name" : "page2_button", "type" : "radio_button",
											"x" : 78, "y" : 0,
											"text" : "2",
											"default_image" : RANKLIST_PATH + "button_page_normal.sub",
											"over_image" : RANKLIST_PATH + "button_page_hover.sub",
											"down_image" : RANKLIST_PATH + "button_page_down.sub",
										},
										{
											"name" : "page3_button", "type" : "radio_button",
											"x" : 111, "y" : 0,
											"text" : "3",
											"default_image" : RANKLIST_PATH + "button_page_normal.sub",
											"over_image" : RANKLIST_PATH + "button_page_hover.sub",
											"down_image" : RANKLIST_PATH + "button_page_down.sub",
										},
										{
											"name" : "page4_button", "type" : "radio_button",
											"x" : 144, "y" : 0,
											"text" : "4",
											"default_image" : RANKLIST_PATH + "button_page_normal.sub",
											"over_image" : RANKLIST_PATH + "button_page_hover.sub",
											"down_image" : RANKLIST_PATH + "button_page_down.sub",
										},
										{
											"name" : "page5_button", "type" : "radio_button",
											"x" : 177, "y" : 0,
											"text" : "5",
											"default_image" : RANKLIST_PATH + "button_page_normal.sub",
											"over_image" : RANKLIST_PATH + "button_page_hover.sub",
											"down_image" : RANKLIST_PATH + "button_page_down.sub",
										},
										{
											"name" : "next_page_button", "type" : "button",
											"x" : 222, "y" : 2,
											"default_image" : RANKLIST_PATH + "button_next_normal.sub",
											"over_image" : RANKLIST_PATH + "button_next_hover.sub",
											"down_image" : RANKLIST_PATH + "button_next_down.sub",
										},
										{
											"name" : "last_page_button", "type" : "button",
											"x" : 242, "y" : 2,
											"default_image" : RANKLIST_PATH + "button_last_normal.sub",
											"over_image" : RANKLIST_PATH + "button_last_hover.sub",
											"down_image" : RANKLIST_PATH + "button_last_down.sub",
										},
										{
											"name" : "refresh_ranklist_button", "type" : "button",
											"x" : 262, "y" : 7,
											"default_image" : RANKLIST_PATH + "button_refresh_normal.sub",
											"over_image" : RANKLIST_PATH + "button_refresh_hover.sub",
											"down_image" : RANKLIST_PATH + "button_refresh_down.sub",
										},
									),
								},
							),
						},
						
						## Mission Info Border
						{
							"name" : "BorderInfoMission", "type" : "thinboard_circle",
							"x" : 320, "y" : 24,
							"width" : 207, "height" : 231,
							"children":
							(
								{
									"name" : "bgImageMission", "type" : "image",
									"x" : 3, "y" : 3,
									"image" : NORMAL_PATH + "mission_info_background.tga",
									"children":
									(
										{
											"name" : "MissionInfoTitle", "type" : "text",
											"x" : 0, "y" : 3,
											"text" : uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_NAME,
											"horizontal_align" : "center",
											"text_horizontal_align" : "center",
											"text_color" : 0xffffffff, "outline" : 1, "fontsize" : "LARGE",
										},
										{
											"name" : "MissionTitleStatus", "type" : "image",
											"x" : 0, "y" : 22,
											"image" : ROOT_PATH + "info_bar_title.tga",
											"children":
											(
												{
													"name" : "MissionTitleStatusText", "type" : "text",
													"x" : 5, "y" : 0,
													"text" : uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_STATUS,
													"fontsize" : "LARGE",
													"color" : 0xFFFEE3AE, "outline" : 1,
												},
											),
										},
										{
											"name" : "MissionStatusText", "type" : "text",
											"x" : 5, "y" : 42,
											"text" : "", "outline" : 1,
										},
										{
											"name" : "MissionTitleInformation", "type" : "image",
											"x" : 0, "y" : 65,
											"image" : ROOT_PATH + "info_bar_title.tga",
											"children":
											(
												{
													"name" : "MissionTitleInformationText", "type" : "text",
													"x" : 5, "y" : 0,
													"text" : uiScriptLocale.EXTENDET_BATTLE_PASS_MISSION_INFO,
													"fontsize" : "LARGE",
													"color" : 0xFFFEE3AE, "outline" : 1,
												},
											),
										},
										{
											"name" : "MissionInformationText1", "type" : "text",
											"x" : 5, "y" : 85,
											"text" : "", "outline" : 1,
										},
										{
											"name" : "mission_image_2", "type" : "image",
											"x" : 0, "y" : 101,
											"image" : ROOT_PATH + "info_bar_normal.tga",
											"children":
											(
												{
													"name" : "MissionInformationText2", "type" : "text",
													"x" : 5, "y" : 2,
													"text" : "", "outline" : 1,
												},
											),
										},
										{
											"name" : "MissionInformationText3", "type" : "text",
											"x" : 5, "y" : 121,
											"text" : "", "outline" : 1,
										},
										{
											"name" : "MissionTitleDescription", "type" : "image",
											"x" : 0, "y" : 143,
											"image" : ROOT_PATH + "info_bar_title.tga",
											"children":
											(
												{
													"name" : "MissionTitleDescriptionText", "type" : "text",
													"x" : 5, "y" : 0,
													"text" : uiScriptLocale.EXTENDET_BATTLE_PASS_DESCRIPTION,
													"fontsize" : "LARGE",
													"color" : 0xFFFEE3AE, "outline" : 1,
												},
											),
										},
									),
								},
							),
						},
						## General Info Border
						{
							"name" : "BorderInfoGeneral", "type" : "thinboard_circle",
							"x" : 320, "y" : 24,
							"width" : 207, "height" : 231,
							"children":
							(
								{
									"name" : "bgImageGeneral", "type" : "image",
									"x" : 3, "y" : 3,
									"image" : NORMAL_PATH + "mission_info_background.tga",
									"children":
									(
										{
											"name" : "GeneralTitleText", "type" : "text",
											"x" : 0, "y" : 3, "horizontal_align" : "center", "text_horizontal_align" : "center",
											"text_color" : 0xffffffff, "outline" : 1, "fontsize" : "LARGE",
											"text" : "",
										},
										{
											"name" : "GeneralInfoTime", "type" : "image",
											"x" : 0, "y" : 22,
											"image" : ROOT_PATH + "info_bar_time.tga",
											"children":
											(
												{
													"name" : "GeneralInfoStartDateFromText", "type" : "text",
													"x" : 38, "y" : 4, "outline" : 1,
													"text" : uiScriptLocale.EXTENDET_BATTLE_PASS_SEASON_DATE_FROM,
												},
												{
													"name" : "GeneralInfoStartDateText", "type" : "text",
													"x" : 67, "y" : 4, "outline" : 1,
													"text" : "",
												},
												{
													"name" : "GeneralInfoEndDateToText", "type" : "text",
													"x" : 38, "y" : 20, "outline" : 1, 
													"text" : uiScriptLocale.EXTENDET_BATTLE_PASS_SEASON_DATE_TO,
												},
												{
													"name" : "GeneralInfoEndDateText", "type" : "text",
													"x" : 67, "y" : 20, "outline" : 1, 
													"text" : "",
												},
												{
													"name" : "GeneralInfoEndTimeText", "type" : "text",
													"x" : 0, "y" : 40, "outline" : 1, "horizontal_align" : "center", "text_horizontal_align" : "center",
													"text" : "",
												},
												{
													"name" : "GaugeTimeImageBack", "type" : "image",
													"x" : 2, "y" : 58,
													"image" : GAUGE_PATH + "large_background.sub",
													"children":
													(
														{
															"name" : "GaugeTime", "type" : "expanded_image",
															"x" : 8, "y" : 2,
															"image" : GAUGE_PATH + "large_red.sub",
														},
													),
												},
											),
										},
										{
											"name" : "GeneralInfoFinishMissionsText", "type" : "text",
											"x" : 0, "y" : 103, "outline" : 1,
											"horizontal_align" : "center", "text_horizontal_align" : "center",
											"text" : "",
										},
										{
											"name" : "TotalProgressText", "type" : "text",
											"x" : 0, "y" : 120, "outline" : 1,
											"horizontal_align" : "center", "text_horizontal_align" : "center",
											"text" : uiScriptLocale.EXTENDET_BATTLE_PASS_SEASON_PROGRESS, "outline" : 1,
										},
										{
											"name" : "GaugeMissionImageBack", "type" : "image",
											"x" : 2, "y" : 137,
											"image" : GAUGE_PATH + "large_background.sub",
											"children":
											(
												{
													"name" : "GaugeMission", "type" : "expanded_image",
													"x" : 8, "y" : 2,
													"image" : GAUGE_PATH + "large_red.sub",
												},
											),
										},
										{
											"name" : "RankingButton", "type" : "button",
											"x" : 5, "y" : 157,
											"default_image" : NORMAL_PATH + "button_ranklist_normal.sub",
											"over_image" : NORMAL_PATH + "button_ranklist_hover.sub",
											"down_image" : NORMAL_PATH + "button_ranklist_down.sub",
										},
										{
											"name" : "RewardButton", "type" : "button",
											"x" : 5, "y" : 192,
											"default_image" : NORMAL_PATH + "button_recive_reward_normal.sub",
											"over_image" : NORMAL_PATH + "button_recive_reward_hover.sub",
											"down_image" : NORMAL_PATH + "button_recive_reward_down.sub",
										},
										{
											"name" : "reward_slot_background", "type" : "expanded_image",
											"x" : 100, "y" : 155,
											"image" : NORMAL_PATH + "reward_slots.sub",
										},
										{
											"name" : "reward_slots", "type" : "slot", "x" : 100, "y" : 155,
											"width" : 32*3, "height" : 32*2,
											
											"slot" : ( 
												{"index":0, "x":0, "y":0, "width":32, "height":32}, 
												{"index":1, "x":0, "y":32, "width":32, "height":32}, 
												{"index":2, "x":32, "y":0, "width":32, "height":32}, 
												{"index":3, "x":32, "y":32, "width":32, "height":32}, 
												{"index":4, "x":64, "y":0, "width":32, "height":32}, 
												{"index":5, "x":64, "y":32, "width":32, "height":32}, 
											),
										},
									),
								},
							),
						},

						## Sub-Tab Control Window
						{
							"name" : "sub_tab_control", "type" : "window",
							"x" : 345, "y" : 0,
							"width" : 158, "height" : 26,
							"children" :
							(
								{
									"name" : "tab_missions", "type" : "image",
									"x" : 0, "y" : 0, "width" : 158, "height" : 26,
									"image" : ROOT_PATH + "tab_sub_mission.sub",
								},
								{
									"name" : "tab_general", "type" : "image",
									"x" : 0, "y" : 0, "width" : 158, "height" : 26,
									"image" : ROOT_PATH + "tab_sub_general.sub",
								},
								{
									"name" : "tab_button_missions", "type" : "radio_button",
									"x" : 6, "y" : 3, "width" : 70, "height" : 21,
								},
								{
									"name" : "tab_button_general", "type" : "radio_button",
									"x" : 82, "y" : 3, "width" : 70, "height" : 21,
								},
							),
						},
					),
				},
				
				## Tab Control Window
				{
					"name" : "tab_control", "type" : "window",
					"x" : 10, "y" : 31,
					"width" : 430, "height" : 34,
					"children" :
					(
						{
							"name" : "tab_normal", "type" : "image",
							"x" : 0, "y" : 0,
							"width" : 430, "height" : 34,
							"image" : ROOT_PATH + "tab_normal.sub",
						},
						{
							"name" : "tab_premium", "type" : "image",
							"x" : 0, "y" : 0,
							"width" : 430, "height" : 34,
							"image" : ROOT_PATH + "tab_premium.sub",
						},
						{
							"name" : "tab_event", "type" : "image",
							"x" : 0, "y" : 0,
							"width" : 430, "height" : 34,
							"image" : ROOT_PATH + "tab_event.sub",
						},
						{
							"name" : "tab_button_normal", "type" : "radio_button",
							"x" : 5, "y" : 5,
							"width" : 134, "height" : 25,
						},
						{
							"name" : "tab_button_premium", "type" : "radio_button",
							"x" : 147, "y" : 5,
							"width" : 134, "height" : 25,
						},
						{
							"name" : "tab_button_event", "type" : "radio_button",
							"x" : 289, "y" : 5,
							"width" : 134, "height" : 25,
						},
					),
				},
			),
		},
	),
}