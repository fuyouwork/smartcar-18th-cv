#include "test.hpp"

void test()
{
#if MOTOR_RUN_TEST
    motor_test();
#elif SCREEN_TEST
    screen_test();
#elif ENCODER_TEST
    encoder_test();
#elif IMU960_TEST
    imu_test();
#elif OMINDIRECT_TEST
    omnidirect_test();
#elif SERVO_ROTARY_TABLE_TEST
    servo_rotary_table_test();
#elif FLASH_TEST
    flash_test();
#elif BLUETOOTH_KEY_TEST
    bluetooth_key_test();
#elif ELECTRIC_TEST
    electriclock_test();
#elif SUB_CORE_GET_COORDINATE_TEST
    openart_flat_get_coordinate_test();
#elif SUB_CORE_CORRECT_CENTER_TEST
    openart_flat_correct_center_test();
#elif DISCHARGE_TEST
    discharge_test();
#elif SINGLE_RACE_TEST
    single_car_race_test();
#elif DATASET_COLLECT
    dataset_collect();
#elif LIGHT_TEST
    light_test();
#elif MTV903T_TEST
    mtv90t_test();
#elif SUB_CAR_COMMUNITY
    sub_car_community();
#elif SUB_CORE_SPI_TEST
    sub_core_spi_test();
#elif SUB_CORE_UART_TEST
    sub_core_uart_test();
#elif SCAN_COORDINATE_GET_TEST
    scan_coordinate_get_test();
#endif
}

void sub_core_uart_test(void)
{
    sub_core_uart_init();
    system_delay_ms(2000);
    uint8 counts = 50;
    while (counts)
    {
        uart_write_buffer(SUB_CORE_UART_INDEX, g_byOpenARTCommand, sizeof(g_byOpenARTCommand));
        system_delay_ms(50);
        counts--;
    }
    while (1)
    {
    }
}
void sub_core_spi_test(void)
{
    sub_core_spi_init();

    send_sub_core_coordinate(0, 0);
    while (1)
    {
    }
}
/// @brief 电机测试程序
/// 检查方向脚设置是否正确，PWM脚是否正确，电机继电器脚是否正确
void motor_test(void)
{
    motor_pwm_init();                    // 电机初始化
    gpio_toggle_level(MOTOR_SWITCH_PIN); // 电机供电开关，默认不启动
    // motor_output(1000, 1000, 1000, 1000);
    motor_output(1000, 1000, 1000, 1000);
    while (1)
    {
    }
    //     system_delay_ms(MOTOR_TEST_EVERY_SECTION_TIME);
    //     motor_output(-1000, -1000, -1000, -1000);
    //     system_delay_ms(MOTOR_TEST_EVERY_SECTION_TIME);
    //     motor_output(500, -1000, 2000, -1500);
    //     system_delay_ms(MOTOR_TEST_EVERY_SECTION_TIME);
}
/// @brief 编码器测试
/// 先检测电机，再检测编码器
/// 检查定时器模块号是否正确，AB相引脚是否正确
void encoder_test(void)
{
    pwm_init(SERVO_35K_PIN, SPT_SERVO_STANDARED_FREQ, servo_times_to_duty(SERVO_35K_ANTICLOCKWISE_SLOW));
    gpio_init(SERVO_SWITCH_PIN, GPO, LEVEL_START, GPO_PUSH_PULL); // 启动舵机供电开关
    motor_pwm_init();                                             // 电机初始化
    gpio_init(MOTOR_SWITCH_PIN, GPO, LEVEL_START, GPO_PUSH_PULL); // 电机供电开关，默认不启动
    g_byMotorStartFlag = 1;
    encoder_init(); // 编码器初始化
    FLMotorInnerPID.pid_set_expect_value(100);
    FRMotorInnerPID.pid_set_expect_value(100);
    BLMotorInnerPID.pid_set_expect_value(100);
    BRMotorInnerPID.pid_set_expect_value(100);

    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
    while (1)
    {
    }
}
/// @brief 角度环测试
/// 先检测电机和编码器，再检测角度环
void imu_test(void)
{
    motor_pwm_init();                                             // 电机初始化
    gpio_init(MOTOR_SWITCH_PIN, GPO, LEVEL_START, GPO_PUSH_PULL); // 电机供电开关，默认不启动
    encoder_init();                                               // 编码器初始化
    imu963ra_init();                                              // 陀螺仪初始化
    gyro_offset_init();                                           // 陀螺仪去零漂初始化
    AnglePID.pid_set_expect_value(0);
    g_fXExpectPosition = 0;
    g_fYExpectPosition = 0;
    g_byMotorStartFlag = 1;
    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
}
/// @brief 全向移动测试
/// 先检测电机、编码器和角度环，再检测全向移动
void omnidirect_test(void)
{
    servo_init();
    motor_pwm_init();   // 电机初始化
    encoder_init();     // 编码器初始化
    imu963ra_init();    // 陀螺仪初始化
    gyro_offset_init(); // 陀螺仪去零漂初始化
    bluetooth_init();   // 初始化蓝牙
    tft_init_refact();
    flash_init();
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载                                                                       // 初始化闪存
    read_param_from_buffer(); // 从flash中调参数量出来
    describe_flash_buff();    // 给每个参数量上描述，并设置各个变换系数
    g_fAngleExpect = 0;
    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
    while (1)
    {
        menu_show();
    }
}

