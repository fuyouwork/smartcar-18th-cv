/****************************************************************
 * @file            thread.c
 * @school         �ϲ����մ�ѧ��ʮ�߽�ȫ����ѧ�����ܳ����������Ӿ���
 * @author          ZHP/CYL/GZW
 * @version         v2.0
 * @Software        KEIL
 * @date            2022-3-1
 ****************************************************************/
#include "thread.hpp"

uint8 g_byOpenArtSearchFlag = 0; // ͼƬ������־λ����ʼ��ʱĬ�ϲ�����

uint8 g_byMT9v03CorrectFinishFlag = 1; // OpenARTУ׼��־λ����ʼ��ʱĬ��У�����?
uint8 g_byPickFinishFlag = 1;		   // ץȡ��ɱ�־λ����ʼ��ʱ�?��ץȡ���?
uint8 g_byOpenArtReadyFlag = 1;
uint8 g_byShowAvgSpeedFlag = 0;
uint8 g_byRunStartFlag = 0;
uint16 g_wServoPickTime = 0;
uint8 g_bySendSDFlag = 0;
uint8 g_byMotorStartFlag = 0;
uint8 g_byWriteSpeedFlag = 0;

uint8 g_byGetCoordinateFinishFlag = 1;
uint8 g_byOpenArtCorrectFlag = 0;
uint8 g_byOpenArtCorrectFinishFlag = 1;
uint8 g_byOpenArtClassifyFlag = 0;
uint8 g_byClassifyFinishFlag = 0;
uint8 g_byReachPointFlag = 0;
uint8 g_byCollectPictureFlag = 0;
bool g_bWaitCoordinateGetFlag;
uint8 g_byStopFlag = 0;
uint8 g_byRotaryPlatformFlag = 0;
uint8 g_byPlaceCarFlag = 0;
uint8 g_byGoSideFlag = 0;
uint8 g_byOpenARTOpenFlag = 0;

uint8 g_byOpenArtCorrectSideFlag = 0;
uint8 g_byOpenElectricLockFlag = 0;
uint8 g_byGoCarPortFlag = 0;

uint8 g_bySideCorrectPositionFlag = 0;
uint8 g_byWaitSideCorrectFlag = 0;

uint8 g_byWaitOpenARTCorrectRectFlag = 0;

uint8 g_byPrepareSetPointFlag = 0;

uint32 g_byWaitSideTimes = 0;

uint32 g_byFindRectTimes = 0;
uint16 g_byElectricOpenTimes = 0;
uint32 g_byWaitSubCarTimes = 0;
void task_1ms(void)
{
}
void task_2ms(void)
{
	if (bEncoderInit)
	{
		encoder_check();
		encoder_counter_clear();
		get_motor_speed();
	}
	if (g_byMotorStartFlag && bMotorInit)
	{
		float fFLMotorSpeedExpect = FLMotorInnerPID.motor_pid_updata_increasement_output(g_SEncoder.SEncoderFL.fSpeedNow);
		float fFRMotorSpeedExpect = FRMotorInnerPID.motor_pid_updata_increasement_output(g_SEncoder.SEncoderFR.fSpeedNow);
		float fBLMotorSpeedExpect = BLMotorInnerPID.motor_pid_updata_increasement_output(g_SEncoder.SEncoderBL.fSpeedNow);
		float fBRMotorSpeedExpect = BRMotorInnerPID.motor_pid_updata_increasement_output(g_SEncoder.SEncoderBR.fSpeedNow);

		motor_output(fFLMotorSpeedExpect, fFRMotorSpeedExpect, fBLMotorSpeedExpect, fBRMotorSpeedExpect);
	}
}

void task_4ms(void)
{
	uint8 times = 4;
	if (bGyroOffSetInit)
		icm_hubu();

#if ENCODER_TEST || SERVO_ROTARY_TABLE_TEST || ELECTRIC_TEST
#elif SUB_CORE_CORRECT_CENTER_TEST || DISCHARGE_TEST || SINGLE_RACE_TEST || SERVO_PARAM_TEST || SCAN_COORDINATE_GET_TEST
	g_byCoordinateMoveFlag = 0;
	if (g_byOutCarPorFlag)
	{
		out_carport_task();
	}
	if (g_bySetScanLineFlag)
	{
		set_scan_line_task();
	}
	if (g_byScanLineFlag)
	{
		scan_line_task();
	}
	if (g_byPrepareScanFlag)
	{
		prepare_scan_task();
	}
	if (g_byGoCarPortFlag)
	{
		go_car_task();
	}
	if (g_bySetPointFlag)
	{
		set_point_task();
	}
	if (g_bChangeAngleFlag)
	{
		change_angle_task();
	}
	if (g_byGoPointFlag == 1)
	{
		go_point_task();
	}
	if (g_byClassifyFinishFlag == 1)
	{
		classify_finish_task();
	}
	if (g_byOpenArtCorrectFlag)
	{
		openart_correct_move();
	}
	if (g_byWaitOpenARTCorrectRectFlag)
	{
		wait_correct_rect_task(times);
	}
	if (g_byCollectPictureFlag == 1)
	{
		collect_picture_task(times);
	}
	if (g_bCorrectPointFlag == 1)
	{
		correct_point_task(times);
	}
#if !SIDE_WITH_AUDIO
	if (g_bySetSideFlag)
	{
		set_side_task();
	}
	if (g_byGoSideFlag)
	{
		go_side_task();
	}
	if (g_bSideAccTaskFlag)
	{
		side_acc_task(times);
	}
#else
	if (g_bGoCenterTaskFlag)
	{
		go_center_task();
	}
	if (g_bSetSideByAudioFlag)
	{
		set_side_by_audio_task();
	}
	if (g_bGoSideByAudioFlag)
	{
		go_side_by_audio_task();
	}
	if (g_bWaitAudioSignalFlag)
	{
		wait_audio_signal_task(times);
	}
#endif
	if (g_byOpenElectricLockFlag)
	{
		open_electric_lock_task(times);
	}

#else
	coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);

