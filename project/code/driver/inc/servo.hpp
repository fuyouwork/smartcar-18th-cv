#ifndef __SERVO_H
#define __SERVO_H

extern "C"
{
#include "zf_driver_pwm.h"
}
#include "user_code_cpp.hpp"
// 舵机IO口定义
#define SERVO_20K_PIN (PWM4_MODULE3_CHA_C31)
#define SERVO_10K_PIN (PWM4_MODULE2_CHA_C30)
#define SERVO_35K_PIN (PWM4_MODULE1_CHA_B25)

// 参数
#define SPT_SERVO_STANDARED_FREQ 50  // 舵机标准频率HZ
#define SPT_SERVO_STANDARED_CYCLE 20 // 舵机标准周期20ms
#define SPT_NEUTRAL_POINT_TIME 1.5   // 舵机中点ms
#define SPT_SERVO_MIN_TIME 0.5       // 舵机下限ms
#define SPT_SERVO_MAX_TIME 2.5       // 舵机上限ms

#define SPT_SERVO_MAX_ANGLE 180 // SPT舵机角度上限

#define SPT_SERVO_CYCLE (SPT_SERVO_STANDARED_CYCLE)                                                         // 舵机周期
#define SPT_NEUTRAL_POINT_DUTY ((uint16)scale(SPT_NEUTRAL_POINT_TIME, 0, SPT_SERVO_CYCLE, 0, PWM_DUTY_MAX)) // 舵机中点占空比
#define SPT_SERVO_DUTY_MIN ((uint16)scale(SPT_SERVO_MIN_TIME, 0, SPT_SERVO_CYCLE, 0, PWM_DUTY_MAX))         // 舵机零度占空比
#define SPT_SERVO_DUTY_MAX ((uint16)scale(SPT_SERVO_MAX_TIME, 0, SPT_SERVO_CYCLE, 0, PWM_DUTY_MAX))         // 舵机180度占空比

#define SERVO_10K_PICK_ANGLE (76)
#define SERVO_20K_PICK_ANGLE (127)
#define SERVO_10K_RUN_DOWN_1_ANGLE (76)
#define SERVO_20K_RUN_DOWN_1_ANGLE (73)

#define SERVO_10K_RUN_DOWN_2_ANGLE (238)
#define SERVO_20K_RUN_DOWN_2_ANGLE (66)
#define SERVO_10K_CENTER_1_ANGLE (238)
#define SERVO_20K_CENTER_1_ANGLE (45)
#define SERVO_10K_CENTER_2_ANGLE (243)
#define SERVO_20K_CENTER_2_ANGLE (45)
#define SERVO_10K_CENTER_3_ANGLE (243)
#define SERVO_20K_CENTER_3_ANGLE (40)
#define SERVO_10K_CENTER_4_ANGLE (255)
#define SERVO_20K_CENTER_4_ANGLE (40)
#define SERVO_10K_PLACE_ANGLE (150)
#define SERVO_20K_PLACE_ANGLE (40)

#define SERVO_10K_RESET_ANGLE (100)
#define SERVO_20K_RESET_ANGLE (30)
#define SERVO_35K_RESET_ANGLE (58)

#define SERVO_10K_RUN_DOWN_2_CAR_ANGLE (207)
#define SERVO_20K_RUN_DOWN_2_CAR_ANGLE (73)
#define SERVO_10K_CENTER_1_CAR_ANGLE (207)
#define SERVO_20K_CENTER_1_CAR_ANGLE (28)
#define SERVO_10K_PLACE_CAR_ANGLE (229)
#define SERVO_20K_PLACE_CAR_ANGLE (28)

#define SPT_SERVO_FREQ (SPT_SERVO_STANDARED_FREQ)            // 舵机频率初始值
#define SPT_SERVO_DUTY_INIT (SPT_NEUTRAL_POINT_DUTY)         // 舵机占空比初始化值
#define SERVO_10K_DUTY_INIT (servo_angle_to_duty(&Servo10K)) // SPT舵机10KG初始占空比
#define SERVO_20K_DUTY_INIT (servo_angle_to_duty(&Servo20K)) // SPT舵机20KG初始占空比
#define SERVO_35K_DUTY_INIT (servo_angle_to_duty(&Servo35K)) // SPT舵机20KG初始占空比

#define SERVO_35K_CLOCKWISE_FAST (500)
#define SERVO_35K_CLOCKWISE_SLOW (1300)
#define SERVO_35K_STOP (1500)
#define SERVO_35K_ANTICLOCKWISE_SLOW (1650)
#define SERVO_35K_ANTICLOCKWISE_FAST (2501)

#define SERVO_35K_US_MIN (1000)
#define SERVO_35K_US_ANTI (1450)
#define SERVO_35K_US_STOP (1500)
#define SERVO_35K_US_CLOCK (1580)
#define SERVO_35K_US_MAX (2000)

#define SERVO_SPEED_MIN (-10.0)
#define SERVO_SPEED_MAX (10.0)

#define BASIC_ERROR 10

// 舵机型号选择
typedef enum
{
    SERVO_10K = 0,
    SERVO_20K = 1,
    SERVO_35K = 2
} servo_select;

typedef struct Servo
{
    pwm_channel_enum pin;
    servo_select WhichServo;
    float fMaxAngle;
    float fAngleSet;
    float fAngleExpect;
} servo_t;

extern servo_t Servo10K, Servo20K, Servo35K;
extern uint16 g_wServo10KAngle;
extern uint16 g_wServo20KAngle;
extern uint16 g_wServo30KAngle;
extern uint8 g_byPickFinishFlag;
extern uint32_t dwRotaryPWM;

typedef struct
{
    char MainClass[20];
    char SubClass[20];
    uint8 MainCategoryCode;
    uint8 SubCategoryCode;
    int8 PhoneTableIndex;
} Category_t;
typedef struct PhonexitTurntable
{
    uint8 PhoenixTurntableIndex;
    uint8 PhoenixAngle;
} PhonexitTurntable_t;
typedef struct PhonexitTurnTable
{
    int16 iAngle;
    uint8 byPhotoNums;
} PhonexitTableState;

extern uint8 g_byOpenARTFlatSaveFifoData[168]; // 串口接收数据缓冲区
extern fifo_struct g_OpenARTFlatFifo;
extern uint8 g_byCategoryIndex;
extern Category_t g_Category[15];
extern Category_t *g_CategoryMap[15];
extern PhonexitTableState PhonexitTurntableCategory[5];
extern int16 g_iPhoneixLastAngle;
extern float g_fServoIncrease;

extern void set_rotary_expect_angle_with_index(uint8 PhoneTableIndex);
extern uint16 servo_times_to_duty(uint16 duty);
extern void servo_angle_turn_speed(servo_t *Servo);
extern uint16 servo_angle_to_duty(servo_t *Servo);
extern void turn_angle(servo_t *Servo);
extern void servo_output(float fServoOut);
#endif
