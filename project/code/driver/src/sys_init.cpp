/****************************************************************
 * @file            Sys_Init.c
 * @school         南昌航空大学第十七届全国大学生智能车大赛智能视觉组
 * @author          ZHP/CYL/GZW
 * @version         v2.0
 * @Software        KEIL
 * @date            2022-3-1
 ****************************************************************/
#include "sys_init.hpp"

unsigned char g_byReportTft180 = 0; // 默认不上报tft180，除非tft180被初始化了
uint8 g_byBuzzerInit = 0;
/**
 *@brief 系统初始化
 *@parame 无
 *@retval 无
 */
void system_init(void)
{
    // tft_init_refact(); // TFT180初始化重构函数
    bluetooth_init(); // 蓝牙初始化,如果初始化蓝牙，就必须一定要把蓝牙插上，并且保证蓝牙另一端有接收，否则会导致蓝牙占用过多时间的CPU
    // mt9v04x_init(); // 总钻风摄像头初始化
    // gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);					  // 蜂鸣器初始化
    timer_init(GPT_TIM_1, TIMER_US);                                          // 初始化毫秒计时器
    gpio_init(SERVO_SWITCH_PIN, GPO, LEVEL_START, GPO_PUSH_PULL);             // 舵机供电开关，默认不启动
    servo_init();                                                             // 舵机初始化
    gpio_init(MOTOR_SWITCH_PIN, GPO, LEVEL_START, GPO_PUSH_PULL);             // 电机供电开关，默认不启动
    motor_pwm_init();                                                         // 电机初始化
    gpio_init(ELECTRIC_SWITCH_PIN, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 电磁铁初始化
    encoder_init();                                                           // 编码器初始化
    imu963ra_init();                                                          // 陀螺仪初始化
    gyro_offset_init();                                                       // 陀螺仪去零漂初始化
    // zw_photo_tranform_init();                                                 // 驻吾图传初始化
}
bool g_bServoStartFlag = 0;
uint8 g_ServoInitFlag = 0;
/**
 *@brief 舵机初始化代码
 *@parame 无
 *@retval 无
 */
void servo_init(void)
{
    pwm_init(SERVO_10K_PIN, SPT_SERVO_FREQ, SERVO_10K_DUTY_INIT); // 初始化
    pwm_init(SERVO_20K_PIN, SPT_SERVO_FREQ, SERVO_20K_DUTY_INIT);
    pwm_init(SERVO_35K_PIN, SPT_SERVO_FREQ, SERVO_35K_DUTY_INIT);

    gpio_init(SERVO_SWITCH_PIN, GPO, LEVEL_STOP, GPO_PUSH_PULL); // 启动舵机供电开关
    g_ServoInitFlag = 1;
}
bool bMotorInit = 0;
/**
 *@brief 电机初始化代码
 *@parame 无
 *@retval 无
 */
void motor_pwm_init(void)
{
    gpio_init(FL_PH_IN, GPO, 0, GPO_PUSH_PULL);
    pwm_init(FL_EN_IN, MOTOR_PWM_FREQ, MOTOR_PWM_DUTY_INIT);
    gpio_init(FR_PH_IN, GPO, 0, GPO_PUSH_PULL);
    pwm_init(FR_EN_IN, MOTOR_PWM_FREQ, MOTOR_PWM_DUTY_INIT);
    gpio_init(BL_PH_IN, GPO, 0, GPO_PUSH_PULL);
    pwm_init(BL_EN_IN, MOTOR_PWM_FREQ, MOTOR_PWM_DUTY_INIT);
    gpio_init(BR_PH_IN, GPO, 0, GPO_PUSH_PULL);
    pwm_init(BR_EN_IN, MOTOR_PWM_FREQ, MOTOR_PWM_DUTY_INIT);
    gpio_init(MOTOR_SWITCH_PIN, GPO, LEVEL_STOP, GPO_PUSH_PULL); // 电机供电开关，默认不启动
    bMotorInit = 1;
}
bool bEncoderInit = 0;
/**
 *@brief 编码器初始化
 *@parame 无
 *@retval 无
 */
void encoder_init(void)
{
    encoder_quad_init(FL_ENCODER_N, FL_ENCODER_A, FL_ENCODER_B);
    encoder_quad_init(FR_ENCODER_N, FR_ENCODER_A, FR_ENCODER_B);
    encoder_quad_init(BL_ENCODER_N, BL_ENCODER_A, BL_ENCODER_B);
    encoder_quad_init(BR_ENCODER_N, BR_ENCODER_A, BR_ENCODER_B);

    bEncoderInit = 1;
}

/**
 *@brief mt9v04初始化函数，与逐飞库里的不同的是，其中包括了是否向tft180报告错误信息，显示reinit表示不正常
 *@parame 无
 *@retval 无
 */
void mt9v04x_init(void)
{
    while (1)
    {
        if (mt9v03x_init())
        {
            if (g_byReportTft180)
                tft180_show_string(16, 16, "mt9v04x reinit.");
        }
        else
            break;
        system_delay_ms(1000); // 闪灯表示异常
    }
    if (g_byReportTft180)
        tft180_show_string(16, 16, " success.");
}
bool g_bBluetoothInitFlag = 0;
/**
 *@brief bluetooth初始化函数，与逐飞库里的不同的是，其中包括了是否向tft180报告错误信息，显示reinit表示不正常
 *@parame 无
 *@retval 无
 */
void bluetooth_init(void)
{
    fifo_init(&g_BluetoothFifo, FIFO_DATA_8BIT, g_byBluetoothSaveFifoData, 168); // 蓝牙读取缓冲区初始化
    while (1)
    {
        if (g_byReportTft180)
            tft180_show_string(0, 16, "bluetooth init.");
        if (bluetooth_ch9141_init())
        {
            if (g_byReportTft180)
                tft180_show_string(0, 16, "bluetooth reinit.");
        }
        else
            break;
    }

    if (g_byReportTft180)
        tft180_show_string(0, 16, "bluetooth success.");
    g_bBluetoothInitFlag = 1;
}
/**
 *@brief 重构tft180初始化函数，如要向tft180传输总钻风图像，请宏定义TFT_SHOW_IMAGE，且在初始化函数之前
 *@parame 无
 *@retval 无
 */
void tft_init_refact(void)
{
    if (TFT_SHOW_IMAGE)
        tft180_set_dir(TFT180_CROSSWISE); // 需要先横屏 不然显示不下
    else
        tft180_set_dir(TFT180_CROSSWISE);      // 根据屏幕的摆放自行更改
    tft180_init();                                 // tft屏幕初始化
    g_byReportTft180 = 1;                          // 开启向tft上报信息
    tft180_full(RGB565_BLACK);                     // 背景颜色设为黑色
    tft180_set_color(RGB565_YELLOW, RGB565_BLACK); // 画笔颜色黄色，背景色黑色
}
/**
 *@brief 中断优先级设置函数
 *@parame 无
 *@retval 无
 */
void set_system_priority(void)
{
    interrupt_set_priority(BLUETOOTH_RX_PRIORITY, 0);    // 设置对应 UART_INDEX 的中断优先级为 0
    interrupt_set_priority(SUB_CORE_RX_PRIORITY, 1); // 设置对应 UART_INDEX 的中断优先级为 0
    interrupt_set_priority(SUB_CAR_RX_PRIORITY, 2);
    interrupt_set_priority(PIT_IRQn, 3); // 设置对应 UART_INDEX 的中断优先级为 0
}
/**
 *@brief 无线图传初始化函数
 *@parame 无
 *@retval 无
 */
void zw_photo_tranform_init(void)
{
    uart_init(ZW_UART_INDEX, ZW_UART_BAURD, ZW_UART_TX, ZW_UART_RX); // 驻吾图传初始化
}
/// @brief 从核串口初始化
/// @param
void sub_core_uart_init(void)
{
    fifo_init(&g_OpenARTFlatFifo, FIFO_DATA_8BIT, g_byOpenARTFlatSaveFifoData, 168); // OpenART读取缓冲区初始化
    uart_init(SUB_CORE_UART_INDEX, SUB_CORE_UART_BAURD,
              SUB_CORE_UART_TX, SUB_CORE_UART_RX); // OpenArt串口初始化
    uart_rx_interrupt(SUB_CORE_UART_INDEX, ZF_ENABLE); // OpenArt串口接收中断允许
}

void openart_slop_init(void)
{
    fifo_init(&g_OpenARTSlopFifo, FIFO_DATA_8BIT, g_byOpenARTSlopSaveFifoData, 168); // OpenART读取缓冲区初始化
    uart_init(OPENART_SLOP_UART_INDEX, OPENART_SLOP_UART_BAURD,
              OPENART_SLOP_UART_TX, OPENART_SLOP_UART_RX); // OpenArt串口初始化
    uart_rx_interrupt(OPENART_SLOP_UART_INDEX, ZF_ENABLE); // OpenArt串口接收中断允许
}
void electric_init(void)
{
    gpio_init(ELECTRIC_PIN_1, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关
    gpio_init(ELECTRIC_PIN_2, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关
    gpio_init(ELECTRIC_PIN_3, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关
    gpio_init(ELECTRIC_PIN_4, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关
    gpio_init(ELECTRIC_PIN_5, GPO, ELECTRIC_CLOSE_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关

    // gpio_init(ELECTRIC_PIN_1, GPO, ELECTRIC_OPEN_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关
    // gpio_init(ELECTRIC_PIN_2, GPO, ELECTRIC_OPEN_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关
    // gpio_init(ELECTRIC_PIN_3, GPO, ELECTRIC_OPEN_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关
    // gpio_init(ELECTRIC_PIN_4, GPO, ELECTRIC_OPEN_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关
    // gpio_init(ELECTRIC_PIN_5, GPO, ELECTRIC_OPEN_LEVEL, GPO_PUSH_PULL); // 关闭电机供电开关
}
void buzzer_init(void)
{
    gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL); // 关闭电机供电开关
    g_byBuzzerInit = 1;
}

bool g_bLightInitFlag = 0;
float g_lBrightnessLevel = 0;
void light_init(void)
{
    gpio_init(LIGHT_PIN, GPO, 0, GPO_PUSH_PULL); // 关闭电机供电开关
    pwm_init(LIGHT_PWM_PIN, LIGHT_FREQUENCY, 0);
    g_bLightInitFlag = 1;
}

bool g_bSubCarUartInitFlag = 0;
void sub_car_uart_init(void)
{
    uart_init(SUB_CAR_UART_INDEX, SUB_CAR_UART_BAURD, SUB_CAR_UART_TX, SUB_CAR_UART_RX);
    g_bSubCarUartInitFlag = 1;
}

bool g_bSubCoreSpiInitFlag = 0;
void sub_core_spi_init(void)
{
    spi_init(SUB_CORE_SPI_INDEX, SUB_CORE_SPI_MODE, SUB_CORE_SPI_BAUD, SUB_CORE_SPI_SCK,
             SUB_CORE_SPI_MOSI, SUB_CORE_SPI_MISO, SUB_CORE_SPI_CS);
    g_bSubCoreSpiInitFlag = 1;
}