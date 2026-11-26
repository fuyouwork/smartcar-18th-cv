#ifndef __UART_HPP
#define __UART_HPP

#define VOFA_FLOAT_CH_COUNT (7) // 向vofa上位机传输的数据量，更改该宏定义即可实现数据量传输的改变


typedef enum
{
	FLMOTOR_SPEED_NOW,
	FRMOTOR_SPEED_NOW,
	BLMOTOR_SPEED_NOW,
	BRMOTOR_SPEED_NOW,
	SPEED_NOW,
	SPEED_COUNTS
} SPEED_ENUM;

typedef enum
{
	SPEED_SET,
	FLMOTOR_PID_P,
	FLMOTOR_PID_I,
	FRMOTOR_PID_P,
	FRMOTOR_PID_I,
	BLMOTOR_PID_P,
	BLMOTOR_PID_I,
	BRMOTOR_PID_P,
	BRMOTOR_PID_I,
	FLMOTOR_INNER_MSE,
	FLMOTOR_INNER_ME,
	FRMOTOR_INNER_MSE,
	FRMOTOR_INNER_ME,
	BLMOTOR_INNER_MSE,
	BLMOTOR_INNER_ME,
	BRMOTOR_INNER_MSE,
	BRMOTOR_INNER_ME,
	STATE_COUNTS
} STATE_ENUM;

extern "C"
{
#include "zf_common_headfile.h"
#include "user_code.h"
}
#include "user_code_cpp.hpp"
// double上位机数据类型共用体
union USendComputerData
{
	double Calcarr[8];
	uint8 Send[64];
};
// float上位机数据类型共用体
union UVofaFloatData
{
	float fData[VOFA_FLOAT_CH_COUNT];
	uint8 bySend[VOFA_FLOAT_CH_COUNT * sizeof(float)];
};
// vofa上位机数据结构体类型定义
struct SVofaFrame
{
	union UVofaFloatData UData;
	unsigned char byTail[4];
};

typedef union float_frame
{
	float fData;
	uint8 bySend[4];
}float_frame_t;
typedef union int16_frame
{
	int16 iData;
	uint8 bySend[2];
}int16_frame_data;
typedef struct CoordinataFrame
{
	int16_frame_data CoordianteX;
	int16_frame_data CoordianteY;
}CoordinataFrame_t;
typedef struct subcardate
{
	uint8 prefix1;
	uint8 prefix2;
	float_frame_t float_data;
	uint8 endl;
}sub_car_float_t;

extern sub_car_float_t g_fsubcardata;
extern uint32 g_byPrior;
extern struct SVofaFrame SWave;

extern fifo_struct g_BluetoothFifo;
extern uint8 g_byBluetoothSaveFifoData[168]; // fifo 输出读出缓冲区

extern void send_to_vofa(void);
extern void send_to_OpenART(void);
extern void send_sub_car_coordinate(float fCoordinateX, float fCoordinateY);
extern void bluetooth_uart_rx_handler(void);
extern void send_sub_core_coordinate(int16 iCoordinateX, int16 iCoordinateY);
#endif
