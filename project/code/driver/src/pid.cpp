#include "pid.hpp"
float g_fSpeedExpectValue = 0;
uint32 g_bMotorSpeed = 200;
pid_increasement_t FLMotorInnerPID(MOTOR_INNER_KP, MOTOR_INNER_KI, MOTOR_INNER_KD, MOTOR_INNER_INCREASEMENTITERM_LIMIT);
pid_increasement_t FRMotorInnerPID(MOTOR_INNER_KP, MOTOR_INNER_KI, MOTOR_INNER_KD, MOTOR_INNER_INCREASEMENTITERM_LIMIT);
pid_increasement_t BLMotorInnerPID(MOTOR_INNER_KP, MOTOR_INNER_KI, MOTOR_INNER_KD, MOTOR_INNER_INCREASEMENTITERM_LIMIT);
pid_increasement_t BRMotorInnerPID(MOTOR_INNER_KP, MOTOR_INNER_KI, MOTOR_INNER_KD, MOTOR_INNER_INCREASEMENTITERM_LIMIT);

pid_position_t XOuterPID(MOTOR_OUTER_KP, MOTOR_OUTER_KI, MOTOR_OUTER_KD, MOTOR_OUTER_INTEGRAL_ITERM_LIMIT, MOTOR_OUTER_DEAD_ZONE, MOTOR_OUTER_ITERM_LIMIT, MOTOR_OUTER_INTEGRAL_START_ERR);
pid_position_t YOuterPID(MOTOR_OUTER_KP, MOTOR_OUTER_KI, MOTOR_OUTER_KD, MOTOR_OUTER_INTEGRAL_ITERM_LIMIT, MOTOR_OUTER_DEAD_ZONE, MOTOR_OUTER_ITERM_LIMIT, MOTOR_OUTER_INTEGRAL_START_ERR);
pid_position_t XOpenArtPID(OPENART_X_KP, OPENART_X_KI, OPENART_X_KD, OPENART_INTEGRAL_ITERM_LIMIT, OPENART_DEAD_ZONE, OPENART_ITERM_LIMIT, OPENART_INTEGRAL_START_ERR);
pid_position_t YOpenArtPID(OPENART_Y_KP, OPENART_Y_KI, OPENART_Y_KD, OPENART_INTEGRAL_ITERM_LIMIT, OPENART_DEAD_ZONE, OPENART_ITERM_LIMIT, OPENART_INTEGRAL_START_ERR);
pid_position_t AnglePID(ANGLE_KP, ANGLE_KI, ANGLE_KD, ANGLE_INTERGRAL_LIMIT, ANGLE_DEADZONE, ANGLE_ITERM_LIMIT, ANGLE_START_ERROR);
/**
 *@brief 位置式PID初始化
 *@parame fKp:PID的比例系数
 *@parame fKi:PID的积分系数
 *@parame fKd:PID的微分系数
 *@retval 无
 */
pid_t::pid_t(float fKp, float fKi, float fKd)
{
	m_Spid.fKp = fKp;
	m_Spid.fKi = fKi;
	m_Spid.fKd = fKd;

	m_SError.fCurrentError = 0;
	m_SError.fLastError = 0;
	m_SError.fPreviousError = 0;
	m_SError.fSumError = 0;
	m_SError.dSquareMeanError = 0;
	m_SError.wErrorCounts = 0;
	m_SError.dMeanError = 0;
	m_SError.bInDeadZoneFlag = 0;
	m_SError.bTestFinishFlag = 0;
	m_fExpectValue = 0;
}

/**
 *@brief 更改PID的三个参数
 *@parame 无
 *@retval 无
 */
void pid_t::pid_set_param_from_buffer(float fKp, float fKi, float fKd)
{
	m_Spid.fKp = fKp;
	m_Spid.fKi = fKi;
	m_Spid.fKd = fKd;
}

/**
 *@brief 设定PID预期值
 *@parame fExpectValue 预期值
 *@retval 无
 */
void pid_t::pid_set_expect_value(float fExpectValue)
{
	m_fExpectValue = fExpectValue;
}
/**
 *@brief 取样MEASURE_ERROR_COUNTS的均方差值，更新均方误差
 *@parame 无
 *@retval 无
 */
