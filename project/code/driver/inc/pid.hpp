/****************************************************************
 * @file            PID.h
 * @school          南昌航空大学第十七届全国大学生智能车竞大赛智能视觉组
 * @author          ZHP/CYL/GZW
 * @version         v2.0
 * @Software        KEIL
 * @Target core     MM32F3277G
 * @date            2021-3-19
 ****************************************************************/
#ifndef _PID_H
#define _PID_H

#define MEASURE_ERROR_COUNTS 4000
// 均方差误差样本量
#define PID_TEST_FLAG 0 // 是否开启PID测试程序

#define MOTOR_OUTER_KP (1)					  // 电机外环(即位置环)Kp
#define MOTOR_OUTER_KI (0)					  // 电机外环(即位置环)Ki
#define MOTOR_OUTER_KD (0)					  // 电机外环(即位置环)Kd
#define MOTOR_OUTER_DEAD_ZONE (1)			  // 电机外环(即位置环)PID调整死区
#define MOTOR_OUTER_INTEGRAL_ITERM_LIMIT (20) // 电机外环(即位置环)积分项限幅

#define MOTOR_OUTER_INTEGRAL_START_ERR (10) // 电机外环(即位置环)积分分离区

#define OPENART_X_KP (-1)				  // X电机外环(即位置环)Kp
#define OPENART_X_KI (0)				  // X电机外环(即位置环)Ki
#define OPENART_X_KD (0)				  // X电机外环(即位置环)Kd
#define OPENART_Y_KP (1)				  // Y电机外环(即位置环)Kp
#define OPENART_Y_KI (0)				  // Y电机外环(即位置环)Ki
#define OPENART_Y_KD (0)				  // Y电机外环(即位置环)Kd
#define OPENART_DEAD_ZONE (0.2)			  // 电机外环(即位置环)PID调整死区
#define OPENART_INTEGRAL_ITERM_LIMIT (20) // 电机外环(即位置环)积分项限幅
#define OPENART_ITERM_LIMIT (60)		  // 电机外环输出限幅
#define OPENART_INTEGRAL_START_ERR (10)	  // 电机外环(即位置环)积分分离区

#define ANGLE_KP (4.989) // 2.049
#define ANGLE_KI (0.2)
#define ANGLE_KD (200)
#define ANGLE_INTERGRAL_LIMIT (150)
#define ANGLE_ITERM_LIMIT (300) // 角度项积分项输出不超过100
#define ANGLE_DEADZONE (0.6)
#define ANGLE_START_ERROR (5) // 积分剥离当误差大于30

#define MOTOR_INNER_KP (41.0)
#define MOTOR_INNER_KI (0.797)
#define MOTOR_INNER_KD (0)
#define MOTOR_INNER_DEAD_ZONE (0)
#define MOTOR_INNER_INCREASEMENTITERM_LIMIT (MOTOR_INPUT_MAX * 10 / 10)

#include "user_code_cpp.hpp"
extern uint32 g_bMotorSpeed;
#define MOTOR_OUTER_ITERM_LIMIT (g_bMotorSpeed) // 电机外环输出限幅
// #define MOTOR_OUTER_ITERM_LIMIT (200)		  // 电机外环输出限幅
typedef struct
{
	float fKp;
	float fKi;
	float fKd;
} PID_t;
typedef struct
{
	float fCurrentError;	 // 这次误差
	float fLastError;		 // 上次誤差
	float fSumError;		 // 积累误差
	float fPreviousError;	 // 上上次误差
	double dSquareMeanError; // 误差均方值
	double dMeanError;		 // 平均误差
	uint16 wErrorCounts;	 // 误差测量次数
	bool bInDeadZoneFlag;
	bool bTestFinishFlag; // 误差测量完成标志
} ERROR_t;
// 类PID
class pid_t
{
protected:
	float m_fRealValue;
	ERROR_t m_SError;
	void pid_update_mean_square_error(void);

public:
	PID_t m_Spid;
	float m_fExpectValue;
	pid_t(float fKp, float fKi, float fKd);
	void pid_set_expect_value(float fExpectValue);
	void pid_set_param_from_buffer(float fKp = 0, float fKi = 0, float fKd = 0);
	ERROR_t get_error_struct(void) { return m_SError; }
	friend void flash_write_param(void);
};
// 类位置式PID
class pid_position_t : public pid_t
{
private:
	float m_fDeadZone;
	float m_fIntegralItermLimit; // 积分限幅
	float m_fIntegralStartErr;
	float m_fPositionIterm;

public:
	float m_fPositionItermLimit;
	pid_position_t(float fKp, float fKi, float fKd, float fIntegralItermLimit,
				   float fDeadZone, float fPositionItermLimit, float fIntegralStartErr);
	float motor_pid_updata_output(float fRealValue);
	void pid_clear(void);
};
// 类增量式PID
class pid_increasement_t : public pid_t
{
private:
	float m_fIncreasementLimit;
	float m_fIncreasementIterm;
	float m_fIncreasementItermLimit;

public:
	pid_increasement_t(float fKp, float fKi, float fKd, float fIncreasementItermLimit);
	float motor_pid_updata_increasement_output(float fRealValue);
	void pid_clear(void);
};
extern void pid_test(void);
extern void pid_restart(void);
extern float g_fSpeedExpectValue;
extern pid_increasement_t FLMotorInnerPID, FRMotorInnerPID, BLMotorInnerPID, BRMotorInnerPID;
extern pid_position_t XOpenArtPID, YOpenArtPID, XOuterPID, YOuterPID, AnglePID;
extern int16 g_bySwitchTestFlag;
extern bool g_bWriteTestFlag;
extern uint8 g_byStopWriteFlag;

#endif