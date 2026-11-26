#include "collect_task.hpp"

uint32 g_byWaitSideCorrectScanTimes = 0;
uint8 g_byPathPlanFlag = 0;

uint8 g_byServoRunDownFlag = 0;
uint8 g_byServoRunBeforeFlag = 0;
uint8 g_byServoRunDown1Flag = 0;
uint8 g_byServoRunDown2Flag = 0;
uint8 g_byServoRunUpFlag = 0;
uint8 g_byServoCenter1Flag = 0;
uint8 g_byServoCenter2Flag = 0;
uint8 g_byServoCenter3Flag = 0;
uint8 g_byServoCenter4Flag = 0;
uint8 g_byServoResetFlag = 0;
uint8 g_byElectricCollectFlag = 0;

uint16 g_byElectricCollectTimes = 0;
uint16 g_byElectricLooseTimes = 0;
int32 g_byServoRunTimes = 0;

void collect_picture_task(uint8 times)
{
    // 开始图片拾取
    g_byServoRunTimes = g_byServoRunTimes - times;
    if (g_byServoRunTimes <= 0)
    {
        g_byServoRunTimes = 0;
        g_byServoRunDownFlag = 1;
        if (g_byServoResetFlag == 1)
        {
            g_fServoIncrease = 1.2;
            Servo10K.fAngleExpect = SERVO_10K_RESET_ANGLE;
            Servo20K.fAngleExpect = SERVO_20K_RESET_ANGLE;
            if (Servo10K.fAngleExpect == Servo10K.fAngleSet || Servo20K.fAngleExpect == Servo20K.fAngleSet)
            {
                g_byServoResetFlag = 0;
                g_byCollectPictureFlag = 0;
            }
        }
        else if (g_byServoRunUpFlag == 1)
        {
            g_fServoIncrease = 1.2;
            if (g_byPlaceCarFlag == 1)
            {
                Servo10K.fAngleExpect = SERVO_10K_PLACE_CAR_ANGLE;
                Servo20K.fAngleExpect = SERVO_20K_PLACE_CAR_ANGLE;
            }
            else
            {
                Servo10K.fAngleExpect = SERVO_10K_PLACE_ANGLE;
                Servo20K.fAngleExpect = SERVO_20K_PLACE_ANGLE;
            }
            if (Servo10K.fAngleExpect == Servo10K.fAngleSet && Servo20K.fAngleExpect == Servo20K.fAngleSet)
            {
                g_byServoResetFlag = 1;
                g_byServoRunUpFlag = 0;
            }
        }
        else if (g_byServoCenter4Flag)
        {
            g_fServoIncrease = 1.2;
            if (g_byPlaceCarFlag)
            {
            }
            else
            {
                Servo20K.fAngleExpect = SERVO_20K_CENTER_4_ANGLE;
                Servo10K.fAngleExpect = SERVO_10K_CENTER_4_ANGLE;
            }
            if (Servo20K.fAngleExpect == Servo20K.fAngleSet && Servo10K.fAngleExpect == Servo10K.fAngleSet)
            {

                gpio_set_level(ELECTRIC_SWITCH_PIN, ELECTRIC_CLOSE_LEVEL);

                g_byElectricLooseTimes += times;
                if (g_byElectricLooseTimes >= 300)
                {
                    g_byServoRunUpFlag = 1;
                    g_byServoCenter4Flag = 0;
                    g_byElectricLooseTimes = 0;
                }
            }
        }
        else if (g_byServoCenter3Flag)
        {
            g_fServoIncrease = 1.2;
            if (g_byPlaceCarFlag)
            {
            }
            else
            {
                Servo20K.fAngleExpect = SERVO_20K_CENTER_3_ANGLE;
                Servo10K.fAngleExpect = SERVO_10K_CENTER_3_ANGLE;
            }
            if (Servo20K.fAngleExpect == Servo20K.fAngleSet && Servo10K.fAngleExpect == Servo10K.fAngleSet)
            {

                g_byServoCenter4Flag = 1;
                g_byServoCenter3Flag = 0;
            }
        }
        else if (g_byServoCenter2Flag)
        {
            g_fServoIncrease = 1.2;
            if (g_byPlaceCarFlag)
            {
            }
            else
            {
                Servo20K.fAngleExpect = SERVO_20K_CENTER_2_ANGLE;
                Servo10K.fAngleExpect = SERVO_10K_CENTER_2_ANGLE;
            }
            if (Servo20K.fAngleExpect == Servo20K.fAngleSet && Servo10K.fAngleExpect == Servo10K.fAngleSet)
            {
                g_byServoCenter3Flag = 1;
                g_byServoCenter2Flag = 0;
            }
        }
        else if (g_byServoCenter1Flag)
        {
            if (g_byPlaceCarFlag)
            {
                g_fServoIncrease = 1.2;
                Servo20K.fAngleExpect = SERVO_20K_CENTER_1_CAR_ANGLE;
                Servo10K.fAngleExpect = SERVO_10K_CENTER_1_CAR_ANGLE;
            }
            else
            {
                g_fServoIncrease = 1.2;
                Servo20K.fAngleExpect = SERVO_20K_CENTER_1_ANGLE;
                Servo10K.fAngleExpect = SERVO_10K_CENTER_1_ANGLE;
            }
            if (Servo20K.fAngleExpect == Servo20K.fAngleSet && Servo10K.fAngleExpect == Servo10K.fAngleSet)
            {
                g_byServoCenter2Flag = 1;
                g_byServoCenter1Flag = 0;
            }
        }
        else if (g_byServoRunDown2Flag == 1)
        {
            g_fServoIncrease = 1.2;
            if (g_byPlaceCarFlag)
            {
                Servo20K.fAngleExpect = SERVO_20K_RUN_DOWN_2_CAR_ANGLE;
                Servo10K.fAngleExpect = SERVO_10K_RUN_DOWN_2_CAR_ANGLE;
            }
            else
            {
                Servo20K.fAngleExpect = SERVO_20K_RUN_DOWN_2_ANGLE;
                Servo10K.fAngleExpect = SERVO_10K_RUN_DOWN_2_ANGLE;
            }

            if (Servo10K.fAngleExpect == Servo10K.fAngleSet && Servo20K.fAngleExpect == Servo20K.fAngleSet)
            {
                static bool s_bNextPointFlag = 1;
                if (s_bNextPointFlag)
                {
                    g_bySetPointFlag = 1;
                    g_byDetectRectFlag = 1;
                    s_bNextPointFlag = 0;
                }
                // 等待转台转动完成再上行
                if (g_byRotaryPlatformFlag == 0)
                {
                    // 开始下一次全向移动
                    s_bNextPointFlag=1;
                    g_byServoCenter1Flag = 1;
                    g_byServoRunDown2Flag = 0;
                }
            }
        }
        else if (g_byServoRunDown1Flag == 1)
        {
            g_fServoIncrease = 1.2;
            // 舵机开始行驶至中间态
            Servo20K.fAngleExpect = SERVO_20K_RUN_DOWN_1_ANGLE;
            Servo10K.fAngleExpect = SERVO_10K_RUN_DOWN_1_ANGLE;

            if (Servo10K.fAngleExpect == Servo10K.fAngleSet && Servo20K.fAngleExpect == Servo20K.fAngleSet)
            {
                g_byServoRunDown1Flag = 0;
                g_byServoRunDown2Flag = 1;
            }
        }
        else if (g_byServoRunDownFlag == 1)
        {
#if !SERVO_PROTECT
            g_fServoIncrease = 1.6;
#else
            g_fServoIncrease = 1.2;
#endif
            // 舵机开始下行
            Servo20K.fAngleExpect = SERVO_20K_PICK_ANGLE;
            Servo10K.fAngleExpect = SERVO_10K_PICK_ANGLE;

            // 舵机下行至期望角度
            if (Servo10K.fAngleExpect == Servo10K.fAngleSet && Servo20K.fAngleExpect == Servo20K.fAngleSet)
            {
                // 打开电磁铁
                g_byElectricCollectFlag = 1;
                gpio_set_level(ELECTRIC_SWITCH_PIN, ELECTRIC_OPEN_LEVEL);

                // 等待电磁铁稳定吸附图片
                g_byElectricCollectTimes += times;
                if (g_byElectricCollectTimes >= 200)
                {
                    // 舵机开始行驶至中间态
                    g_byServoRunDown1Flag = 1;
                    g_byServoRunDownFlag = 0;
                    g_byElectricCollectTimes = 0;
                }
            }
        }

        if (float_abs(Servo20K.fAngleExpect - Servo20K.fAngleSet) > float_abs(Servo10K.fAngleExpect - Servo10K.fAngleSet))
        {
#if SERVO_PROTECT
            g_byServoRunTimes = float_abs(Servo20K.fAngleExpect - Servo20K.fAngleSet) * 3.5;
#else
            g_byServoRunTimes = float_abs(Servo20K.fAngleExpect - Servo20K.fAngleSet) * 3;
#endif
        }
        else
        {
#if SERVO_PROTECT
            g_byServoRunTimes = float_abs(Servo10K.fAngleExpect - Servo10K.fAngleSet) * 3.5;
#else
            g_byServoRunTimes = float_abs(Servo10K.fAngleExpect - Servo10K.fAngleSet) * 3;
#endif
        }
    }
}
