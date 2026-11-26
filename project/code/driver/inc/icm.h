/*********************************************************************************************************************
* 接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   // 硬件 SPI 引脚
*                   SCL/SPC             查看 zf_device_icm20602.h 中 ICM20602_SPC_PIN 宏定义
*                   SDA/DSI             查看 zf_device_icm20602.h 中 ICM20602_SDI_PIN 宏定义
*                   SA0/SDO             查看 zf_device_icm20602.h 中 ICM20602_SDO_PIN 宏定义
*                   CS                  查看 zf_device_icm20602.h 中 ICM20602_CS_PIN 宏定义
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   其余引脚悬空
*
*                   // 软件 IIC 引脚
*                   SCL/SPC             查看 zf_device_icm20602.h 中 ICM20602_SCL_PIN 宏定义
*                   SDA/DSI             查看 zf_device_icm20602.h 中 ICM20602_SDA_PIN 宏定义
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   其余引脚悬空
*                   ------------------------------------
********************************************************************************************************************/
#ifndef _ICM_H
#define _ICM_H

#define DT 0.004f //采样时间

#include "zf_common_headfile.h"

extern float fYawAngle;
extern float fZGyro;
extern float fZGyroFinal;

extern float fYaw;
extern float fYawAdd;

extern bool bGyroOffSetInit;

typedef struct {
	float fXdata;
	float fYdata;
	float fZdata;
}GyroParamT;

typedef struct {
	float fYawAngle;
	float fZGyro;
}ResStruct;

extern GyroParamT s_ICMGyroParam;
extern ResStruct s_ICMresult;

extern void icm_hubu(void);
extern void tft_icm_show(void);
extern void gyro_offset_init(void);
extern double low_pass_filter_silding(double dDataNewSet, double dDataMiddle, double dDataLast);

#endif
