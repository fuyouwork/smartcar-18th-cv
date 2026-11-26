#ifndef __MOTOR_H
#define __MOTOR_H

extern "C"
{
#include "zf_common_headfile.h"
}
#include "usr_common.hpp"
//定义每个电机PWM引脚
#define FL_EN_IN	(PWM1_MODULE3_CHB_D1)
#define FL_PH_IN	(C7)
#define FR_EN_IN	(PWM2_MODULE3_CHA_D2)
#define FR_PH_IN	(C12)
#define BL_EN_IN	(PWM1_MODULE3_CHA_D0)
#define BL_PH_IN	(C14)
#define BR_EN_IN	(PWM2_MODULE3_CHB_D3)
#define BR_PH_IN	(C10)

//定义正转电压
#define FVOLTAG		(1)
#define BVOLTAG		(0)

//定义每个电机初始化函数
#define MOTOR_PWM_FREQ_STANDARED 	(17000)	//电机标准频率，占空比单位：10us，由1/PWM_DUTY_MAX决定
#define MOTOR_PWM_FREQ				(MOTOR_PWM_FREQ_STANDARED)	//电机频率
#define MOTOR_PWM_DUTY_INIT 		(0)	//电机占空比初始化值
//#define MOTOR_PWM_DUTY_FAC MOTOR_PWM_FREQ

//电机输入限幅
#define MOTOR_INPUT_MIN 	(-10000.0)
#define MOTOR_INPUT_MAX 	( 10000.0)
//电机输出限幅
#define       MotorOUTMax                        (PWM_DUTY_MAX*8/10)
#define       MotorOUTMin                        (-PWM_DUTY_MAX*8/10)	// PWM输出最多一半


extern void motor_output(float fFLMotorOut, float fFRMotorOut, float fBLMotorOut, float fBRMotorOut);


#endif