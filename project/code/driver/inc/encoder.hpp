#ifndef __ENCODER_H
#define __ENCODER_H

extern "C"
{
#include "zf_common_headfile.h"
}
#include "user_code_cpp.hpp"
#define FL_ENCODER_N	(QTIMER3_ENCODER2)	// 左前编码器对应模块号
#define FR_ENCODER_N	(QTIMER1_ENCODER2)	// 右前编码器对应模块号
#define BL_ENCODER_N	(QTIMER2_ENCODER1)	// 左后编码器对应模块号
#define BR_ENCODER_N	(QTIMER1_ENCODER1)	// 右后编码器对应模块号
#define SERVO_ENCODER_N	(QTIMER4_ENCODER2)	// 右后编码器对应模块号

#define FR_ENCODER_A	(QTIMER1_ENCODER2_CH1_C2)	
#define FR_ENCODER_B	(QTIMER1_ENCODER2_CH2_C24)	
#define FL_ENCODER_A	(QTIMER3_ENCODER2_CH1_B18)
#define FL_ENCODER_B	(QTIMER3_ENCODER2_CH2_B19)
#define BL_ENCODER_A	(QTIMER2_ENCODER1_CH1_C3)	
#define BL_ENCODER_B	(QTIMER2_ENCODER1_CH2_C25)
#define BR_ENCODER_A	(QTIMER1_ENCODER1_CH1_C0)	// 左前编码器A相引脚
#define BR_ENCODER_B	(QTIMER1_ENCODER1_CH2_C1)	// 左前编码器B相引脚	
#define SERVO_ENCODER_A	(QTIMER4_ENCODER2_CH1_C11)	
#define SERVO_ENCODER_B	(QTIMER4_ENCODER2_CH2_C27)	


//编码器选择
typedef enum{
	ENUM_ENCODER_FL = 0,
	ENUM_ENCODER_FR = 1,
	ENUM_ENCODER_BL = 2,
	ENUM_ENCODER_BR = 3,
	ENUM_ENCODER_SERVO = 4
}encoder_select;
//编码器各部分情况
typedef struct
{
	short sCountNow;
	int32 sCountSum;
	short sCountLast;
	double dRoute;
	float fSpeedNow;	
}Encoder_N;
//所有编码器
typedef struct
{
	Encoder_N SEncoderFL;
	Encoder_N SEncoderFR;
	Encoder_N SEncoderBL;
	Encoder_N SEncoderBR;
	Encoder_N SEncoderServo;
	Encoder_N SEncoderResult;
}Encoder;

union encoder_data_quaddec
{
	int16 encoder_data_quaddec;
	uint8 data[2];
};


extern Encoder g_SEncoder;

extern void get_motor_speed(void);
extern void encoder_counter_clear(void);
extern void encoder_check(void);


#endif


