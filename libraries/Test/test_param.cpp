#include "test_param.hpp"

void test_param(void)
{
#if SERVO_PARAM_TEST
    servo_param_test();
#elif OMNIDIRECT_PARAM_TEST
    omnidirect_param_test();
#elif SPEED_PID_TEST
    speed_pid_test();
#elif OUTER_PID_TEST
    outer_pid_test();
#endif
}
char ServoEncoderMessage[30];
/// @brief 舵机角度参数测试
/// @param
void servo_param_test(void)
{
    encoder_init();                                                           // 编码器初始化
    gpio_init(ELECTRIC_SWITCH_PIN, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 关闭电磁铁
    flash_init();
    // flash_write_param();                                                        // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载                                                                       // 初始化闪存
    read_param_from_buffer();                                                   // 从flash中调参数量出来
    describe_flash_buff();                                                      // 给每个参数量上描述，并设置各个变换系数
    bluetooth_init();
    tft_init_refact(); // TFT180初始化重构函数
    servo_init();
    electric_init();
    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
    while(1)
    {
        menu_show();
    }
}
float X_TEST[14][2] = {
    {100, 0},
    {200, 0},
    {300, 0},
    {400, 0},
    {500, 0},
    {600, 0},
    {700, 0},
    {-100, 0},
    {-200, 0},
    {-300, 0},
    {-400, 0},
    {-500, 0},
    {-600, 0},
    {-700, 0}};
float Y_TEST[10][2] = {
    {0, 100},
    {0, 200},
    {0, 300},
    {0, 400},
    {0, 500},

    {0, -100},
    {0, -200},
    {0, -300},
    {0, -400},
    {0, -500}};
char temp[700];
/// @brief 全向移动误差函数测试
/// @param
void omnidirect_param_test(void)
{
    encoder_init();
    motor_pwm_init();   // 电机初始化
    imu963ra_init();    // 陀螺仪初始化
    gyro_offset_init(); // 陀螺仪去零漂初始化
    flash_init();
    flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer(); // 从flash中调参数量出来
    describe_flash_buff();    // 给每个参数量上描述，并设置各个变换系数
    bluetooth_init();
    tft_init_refact(); // TFT180初始化重构函数
    servo_init();
    light_init();
    gpio_toggle_level(LIGHT_PIN);
    pit_ms_init(PIT_CH, 2);

    while (1)
    {
        menu_show();
    }
}
float sdcard_test[4] = {0, 0, 0, 0};
uint8 bySdcardTestFlag = 0;
void speed_pid_test(void)
{
    flash_init();
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer(); // 从flash中调参数量出来
    describe_flash_buff();    // 给每个参数量上描述，并设置各个变换系数
    imu963ra_init();          // 陀螺仪初始化
    gyro_offset_init();       // 陀螺仪去零漂初始化
    tft_init_refact();        // TFT180初始化重构函数
    motor_pwm_init();         // 电机初始化
    encoder_init();
    if (bySdcardTestFlag)
        sdcard_init_excel();
    else
    {
        bluetooth_init();
    }

    g_byMenuSwitch = PARAM_MENU;
    pit_ms_init(PIT_CH, 2);

    while (1)
    {
        if (g_byMotorStartFlag)
        {
            FLMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
            FRMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
            BLMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
            BRMotorInnerPID.pid_set_expect_value(g_fSpeedExpectValue);
            // system_delay_ms(2500);
            // FLMotorInnerPID.pid_set_expect_value(0);
            // FRMotorInnerPID.pid_set_expect_value(0);
            // BLMotorInnerPID.pid_set_expect_value(0);
            // BRMotorInnerPID.pid_set_expect_value(0);
            // system_delay_ms(2000);
            // g_byMotorStartFlag = 0;
            // gpio_set_level(MOTOR_SWITCH_PIN, LEVEL_STOP);
        }
        menu_show();
    }
}
void outer_pid_test(void)
{
    flash_init();
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer(); // 从flash中调参数量出来
    describe_flash_buff();    // 给每个参数量上描述，并设置各个变换系数
    imu963ra_init();          // 陀螺仪初始化
    gyro_offset_init();       // 陀螺仪去零漂初始化
    tft_init_refact();        // TFT180初始化重构函数
    motor_pwm_init();         // 电机初始化
    encoder_init();
    bluetooth_init();
    
    g_byMenuSwitch = PARAM_MENU;
    pit_ms_init(PIT_CH, 2);

    while (1)
    {
        menu_show();
    }
}