void pid_t::pid_update_mean_square_error(void)
{
	if (m_SError.bTestFinishFlag == 0)
	{
		if (m_SError.wErrorCounts <= MEASURE_ERROR_COUNTS) // 5000 * 2 = 10000
		{
			m_SError.dSquareMeanError += m_SError.fCurrentError * m_SError.fCurrentError;
			m_SError.dMeanError += m_SError.fCurrentError;
			m_SError.wErrorCounts++;
			if (m_SError.wErrorCounts == MEASURE_ERROR_COUNTS)
			{
				m_SError.dSquareMeanError = sqrt(m_SError.dSquareMeanError / m_SError.wErrorCounts);
				m_SError.dMeanError = m_SError.dMeanError / m_SError.wErrorCounts;
				m_SError.bTestFinishFlag = 1;				  // 测试程序完成
				g_byMotorStartFlag = 0;						  // 测试完成关闭电机
				gpio_set_level(MOTOR_SWITCH_PIN, LEVEL_STOP); // 使能电机
			}
		}
	}
}
/**
 *@brief 位置式PID初始化
 *@parame fKp:PID的比例系数
 *@parame fKi:PID的积分系数
 *@parame fKd:PID的微分系数
 *@parame fDeadZone:误差死区
 *@parame fIntegralItermLimit:积分项限幅
 *@parame fIntegralStartErr:积分分离的误差范围
 *@retval 无
 */
pid_position_t::pid_position_t(float fKp, float fKi, float fKd, float fIntegralItermLimit,
							   float fDeadZone, float fPositionItermLimit, float fIntegralStartErr) : pid_t(fKp, fKi, fKd)
{
	m_fIntegralItermLimit = fIntegralItermLimit;
	m_fIntegralStartErr = fIntegralStartErr;
	m_fDeadZone = fDeadZone;
	m_fPositionItermLimit = fPositionItermLimit;
	m_fPositionIterm = 0;
}
/**
 *@brief 位置式PID更新速度
 *@parame 无
 *@retval 无
 */
float pid_position_t::motor_pid_updata_output(float fRealValue)
{
	float fProportionIterm = 0;
	float fDifferentialIterm = 0;
	float fIntegralIterm = 0;

	m_SError.fCurrentError = m_fExpectValue - fRealValue;
	if (float_abs(m_SError.fCurrentError) <= float_abs(m_fIntegralStartErr))
		m_SError.fSumError = m_SError.fSumError + m_SError.fCurrentError;
	else
	{
		m_SError.fSumError = 0;
	}
	// 设定闭环死区
	if (float_abs(m_SError.fCurrentError) <= float_abs(m_fDeadZone))
	{
		m_SError.fCurrentError = 0;
		m_SError.fLastError = 0;
		m_SError.fSumError = 0;
		m_SError.bInDeadZoneFlag = 1;
	}
	else
	{
		m_SError.bInDeadZoneFlag = 0;
	}
	// 结算比例项
	fProportionIterm = m_Spid.fKp * m_SError.fCurrentError;
	fDifferentialIterm = m_Spid.fKd * (m_SError.fCurrentError - m_SError.fLastError);
	fIntegralIterm = m_Spid.fKi * m_SError.fSumError;

	// 积分限幅
	if (float_abs(fIntegralIterm) >= float_abs(m_fIntegralItermLimit))
	{
		if (fIntegralIterm >= 0)
			fIntegralIterm = m_fIntegralItermLimit;
		else
			fIntegralIterm = -m_fIntegralItermLimit;
		m_SError.fSumError = fIntegralIterm / m_Spid.fKi;
	}

	// 位置式PID公式
	m_fPositionIterm = fProportionIterm + fIntegralIterm + fDifferentialIterm;
	m_SError.fLastError = m_SError.fCurrentError; // 更新记录的上一次误差

	if (float_abs(m_fPositionIterm) >= float_abs(m_fPositionItermLimit))
	{
		if (m_fPositionIterm > 0)
			m_fPositionIterm = m_fPositionItermLimit;
		else
			m_fPositionIterm = -m_fPositionItermLimit;
	}
	return m_fPositionIterm;
}
void pid_position_t::pid_clear(void)
{
	m_SError.fCurrentError = 0;
	m_SError.fLastError = 0;
	m_SError.fPreviousError = 0;
	m_SError.fSumError = 0;
	m_SError.bInDeadZoneFlag = 0;
	m_fPositionIterm = 0;
}

