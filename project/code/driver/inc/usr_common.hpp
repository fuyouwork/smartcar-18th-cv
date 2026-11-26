#ifndef __USR_COMMON_H
#define __USR_COMMON_H


extern "C"
{
	#include "zf_common_headfile.h"
}

extern float scale(float input, float inputMin, float inputMax, float outputMin, float outputMax);
extern float float_abs(float fVariable);



#endif

