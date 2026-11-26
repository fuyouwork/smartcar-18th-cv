#include "omnidirect.hpp"

float g_fXExpectPosition = 0; // 小车期望位置
float g_fYExpectPosition = 0;
float g_fXPositionRealAbsoluteCoordinate = 0; // 场地绝对坐标系实际X坐标
float g_fYPositionRealAbsoluteCoordinate = 0; // 场地绝对坐标系实际Y坐标

float g_fXPositionRealInertialSpeed = 0;
float g_fYPositionRealInertialSpeed = 0;

float g_fXOpenArtPosition = 172;	 // 74; // OpenART反馈的位置信息
float g_fYOpenArtPosition = 116;	 // 66;
float g_fXNowOpenArtPosition = 172;	 // 170; // OpenART反馈的位置信息
float g_fYNowOpenArtPosition = 116;	 // 133;
float g_fXLastOpenArtPosition = 170; // 69;
float g_fYLastOpenArtPosition = 116; // 75;

float g_fXOpenArtPositionExpect = 172; // 69; // OpenART期望的位置信息
float g_fYOpenArtPositionExpect = 116; // 75;
uint8 g_fXScc8660Position[10];		   // 凌瞳摄像头反馈的矩形中心点信息
uint8 g_fYScc8660Position[10];
uint8 g_fXScc8660SideArray[10]; // 边线中心点数组
uint8 g_fYScc8660SideArray[10];
uint16 g_fXScc8660SidePosition = 0; // 凌瞳反馈的边线中心点信息
uint16 g_fYScc8660SidePosition = 0;

uint8 g_byCenterNum = 0; // 图片中心点数量
uint8 g_bySideNum = 0;

float g_fXPositionCoeffient = 0.925;
float g_fYPositionCoeffient = 0.965;

float g_fXPositionAbsoluteSpeed = 0;
float g_fYPositionAbsoluteSpeed = 0;

uint8 g_byPointNum = 0; // 坐标点数量

