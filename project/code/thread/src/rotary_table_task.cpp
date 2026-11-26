#include "rotary_table_task.hpp"

uint16 g_byOpenElectricLockTimes = 0;
void open_electric_lock_task(uint8 times)
{
    if (g_byRotaryPlatformFlag == 0)
    {
        g_byOpenElectricLockTimes += times;

        if (g_byOpenElectricLockTimes >= 500)
        {
            gpio_set_level(g_electric_pin, ELECTRIC_CLOSE_LEVEL);
#if !SIDE_WITH_AUDIO
            if (!g_byGoCarPortFlag)
            {
                g_bSideAccTaskFlag = 1;
            }
#else
            g_bGoCenterTaskFlag = 1;
#endif
            g_byOpenElectricLockTimes = 0;
            g_byOpenElectricLockFlag = 0;
        }
        else if (g_byOpenElectricLockTimes >= 300)
        {
            gpio_set_level(g_electric_pin, ELECTRIC_OPEN_LEVEL);
        }
        else if (g_byOpenElectricLockTimes >= 200)
        {
            gpio_set_level(g_electric_pin, ELECTRIC_CLOSE_LEVEL);
        }
        else
        {
            gpio_set_level(g_electric_pin, ELECTRIC_OPEN_LEVEL);
        }
        openart_xy_speed(0, 0);
    }
}
uint8 g_byBasicError = BASIC_ERROR;
uint8 g_byServoPlatformSpeedMax = 7;
float g_fRotaryTimes = 0;
void rotary_platform_task(void)
{
    g_fRotaryTimes = g_fRotaryTimes - 4;
    if (g_fRotaryTimes <= 0)
    {
        g_fRotaryTimes = 0;
        g_byRotaryPlatformFlag = 0;
    }
}