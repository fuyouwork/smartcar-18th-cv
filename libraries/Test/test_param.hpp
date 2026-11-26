#ifndef __TEST_PARAM_HPP
#define __TEST_PARAM_HPP

#define SERVO_PARAM_TEST 0
#define OMNIDIRECT_PARAM_TEST 0
#define SPEED_PID_TEST 0
#define OUTER_PID_TEST 0

extern "C"
{
    #include "user_code.h"
}
#include "user_code_cpp.hpp"

extern char ServoEncoderMessage[30];

extern void test_param(void);
extern void servo_param_test(void);
extern void omnidirect_param_test(void);
extern void speed_pid_test(void);
extern void outer_pid_test(void);

extern uint8 bySdcardTestFlag;
#endif