float ServoSpeed = 0;

/// @brief 转台舵机测试
/// 检查舵机PWM脚是否正确，舵机继电器引脚是否正确
void servo_rotary_table_test(void)
{
    tft_init_refact();
    bluetooth_init();
    tft180_clear();
    pwm_init(SERVO_35K_PIN, SPT_SERVO_STANDARED_FREQ, SERVO_35K_STOP);
    gpio_init(SERVO_SWITCH_PIN, GPO, LEVEL_START, GPO_PUSH_PULL); // 启动舵机供电开关
    g_ServoInitFlag = 1;
    encoder_init();
    // pwm_set_duty(SERVO_35K_PIN, servo_times_to_duty(SERVO_35K_CLOCKWISE_SLOW));
    // system_delay_ms(SERVO_TEST_EVERY_SECTION_TIME);
    // pwm_set_duty(SERVO_35K_PIN, servo_times_to_duty(SERVO_35K_ANTICLOCKWISE_SLOW));
    // system_delay_ms(SERVO_TEST_EVERY_SECTION_TIME);
    // pwm_set_duty(SERVO_35K_PIN, servo_times_to_duty(SERVO_35K_STOP));
    servo_output(0);
    pit_ms_init(PIT_CH, 2);
    while (1)
    {
        servo_output(1);
        system_delay_ms(1000);
        servo_output(-1);
        system_delay_ms(1000);
        servo_output(10);
        system_delay_ms(1000);
        servo_output(0);
        system_delay_ms(1000);
        servo_output(-10);
        system_delay_ms(1000);
    }
}
/// @brief 屏幕测试
/// @param  无
void screen_test(void)
{
    tft_init_refact(); // TFT180初始化重构函数
    tft180_show_string(0, 0, "Fly Fish Team!!");
    tft180_show_string(0, 16, "I am tft180!!");
    tft180_show_string(0, 32, "Test my func");
    tft180_show_string(0, 48, "It is OK!!");
    tft180_show_string(0, 64, "--------------");
    tft180_show_string(0, 80, "..............");
    tft180_show_string(0, 96, "##############");
    tft180_show_string(0, 112, "*************");
}
/// @brief flash测试
/// @param  无
/// 先测试屏幕，再测试flash，flash的测试会覆写原本的flash
void flash_test(void)
{
    g_byMenuSwitch = PARAM_MENU;
    tft_init_refact(); // TFT180初始化重构函数
    flash_init();      // 初始化闪存
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer(); // 从flash中调参数量出来
    describe_flash_buff();    // 给每个参数量上描述，并设置各个变换系数
}
union UnioTypeTemp
{
    float fData;
    uint8 byData[4];
};

