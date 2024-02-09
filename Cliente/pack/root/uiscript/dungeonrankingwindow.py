import uiScriptLocale
import app

BOARD_WIDTH = 355
BACK_IMG_PATH = "d:/ymir work/ui/public/public_board_back/"
ROOT_PATH = "d:/ymir work/ui/game/guild/dragonlairranking/"

window = {
	"name" : "DungeonRankingWindow",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width" : BOARD_WIDTH,
	"height" : 238,

	"children" :
	(
		{
			"name" : "Board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : BOARD_WIDTH,
			"height" : 238,

			"children" :
			(
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : BOARD_WIDTH - 13,

					"children" :
					(
						{
							"name" : "TitleBarName",
							"type" : "text",

							"x" : 0,
							"y" : -2,

							"text" : uiScriptLocale.DUNGEON_RANKING,
							"all_align" : "center"
						},
					),
				},
				{
					"name" : "LeftTop",
					"type" : "image",
					"x" : 17,
					"y" : 38,
					"image" : BACK_IMG_PATH + "boardback_mainboxlefttop.sub",
				},
				{
					"name" : "RightTop",
					"type" : "image",
					"x" : 318,
					"y" : 38,
					"image" : BACK_IMG_PATH + "boardback_mainboxrighttop.sub",
				},
				{
					"name" : "LeftBottom",
					"type" : "image",
					"x" : 17,
					"y" : 173,
					"image" : BACK_IMG_PATH + "boardback_mainboxleftbottom.sub",
				},
				{
					"name" : "RightBottom",
					"type" : "image",
					"x" : 318,
					"y" : 173,
					"image" : BACK_IMG_PATH + "boardback_mainboxrightbottom.sub",
				},
				{
					"name" : "LeftCenterImg",
					"type" : "expanded_image",
					"x" : 17,
					"y" : 38 + 16,
					"image" : BACK_IMG_PATH + "boardback_leftcenterImg.tga",
					"rect" : (0.0, 0.0, 0, 6),
				},
				{
					"name" : "RightCenterImg",
					"type" : "expanded_image",
					"x" : 317,
					"y" : 38 + 16,
					"image" : BACK_IMG_PATH + "boardback_rightcenterImg.tga",
					"rect" : (0.0, 0.0, 0, 6),
				},
				{
					"name" : "TopCenterImg",
					"type" : "expanded_image",
					"x" : 17 + 15,
					"y" : 38,
					"image" : BACK_IMG_PATH + "boardback_topcenterImg.tga",
					"rect" : (0.0, 0.0, 16, 0),
				},
				{
					"name" : "BottomCenterImg",
					"type" : "expanded_image",
					"x" : 17 + 15,
					"y" : 173,
					"image" : BACK_IMG_PATH + "boardback_bottomcenterImg.tga",
					"rect" : (0.0, 0.0, 16, 0),
				},
				{
					"name" : "CenterImg",
					"type" : "expanded_image",
					"x" : 17 + 15,
					"y" : 38 + 15 + 1,
					"image" : BACK_IMG_PATH + "boardback_centerImg.tga",
					"rect" : (0.0, 0.0, 16, 6),
				},
				{
					"name" : "RankingTiTleImg",
					"type" : "image",
					"x" : 20,
					"y" : 41,
					"image" : ROOT_PATH + "ranking_list_menu.sub",
					"children" :
					(
						{
							"name" : "ResultPosition",
							"type" : "text",
							"x" : 10,
							"y" : 4,
							"text" : uiScriptLocale.DUNGEON_RANKING_POSITION,
						},
						{
							"name" : "ResultName",
							"type" : "text",
							"x" : 95,
							"y" : 4,
							"text" : uiScriptLocale.DUNGEON_RANKING_NAME,
						},
						{
							"name" : "ResultLevel",
							"type" : "text",
							"x" : 180,
							"y" : 4,
							"text" : uiScriptLocale.DUNGEON_RANKING_LEVEL,
						},
						{
							"name" : "ResultPoints",
							"type" : "text",
							"x" : 240,
							"y" : 4,
							"text" : uiScriptLocale.DUNGEON_RANKING_POINTS,
						},
					),
				},
				{
					"name" : "LeftTopSelf",
					"type" : "image",
					"x" : 17,
					"y" : 190,
					"image" : BACK_IMG_PATH + "boardback_mainboxlefttop.sub",
				},
				{
					"name" : "RightTopSelf",
					"type" : "image",
					"x" : 318,
					"y" : 190,
					"image" : BACK_IMG_PATH + "boardback_mainboxrighttop.sub",
				},
				{
					"name" : "LeftBottomSelf",
					"type" : "image",
					"x" : 17,
					"y" : 190 + 15,
					"image" : BACK_IMG_PATH + "boardback_mainboxleftbottom.sub",
				},
				{
					"name" : "RightBottomSelf",
					"type" : "image",
					"x" : 318,
					"y" : 190 + 15,
					"image" : BACK_IMG_PATH + "boardback_mainboxrightbottom.sub",
				},
				{
					"name" : "TopCenterImgSelf",
					"type" : "expanded_image",
					"x" : 17 + 15,
					"y" : 190,
					"image" : BACK_IMG_PATH + "boardback_topcenterImg.tga",
					"rect" : (0.0, 0.0, 16, 0),
				},
				{
					"name" : "BottomCenterImgSelf",
					"type" : "expanded_image",
					"x" : 17 + 15,
					"y" : 190 + 15,
					"image" : BACK_IMG_PATH + "boardback_bottomcenterImg.tga",
					"rect" : (0.0, 0.0, 16, 0),
				},
				{
					"name" : "WaitAniImg",
					"type" : "ani_image",

					"x" : 0,
					"y" : 40,
					"width" : 32,
					"height" : 128,
					"horizontal_align" : "center",
					"vertical_align" : "center",

					"delay" : 6,

					"images" :
					(
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/00.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/01.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/02.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/03.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/04.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/05.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/06.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/07.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/08.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/09.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/11.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/12.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/13.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/14.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/15.sub",
						"d:/ymir work/ui/game/" + "TaskBar/Rampage_01/16.sub",
					),

					"children" :
					(
						{
							"name" : "WaitTextLine",
							"type" : "text",

							"x" : 2,
							"y" : -15,

							"text" : "...",
							"all_align" : "center"
						},
					)
				},
			),
		},
	),
}
