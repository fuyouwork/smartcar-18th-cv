#ifndef __SIDE_TASK_HPP
#define __SIDE_TASK_HPP

#include "user_code_cpp.hpp"

extern "C"
{
#include "user_code.h"
}

extern bool g_bPrepareGoSideFinishFlag;
extern bool g_bSideAccTaskFlag;
extern void side_acc_task(uint8 times);
#endif