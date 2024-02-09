import uiScriptLocale

if SCREEN_WIDTH >= 1036 and SCREEN_HEIGHT >= 695:
    WEB_WIDTH = 1016
    WEB_HEIGHT = 655
else:
    WEB_WIDTH = 740
    WEB_HEIGHT = 550

window = {
	"name" : "MallWindow",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width"  : WEB_WIDTH  + 20,
	"height" : WEB_HEIGHT + 40,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width"	 : WEB_WIDTH  + 20,
			"height" : WEB_HEIGHT + 40,

			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 7,

					"width" : WEB_WIDTH + 10,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":50, "y":3, "text":uiScriptLocale.SYSTEM_MALL, "text_horizontal_align":"center" },
					),
				},
			),
		},
	),
}
