#ifndef _COMMON_CONSTANTS_
#define _COMMON_CONSTANTS_

#define IsDebugMode 0


namespace Constants {
	const float DESIGN_WIDTH = 1136.0f;
	const float DESIGN_HEIGHT = 640.0f;
 
	const float FADE_IN_TIME = 0.6f;
	const float FADE_OUT_TIME = 0.4f;
	const float FADE_OUT_DELAY_TIME = 0.2f;

	const float SCALE_IN_TIME = 0.15f;
	const float SCALE_OUT_TIME = 0.1f;

	const int ZORDER_MONSTER = 100;
	const int ZORDER_SKILL = 200;
	const int ZORDER_UI = 800;
	const int ZORDER_POP = 900;
}
#endif