/**
 *@brief 增量式PID初始化
 *@parame fKp:PID的比例系数
 *@parame fKi:PID的积分系数
 *@parame fKd:PID的微分系数
 *@retval 无
 */
pid_increasement_t::pid_increasement_t(float fKp, float fKi, float fKd, float fIncreasementItermLimit) : pid_t(fKp, fKi, fKd)
{
	m_fIncreasementItermLimit = fIncreasementItermLimit;
	m_fIncreasementIterm = 0;
}

/**
 *@brief 增量式PID更新速度，适用于带积分器件
 *@parame 无
 *@retval 无
 */
float pid_increasement_t ::motor_pid_updata_increasement_output(float fRealValue)
{
	float fIncreasement = 0;
	m_fRealValue = fRealValue;

	m_SError.fCurrentError = m_fExpectValue - fRealValue; // 更新误差，更新误差顺序不能改变
	if (PID_TEST_FLAG)
		pid_update_mean_square_error();
	fIncreasement = m_Spid.fKp * (m_SError.fCurrentError - m_SError.fLastError) + m_Spid.fKi * m_SError.fCurrentError + m_Spid.fKd * (m_SError.fCurrentError - 2 * m_SError.fLastError + m_SError.fPreviousError);

	m_fIncreasementIterm += fIncreasement;
	if (float_abs(m_fIncreasementIterm) >= float_abs(m_fIncreasementItermLimit))
	{
		if (m_fIncreasementIterm >= 0)
			m_fIncreasementIterm = m_fIncreasementItermLimit; // 负反馈系数
		else
			m_fIncreasementIterm = -m_fIncreasementItermLimit; // 负反馈系数
	}
	m_SError.fPreviousError = m_SError.fLastError;
	m_SError.fLastError = m_SError.fCurrentError;
	return m_fIncreasementIterm;
}
/**
 *@brief 清除PID缓存
 *@parame 无
 *@retval 无
 */
void pid_increasement_t ::pid_clear(void)
{
	m_fRealValue = 0;
	m_fIncreasementIterm = 0;
	m_SError.fCurrentError = 0;
	m_SError.fLastError = 0;
	m_SError.fPreviousError = 0;
	m_SError.dSquareMeanError = 0;
	m_SError.dMeanError = 0;
	m_SError.wErrorCounts = 0;
	m_SError.bTestFinishFlag = 0;
}

/**
 *@brief PID重启函数
 *@parame 无
 *@retval 无
 */
void pid_restart(void)
{
	FLMotorInnerPID.pid_clear();
	FRMotorInnerPID.pid_clear();
	BLMotorInnerPID.pid_clear();
	BRMotorInnerPID.pid_clear();
	XOuterPID.pid_clear();
	YOuterPID.pid_clear();
	XOpenArtPID.pid_clear();
	YOpenArtPID.pid_clear();
	AnglePID.pid_clear();
	motor_output(0, 0, 0, 0);
}
uint32 g_dwFlag2s = 0;
uint32 g_dwFlag10s = 0;
bool g_bWriteTestFlag = 0;
int16 g_bySwitchTestFlag = 0;
uint8 g_byStopWriteFlag = 0;
/**
 *@brief PID测试函数
 *@parame 无
 *@retval 无
 */
