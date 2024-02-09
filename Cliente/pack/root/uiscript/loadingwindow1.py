import uiScriptLocale
import app

INTERFACE_PATH = "d:/ymir work/ui/gui_interface/"

window = {
	"name" : "LoadingWindow",
	"sytle" : ("movable","ltr",),

	"x" : 0,
	"y" : 0,

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" :
	(
		{
			"name":"ErrorMessage", 
			"type":"text", "x":10, "y":10, 
			"text": uiScriptLocale.LOAD_ERROR, 
		},
		
		{
			"name" : "GageBoard",
			"type" : "window",
			"x" : float(SCREEN_WIDTH) / 2  - 268/2,
			"y" : float(SCREEN_HEIGHT) / 2 - 50,
			"width" : 268, 
			"height": 268,

			"children" :
			(
				{
					"name" : "BackGage",
					"type" : "ani_image",


					"x" : 0,
					"y" : -120,

					"delay" : 1,

					"images" :
					(
						"locale/common/ui/loading/load/10.png",
						"locale/common/ui/loading/load/11.png",
						"locale/common/ui/loading/load/12.png",
						"locale/common/ui/loading/load/13.png",
						"locale/common/ui/loading/load/14.png",
						"locale/common/ui/loading/load/15.png",
						"locale/common/ui/loading/load/16.png",
						"locale/common/ui/loading/load/17.png",
						"locale/common/ui/loading/load/18.png",
						"locale/common/ui/loading/load/19.png",
						"locale/common/ui/loading/load/20.png",
						"locale/common/ui/loading/load/21.png",
						"locale/common/ui/loading/load/22.png",
						"locale/common/ui/loading/load/10.png",
						"locale/common/ui/loading/load/11.png",
						"locale/common/ui/loading/load/12.png",
						"locale/common/ui/loading/load/13.png",
						"locale/common/ui/loading/load/14.png",
						"locale/common/ui/loading/load/15.png",
						"locale/common/ui/loading/load/16.png",
						"locale/common/ui/loading/load/17.png",
						"locale/common/ui/loading/load/18.png",
						"locale/common/ui/loading/load/19.png",
						"locale/common/ui/loading/load/20.png",
						"locale/common/ui/loading/load/21.png",
						"locale/common/ui/loading/load/22.png",
					)
				},
			),
		},
	),
}
