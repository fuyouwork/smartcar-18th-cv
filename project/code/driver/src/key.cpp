#include "key.hpp"
key_t::key_t(gpio_pin_enum KEY_PIN)
{
	m_KEY_PIN = KEY_PIN;

	m_bKeyNowState = KEY_OFF;
	m_bKeyLastState = KEY_OFF;

	m_bFirstJudgeKeyState = 0;
	m_byChangeDelayFlag = KEY_EXECUTE;
}

void key_t::judge_key_state(void)
{
	if (!m_bFirstJudgeKeyState)
	{
		gpio_init(m_KEY_PIN, GPI, KEY_OFF, GPI_PULL_UP);
		m_bFirstJudgeKeyState = 1;
	}

	m_bKeyLastState = m_bKeyNowState;
	m_bKeyNowState = gpio_get_level(m_KEY_PIN);

	if (m_bKeyNowState == 0)
	{
		m_dwKeyPressTimes++;
		if (m_dwKeyPressTimes >= 20)
			m_byKeyState = KEY_LONG_PRESS;
		else if (m_bKeyLastState == 1)
			m_byKeyState = KEY_SHORT_PRESS;
	}

	else
	{
		m_dwKeyPressTimes = 0;
		m_byKeyState = KEY_RELEASE;
	}
}

void key_t::clear_key_state(void)
{
	m_byKeyState = KEY_RELEASE;
}

bool g_bWriteFlag = 0;
uint16 g_bySendPhotoNums = 0;

key_t g_Key1(SW1_PIN);
key_t g_Key2(SW2_PIN);
key_t g_Key3(SW3_PIN);
key_t g_Key4(SW4_PIN);
key_t g_Key5(SW5_PIN);