/// @brief 蓝牙测试程序
/// @param  无
/// 先测试屏幕和flash, 再测试蓝牙
void bluetooth_key_test(void)
{
    union UnioTypeTemp DataBuffer;

    DataBuffer.fData = 0.5;
    flash_init();
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载                                                                       // 初始化闪存
    read_param_from_buffer(); // 从flash中调参数量出来
    describe_flash_buff();    // 给每个参数量上描述，并设置各个变换系数

    fifo_init(&g_BluetoothFifo, FIFO_DATA_8BIT, g_byBluetoothSaveFifoData, 168); // 蓝牙读取缓冲区初始化
    bluetooth_init();
    tft_init_refact();                                                                      // TFT180初始化重构函数
    pwm_init(SERVO_35K_PIN, SPT_SERVO_STANDARED_FREQ, servo_times_to_duty(SERVO_35K_STOP)); // 35k转台舵机初始化
    gpio_init(SERVO_SWITCH_PIN, GPO, LEVEL_STOP, GPO_PUSH_PULL);                            // 关闭舵机供电开关
    motor_pwm_init();                                                                       // 电机初始化
    gpio_init(MOTOR_SWITCH_PIN, GPO, LEVEL_STOP, GPO_PUSH_PULL);                            // 关闭电机供电开关
    encoder_init();                                                                         // 编码器初始化
    FLMotorInnerPID.pid_set_expect_value(100);
    FRMotorInnerPID.pid_set_expect_value(100);
    BLMotorInnerPID.pid_set_expect_value(100);
    BRMotorInnerPID.pid_set_expect_value(100);
    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
}
/// @brief 电磁锁功能测试
void electriclock_test()
{
    servo_init();
    electric_init();
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_OPEN_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(200);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(100);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_OPEN_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(200);

    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_OPEN_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(200);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(100);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_OPEN_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(200);

    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_OPEN_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(200);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(200);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_OPEN_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(200);

    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_OPEN_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(200);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(100);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_OPEN_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(200);

    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_OPEN_LEVEL);
    system_delay_ms(200);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    system_delay_ms(100);
    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_OPEN_LEVEL);
    system_delay_ms(200);

    gpio_set_level(ELECTRIC_PIN_1, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_2, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_3, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_4, ELECTRIC_CLOSE_LEVEL);
    gpio_set_level(ELECTRIC_PIN_5, ELECTRIC_CLOSE_LEVEL);
    while (1)
    {

        // gpio_toggle_level(ELECTRIC_PIN_3);
        // system_delay_ms(1000);
        // gpio_toggle_level(ELECTRIC_PIN_1);
        // gpio_toggle_level(ELECTRIC_PIN_2);
        // system_delay_ms(400);
        // gpio_toggle_level(ELECTRIC_PIN_2);
        // gpio_toggle_level(ELECTRIC_PIN_3);
        // system_delay_ms(200);
        // gpio_toggle_level(ELECTRIC_PIN_3);
        // gpio_toggle_level(ELECTRIC_PIN_4);
        // system_delay_ms(200);
        // gpio_toggle_level(ELECTRIC_PIN_4);
    }
}
/// @brief 平放OpenART测试
/// @param  无
void openart_flat_get_coordinate_test(void)
{
    tft_init_refact(); // TFT180初始化重构函数
    flash_init();      // 初始化闪存
    // flash_write_param();                                                             // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer();                                                        // 从flash中调参数量出来
    describe_flash_buff();                                                           // 给每个参数量上描述，并设置各个变换系数
    fifo_init(&g_OpenARTFlatFifo, FIFO_DATA_8BIT, g_byOpenARTFlatSaveFifoData, 168); // 蓝牙读取缓冲区初始化
    uart_init(SUB_CORE_UART_INDEX, SUB_CORE_UART_BAURD,
              SUB_CORE_UART_TX, SUB_CORE_UART_RX);     // OpenArt串口初始化
    uart_rx_interrupt(SUB_CORE_UART_INDEX, ZF_ENABLE); // OpenArt串口接收中断允许
    uart_write_byte(SUB_CORE_UART_INDEX, UART_GET_COORDINATE);
    bluetooth_ch9141_init();
    fifo_init(&g_BluetoothFifo, FIFO_DATA_8BIT, g_byBluetoothSaveFifoData, 168); // 蓝牙读取缓冲区初始化
    while (!g_byGetCoordinateFinishFlag)
    {
    }
    while (1)
    {
        uart_write_byte(SUB_CORE_UART_INDEX, UART_GET_RECT_CENTER);
        menu_show();
    }
}

