#include "uart.hpp"
sub_car_float_t g_fsubcardata;
char charString[STATE_COUNTS][20] = {};

// vofa上位机数据结构体
struct SVofaFrame SWave =
	{
		.byTail = {0x00, 0x00, 0x80, 0x7f}};
/**
 *@brief vofa上位机数据传输程序
 *@parame 无
 *@retval 无
 */
void send_to_vofa(void)
{
	SWave.UData.fData[0] = g_SEncoder.SEncoderFL.fSpeedNow;
	SWave.UData.fData[1] = g_SEncoder.SEncoderFR.fSpeedNow;
	SWave.UData.fData[2] = g_SEncoder.SEncoderBL.fSpeedNow;
	SWave.UData.fData[3] = g_SEncoder.SEncoderBR.fSpeedNow;
	SWave.UData.fData[4] = g_fSpeedExpectValue;
	SWave.UData.fData[5] = s_ICMresult.fYawAngle;
	SWave.UData.fData[6] = 0;
	bluetooth_ch9141_send_buffer(SWave.UData.bySend, VOFA_FLOAT_CH_COUNT * sizeof(float));
	bluetooth_ch9141_send_buffer(SWave.byTail, 4);
}
uint32 g_byPrior = 0;
float g_fSpeedNow[SPEED_COUNTS];
float g_fState[STATE_COUNTS];
/**
 *@brief 发送给OpenART,在波特率高达300万时可以放置在定时器中断中，耗时是226us，串口发送函数不要在定时器中断里放多个，即使是不同线程
 *@parame 无
 *@retval 无
 */
void send_to_OpenART(void)
{
	strcpy(charString[SPEED_SET], "SPEED\n");

	strcpy(charString[FLMOTOR_PID_P], "FLP\n");
	strcpy(charString[FLMOTOR_PID_I], "FLI\n");
	strcpy(charString[FRMOTOR_PID_P], "FRP\n");
	strcpy(charString[FRMOTOR_PID_I], "FRI\n");
	strcpy(charString[BLMOTOR_PID_P], "BLP\n");
	strcpy(charString[BLMOTOR_PID_I], "BLI\n");
	strcpy(charString[BRMOTOR_PID_P], "BRP\n");
	strcpy(charString[BRMOTOR_PID_I], "BRI\n");

	strcpy(charString[FLMOTOR_INNER_MSE], "FLMSE\n");
	strcpy(charString[FLMOTOR_INNER_ME], "FLME\n");
	strcpy(charString[FRMOTOR_INNER_MSE], "FRMSE\n");
	strcpy(charString[FRMOTOR_INNER_ME], "FRME\n");
	strcpy(charString[BLMOTOR_INNER_MSE], "BLMSE\n");
	strcpy(charString[BLMOTOR_INNER_ME], "BLME\n");
	strcpy(charString[BRMOTOR_INNER_MSE], "BRMSE\n");
	strcpy(charString[BRMOTOR_INNER_ME], "BRME\n");

	if (FLMotorInnerPID.get_error_struct().bTestFinishFlag) // 由于PID更新误差线程同属一个线程，故当一个PID更新完成后全部更新完成
	{
		g_fState[SPEED_SET] = g_fSpeedExpectValue;

		g_fState[FLMOTOR_INNER_MSE] = (float)FLMotorInnerPID.get_error_struct().dSquareMeanError;
		g_fState[FLMOTOR_INNER_ME] = (float)FLMotorInnerPID.get_error_struct().dMeanError;

		g_fState[FRMOTOR_INNER_MSE] = (float)FRMotorInnerPID.get_error_struct().dSquareMeanError;
		g_fState[FRMOTOR_INNER_ME] = (float)FRMotorInnerPID.get_error_struct().dMeanError;

		g_fState[BLMOTOR_INNER_MSE] = (float)BLMotorInnerPID.get_error_struct().dSquareMeanError;
		g_fState[BLMOTOR_INNER_ME] = (float)BLMotorInnerPID.get_error_struct().dMeanError;

		g_fState[BRMOTOR_INNER_MSE] = (float)BRMotorInnerPID.get_error_struct().dSquareMeanError;
		g_fState[BRMOTOR_INNER_ME] = (float)BRMotorInnerPID.get_error_struct().dMeanError;

		g_fState[FLMOTOR_PID_P] = FLMotorInnerPID.m_Spid.fKp;
		g_fState[FLMOTOR_PID_I] = FLMotorInnerPID.m_Spid.fKi;

		g_fState[FRMOTOR_PID_P] = FRMotorInnerPID.m_Spid.fKp;
		g_fState[FRMOTOR_PID_I] = FRMotorInnerPID.m_Spid.fKi;

		g_fState[BLMOTOR_PID_P] = BLMotorInnerPID.m_Spid.fKp;
		g_fState[BLMOTOR_PID_I] = BLMotorInnerPID.m_Spid.fKi;

		g_fState[BRMOTOR_PID_P] = BRMotorInnerPID.m_Spid.fKp;
		g_fState[BRMOTOR_PID_I] = BRMotorInnerPID.m_Spid.fKi;
		sdcard_write(g_fState, STATE_COUNTS);
		g_byStopWriteFlag = 1;
	}
	else if (g_byMotorStartFlag == 1)
	{
		g_fSpeedNow[FLMOTOR_SPEED_NOW] = g_SEncoder.SEncoderFL.fSpeedNow;
		g_fSpeedNow[FRMOTOR_SPEED_NOW] = g_SEncoder.SEncoderFR.fSpeedNow;
		g_fSpeedNow[BLMOTOR_SPEED_NOW] = g_SEncoder.SEncoderBL.fSpeedNow;
		g_fSpeedNow[BRMOTOR_SPEED_NOW] = g_SEncoder.SEncoderBR.fSpeedNow;
		g_fSpeedNow[SPEED_NOW] = g_fSpeedExpectValue;
		sdcard_write(g_fSpeedNow, SPEED_COUNTS);
	}
}

