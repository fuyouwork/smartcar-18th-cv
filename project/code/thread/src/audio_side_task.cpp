#include "audio_side_task.hpp"

bool g_bGoCenterTaskFlag = 0;
float factor = 1;
extern uint8 g_bySetSideByAudioCounts;
void go_center_task(void)
{
    static bool bCenterGoalErrorSetFlag = 1;

    if (g_bySetSideByAudioCounts <= 3)
    {
        g_fXExpectPosition = 350 * factor;
        g_fYExpectPosition = 250 * factor;
    }
    else
    {
        g_fXExpectPosition = g_fXExpectPosition;
        g_fYExpectPosition = g_fYExpectPosition;
    }
    if (bCenterGoalErrorSetFlag)
    {
        calculate_goal_error();
        bCenterGoalErrorSetFlag = 0;
    }
    coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);
    if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 10 &&
        float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 10)
    {
        g_bSetSideByAudioFlag = 1;
        bCenterGoalErrorSetFlag = 1;
        openart_xy_speed(0, 0);
        g_bGoCenterTaskFlag = 0;
    }
}
uint8 g_byWaitSubCarFlag = 0;
uint8 g_byDetectSideFlag = 0;
uint8 g_bySetSideFlag = 0;
gpio_pin_enum g_electric_pin;

bool g_bSetSideByAudioFlag = 0;
uint8 g_bySetSideByAudioCounts = 0;
int32 g_fAudioIncreasementX = 0;
int32 g_fAudioIncreasementY = 0;

void set_side_by_audio_task(void)
{
    g_bySetSideByAudioCounts++;
    switch (g_bySetSideByAudioCounts)
    {
    case 1:
        g_fAudioIncreasementX = 80 * factor;
        g_fAudioIncreasementY = 0;
        g_electric_pin = ELECTRIC_PIN_2;
        set_rotary_expect_angle_with_index(1); // 设定转台索引
        break;
    case 2:
        g_fAudioIncreasementX = 0;
        g_fAudioIncreasementY = 80 * factor;
        g_electric_pin = ELECTRIC_PIN_1;
        set_rotary_expect_angle_with_index(4); // 设定转台索引
        break;
    case 3:
        g_fAudioIncreasementX = 0;
        g_fAudioIncreasementY = -80 * factor;
        g_electric_pin = ELECTRIC_PIN_3;
        set_rotary_expect_angle_with_index(1); // 设定转台索引
        break;
    case 4:
        g_fAudioIncreasementX = -80 * factor;
        g_fAudioIncreasementY = 0;
        g_electric_pin = ELECTRIC_PIN_4;
        set_rotary_expect_angle_with_index(1); // 设定转台索引
        break;
    case 5:
        g_fAudioIncreasementX = 30 * factor;
        g_fAudioIncreasementY = 0;
        set_rotary_expect_angle_with_index(1); // 设定转台索引
        break;
    case 6:
        g_fAudioIncreasementX = 0;
        g_fAudioIncreasementY = -50 * factor;
        set_rotary_expect_angle_with_index(1); // 设定转台索引
        break;
    default:
        g_bSetSideByAudioFlag = 0;
        openart_xy_speed(0, 0);
        return;
        break;
    }
    g_byRotaryPlatformFlag = 1; // 开始转转台
    g_byRingCounts += 2;
    g_bGoSideByAudioFlag = 1;
    g_bSetSideByAudioFlag = 0;
    openart_xy_speed(0, 0);
}
bool g_bGoSideByAudioFlag = 0;
bool g_bAudioGoalErrorSetFlag = 1;
void go_side_by_audio_task(void)
{
    coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);
    if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 10 &&
        float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 10)
    {
        g_bWaitAudioSignalFlag = 1;
    }
    openart_xy_speed(0, 0);
}
uint32 g_byWaitAudioSignalTimes = 0;
bool g_bWaitAudioSignalFlag = 0;
void wait_audio_signal_task(uint8 times)
{
    g_byWaitAudioSignalTimes += times;
    if (g_byWaitAudioSignalTimes >= 5000)
    {
        g_byOpenElectricLockFlag = 1;
        g_bWaitAudioSignalFlag = 0;
        g_byWaitAudioSignalTimes = 0;
        g_bGoSideByAudioFlag = 0;
    }
    openart_xy_speed(0, 0);
}