void read_key_command(bool bClearFlag)
{

	g_Key1.judge_key_state();
	g_Key2.judge_key_state();
	g_Key3.judge_key_state();
	g_Key4.judge_key_state();
	g_Key5.judge_key_state();
	if (g_Key1.m_byKeyState != KEY_RELEASE)
	{
		if (g_Key1.m_byKeyState == KEY_LONG_PRESS)
		{
			if (g_Key1.m_byChangeDelayFlag == KEY_EXECUTE)
			{
				g_Key1.m_byChangeDelayFlag = KEY_DELAY;
			}
		}
		else if (g_Key1.m_byKeyState == KEY_SHORT_PRESS)
		{
			pid_restart();
			g_byMotorStartFlag = !g_byMotorStartFlag;
			gpio_toggle_level(MOTOR_SWITCH_PIN);
			gpio_toggle_level(SERVO_SWITCH_PIN);
			g_bServoStartFlag = !g_bServoStartFlag;
			g_byOutCarPorFlag = 1;
			g_bMotorSpeed = 200;
		}
	}

	if (g_Key2.m_byKeyState != KEY_RELEASE)
	{
		if (g_Key2.m_byKeyState == KEY_LONG_PRESS)
		{
			if (g_Key2.m_byChangeDelayFlag == KEY_EXECUTE)
			{
				if (g_byPoint > 0)
				{
					g_byPoint--;
				}
				if (g_byShowPoint > 0 && g_byStart == 0)
					g_byShowPoint--;

				g_Key2.m_byChangeDelayFlag = KEY_DELAY;
			}
			else if (g_Key2.m_byChangeDelayFlag == KEY_DELAY_MS)
			{
				g_Key2.m_byChangeDelayFlag = KEY_EXECUTE;
			}
			else
			{
				g_Key2.m_byChangeDelayFlag++;
			}
		}
		else if (g_Key2.m_byKeyState == KEY_SHORT_PRESS)
		{
			pid_restart();
			g_byMotorStartFlag = !g_byMotorStartFlag;
			gpio_toggle_level(MOTOR_SWITCH_PIN);
			gpio_toggle_level(SERVO_SWITCH_PIN);
			g_bServoStartFlag = !g_bServoStartFlag;
			g_bySetSideFlag = 1;
			g_bMotorSpeed = 200;
		}
	}

	if (g_Key3.m_byKeyState != KEY_RELEASE)
	{
		if (g_Key3.m_byKeyState == KEY_LONG_PRESS)
		{
			if (g_Key3.m_byChangeDelayFlag == KEY_EXECUTE)
			{
				g_byPoint = ((++g_byPoint) < g_byShowLines) ? g_byPoint : (g_byShowLines - 1);
				if (g_byShowPoint < PARAM_LINES - 1 && g_byShowPoint < g_byShowLines - 1)
					g_byShowPoint++;

				g_Key3.m_byChangeDelayFlag = KEY_DELAY;
			}
			else if (g_Key3.m_byChangeDelayFlag == KEY_DELAY_MS)
			{
				g_Key3.m_byChangeDelayFlag = KEY_EXECUTE;
			}
			else
			{
				g_Key3.m_byChangeDelayFlag++;
			}
		}
		else if (g_Key3.m_byKeyState == KEY_SHORT_PRESS)
		{
			pid_restart();
			g_byMotorStartFlag = !g_byMotorStartFlag;
			gpio_toggle_level(MOTOR_SWITCH_PIN);
			gpio_toggle_level(SERVO_SWITCH_PIN);
			g_bServoStartFlag = !g_bServoStartFlag;
			g_byOutCarPorFlag = 1;
			g_bMotorSpeed = 200;
			for (uint8 i = 0; i < 5; i++)
			{
				g_Category[i].PhoneTableIndex = 3;
			}
		}
	}

	if (g_Key4.m_byKeyState != KEY_RELEASE)
	{
		if (g_Key4.m_byKeyState == KEY_LONG_PRESS)
		{
			if (g_Key4.m_byChangeDelayFlag == KEY_EXECUTE)
			{
				SFlashBuf.flash_buffer[g_byPoint].float_type = SFlashBuf.flash_buffer[g_byPoint].float_type + g_fChangeFactor[g_byPoint];
				g_bWriteFlag = 1;

				g_Key4.m_byChangeDelayFlag = KEY_DELAY;
			}
			else if (g_Key4.m_byChangeDelayFlag == KEY_DELAY_MS)
			{
				g_Key4.m_byChangeDelayFlag = KEY_EXECUTE;
			}
			else
			{
				g_Key4.m_byChangeDelayFlag++;
			}
		}
		else if (g_Key4.m_byKeyState == KEY_SHORT_PRESS)
		{
			pid_restart();
			g_byMotorStartFlag = !g_byMotorStartFlag;
			gpio_toggle_level(MOTOR_SWITCH_PIN);
			gpio_toggle_level(SERVO_SWITCH_PIN);
			g_bServoStartFlag = !g_bServoStartFlag;
			g_byOutCarPorFlag = 1;
			g_bMotorSpeed = 200;
			for (uint8 i = 5; i < 10; i++)
			{
				g_Category[i].PhoneTableIndex = 3;
			}
		}
	}

	if (g_Key5.m_byKeyState != KEY_RELEASE)
	{
		if (g_Key5.m_byKeyState == KEY_LONG_PRESS)
		{
			if (g_Key5.m_byChangeDelayFlag == KEY_EXECUTE)
			{
				SFlashBuf.flash_buffer[g_byPoint].float_type = SFlashBuf.flash_buffer[g_byPoint].float_type - g_fChangeFactor[g_byPoint];
				g_bWriteFlag = 1;

				g_Key5.m_byChangeDelayFlag = KEY_DELAY;
			}
			else if (g_Key5.m_byChangeDelayFlag == KEY_DELAY_MS)
			{
				g_Key5.m_byChangeDelayFlag = KEY_EXECUTE;
			}
			else
			{
				g_Key5.m_byChangeDelayFlag++;
			}
		}
		else if (g_Key5.m_byKeyState == KEY_SHORT_PRESS)
		{
			pid_restart();
			g_byMotorStartFlag = !g_byMotorStartFlag;
			gpio_toggle_level(MOTOR_SWITCH_PIN);
			gpio_toggle_level(SERVO_SWITCH_PIN);
			g_bServoStartFlag = !g_bServoStartFlag;
			g_byOutCarPorFlag = 1;
			g_bMotorSpeed = 200;
			for (uint8 i = 10; i < 15; i++)
			{
				g_Category[i].PhoneTableIndex = 3;
			}
		}
	}
	clear_key_all_state();
}

