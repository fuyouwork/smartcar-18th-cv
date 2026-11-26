#ifndef __TFT180_H
#define __TFT180_H

#define PARAM_LINES 7
typedef enum
{
	COMMON_MENU,
	SPEED_MENU,
	PARAM_MENU,
	RAW_COORDINATE_MENU,
	COORDINATE_MENU,
	ENCODER_MENU,
	MENUNUM
} MENUn_e;
extern "C"
{
#include "zf_common_headfile.h"
#include "icm.h"

}
#include "flash.hpp"
#include "omnidirect.hpp"
#include "imgProc/TSP.hpp"

extern void tft180_show_menu(void);
extern void tft180_show_param(void);
extern void test_coordinate_menu();
extern void menu_show(void);

extern uint8 g_byPoint, g_byShowPoint;
extern uint8 g_byMenuSwitch;
extern uint8 g_byOpenArtShowFlag;
extern uint8 g_byShowLines;
extern uint8 g_byLastMenuSwitch;

#endif
