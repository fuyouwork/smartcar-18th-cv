#ifndef _SYS_INIT_HPP
#define _SYS_INIT_HPP

#include "user_code_cpp.hpp"
extern "C"
{
#include "zf_common_headfile.h"
#include "fsl_qtmr.h"
#include "sdcard.h"
}
#define OPENART_SLOP_UART_INDEX (UART_4)
#define OPENART_SLOP_UART_TX (UART4_TX_C16)
#define OPENART_SLOP_UART_RX (UART4_RX_C17)
#define OPENART_SLOP_UART_BAURD (115200)
#define OPENART_SLOP_RX_PRIORITY (LPUART4_IRQn)

#define SUB_CORE_SPI_INDEX (SPI_1)
#define SUB_CORE_SPI_MODE (SPI_MODE0)
#define SUB_CORE_SPI_BAUD (1 * 1000 * 1000)
#define SUB_CORE_SPI_MISO (SPI1_MISO_D15)
#define SUB_CORE_SPI_MOSI (SPI1_MOSI_D14)
#define SUB_CORE_SPI_CS (SPI1_CS0_D13)
#define SUB_CORE_SPI_SCK (SPI1_SCK_D12)


#define SUB_CORE_UART_INDEX (UART_1)
#define SUB_CORE_UART_TX (UART1_TX_B12)
#define SUB_CORE_UART_RX (UART1_RX_B13)
#define SUB_CORE_UART_BAURD (460800)
#define SUB_CORE_RX_PRIORITY (LPUART1_IRQn)

#define SUB_CAR_UART_INDEX (UART_4)
#define SUB_CAR_UART_TX (UART4_TX_C16)
#define SUB_CAR_UART_RX (UART4_RX_C17)
#define SUB_CAR_UART_BAURD (115200)
#define SUB_CAR_RX_PRIORITY    (LPUART4_IRQn)

#define ZW_UART_INDEX (UART_8)
#define ZW_UART_TX (UART8_TX_D16)
#define ZW_UART_RX (UART8_RX_D17)
#define ZW_UART_BAURD (3000000)

// 顺时针编号
// #define ELECTRIC_PIN_1 (B16)
// #define ELECTRIC_PIN_2 (D27)
// #define ELECTRIC_PIN_3 (B9)
// #define ELECTRIC_PIN_4 (B17)
// #define ELECTRIC_PIN_5 (B26)

#define ELECTRIC_PIN_1 (B26)
#define ELECTRIC_PIN_2 (B9)
#define ELECTRIC_PIN_3 (B16)
#define ELECTRIC_PIN_4 (B17)
#define ELECTRIC_PIN_5 (D27)

#define BUZZER_PIN (C4)
#define LIGHT_PIN (B24)
#define LIGHT_PWM_PIN (PWM4_MODULE0_CHA_B24)
#define LIGHT_FREQUENCY (1000)

#define TFT_SHOW_IMAGE (0) // tft180传输总钻风图像,0不传递，1传递，请注意传递图像会改变tft显示方式

#define PIT_CH (PIT_CH0)        // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY (PIT_IRQn) // 对应周期中断的中断编号

#define SERVO_SWITCH_PIN (C8)    // 舵机供电控制引脚
#define MOTOR_SWITCH_PIN (C9)    // 电机供电控制引脚
#define ELECTRIC_SWITCH_PIN (C5) // 电磁铁控制引脚
#define ELECTRIC_OPEN_LEVEL (1)
#define ELECTRIC_CLOSE_LEVEL (0) // 关闭电磁铁
#define LEVEL_START (1)          // 开始供电
#define LEVEL_STOP (0)           // 停止供电

#define SERVO_TEST 1

extern void system_init(void);
extern void servo_init(void);
extern void gpio_key_init(void);
extern void motor_pwm_init(void);
extern void encoder_init(void);
extern void mt9v04x_init(void);
extern void tft_init_refact(void);
extern void bluetooth_init(void);
extern void zw_photo_tranform_init(void);
extern void set_system_priority(void);
extern void sub_core_uart_init(void);
extern void electric_init(void);
extern void buzzer_init(void);
extern void light_init(void);
extern void sub_car_uart_init(void);
extern void sub_core_spi_init(void);

extern float g_lBrightnessLevel;
extern uint8 g_ServoInitFlag;
extern bool bEncoderInit, bMotorInit, g_bServoStartFlag, g_bBluetoothInitFlag, g_bAudioInitFlag;
extern bool g_bLightInitFlag;
extern uint8 g_byBuzzerInit;
extern uint8 g_byRingCounts;
extern bool g_bSubCoreSpiInitFlag;
#endif