#endif
	if (g_byMotorStartFlag && bMotorInit)
	{
#if SUB_CORE_CORRECT_CENTER_TEST || DISCHARGE_TEST || SINGLE_RACE_TEST || DATASET_COLLECT || SCAN_COORDINATE_GET_TEST
		if (!g_byCoordinateMoveFlag)
			angle_correct();
#endif
	}
	if (g_ServoInitFlag)
	{
		servo_angle_turn_speed(&Servo10K);
		servo_angle_turn_speed(&Servo20K);
		servo_angle_turn_speed(&Servo35K);
	}
	if (g_byRotaryPlatformFlag == 1)
	{
		rotary_platform_task();
	}
}

void task_8ms(void)
{
}

void task_10ms(void)
{
#if SPEED_PID_TEST || OUTER_PID_TEST
	if (g_byMotorStartFlag)
		send_to_vofa();
#endif
}

void task_12ms(void)
{
}

void task_30ms(void)
{

	static uint8 counts = 0;
	if (g_byRingCounts && g_byBuzzerInit)
	{
		gpio_toggle_level(BUZZER_PIN);
		counts++;
		if (counts >= 2)
		{
			g_byRingCounts--;
			counts = 0;
		}
	}
}
extern float ServoSpeed;
bool g_bRequireCenterFlag = 0;
bool g_bGetCategoryFlag = 0;
bool g_bUseDefaultCategoryFLag = 0;
bool g_bSendCoordinateFlag = 0;
bool g_bRequireCoordinateFlag = 0;
void task_50ms(void)
{
	if (g_bRequireCenterFlag == 1)
	{
#if RECT_BY_ART
		g_byFlatOpenARTCommand[2] = UART_GET_RECT_CENTER;
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byFlatOpenARTCommand, sizeof(g_byFlatOpenARTCommand));
#else
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byResultCenCommand, sizeof(g_byResultCenCommand));
#endif
	}
	if (g_bGetSideFlag == 1)
	{
#if RECT_BY_ART
		if (g_byCorrectMode == CORRECT_Y)
			g_byFlatOpenARTCommand[2] = UART_GET_VLIP_SIDE_LINE;
		else
		{
			g_byFlatOpenARTCommand[2] = UART_GET_HORI_SIDE_LINE;
		}
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byFlatOpenARTCommand, sizeof(g_byFlatOpenARTCommand));
#else
		if (g_byCorrectMode == CORRECT_Y)
			g_byOpenARTCommand[2] = UART_GET_VLIP_SIDE_LINE;
		if (g_byCorrectMode == CORRECT_X)
		{
			g_byOpenARTCommand[2] = UART_GET_HORI_SIDE_LINE;
		}

		uart_write_buffer(SUB_CORE_UART_INDEX, g_byOpenARTCommand, sizeof(g_byOpenARTCommand));
#endif
	}
	if (g_bGetCategoryFlag == 1)
	{
		g_byOpenARTCommand[2] = UART_OPENART_CATEGORY;
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byOpenARTCommand, sizeof(g_byOpenARTCommand));
	}

	if (g_bUseDefaultCategoryFLag == 1)
	{
		g_byOpenARTCommand[2] = UART_USE_DEFAULT_CATEGORY;
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byOpenARTCommand, sizeof(g_byOpenARTCommand));
	}
	if (g_bRequireCoordinateFlag == 1)
	{
		uart_write_buffer(SUB_CORE_UART_INDEX, g_byGetPhotoCoordinateCommand, sizeof(g_byGetPhotoCoordinateCommand));
	}
	if (g_bSendCoordinateFlag == 1)
	{
		send_sub_core_coordinate(g_fXPositionRealAbsoluteCoordinate, g_fYPositionRealAbsoluteCoordinate);
	}
}
void task_100ms(void)
{
}
void task_20ms(void)
{
	
	if (g_bLightInitFlag)
	{
		g_lBrightnessLevel = 50;
		if (g_lBrightnessLevel >= 100)
			g_lBrightnessLevel = 100;
		pwm_set_duty(LIGHT_PWM_PIN, g_lBrightnessLevel * 100);
	}
	read_key_command(KEY_CLEAR_STATE);
}

void task_300ms(void)
{
	uint32 times = 300;
}

void task_1000ms(void)
{
}
