#ifndef __TASK_HPP
#define __TASK_HPP

#include "user_code_cpp.hpp"

extern "C"
{
#include "user_code.h"
}


extern void go_center_task(void);
extern void go_side_by_audio_task(void);
extern void set_side_by_audio_task(void);
extern void wait_audio_signal_task(uint8 times);
extern void collect_picture_task(uint8 times);
extern void set_side_task(void);
extern void set_point_task(void);
extern void go_point_task(void);
extern void classify_finish_task(void);
extern void wait_correct_rect_task(uint8 times);
extern void rotary_platform_task(void);
extern void open_electric_lock_task(uint8 times);
extern void go_side_task(void);
extern void go_car_task(void);
extern void out_carport_task(void);
extern void set_scan_line_task(void);
extern void scan_line_task(void);
extern void prepare_scan_task(void);
extern uint32 g_byWaitCorrectFinishTimes;
extern int32 g_fAudioIncreasementX;
extern int32 g_fAudioIncreasementY;
extern void calculate_goal_error(void);
#endif