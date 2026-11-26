/****************************************************************
 * @file            ICM.c
 * @author          WFS
 * @version         v1.0
 * @relase          1.0：变量名规范，增加函数开头函数参数注释
 * @Software        KEIL v5.38a
 * @date            2023-1-14
 * @parame          宁一定要幸福啊，可恶
****************************************************************/
#include "icm.h"
#include "math.h"

float fYawAngle = 0;	// 偏航角角度
float fYaw;
float fYawAdd;

// z轴陀螺仪 //
float fZGyro = 0;	
float fZGyroLess = 0;
float fZGyroLast = 0;
float fZGyroFinal = 0;

// 初始化完成标志 //
bool bGyroOffSetInit = 0;

// 陀螺仪校准数据 //
GyroParamT s_ICMGyroParam = {
	.fXdata = 0,
	.fYdata = 0,
	.fZdata = 0,
};

// 结果结构体 //
ResStruct s_ICMresult = {
	.fYawAngle = 0,
	.fZGyro = 0,
};

/**
   *@brief 陀螺仪去零漂初始化
   *@parame 无
   *@retval 无
   */
void gyro_offset_init(void)
{
	uint16 i;
	s_ICMGyroParam.fZdata = 0;
	for(i = 0;i < 1000;i++)		// 采集1000次z轴数据做平均去零漂
	{
		imu963ra_get_gyro();
		s_ICMGyroParam.fZdata += imu963ra_gyro_y;
		system_delay_ms(5);
	}
	s_ICMGyroParam.fZdata /= 1000;
	bGyroOffSetInit = 1;
}

/**
   *@brief 陀螺仪z轴互补解算
   *@parame 无
   *@retval 无
   */
void icm_hubu(void)
{
	imu963ra_get_gyro();	// icm20602采集数据
	fZGyro = ((float)imu963ra_gyro_y - s_ICMGyroParam.fZdata);
	fYaw = (float)((fZGyro) * PI / 180 / 14.28f);
	if (fYaw < 0.005 && fYaw > -0.005) fYaw = 0;
	fYawAngle += RAD_TO_ANGLE(fYaw * DT);
	s_ICMresult.fYawAngle = fYawAngle;
	s_ICMresult.fZGyro = fZGyroFinal;
//	fZGyroLast = fZGyroLess;
//	fZGyroLess = fZGyro;
//	fZGyro = ((float)imu963ra_gyro_y - s_ICMGyroParam.fZdata);	// 数据去零漂
//	fZGyroFinal = low_pass_filter_silding(fZGyro, fZGyroLess, fZGyroLast);
//	fYaw = (float)((fZGyroFinal) / 14.28f) * DT;		// 陀螺仪量程为:±2000dps，获取到的陀螺仪数据除以16.4，可以转化为带物理单位的数据，单位为：°/s
//	if(fabs(fYaw) < 0.001) fYaw = 0;
//	fYawAngle += fYaw;
//	s_ICMresult.fYawAngle = fYawAngle;
//	s_ICMresult.fZGyro = fZGyroFinal;
}

/**
   *@brief 折叠低通滤波
   *@parame dDataNewSet 当前数据
   *@parame dDataMiddle 上一次数据
   *@parame dDataLast 上上次数据
   *@retval dResult 加权相加结果
   */
double low_pass_filter_silding(double dDataNewSet, double dDataMiddle, double dDataLast)
{
	double dResult;
	dResult = 0.5f * dDataNewSet + 0.3f * dDataMiddle + 0.2f * dDataLast;
	return dResult;
}

/**
   *@brief 陀螺仪数据tft180显示（调试用）
   *@parame 无
   *@retval 无
   */
void tft_icm_show()
{
	tft180_show_float(0, 0, s_ICMresult.fYawAngle, 8, 6);
	tft180_show_float(0, 15, s_ICMresult.fZGyro, 8, 6);
}