// 角度制换算弧度制
float g_fYawAngleRad = s_ICMresult.fYawAngle / 57.3;
float g_fXErrorALL = 0;
float g_fYErrorALL = 0;
float g_fXCompensationErrorAll = 0;
float g_fYCompensationErrorAll = 0;
float g_fAlpha = 0;
void absolute_coordinate_calculate(void)
{
	float fYawAngleRad = 0;
	float fXPositionRealInertialSpeed = 0;
	float fYPositionRealInertialSpeed = 0;
	float fXPositionRealInertialError = 0;
	float fYPositionRealInertialError = 0;
	float fXPositionRealInertialCoordinate = 0; // 小车惯性坐标系实际X坐标
	float fYPositionRealInertialCoordinate = 0; // 小车惯性坐标系实际Y坐标
	float fXPositionRealAbsoluteCoordinate = 0;
	float fYPositionRealAbsoluteCoordinate = 0;
	float fXError = 0;
	float fYError = 0;
	float fXCompensationError = 0;
	float fYCompensationError = 0;
	static uint8 s_byTimeCounts;
	// 角度制换算弧度制
	g_fYawAngleRad = s_ICMresult.fYawAngle / 57.3;

	// 结算小车惯性坐标系X和Y实际坐标值，不做累加，根据逆解算公式得出惯性坐标系X,Y坐标值
	fXPositionRealInertialCoordinate = (float)(g_SEncoder.SEncoderFL.dRoute - g_SEncoder.SEncoderFR.dRoute - g_SEncoder.SEncoderBL.dRoute + g_SEncoder.SEncoderBR.dRoute) / 4;
	fYPositionRealInertialCoordinate = (float)(g_SEncoder.SEncoderFL.dRoute + g_SEncoder.SEncoderFR.dRoute + g_SEncoder.SEncoderBL.dRoute + g_SEncoder.SEncoderBR.dRoute) / 4;

	// 结算小车绝对坐标系X和Y实际坐标值，必须累加，根据惯性坐标系与绝对坐标系转换得到的绝对坐标系X，Y值

	fXPositionRealAbsoluteCoordinate = fYPositionRealInertialCoordinate * sin(g_fYawAngleRad) + fXPositionRealInertialCoordinate * cos(g_fYawAngleRad);
	fYPositionRealAbsoluteCoordinate = fYPositionRealInertialCoordinate * cos(g_fYawAngleRad) - fXPositionRealInertialCoordinate * sin(g_fYawAngleRad);
	if (g_byGoPointFlag || g_byPrepareScanFlag)
	{
		g_fXPositionRealAbsoluteCoordinate = g_fXPositionRealAbsoluteCoordinate + 0.902 * fXPositionRealAbsoluteCoordinate; // 0.914
		g_fYPositionRealAbsoluteCoordinate = g_fYPositionRealAbsoluteCoordinate + 1 * fYPositionRealAbsoluteCoordinate;
	}
	else
	{
		g_fXPositionRealAbsoluteCoordinate = g_fXPositionRealAbsoluteCoordinate + 0.914 * fXPositionRealAbsoluteCoordinate; // 0.914
		g_fYPositionRealAbsoluteCoordinate = g_fYPositionRealAbsoluteCoordinate + 1 * fYPositionRealAbsoluteCoordinate;
	}
	// 编码器积分路程清零
	g_SEncoder.SEncoderFL.dRoute = 0;
	g_SEncoder.SEncoderFR.dRoute = 0;
	g_SEncoder.SEncoderBL.dRoute = 0;
	g_SEncoder.SEncoderBR.dRoute = 0;
}
float g_fAngleExpect = 0;
float g_fXError = 0;
float g_fYError = 0;
float g_fXGoalError = 0;
float g_fYGoalError = 0;
uint8 g_byCoordinateMoveFlag = 0;
#define SPEED_LIMIT 300
void coordinate_omnidirect_move(float fXPositionExpect, float fYPositionExpect)
{
	float fFLSpeedExpect = 0; // 左上轮期望速度
	float fFRSpeedExpect = 0;
	float fBLSpeedExpect = 0;
	float fBRSpeedExpect = 0;

	float fYawAngle = s_ICMresult.fYawAngle / 57.3;
	float fAlpha = 1;
	float fBeta = 1;
	uint16 wRunStartTimeCounts = 0;
	// 清除OpenART的PID影响
	XOpenArtPID.pid_clear();
	YOpenArtPID.pid_clear();

	// 计算缓加速路程
	g_fXError = float_abs(fXPositionExpect - g_fXPositionRealAbsoluteCoordinate);
	g_fYError = float_abs(fYPositionExpect - g_fYPositionRealAbsoluteCoordinate);
	float x = float_abs(g_fXError - g_fXGoalError);
	float y = float_abs(g_fYError - g_fYGoalError);

	// 分段PID
	if (x > 50)
	{
		XOuterPID.m_fPositionItermLimit = 0.004 * x * MOTOR_OUTER_ITERM_LIMIT; // 0.004
	}
	else
	{
		XOuterPID.m_fPositionItermLimit = 40; // 80
	}
	if (y > 50)
	{
		YOuterPID.m_fPositionItermLimit = 0.006 * y * MOTOR_OUTER_ITERM_LIMIT;
	}
	else
	{
		YOuterPID.m_fPositionItermLimit = 60;
	}
	if (XOuterPID.m_fPositionItermLimit > MOTOR_OUTER_ITERM_LIMIT)
	{
		XOuterPID.m_fPositionItermLimit = MOTOR_OUTER_ITERM_LIMIT;
	}
	if (YOuterPID.m_fPositionItermLimit > MOTOR_OUTER_ITERM_LIMIT)
	{
		YOuterPID.m_fPositionItermLimit = MOTOR_OUTER_ITERM_LIMIT;
	}
	if (g_bSideAccTaskFlag)
	{
		XOuterPID.m_fPositionItermLimit = MOTOR_OUTER_ITERM_LIMIT;
	}
	XOuterPID.pid_set_param_from_buffer(2, 0, 0); // d:100
	YOuterPID.pid_set_param_from_buffer(2, 0, 0);
	if (g_bCorrectPointFlag)
	{
		XOuterPID.pid_set_param_from_buffer(5, 0, 0); // d:100
		YOuterPID.pid_set_param_from_buffer(5, 0, 0);
	}
#if OMINIDIRECT_MODE
	g_fAngleExpect = 0;
#endif
	AnglePID.pid_set_expect_value(g_fAngleExpect);

	XOuterPID.pid_set_expect_value(fXPositionExpect);
	YOuterPID.pid_set_expect_value(fYPositionExpect);
	float fXOuterSpeedExpect = XOuterPID.motor_pid_updata_output(g_fXPositionRealAbsoluteCoordinate);
	float fYOuterSpeedExpect = YOuterPID.motor_pid_updata_output(g_fYPositionRealAbsoluteCoordinate);
	float fXOuterSpeedOut;
	float fYOuterSpeedOut;
	if (cos(fYawAngle) == 0)
	{
		fXOuterSpeedOut = fYOuterSpeedExpect;
		fYOuterSpeedOut = -fXOuterSpeedExpect;
	}
	else if (sin(fYawAngle) == 0)
	{
		fXOuterSpeedOut = fXOuterSpeedExpect;
		fYOuterSpeedOut = fYOuterSpeedExpect;
	}
	else
	{
		fYOuterSpeedOut = (fXOuterSpeedExpect * tan(fYawAngle) + fYOuterSpeedExpect) /
						  (cos(fYawAngle) + sin(fYawAngle) * sin(fYawAngle) / cos(fYawAngle));
		fXOuterSpeedOut = (fXOuterSpeedExpect - fYOuterSpeedOut * sin(fYawAngle)) / cos(fYawAngle);
	}
	float fAngleSpeedExpect = AnglePID.motor_pid_updata_output(s_ICMresult.fYawAngle);

	fFLSpeedExpect = (fYOuterSpeedOut + fXOuterSpeedOut) + fAngleSpeedExpect;
	fFRSpeedExpect = (fYOuterSpeedOut - fXOuterSpeedOut) - fAngleSpeedExpect;
	fBLSpeedExpect = (fYOuterSpeedOut - fXOuterSpeedOut) + fAngleSpeedExpect;
	fBRSpeedExpect = (fYOuterSpeedOut + fXOuterSpeedOut) - fAngleSpeedExpect;
	// 串级PID内环
	FLMotorInnerPID.pid_set_expect_value(fFLSpeedExpect);
	FRMotorInnerPID.pid_set_expect_value(fFRSpeedExpect);
	BLMotorInnerPID.pid_set_expect_value(fBLSpeedExpect);
	BRMotorInnerPID.pid_set_expect_value(fBRSpeedExpect);

	g_byCoordinateMoveFlag = 1;
}
void openart_xy_speed(float fXSpeed, float fYSpeed)
{
	g_fFLOpenARTSpeedExpect = fYSpeed + fXSpeed;
	g_fFROpenARTSpeedExpect = fYSpeed - fXSpeed;
	g_fBLOpenARTSpeedExpect = fYSpeed - fXSpeed;
	g_fBROpenARTSpeedExpect = fYSpeed + fXSpeed;
}
int sign(float data)
{
	if (data > 0)
		return 1;
	else if (data < 0)
		return -1;
	else
		return 0;
}
void angle_correct(void)
{
	float fFLSpeedExpect = 0; // 左上轮期望的路程
	float fFRSpeedExpect = 0;
	float fBLSpeedExpect = 0;
	float fBRSpeedExpect = 0;

	float fAngleSpeedExpect = AnglePID.motor_pid_updata_output(s_ICMresult.fYawAngle);

	fFLSpeedExpect = (g_fFLOpenARTSpeedExpect) + fAngleSpeedExpect;
	fFRSpeedExpect = (g_fFROpenARTSpeedExpect)-fAngleSpeedExpect;
	fBLSpeedExpect = (g_fBLOpenARTSpeedExpect) + fAngleSpeedExpect;
	fBRSpeedExpect = (g_fBROpenARTSpeedExpect)-fAngleSpeedExpect;

	FLMotorInnerPID.pid_set_expect_value(fFLSpeedExpect);
	FRMotorInnerPID.pid_set_expect_value(fFRSpeedExpect);
	BLMotorInnerPID.pid_set_expect_value(fBLSpeedExpect);
	BRMotorInnerPID.pid_set_expect_value(fBRSpeedExpect);
}
float g_fFLOpenARTSpeedExpect = 0;
float g_fFROpenARTSpeedExpect = 0;
float g_fBLOpenARTSpeedExpect = 0;
float g_fBROpenARTSpeedExpect = 0;