/// @brief 平放OpenART测试
/// @param  无
void openart_flat_correct_center_test(void)
{
    servo_init();     // 初始化舵机
    motor_pwm_init(); // 电机初始化
    encoder_init();
    imu963ra_init();    // 陀螺仪初始化
    gyro_offset_init(); // 陀螺仪去零漂初始化                                                                 // 编码器初始化
    tft_init_refact();  // TFT180初始化重构函数
    flash_init();       // 初始化闪存
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer();                                                 // 从flash中调参数量出来
    describe_flash_buff();                                                    // 给每个参数量上描述，并设置各个变换系数
    gpio_init(ELECTRIC_SWITCH_PIN, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 初始化电磁铁

    sub_core_uart_init(); // 初始化openart
    bluetooth_init();     // 初始化蓝牙
    g_fXPositionRealAbsoluteCoordinate = 20;
    g_fYPositionRealAbsoluteCoordinate = 20;

    g_fXExpectPosition = g_fPointPositionX[1] * 20;
    g_fYExpectPosition = g_fPointPositionY[1] * 20;
    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
    set_system_priority();  // 设定优先级

    // 发送一次坐标获取指令，OpenART会循环获取坐标直至成功
    // uart_write_byte(SUB_CORE_UART_INDEX, UART_GET_COORDINATE);
    // 等待坐标获取完成
    // while (g_byGetCoordinateFinishFlag)
    // {
    // }
    // 路径规划
    // path_plan();
    // 载入小车起始坐标位置和第一个坐标点

    // g_fXExpectPosition = g_fPointPositionX[tsp.hamilton_path[1]];
    // g_fYExpectPosition = g_fPointPositionY[tsp.hamilton_path[1]];
    // 开始惯导全向移动
    g_byGoPointFlag = 1;
    while (1)
    {
        menu_show();
    }
}

void discharge_test(void)
{
    servo_init();     // 初始化舵机
    motor_pwm_init(); // 电机初始化
    encoder_init();
    imu963ra_init();    // 陀螺仪初始化
    gyro_offset_init(); // 陀螺仪去零漂初始化                                                                 // 编码器初始化
    tft_init_refact();  // TFT180初始化重构函数
    flash_init();       // 初始化闪存
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer();                                                 // 从flash中调参数量出来
    describe_flash_buff();                                                    // 给每个参数量上描述，并设置各个变换系数
    gpio_init(ELECTRIC_SWITCH_PIN, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 初始化电磁铁
    electric_init();
    sub_core_uart_init(); // 初始化openart
    bluetooth_init();     // 初始化蓝牙
    buzzer_init();
    light_init();
    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
    set_system_priority();  // 设定优先级

    // 开始惯导全向移动
    g_bySetSideFlag = 1;
    while (1)
    {
        menu_show();
    }
}

/// @brief 单车完赛测试
/// @param  无
void single_car_race_test(void)
{
    servo_init();     // 初始化舵机
    motor_pwm_init(); // 电机初始化
    electric_init();
    encoder_init();
    imu963ra_init();    // 陀螺仪初始化
    gyro_offset_init(); // 陀螺仪去零漂初始化                                                                 // 编码器初始化
    tft_init_refact();  // TFT180初始化重构函数
    flash_init();       // 初始化闪存
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer();                                                 // 从flash中调参数量出来
    describe_flash_buff();                                                    // 给每个参数量上描述，并设置各个变换系数
    gpio_init(ELECTRIC_SWITCH_PIN, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 初始化电磁铁
    gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);                             // 关闭电机供电开关
    sub_core_uart_init();                                                     // 初始化openart
    light_init();
    buzzer_init();
    sub_car_uart_init();

    bluetooth_init(); // 初始化蓝牙
                      // audio_init();
    // tflite_model_init();

    set_system_priority(); // 设定优先级

    // 发送一次坐标获取指令，OpenART会循环获取坐标直至成功

    g_lBrightnessLevel = 20;
    if (g_bLightInitFlag)
    {
        if (g_lBrightnessLevel >= 100)
            g_lBrightnessLevel = 100;
        pwm_set_duty(LIGHT_PWM_PIN, g_lBrightnessLevel * 100);
    }
    // g_byOpenARTCommand[2] = UART_GET_COORDINATE;
    // uart_write_buffer(SUB_CORE_UART_INDEX, g_byOpenARTCommand, sizeof(g_byOpenARTCommand));
    // // 等待坐标获取完成
    // while (g_byGetCoordinateFinishFlag)
    // {

    // }
    gpio_toggle_level(SERVO_SWITCH_PIN);
    g_bServoStartFlag = !g_bServoStartFlag;
    pid_restart();
    g_byMotorStartFlag = !g_byMotorStartFlag;
    gpio_toggle_level(MOTOR_SWITCH_PIN);

    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
    system_delay_ms(1000);
    // 亮度调到最高

    // 载入小车起始坐标位置和第一个坐标点
    g_fXPositionRealAbsoluteCoordinate = 20;
    g_fYPositionRealAbsoluteCoordinate = -20;
    g_fXExpectPosition = g_fPointPositionX[tsp.hamilton_path[1]] * 20;
    g_fYExpectPosition = g_fPointPositionY[tsp.hamilton_path[1]] * 20;
    // 开始惯导全向移动
    // g_byGoPointFlag = 1;
    g_bySetSideFlag = 1;
    // set_rotary_expect_angle_with_index(4);
    // g_byRotaryPlatformFlag = 1;
    while (1)
    {
        menu_show();
    }
}

uint8 g_byRingCounts = 0;
void dataset_collect(void)
{
    motor_pwm_init(); // 电机初始化
    encoder_init();
    imu963ra_init();    // 陀螺仪初始化
    gyro_offset_init(); // 陀螺仪去零漂初始化
    tft_init_refact();  // TFT180初始化重构函数
    flash_init();       // 初始化闪存
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer(); // 从flash中调参数量出来
    describe_flash_buff();    // 给每个参数量上描述，并设置各个变换系数
    sub_core_uart_init();     // 初始化openart
    bluetooth_init();         // 初始化蓝牙
    buzzer_init();
    light_init();
    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
    while (1)
    {
        menu_show();
    }
}

/// @brief
/// @param
void light_test(void)
{
    light_init();
    flash_init();
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer(); // 从flash中调参数量出来
    describe_flash_buff();    // 给每个参数量上描述，并设置各个变换系数
    tft_init_refact();        // TFT180初始化重构函数
    bluetooth_init();         // 初始化蓝牙
    pit_ms_init(PIT_CH, 2);   // 初始化pit外设,2ms进一次
    while (1)
    {
        menu_show();
    }
}

void mtv90t_test(void)
{
    motor_pwm_init(); // 电机初始化
    encoder_init();
    imu963ra_init();    // 陀螺仪初始化
    gyro_offset_init(); // 陀螺仪去零漂初始化
    tft_init_refact();  // TFT180初始化重构函数
    flash_init();       // 初始化闪存
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer(); // 从flash中调参数量出来
    describe_flash_buff();    // 给每个参数量上描述，并设置各个变换系数
    sub_core_uart_init();     // 初始化openart
    bluetooth_init();         // 初始化蓝牙
    buzzer_init();
    light_init();

    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
    while (1)
    {
        menu_show();
    }
}

void sub_car_community(void)
{
    sub_car_uart_init();

    while (1)
    {
        send_sub_car_coordinate(1.0, 1.0);
        system_delay_ms(500);
    }
}
void scan_coordinate_get_test(void)
{
    servo_init();     // 初始化舵机
    motor_pwm_init(); // 电机初始化
    electric_init();
    encoder_init();     // 编码器初始化
    imu963ra_init();    // 陀螺仪初始化
    gyro_offset_init(); // 陀螺仪去零漂初始化
    tft_init_refact();  // TFT180初始化重构函数
    flash_init();       // 初始化闪存
    // flash_write_param();      // 用程序中的参数覆盖flash中的参数请调用该函数，完成后，请将该函数注释掉后，再次下载
    read_param_from_buffer();                                                 // 从flash中调参数量出来
    describe_flash_buff();                                                    // 给每个参数量上描述，并设置各个变换系数
    gpio_init(ELECTRIC_SWITCH_PIN, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 初始化电磁铁
    gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);                             // 关闭电机供电开关
    sub_core_uart_init();                                                     // 初始化openart
    buzzer_init();
    g_fXPositionRealAbsoluteCoordinate = 20; // 场地绝对坐标系实际X坐标
    g_fYPositionRealAbsoluteCoordinate = -20;
    // bluetooth_init();       // 初始化蓝牙
    pit_ms_init(PIT_CH, 2); // 初始化pit外设,2ms进一次
    system_delay_ms(5000);
    g_bGetSideFlag = 1;
    g_byCorrectMode = CORRECT_Y;
    // set_system_priority();  // 设定优先级
    while (1)
    {
        if (bPathPlanFlag)
        {
            sorted(g_fPointPositionY, g_fPointPositionX, g_byPointNum, tsp.hamilton_path); // 开始路径规划，得到规划索引
            g_byRingCounts = 1;                                                            // 鸣叫一声表示扫到图片并规划完毕
            g_bySetPointFlag = 1;                                                          // 开始坐标点采集任务
            bPathPlanFlag = 0;                                                             // 结束路径规划任务
        }
        // g_bRequireCenterFlag = 1;      // 向从核请求中心点
        menu_show();
    }
}