uint8 g_byBluetoothSaveFifoData[168];
uint8 g_byBluetoothReadFifoData[168]; // fifo 输出读出缓冲区
uint8 g_byBluetoothDataTemp = 0;	  // 接收数据变量
uint32 g_dwBluetoothFifoCount = 0;	  // fifo 数据个数
fifo_struct g_BluetoothFifo;
/// @brief 蓝牙接收函数
/// @param  无
void bluetooth_uart_rx_handler(void)
{
	uint8 i = 0;
	uart_query_byte(BLUETOOTH_CH9141_INDEX, &g_byBluetoothDataTemp); // 读取串口数据
	fifo_write_buffer(&g_BluetoothFifo, &g_byBluetoothDataTemp, 1);	 // 存入 FIFO
	g_dwBluetoothFifoCount = fifo_used(&g_BluetoothFifo);
	fifo_read_buffer(&g_BluetoothFifo, g_byBluetoothReadFifoData, &g_dwBluetoothFifoCount, FIFO_READ_AND_CLEAN);
	while (g_dwBluetoothFifoCount--)
	{
		read_bluetooth_key(g_byBluetoothReadFifoData[i]);
		i++;
	}
}
void send_sub_car_data(void)
{
	uart_write_byte(SUB_CAR_UART_INDEX, g_fsubcardata.prefix1);
	uart_write_byte(SUB_CAR_UART_INDEX, g_fsubcardata.prefix2);
	uart_write_buffer(SUB_CAR_UART_INDEX, g_fsubcardata.float_data.bySend, sizeof(float));
	uart_write_byte(SUB_CAR_UART_INDEX, g_fsubcardata.endl);
}
char send_buffer[128] = {0};
void send_sub_car_coordinate(float fCoordinateX, float fCoordinateY)
{
	// g_fsubcardata.prefix1 = 0x10;
	// g_fsubcardata.prefix1 = 0x01;
	// uart_write_byte(SUB_CAR_UART_INDEX, g_fsubcardata.prefix1);
	// uart_write_byte(SUB_CAR_UART_INDEX, g_fsubcardata.prefix2);
	// g_fsubcardata.float_data.fData = fCoordinateX;
	// uart_write_buffer(SUB_CAR_UART_INDEX, g_fsubcardata.float_data.bySend, sizeof(float));
	// g_fsubcardata.float_data.fData = fCoordinateY;
	// uart_write_buffer(SUB_CAR_UART_INDEX, g_fsubcardata.float_data.bySend, sizeof(float));
	// g_fsubcardata.endl = 0x9F;
	// uart_write_byte(SUB_CAR_UART_INDEX, g_fsubcardata.endl);
	sprintf(send_buffer, "(%3.0f, %3.0f)\n", fCoordinateX, fCoordinateY);
	uart_write_string(SUB_CAR_UART_INDEX, send_buffer);
	// uart_write_byte(SUB_CAR_UART_INDEX, '\n');
}
CoordinataFrame_t g_CoordinateFrame;
uint8 g_bySubCoreSpiCoordinateData[] = {0X4F, 0X20, 0x00, 0x00, 0x00, 0x00, 0X76, 0X76};

void send_sub_core_coordinate(int16 iCoordinateX, int16 iCoordinateY)
{
	g_CoordinateFrame.CoordianteX.iData = iCoordinateX;
	g_CoordinateFrame.CoordianteY.iData = iCoordinateY;
	if (!g_bSubCoreSpiInitFlag)
	{
		uart_write_byte(SUB_CORE_UART_INDEX, 0X4F);
		uart_write_byte(SUB_CORE_UART_INDEX, 0X20);
		uart_write_byte(SUB_CORE_UART_INDEX, g_CoordinateFrame.CoordianteX.bySend[0]);
		uart_write_byte(SUB_CORE_UART_INDEX, g_CoordinateFrame.CoordianteX.bySend[1]);
		uart_write_byte(SUB_CORE_UART_INDEX, g_CoordinateFrame.CoordianteY.bySend[0]);
		uart_write_byte(SUB_CORE_UART_INDEX, g_CoordinateFrame.CoordianteY.bySend[1]);
		uart_write_byte(SUB_CORE_UART_INDEX, 0X76);
		uart_write_byte(SUB_CORE_UART_INDEX, 0X76);
	}
	else
	{

		g_bySubCoreSpiCoordinateData[2] = g_CoordinateFrame.CoordianteX.bySend[0];
		g_bySubCoreSpiCoordinateData[3] = g_CoordinateFrame.CoordianteX.bySend[1];
		g_bySubCoreSpiCoordinateData[4] = g_CoordinateFrame.CoordianteY.bySend[0];
		g_bySubCoreSpiCoordinateData[5] = g_CoordinateFrame.CoordianteY.bySend[1];
		spi_write_8bit_array(SUB_CORE_SPI_INDEX, g_bySubCoreSpiCoordinateData, sizeof(g_bySubCoreSpiCoordinateData));
	}
}
