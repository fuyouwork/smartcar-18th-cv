#include "side_task.hpp"
#define TOP_LEFT_ROI_X 40
#define TOP_LEFT_ROI_Y 450
#define LEFT_ACC_MODE 1
#define RIGHT_ACC_MODE 2

uint8 g_byAccMode;
void set_side_task(void)
{
    static bool bFirstSideFlag = 1;
    static bool bGoalErrorSetFlag = 1;
    static bool bGoalErrorSetFlag1 = 1;
    static bool bGoalErrorSetFlag2 = 1;
    static bool bGoalErrorSetFlag3 = 1;
    static bool bGoalErrorSetFlag4 = 1;
    static bool bGoalErrorSetFlag5 = 1;
    static uint8 s_bySideCount = 0;
    if (!g_byCollectPictureFlag)
    {
        Servo10K.fAngleExpect = SERVO_10K_RESET_ANGLE;
        Servo20K.fAngleExpect = SERVO_20K_RESET_ANGLE;
    }
    switch (s_bySideCount)
    {
    case 0:
        if (bFirstSideFlag)
        {
            g_fXExpectPosition = 750;
            g_fYExpectPosition = 250;

            g_bPrepareGoSideFinishFlag = 0; // 先到y轴中点位置，防止x轴太近，y轴还没下移就看到边线了

            if (bGoalErrorSetFlag)
            {
                calculate_goal_error();
                bGoalErrorSetFlag = 0;
            }
            bFirstSideFlag = 0;
        }
        if (!g_byCollectPictureFlag)
        {
// 设定墙位置
#if !SIDE_DETECT_BY_OPENART
            g_bySetSideCommand[2] = 0x01;
            uart_write_buffer(SUB_CORE_UART_INDEX, g_bySetSideCommand, sizeof(g_bySetSideCommand));
            uart_write_buffer(SUB_CORE_UART_INDEX, g_bySetSideCommand, sizeof(g_bySetSideCommand));
#endif

            // 设定电磁锁
            g_electric_pin = ELECTRIC_PIN_5;
            if (g_byRotaryPlatformFlag == 0)
            {
                set_rotary_expect_angle_with_index(1); // 设定转台索引
                s_bySideCount++;                       // 索引增加
                g_byRotaryPlatformFlag = 1;            // 开始转转台
                bGoalErrorSetFlag = 1;
                g_bySetSideFlag = 0; // 结束任务
            }
            g_byCorrectMode = CORRECT_X;
            g_byAccMode = LEFT_ACC_MODE;
        }
        break;
    case 1:
        g_fXExpectPosition = 450;
        g_fYExpectPosition = -60;

        if (bGoalErrorSetFlag3)
        {
            calculate_goal_error();
            bGoalErrorSetFlag3 = 0;
        }
        g_electric_pin = ELECTRIC_PIN_2;
        set_rotary_expect_angle_with_index(1);
        g_byRotaryPlatformFlag = 1;
        s_bySideCount++;
        bGoalErrorSetFlag3 = 1;
        g_bySetSideFlag = 0;
        g_byCorrectMode = CORRECT_Y;
        g_byAccMode = RIGHT_ACC_MODE;
        break;
    case 2:
        g_fXExpectPosition = 350;
        g_fYExpectPosition = 580;

        if (bGoalErrorSetFlag1)
        {
            calculate_goal_error();
            bGoalErrorSetFlag1 = 0;
        }
#if !SIDE_DETECT_BY_OPENART
        g_bySetSideCommand[2] = 0x00;
        uart_write_buffer(SUB_CORE_UART_INDEX, g_bySetSideCommand, sizeof(g_bySetSideCommand));
        uart_write_buffer(SUB_CORE_UART_INDEX, g_bySetSideCommand, sizeof(g_bySetSideCommand));
#endif

        g_electric_pin = ELECTRIC_PIN_1;
        set_rotary_expect_angle_with_index(2);
        g_byRotaryPlatformFlag = 1;
        s_bySideCount++;        // 索引增加
        bGoalErrorSetFlag1 = 1; // 复位
        g_bySetSideFlag = 0;    // 结束任务
        g_byCorrectMode = CORRECT_Y;
        g_byAccMode = LEFT_ACC_MODE;
        break;
    case 3:
        g_fXExpectPosition = 60;
        g_fYExpectPosition = 440;

        if (bGoalErrorSetFlag5)
        {
            calculate_goal_error();
            bGoalErrorSetFlag5 = 0;
        }
        g_electric_pin = ELECTRIC_PIN_3;
        set_rotary_expect_angle_with_index(2);
        g_byRotaryPlatformFlag = 1;
        s_bySideCount++;        // 索引增加
        bGoalErrorSetFlag5 = 1; // 复位
        g_bPrepareGoSideFinishFlag = 1;

        g_byCorrectMode = CORRECT_Y;
        g_byAccMode = RIGHT_ACC_MODE;
        g_bySetSideFlag = 0; // 结束任务
        break;

    case 4:
        g_fXExpectPosition = -70;
        g_fYExpectPosition = 250;

        if (bGoalErrorSetFlag2)
        {
            calculate_goal_error();
            bGoalErrorSetFlag2 = 0;
        }
#if !SIDE_DETECT_BY_OPENART
        g_bySetSideCommand[2] = 0x01;
        uart_write_buffer(SUB_CORE_UART_INDEX, g_bySetSideCommand, sizeof(g_bySetSideCommand));
        uart_write_buffer(SUB_CORE_UART_INDEX, g_bySetSideCommand, sizeof(g_bySetSideCommand));
#endif
        g_electric_pin = ELECTRIC_PIN_4;
        set_rotary_expect_angle_with_index(3);
        g_byRotaryPlatformFlag = 1;
        s_bySideCount++;
        bGoalErrorSetFlag2 = 1;
        g_bySetSideFlag = 0;
        g_byCorrectMode = CORRECT_X;
        g_byAccMode = RIGHT_ACC_MODE;
        break;
    case 5:
        g_electric_pin = ELECTRIC_PIN_4;
        set_rotary_expect_angle_with_index(3);
        g_byRotaryPlatformFlag = 1;
        s_bySideCount++; // 索引增加
        g_bySetSideFlag = 0;
        g_byCorrectMode = CORRECT_Y;
        break;
    default:
        break;
    }
    // 索引0-3
    if (s_bySideCount < 6)
    {
        g_byGoSideFlag = 1; // 开始边线任务
    }
    else
    {
        g_byGoCarPortFlag = 1;
    }
}

