#ifndef __COMMON_H
#define __COMMON_H


extern "C"
{
	#include "zf_common_headfile.h"
}

extern float scale(float input, float inputMin, float inputMax, float outputMin, float outputMax);
extern float float_abs(float fVariable);



#endif

