/****************************************************************
 * @file            motor.c
 * @school         南昌航空大学第十七届全国大学生智能车大赛智能视觉组
 * @author          ZHP/CYL/GZW
 * @version         v2.0
 * @Software        KEIL
 * @date            2022-3-1
 ****************************************************************/
#include "motor.hpp"

#define PWM_RESTRICT
/**
 *@brief motor finall output
 *@parame iFLMotorOut Front left motor speed set. Range:(MOTOR_INPUT_MIN , MOTOR_INPUT_MAX)
 *@parame iBLMotorOut Back left motor speed set. Range:(MOTOR_INPUT_MIN , MOTOR_INPUT_MAX)
 *@parame iFRMotorOut Front Right motor speed set. Range:(MOTOR_INPUT_MIN , MOTOR_INPUT_MAX)
 *@parame iBRMotorOut Back Right motor speed set. Range:(MOTOR_INPUT_MIN , MOTOR_INPUT_MAX)
 *@retval 无
 */
void motor_output(float fFLMotorOut, float fFRMotorOut, float fBLMotorOut, float fBRMotorOut)
{
    int32_t iFLMotorOut = 0;
    int32_t iFRMotorOut = 0;
    int32_t iBLMotorOut = 0;
    int32_t iBRMotorOut = 0;

    // 对输入值进行归一化处理
    iFLMotorOut = (int32_t)scale(fFLMotorOut, MOTOR_INPUT_MIN, MOTOR_INPUT_MAX, -PWM_DUTY_MAX, PWM_DUTY_MAX);
    iFRMotorOut = (int32_t)scale(fFRMotorOut, MOTOR_INPUT_MIN, MOTOR_INPUT_MAX, -PWM_DUTY_MAX, PWM_DUTY_MAX);
    iBLMotorOut = (int32_t)scale(fBLMotorOut, MOTOR_INPUT_MIN, MOTOR_INPUT_MAX, -PWM_DUTY_MAX, PWM_DUTY_MAX);
    iBRMotorOut = (int32_t)scale(fBRMotorOut, MOTOR_INPUT_MIN, MOTOR_INPUT_MAX, -PWM_DUTY_MAX, PWM_DUTY_MAX);

    // 宏定义PWM_RESTRICT开启电机限幅
#ifdef PWM_RESTRICT
    if (iFLMotorOut > MotorOUTMax)
        iFLMotorOut = MotorOUTMax;
    if (iFRMotorOut > MotorOUTMax)
        iFRMotorOut = MotorOUTMax;
    if (iBLMotorOut > MotorOUTMax)
        iBLMotorOut = MotorOUTMax;
    if (iBRMotorOut > MotorOUTMax)
        iBRMotorOut = MotorOUTMax;
    if (iFLMotorOut < MotorOUTMin)
        iFLMotorOut = MotorOUTMin;
    if (iFRMotorOut < MotorOUTMin)
        iFRMotorOut = MotorOUTMin;
    if (iBLMotorOut < MotorOUTMin)
        iBLMotorOut = MotorOUTMin;
    if (iBRMotorOut < MotorOUTMin)
        iBRMotorOut = MotorOUTMin;
#endif
    // 左上电机
    if (iFLMotorOut > 0)
    {
        gpio_set_level(FL_PH_IN, FVOLTAG);
        pwm_set_duty(FL_EN_IN, iFLMotorOut);
    }
    else
    {
        iFLMotorOut = -iFLMotorOut;
        gpio_set_level(FL_PH_IN, BVOLTAG);
        pwm_set_duty(FL_EN_IN, iFLMotorOut);
    }

    // 右前电机
    if (iFRMotorOut > 0)
    {
        gpio_set_level(FR_PH_IN, FVOLTAG);
        pwm_set_duty(FR_EN_IN, iFRMotorOut);
    }
    else
    {
        iFRMotorOut = -iFRMotorOut;
        gpio_set_level(FR_PH_IN, BVOLTAG);
        pwm_set_duty(FR_EN_IN, iFRMotorOut);
    }

    // 左后电机
    if (iBLMotorOut > 0)
    {
        gpio_set_level(BL_PH_IN, FVOLTAG);
        pwm_set_duty(BL_EN_IN, iBLMotorOut);
    }
    else
    {
        iBLMotorOut = -iBLMotorOut;
        gpio_set_level(BL_PH_IN, BVOLTAG);
        pwm_set_duty(BL_EN_IN, iBLMotorOut);
    }

    // 右后电机
    if (iBRMotorOut > 0)
    {
        gpio_set_level(BR_PH_IN, FVOLTAG);
        pwm_set_duty(BR_EN_IN, iBRMotorOut);
    }
    else
    {
        iBRMotorOut = -iBRMotorOut;
        gpio_set_level(BR_PH_IN, BVOLTAG);
        pwm_set_duty(BR_EN_IN, iBRMotorOut);
    }
}
