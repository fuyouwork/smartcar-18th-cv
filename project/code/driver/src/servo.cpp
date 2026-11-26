#include "servo.hpp"
uint32_t dwRotaryPWM = 500;
servo_t Servo10K = {
	.pin = SERVO_10K_PIN,
	.WhichServo = SERVO_10K,
	.fMaxAngle = 360,
	.fAngleSet = SERVO_10K_RESET_ANGLE,
	.fAngleExpect = SERVO_10K_RESET_ANGLE}; // 下面
servo_t Servo20K = {
	.pin = SERVO_20K_PIN,
	.WhichServo = SERVO_20K,
	.fMaxAngle = SPT_SERVO_MAX_ANGLE,
	.fAngleSet = SERVO_20K_RESET_ANGLE,
	.fAngleExpect = SERVO_20K_RESET_ANGLE}; // 上面
servo_t Servo35K = {
	.pin = SERVO_35K_PIN,
	.WhichServo = SERVO_35K,
	.fMaxAngle = 360,
	.fAngleSet = SERVO_35K_RESET_ANGLE,
	.fAngleExpect = SERVO_35K_RESET_ANGLE};
int16 g_iPhoneixLastAngle = 0;
int8 g_iPhoneixNowAngle = 0;
PhonexitTableState PhonexitTurntableCategory[5] = {{90, 0}, {0, 0}, {90, 0}, {180, 0}, {270, 0}};

Category_t g_Category[15] = {
	{"fruits", "durian", 0, 0, 4},
	{"fruits", "orange", 0, 1, 1},
	{"fruits", "apple", 0, 2, 5},
	{"fruits", "grape", 0, 3, 0},
	{"fruits", "banana", 0, 4, 2},
	{"food", "rice", 1, 5, 2},
	{"food", "corn", 1, 6, 0},
	{"food", "potato", 1, 7, 4},
	{"food", "peanut", 1, 8, 5},
	{"food", "bean", 1, 9, 1},
	{"greens", "cabbage", 2, 10, 1},
	{"greens", "eggplant", 2, 11, 0},
	{"greens", "radish", 2, 12, 2},
	{"greens", "chili", 2, 13, 5},
	{"greens", "cucumber", 2, 14, 4}};
Category_t *g_CategoryMap[15] = {
	&g_Category[9],	 // 0
	&g_Category[6],	 // 1
	&g_Category[8],	 // 2
	&g_Category[7],	 // 3
	&g_Category[5],	 // 4
	&g_Category[2],	 // 5
	&g_Category[4],	 // 6
	&g_Category[0],	 // 7
	&g_Category[3],	 // 8
	&g_Category[1],	 // 9
	&g_Category[10], // 10
	&g_Category[13], // 11
	&g_Category[14], // 12
	&g_Category[11], // 13
	&g_Category[12], // 14
};

uint16 servo_times_to_duty(uint16 duty)
{
	return (uint16)scale(duty, 500, 2500, SPT_SERVO_DUTY_MIN, SPT_SERVO_DUTY_MAX);
}
/**
 *@brief 舵机角度转化占空比函数
 *@parame 要转换的角度,Range:0-max_angle
 *@parame 角度最大值,Range:>0
 *@retval 转换后的占空比
 */
