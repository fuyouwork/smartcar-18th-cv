#ifndef _PATH_PLAN_hpp
#define _PATH_PLAN_hpp

#include "TSP.hpp"
#include "connection.hpp"
extern float g_fPointPositionX[30], g_fPointPositionY[30];

extern void path_plan(void);
extern void sorted(float* array_y, float* array_x, int count, int* index);

#endif