bool g_bPrepareGoSideFinishFlag = 1;
bool g_bGetSideFlag = 0;
void go_side_task(void)
{
    float fXCorrect = 0, fYCorrect = 0;
    static bool bDetectSideFlag = 1;

    if (g_byCorrectMode == CORRECT_X)
    {
        if (g_fXExpectPosition <= 0)
        {
            fXCorrect = 0;
        }
        else if (g_fXExpectPosition >= 700)
        {
            fXCorrect = 700;
        }
        if (!g_bPrepareGoSideFinishFlag)
        {
            coordinate_omnidirect_move(g_fXPositionRealAbsoluteCoordinate, g_fYExpectPosition);
            if (float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 40)
            {
                g_bPrepareGoSideFinishFlag = 1;
            }
        }
        else
        {
            coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);
            if (float_abs(fXCorrect - g_fXPositionRealAbsoluteCoordinate) < 100)
            {
                if (bDetectSideFlag)
                {
#if SIDE_DETECT_BY_OPENART
                    if (!(g_fXExpectPosition == TOP_LEFT_ROI_X && g_fYExpectPosition == TOP_LEFT_ROI_Y))
                        g_bGetSideFlag = 1;
#else
                    g_bRequireCenterFlag = 1;
                    uart_write_buffer(SUB_CORE_UART_INDEX, g_bySideCenCommand, sizeof(g_bySideCenCommand));
                    uart_write_buffer(SUB_CORE_UART_INDEX, g_bySideCenCommand, sizeof(g_bySideCenCommand));
#endif
                    bDetectSideFlag = 0;
                }
            }
        }
    }
    if (g_byCorrectMode == CORRECT_Y)
    {
        if (g_fYExpectPosition <= 0)
        {
            fYCorrect = 0;
        }
        else if (g_fYExpectPosition >= 500)
        {
            fYCorrect = 500;
        }
        if (!g_bPrepareGoSideFinishFlag)
        {
            coordinate_omnidirect_move(g_fXExpectPosition, g_fYPositionRealAbsoluteCoordinate);
            if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 40)
            {
                g_bPrepareGoSideFinishFlag = 1;
            }
        }
        else
        {
            coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);
            if (float_abs(fYCorrect - g_fYPositionRealAbsoluteCoordinate) < 100)
            {
                if (bDetectSideFlag) // 左上角没有边线可以检测
                {
#if SIDE_DETECT_BY_OPENART
                    if (!(g_fXExpectPosition == TOP_LEFT_ROI_X && g_fYExpectPosition == TOP_LEFT_ROI_Y))
                        g_bGetSideFlag = 1;
#else
                    uart_write_buffer(SUB_CORE_UART_INDEX, g_bySideCenCommand, sizeof(g_bySideCenCommand));
                    uart_write_buffer(SUB_CORE_UART_INDEX, g_bySideCenCommand, sizeof(g_bySideCenCommand));
                    // uart_write_buffer(SUB_CORE_UART_INDEX, g_bySideCenCommand, sizeof(g_bySideCenCommand));
                    g_bRequireCenterFlag = 1;
#endif
                    bDetectSideFlag = 0;
                }
            }
        }
    }

    if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 30 &&
        float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 30)
    {
        if (!g_byCollectPictureFlag)
        {
            g_byOpenElectricLockFlag = 1;
#if SIDE_DETECT_BY_OPENART
            g_bGetSideFlag = 0;
#if !GO_SIDE_WITHOUT_PREPARE
            g_bPrepareGoSideFinishFlag = 0;
#endif
#else
            uart_write_buffer(SUB_CORE_UART_INDEX, g_byALLCenCommand, sizeof(g_byALLCenCommand));
            uart_write_buffer(SUB_CORE_UART_INDEX, g_byALLCenCommand, sizeof(g_byALLCenCommand));
            g_bRequireCenterFlag = 0;
#endif
            g_byGoSideFlag = 0;
            bDetectSideFlag = 1;
        }
    }
}

bool g_bSideAccTaskFlag = 0;

void side_acc_task(uint8 times)
{
    static uint32 s_dwAccTimes = 0;
    s_dwAccTimes += times;
    if (g_byAccMode == RIGHT_ACC_MODE)
        coordinate_omnidirect_move(g_fXPositionRealAbsoluteCoordinate + 50, g_fYPositionRealAbsoluteCoordinate);
    else
        coordinate_omnidirect_move(g_fXPositionRealAbsoluteCoordinate - 50, g_fYPositionRealAbsoluteCoordinate);
    if (s_dwAccTimes >= 700)
    {
        g_bySetSideFlag = 1;
        s_dwAccTimes = 0;
        g_bSideAccTaskFlag = 0;
    }
}