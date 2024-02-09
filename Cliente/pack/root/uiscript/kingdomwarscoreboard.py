import localeInfo

THINBOARD_WIDTH		= 160
THINBOARD_HEIGHT	= 85

POS_X = 25
POS_Y = 210

window = {
	"name" : "EventRewardListWindow",
	"type" : "window",
	"style" : ("movable", "float",),

	"x" : POS_X,
	"y" : SCREEN_HEIGHT - POS_Y,
	
	"width" : THINBOARD_WIDTH,
	"height" : THINBOARD_HEIGHT,

	"children" :
	[
		{ 
			"name" : "board", 
			"type" : "thinboard", 
			"style" : ("attach",),
			
			"x" : 0,
			"y" : 0,
			
			"width" : THINBOARD_WIDTH,
			"height" : THINBOARD_HEIGHT,
			
			"children" :
			[
				# EMPIRE_A
				{
					"name" : "ShinsooFlag",
					"type" : "image",

					"x" : 10,
					"y" : 10,

					"image" : "d:/ymir work/ui/public/battle/empire_shinsu.sub",
				},
				{
					"name" : "ShinsooScoreText",
					"type" : "text",
					"x" : 40,
					"y" : 10,
					"text" : localeInfo.EMPIRE_A,
				},
				
				# EMPIRE_B
				{
					"name" : "ChunjoFlag",
					"type" : "image",

					"x" : 10,
					"y" : 35,

					"image" : "d:/ymir work/ui/public/battle/empire_chunjo.sub",
				},
				{
					"name" : "ChunjoScoreText",
					"type" : "text",
					"x" : 40,
					"y" : 35,
					"text" : localeInfo.EMPIRE_B,
				},
				
				# EMPIRE_C
				{
					"name" : "JinnoFlag",
					"type" : "image",

					"x" : 10,
					"y" : 60,

					"image" : "d:/ymir work/ui/public/battle/empire_jinno.sub",
				},
				{
					"name" : "JinnoScoreText",
					"type" : "text",
					"x" : 40,
					"y" : 60,
					"text" : localeInfo.EMPIRE_C,
				},
			],
		},
	],
}