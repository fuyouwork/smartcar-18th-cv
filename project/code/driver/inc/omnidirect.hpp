#ifndef _OMNIDIRECT_H
#define _OMNIDIRECT_H
extern "C"{
	#include "zf_common_headfile.h"
	#include "user_code.h"
	#include "math.h"
}
#include "user_code_cpp.hpp"
#define CORRECT_ALL 0
#define CORRECT_X 1
#define CORRECT_Y 2
typedef struct LineXY
{
	int16 x;
	int16 y;
}LineXY_t;

extern LineXY_t g_ScanLine[10], g_PrepareScan[10];

extern float g_fXPositionRealAbsoluteCoordinate;			// 场地绝对坐标系实际X坐标
extern float g_fYPositionRealAbsoluteCoordinate;			// 场地绝对坐标系实际Y坐标
extern float g_fXPositionCoeffient;
extern float g_fYPositionCoeffient;
extern float g_fXExpectPosition;
extern float g_fYExpectPosition;

extern uint8 g_fXScc8660Position[10];
extern uint8 g_fYScc8660Position[10];
extern uint8 g_fXScc8660SideArray[10]; // 边线中心点数组
extern uint8 g_fYScc8660SideArray[10];
extern uint16 g_fXScc8660SidePosition; // 凌瞳反馈的边线中心点信息
extern uint16 g_fYScc8660SidePosition;
extern uint8 g_byPointNum;
extern uint8 g_byCenterNum;
extern uint8 g_bySideNum;
extern float g_fXOpenArtPosition, g_fYOpenArtPosition;												// OpenART反馈的位置信息
extern float g_fXLastOpenArtPosition;
extern float g_fYLastOpenArtPosition;
extern float g_fXNowOpenArtPosition; // OpenART反馈的位置信息
extern float g_fYNowOpenArtPosition;
extern float g_fXOpenArtPositionExpect, g_fYOpenArtPositionExpect;
extern uint8 g_byPointNum;

extern float g_fFLOpenARTSpeedExpect;							// 左上轮OpenART期望速度
extern float g_fFROpenARTSpeedExpect;
extern float g_fBLOpenARTSpeedExpect;
extern float g_fBROpenARTSpeedExpect;
extern uint8 g_byCorrectMode;

extern float g_fXPositionAbsoluteSpeed;
extern float g_fYPositionAbsoluteSpeed;
extern float g_fXPositionRealInertialSpeed;
extern float g_fYPositionRealInertialSpeed;
extern float g_fXErrorALL;
extern float g_fYErrorALL;
extern float g_fXGoalError, g_fYGoalError;
extern float g_fXError;
extern float g_fAngleExpect;
//extern std::queue<uint32> g_que_byPointPositionX, g_que_byPointPositionY;
extern void coordinate_omnidirect_move(float fXPositionExpect, float fYPositionExpect);
extern void openart_correct_move(void);
extern void angle_correct(void);
extern void openart_xy_speed(float fXSpeed, float fYSpeed);
extern void absolute_coordinate_calculate(void);
extern int sign(float data);
#endif