void pid_test(void)
{
	if (PID_TEST_FLAG)
	{
		g_dwFlag2s++;
		if (!g_byMotorStartFlag) // 关闭电机后自动重启
		{
			if (g_dwFlag2s == 500) // 等待1S,完成发送工作
			{
			}
			if (g_dwFlag2s == 1000) // 等待2s，完成修改工作
			{
				g_bySwitchTestFlag++;
				if (g_bySwitchTestFlag == 1)
				{
					g_fSpeedExpectValue = -g_fSpeedExpectValue;
					FLMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
					FRMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
					BLMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
					BRMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);

					FLMotorInnerPID.m_Spid.fKp = FLMotorInnerPID.m_Spid.fKp + g_fChangeFactor[FLP]; // 修改Kp值
					FRMotorInnerPID.m_Spid.fKp = FRMotorInnerPID.m_Spid.fKp + g_fChangeFactor[FRP];
					BLMotorInnerPID.m_Spid.fKp = BLMotorInnerPID.m_Spid.fKp + g_fChangeFactor[BLP];
					BRMotorInnerPID.m_Spid.fKp = BRMotorInnerPID.m_Spid.fKp + g_fChangeFactor[BRP];

					if (FLMotorInnerPID.m_Spid.fKp > 40 + (FLMotorInnerPID.m_Spid.fKi - 1) * 30)
					{
						FLMotorInnerPID.m_Spid.fKi = FLMotorInnerPID.m_Spid.fKi + g_fChangeFactor[FLI]; // 修改Kp值
						FRMotorInnerPID.m_Spid.fKi = FRMotorInnerPID.m_Spid.fKi + g_fChangeFactor[FRI];
						BLMotorInnerPID.m_Spid.fKi = BLMotorInnerPID.m_Spid.fKi + g_fChangeFactor[BLI];
						BRMotorInnerPID.m_Spid.fKi = BRMotorInnerPID.m_Spid.fKi + g_fChangeFactor[BRI];

						FLMotorInnerPID.m_Spid.fKp = 20 + (FLMotorInnerPID.m_Spid.fKi - 1) * 20; // 修改Kp值
						FRMotorInnerPID.m_Spid.fKp = 20 + (FRMotorInnerPID.m_Spid.fKi - 1) * 20;
						BLMotorInnerPID.m_Spid.fKp = 20 + (BLMotorInnerPID.m_Spid.fKi - 1) * 20;
						BRMotorInnerPID.m_Spid.fKp = 20 + (BRMotorInnerPID.m_Spid.fKi - 1) * 20;
					}
					g_bWriteTestFlag = 1;
				}
				if (g_bySwitchTestFlag == 2)
				{
					g_fSpeedExpectValue = -g_fSpeedExpectValue;
					FLMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
					FRMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
					BLMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
					BRMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);

					FLMotorInnerPID.m_Spid.fKp = FLMotorInnerPID.m_Spid.fKp + g_fChangeFactor[FLP]; // 修改Kp值
					FRMotorInnerPID.m_Spid.fKp = FRMotorInnerPID.m_Spid.fKp + g_fChangeFactor[FRP];
					BLMotorInnerPID.m_Spid.fKp = BLMotorInnerPID.m_Spid.fKp + g_fChangeFactor[BLP];
					BRMotorInnerPID.m_Spid.fKp = BRMotorInnerPID.m_Spid.fKp + g_fChangeFactor[BRP];

					if (FLMotorInnerPID.m_Spid.fKp > 40 + (FLMotorInnerPID.m_Spid.fKi - 1) * 30)
					{
						FLMotorInnerPID.m_Spid.fKi = FLMotorInnerPID.m_Spid.fKi + g_fChangeFactor[FLI]; // 修改Ki值
						FRMotorInnerPID.m_Spid.fKi = FRMotorInnerPID.m_Spid.fKi + g_fChangeFactor[FRI];
						BLMotorInnerPID.m_Spid.fKi = BLMotorInnerPID.m_Spid.fKi + g_fChangeFactor[BLI];
						BRMotorInnerPID.m_Spid.fKi = BRMotorInnerPID.m_Spid.fKi + g_fChangeFactor[BRI];

						FLMotorInnerPID.m_Spid.fKp = 20 + (FLMotorInnerPID.m_Spid.fKi - 1) * 20; // 修改Kp值
						FRMotorInnerPID.m_Spid.fKp = 20 + (FRMotorInnerPID.m_Spid.fKi - 1) * 20;
						BLMotorInnerPID.m_Spid.fKp = 20 + (BLMotorInnerPID.m_Spid.fKi - 1) * 20;
						BRMotorInnerPID.m_Spid.fKp = 20 + (BRMotorInnerPID.m_Spid.fKi - 1) * 20;
					}
					g_bySwitchTestFlag = 0;
					g_bWriteTestFlag = 1;
				}
			}
			if (g_dwFlag2s == 3000) // 等待3s，完成测试重启工作
			{
				g_byStopWriteFlag = 0;
				g_byMotorStartFlag = 1;						   // 重新启动电机
				gpio_set_level(MOTOR_SWITCH_PIN, LEVEL_START); // 使能电机
				pid_restart();								   // 清除PID原先状态
				g_byPrior = 0;								   // 进入下一个循环
				g_dwFlag2s = 0;								   // 等待下一个测试程序的完成
			}
		}
		else
		{
			g_dwFlag2s = 0;
		}
	}
}