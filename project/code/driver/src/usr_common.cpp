#include "usr_common.hpp"

/**
   *@brief 归一化函数
   *@parame inputMin 输入最小值
   *@parame inputMax 输入最大值
   *@parame outputMin 输出最小值
   *@parame outputMax 输出最小值
   *@retval 返回float类型数据output
   */
float scale(float input, float inputMin, float inputMax, float outputMin, float outputMax) {
    float output;
    if (inputMin < inputMax)
        output = outputMin + ((input - inputMin) / ((inputMax - inputMin) / (outputMax - outputMin)));//在inputMin基准位置位移相应位置
    else
        output = outputMin + ((inputMin - input) / ((inputMin - inputMax) / (outputMax - outputMin)));//在inputMin基准位置位移相应位置
    if (output > outputMax)//对输出进行限幅，正常不会进入，进入了会报错
    {
        output = outputMax;
    }
    else if (output < outputMin)
    {
        output = outputMin;
    }
    return output;
}
float float_abs(float fVariable)
{
	if (fVariable >= 0)
		return fVariable;
	else
		return -fVariable;
}