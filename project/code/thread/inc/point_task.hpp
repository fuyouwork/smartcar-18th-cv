#ifndef __POINT_TASK_HPP
#define __POINT_TASK_HPP

extern "C"
{
#include "user_code.h"
}
#include "user_code_cpp.hpp"
extern bool g_bCorrectPointFlag;
extern bool g_bChangeAngleFlag;
extern float g_fXTempExpection;
extern float g_fYTempExpection;
extern bool g_bGoPointGoalErrFlag;
extern void change_angle_task(void);
extern void correct_point_task(uint8 times);
#endif