void clear_key_all_state(void)
{
	g_Key1.clear_key_state();
	g_Key2.clear_key_state();
	g_Key3.clear_key_state();
	g_Key4.clear_key_state();
	g_Key5.clear_key_state();
}
extern float ServoSpeed;
void read_bluetooth_key(uint8 KeyIndex)
{
	float temp;
	float temp2;
	using namespace bluetooth;
	switch (KeyIndex)
	{
	case MOTOR_KEY:
		pid_restart();
		g_byMotorStartFlag = !g_byMotorStartFlag;
		gpio_toggle_level(MOTOR_SWITCH_PIN);
		break;
	case SERVO_KEY:
		gpio_toggle_level(SERVO_SWITCH_PIN);
		g_bServoStartFlag = !g_bServoStartFlag;
		break;

	case KEY1:
#if SUB_CORE_CORRECT_CENTER_TEST
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byRectCenCommand, sizeof(g_byRectCenCommand));
		g_byOpenArtCorrectFlag = 1;
#elif DISCHARGE_TEST
		g_bySetSideFlag = 1;
#elif DATASET_COLLECT

#elif LIGHT_TEST || MTV903T_TEST
		if (g_bLightInitFlag)
			g_lBrightnessLevel = 10;
#elif SERVO_PARAM_TEST
		gpio_toggle_level(ELECTRIC_SWITCH_PIN);
#elif SCAN_COORDINATE_GET_TEST
		g_byOpenARTCommand[2] = UART_CAPTURE_DATASET;
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byOpenARTCommand, sizeof(g_byOpenARTCommand));
		g_byRingCounts = 2;
		// g_bySlopOpenARTCommand[2] = UART_PHOTO_COORDINATE_START;
		// uart_write_buffer(SUB_CORE_UART_INDEX, g_bySlopOpenARTCommand, sizeof(g_bySlopOpenARTCommand));
#else
		g_byOpenARTCommand[2] = UART_GET_COORDINATE;
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byOpenARTCommand, sizeof(g_byOpenARTCommand));
#endif
		break;

	case KEY1_LONG:
#if DISCHARGE_TEST || SERVO_PARAM_TEST
		g_byOpenElectricLockFlag = 1;
#elif SCAN_COORDINATE_GET_TEST
		g_bySlopOpenARTCommand[2] = UART_PHOTO_COORDINATE_STOP;
		uart_write_buffer(SUB_CORE_UART_INDEX, g_bySlopOpenARTCommand, sizeof(g_bySlopOpenARTCommand));
#else
		g_byOpenARTCommand[2] = UART_CAPTURE_DATASET;
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byOpenARTCommand, sizeof(g_byOpenARTCommand));
#endif
		break;

	case KEY3:
#if MTV903T_TEST
		g_byUseSideCommand[2] = 0X01;
		if (g_bySetSideCommand[2] == 0x00)
			g_bySetSideCommand[2] = 0x01;
		else
		{
			g_bySetSideCommand[2] = 0x00;
		}
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byUseSideCommand, sizeof(g_byUseSideCommand));
		uart_write_buffer(SUB_CORE_UART_INDEX, g_bySetSideCommand, sizeof(g_bySetSideCommand));
#elif SCAN_COORDINATE_GET_TEST
		g_byCollectPictureFlag = 1;
#else
		g_byOpenArtClassifyFlag = 1;
		g_byRingCounts = 1;
#endif
		break;

	case KEY3_LONG:
#if MTV903T_TEST
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byRectCenCommand, sizeof(g_bySideCenCommand));
#elif SCAN_COORDINATE_GET_TEST
		g_bGetCategoryFlag = 1;
#else
		g_byCollectPictureFlag = 1;
#endif
		break;

	case KEY6:
		g_byFlatOpenARTCommand[2] = UART_GET_HORI_SIDE_LINE;
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byFlatOpenARTCommand, sizeof(g_byFlatOpenARTCommand));
		break;
	case KEY6_LONG:
		g_byFlatOpenARTCommand[2] = UART_GET_VLIP_SIDE_LINE;
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byFlatOpenARTCommand, sizeof(g_byFlatOpenARTCommand));
		break;

	case KEY4:
		SFlashBuf.flash_buffer[g_byPoint].float_type = SFlashBuf.flash_buffer[g_byPoint].float_type + g_fChangeFactor[g_byPoint];
		g_bWriteFlag = 1;
		break;

	case KEY7:
		SFlashBuf.flash_buffer[g_byPoint].float_type = SFlashBuf.flash_buffer[g_byPoint].float_type - g_fChangeFactor[g_byPoint];
		g_bWriteFlag = 1;
		break;

	case KEY2:
		if (++g_byMenuSwitch < MENUNUM)
			;
		else
		{
			g_byMenuSwitch = COMMON_MENU;
		}
		break;

	case KEY5:
		if (g_byPoint > 0)
			g_byPoint--;
		if (g_byShowPoint > 0 && g_byStart == 0)
			g_byShowPoint--;
		break;

	case KEY8:
		g_byPoint = ((++g_byPoint) < g_byShowLines) ? g_byPoint : (g_byShowLines - 1);
		if (g_byShowPoint < PARAM_LINES - 1 && g_byShowPoint < g_byShowLines - 1)
			g_byShowPoint++;
		break;

	default:
		break;
	}
}