uint8 g_byCorrectMode = CORRECT_ALL;
void openart_correct_move(void)
{
	float fXOpenArtPIDOut = 0;
	float fYOpenArtPIDOut = 0;
	XOuterPID.pid_clear();
	YOuterPID.pid_clear();

	XOpenArtPID.pid_set_param_from_buffer(-1, 0, 0);
	YOpenArtPID.pid_set_param_from_buffer(1, 0, 0);

	XOpenArtPID.pid_set_expect_value(g_fXOpenArtPositionExpect);
	YOpenArtPID.pid_set_expect_value(g_fYOpenArtPositionExpect);

	if (g_byCorrectMode == CORRECT_X || g_byCorrectMode == CORRECT_ALL)
		fXOpenArtPIDOut = XOpenArtPID.motor_pid_updata_output(g_fXOpenArtPosition);
	if (g_byCorrectMode == CORRECT_Y || g_byCorrectMode == CORRECT_ALL)
		fYOpenArtPIDOut = YOpenArtPID.motor_pid_updata_output(g_fYOpenArtPosition);
	g_fFLOpenARTSpeedExpect = (fYOpenArtPIDOut + fXOpenArtPIDOut);
	g_fFROpenARTSpeedExpect = (fYOpenArtPIDOut - fXOpenArtPIDOut);
	g_fBLOpenARTSpeedExpect = (fYOpenArtPIDOut - fXOpenArtPIDOut);
	g_fBROpenARTSpeedExpect = (fYOpenArtPIDOut + fXOpenArtPIDOut);
}
// 里程计算
void calculate_goal_error(void)
{
	g_fXGoalError = float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate);
	g_fYGoalError = float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate);
}