uint16 servo_angle_to_duty(servo_t *Servo)
{
	return (uint16)scale(Servo->fAngleSet, 0, Servo->fMaxAngle, SPT_SERVO_DUTY_MIN, SPT_SERVO_DUTY_MAX);
}
float g_fServoIncrease = 1.5;
/// @brief 舵机移动控制
/// @param times 移动时间设置
void servo_angle_turn_speed(servo_t *Servo)
{
#if SERVO_PROTECT
	float fServoIncrease = 1.0;
	if (Servo->WhichServo == SERVO_35K)
	{
		fServoIncrease = 1.5;
	}
#else
	float fServoIncrease = 1.5;
#endif

	if (float_abs(Servo->fAngleSet - Servo->fAngleExpect) < fServoIncrease)
	{
		fServoIncrease = float_abs(Servo->fAngleSet - Servo->fAngleExpect);
	}
	if (Servo->fAngleSet > Servo->fAngleExpect)
		Servo->fAngleSet -= fServoIncrease;
	else if (Servo->fAngleSet < Servo->fAngleExpect)
		Servo->fAngleSet += fServoIncrease;
	turn_angle(Servo);
}
//------------------------------------------------------------------------------------
// @brief 舵机旋转角度设置
// @param whichsteer 选择舵机：SERVO_10K_PIN,SERVO_20K_PIN
// @param angle 设置的旋转角度
// @param maxAngle 舵机的最大角度（由舵机本身参数决定，选择 180°和 270°或 360°）
// @return void
// Sample usage: turn_Angle(firststeer,90,270) 270°舵机旋转到 90°位置
//------------------------------------------------------------------------------------
void turn_angle(servo_t *Servo)
{
	switch (Servo->WhichServo)
	{
		// 舵机10K
	case SERVO_10K:
	{
		if (Servo->fMaxAngle == 180)
		{
			if (Servo->fAngleSet < 181 && Servo->fAngleSet >= 0)
			{
				pwm_set_duty(SERVO_10K_PIN, servo_angle_to_duty(Servo));
			}
		}
		else if (Servo->fMaxAngle == 270)
		{
			if (Servo->fAngleSet < 271 && Servo->fAngleSet >= 0)
			{
				pwm_set_duty(SERVO_10K_PIN, servo_angle_to_duty(Servo));
			}
		}
		else if (Servo->fMaxAngle == 360)
		{
			if (Servo->fAngleSet < 361 && Servo->fAngleSet >= 0)
			{
				pwm_set_duty(SERVO_10K_PIN, servo_angle_to_duty(Servo));
			}
		}
		else
			return;
	}
	break;
	// 舵机20K
	case SERVO_20K:
	{
		if (Servo->fMaxAngle == 180)
		{
			if (Servo->fAngleSet < 181 && Servo->fAngleSet >= 0)
			{
				pwm_set_duty(SERVO_20K_PIN, servo_angle_to_duty(Servo));
			}
		}
		else if (Servo->fMaxAngle == 270)
		{
			if (Servo->fAngleSet < 271 && Servo->fAngleSet >= 0)
			{
				pwm_set_duty(SERVO_20K_PIN, servo_angle_to_duty(Servo));
			}
		}
		else if (Servo->fMaxAngle == 360)
		{
			if (Servo->fAngleSet < 361 && Servo->fAngleSet >= 0)
			{
				pwm_set_duty(SERVO_20K_PIN, servo_angle_to_duty(Servo));
			}
		}
		else
			return;
	}
	break;
	case SERVO_35K:
	{
		if (Servo->fMaxAngle == 180)
		{
			if (Servo->fAngleSet < 181 && Servo->fAngleSet >= 0)
			{
				pwm_set_duty(SERVO_35K_PIN, servo_angle_to_duty(Servo));
			}
		}
		else if (Servo->fMaxAngle == 270)
		{
			if (Servo->fAngleSet < 271 && Servo->fAngleSet >= 0)
			{
				pwm_set_duty(SERVO_35K_PIN, servo_angle_to_duty(Servo));
			}
		}
		else if (Servo->fMaxAngle == 360)
		{
			if (Servo->fAngleSet < 361 && Servo->fAngleSet >= 0)
			{
				pwm_set_duty(SERVO_35K_PIN, servo_angle_to_duty(Servo));
			}
		}
		else
			return;
	}
	break;
	default:
		break;
	}
}
void servo_output(float fServoOut)
{
	uint16 dwServoUs = SERVO_35K_US_STOP;
	if (fServoOut < 0)
	{
		dwServoUs = scale(fServoOut, SERVO_SPEED_MIN, 0, SERVO_35K_US_MIN, SERVO_35K_US_ANTI);
	}
	else if (fServoOut > 0)
	{
		dwServoUs = scale(fServoOut, 0, SERVO_SPEED_MAX, SERVO_35K_US_CLOCK, SERVO_35K_US_MAX);
	}
	else
	{
		dwServoUs = SERVO_35K_US_STOP;
	}
	pwm_set_duty(SERVO_35K_PIN, servo_times_to_duty(dwServoUs));
}

void set_rotary_expect_angle_with_index(uint8 PhoneTableIndex)
{
	switch (PhoneTableIndex)
	{
	case 1:
		Servo35K.fAngleExpect = 56;
		break;
	case 2:
		Servo35K.fAngleExpect = 130;
		break;
	case 3:
		Servo35K.fAngleExpect = 200;
		break;
	case 4:
		Servo35K.fAngleExpect = 272;
		break;
	case 5:
		Servo35K.fAngleExpect = 342;
		break;
	default:
		break;
	}

	g_fRotaryTimes = float_abs(Servo35K.fAngleExpect - Servo35K.fAngleSet) * 4;

}