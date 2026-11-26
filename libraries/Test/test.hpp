#ifndef __TEST_HPP
#define __TEST_HPP

extern "C"
{
#include "user_code.h"
}
#include "user_code_cpp.hpp"

#define MOTOR_TEST_EVERY_SECTION_TIME 3000 // ms
#define SERVO_TEST_EVERY_SECTION_TIME 3000 // ms

#define MOTOR_RUN_TEST 0
#define SCREEN_TEST 0
#define ENCODER_TEST 0
#define IMU960_TEST 0
#define OMINDIRECT_TEST 0
#define SERVO_ROTARY_TABLE_TEST 0
#define BLUETOOTH_KEY_TEST 0
#define FLASH_TEST 0
#define ELECTRIC_TEST 0
#define SUB_CORE_GET_COORDINATE_TEST 0
#define SUB_CORE_CORRECT_CENTER_TEST 0
#define DISCHARGE_TEST 0
#define SUB_CORE_SPI_TEST 0
#define SUB_CORE_UART_TEST 0

#define SINGLE_RACE_TEST 0
#define DATASET_COLLECT 0
#define LIGHT_TEST 0
#define MTV903T_TEST 0
#define SUB_CAR_COMMUNITY 0
#define SCAN_COORDINATE_GET_TEST 1

extern void sub_core_uart_test(void);
extern void sub_core_spi_test(void);
extern void motor_test(void);
extern void encoder_test(void);
extern void imu_test(void);
extern void test(void);
extern void omnidirect_test(void);
extern void servo_rotary_table_test(void);
extern void screen_test(void);
extern void flash_test(void);
extern void bluetooth_key_test(void);
extern void electriclock_test();
extern void openart_flat_get_coordinate_test(void);
extern void openart_flat_correct_center_test(void);
extern void discharge_test(void);
extern void single_car_race_test(void);
extern void dataset_collect(void);
extern void light_test(void);
extern void mtv90t_test(void);
extern void sub_car_community(void);
extern void scan_coordinate_get_test(void);
#endif