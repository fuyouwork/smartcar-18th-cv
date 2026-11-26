#include "encoder.hpp"

Encoder g_SEncoder =
{
	.SEncoderFL ={0,0,0,0,0},
	.SEncoderFR ={0,0,0,0,0},
	.SEncoderBL ={0,0,0,0,0},
	.SEncoderBR ={0,0,0,0,0},
	.SEncoderServo = {0,0,0,0,0}
};
/**
   *@brief 获取编码器速度以及路W程
   *@parame 无
   *@retval 无
   */
void get_motor_speed(void)
{
	//正交编码器读数是4倍频
	//2400需要从上位机读取，轮子转一圈编码器的读值
	//因此速度是(AWheelEncoderNow - AWheelEncoderLast)/2400*19.7920337176 / 0.03s = (AWheelEncoderNow - AWheelEncoderLast)*0.274889357cm/s
	//30ms读取一次 2400对应的是轮子的周长为3.1415926*6.3cm = 19.7920337176cm
	
	// g_SEncoder.SEncoderFL.fSpeedNow = g_SEncoder.SEncoderFL.sCountNow * 0.20616701789 * 5; 
	// g_SEncoder.SEncoderFR.fSpeedNow = g_SEncoder.SEncoderFR.sCountNow * 0.20616701789 * 5;
	// g_SEncoder.SEncoderBL.fSpeedNow = g_SEncoder.SEncoderBL.sCountNow * 0.20616701789 * 5;
	// g_SEncoder.SEncoderBR.fSpeedNow = g_SEncoder.SEncoderBR.sCountNow * 0.20616701789 * 5;
	
	g_SEncoder.SEncoderFL.fSpeedNow = g_SEncoder.SEncoderFL.sCountNow * 0.20616701789 * 5; 
	g_SEncoder.SEncoderFR.fSpeedNow = g_SEncoder.SEncoderFR.sCountNow * 0.20616701789 * 5;
	g_SEncoder.SEncoderBL.fSpeedNow = g_SEncoder.SEncoderBL.sCountNow * 0.20616701789 * 5;
	g_SEncoder.SEncoderBR.fSpeedNow = g_SEncoder.SEncoderBR.sCountNow * 0.20616701789 * 5;

	g_SEncoder.SEncoderFL.dRoute += g_SEncoder.SEncoderFL.fSpeedNow * 0.002;
	g_SEncoder.SEncoderFR.dRoute += g_SEncoder.SEncoderFR.fSpeedNow * 0.002;
	g_SEncoder.SEncoderBL.dRoute += g_SEncoder.SEncoderBL.fSpeedNow * 0.002;
	g_SEncoder.SEncoderBR.dRoute += g_SEncoder.SEncoderBR.fSpeedNow * 0.002;
	// if(g_SEncoder.SEncoderServo.dRoute > 360)
	// {
	// 	g_SEncoder.SEncoderServo.dRoute = 0;
	// }
	// else if(g_SEncoder.SEncoderServo.dRoute < -360)
	// {
	// 	g_SEncoder.SEncoderServo.dRoute = 0;
	// }
	//更新记录的圈数
	g_SEncoder.SEncoderFL.sCountLast = g_SEncoder.SEncoderFL.sCountNow;
	g_SEncoder.SEncoderFR.sCountLast = g_SEncoder.SEncoderFR.sCountNow;
	g_SEncoder.SEncoderBL.sCountLast = g_SEncoder.SEncoderBL.sCountNow;
	g_SEncoder.SEncoderBR.sCountLast = g_SEncoder.SEncoderBR.sCountNow;
}
/**
   *@brief 	清除编码器计数
   *@parame 无
   *@retval 无
   */
void encoder_counter_clear(void)
{
	encoder_clear_count(FL_ENCODER_N);
	encoder_clear_count(FR_ENCODER_N);
	encoder_clear_count(BL_ENCODER_N);
	encoder_clear_count(BR_ENCODER_N);
}
extern int16 encoder_data_quaddec;
/**
   *@brief 得到编码器转过的圈数
   *@parame 无
   *@retval 无
   */
void encoder_check(void)
{
	g_SEncoder.SEncoderFL.sCountNow = encoder_get_count(FL_ENCODER_N);			//由于轮子正转时编码器对应的轴反转，故取反
	g_SEncoder.SEncoderFR.sCountNow = -encoder_get_count(FR_ENCODER_N);			//由于左右编码器镜像翻转，故不作取反
	g_SEncoder.SEncoderBL.sCountNow = encoder_get_count(BL_ENCODER_N);
	g_SEncoder.SEncoderBR.sCountNow = -encoder_get_count(BR_ENCODER_N);
	g_SEncoder.SEncoderFL.sCountSum += g_SEncoder.SEncoderFL.sCountNow;
	g_SEncoder.SEncoderFR.sCountSum += g_SEncoder.SEncoderFR.sCountNow;
	g_SEncoder.SEncoderBL.sCountSum += g_SEncoder.SEncoderBL.sCountNow;
	g_SEncoder.SEncoderBR.sCountSum += g_SEncoder.SEncoderBR.sCountNow;
}