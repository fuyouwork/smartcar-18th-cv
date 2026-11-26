#ifndef __CAMMERA_CALLBACK_HPP
#define __CAMMERA_CALLBACK_HPP

#include "user_code_cpp.hpp"
extern "C"
{
#include "user_code.h"
}
extern void picture_rect_center_callback(void);
extern void picture_rect_center_num_callback(void);
extern void picture_category_callback(void);
extern void side_center_callback(void);
extern void side_center_num_callback(void);
extern void map_coordinate_num_callback(void);
extern void map_coordinate_get_callback(void);
extern void map_coordinate_finish_callback(void);
extern void openart_side_callback(void);
extern void photo_coordinate_nums_callback(void);
extern void photo_get_coordinate_callback(void);
extern void photo_get_coordinate_end_callback(void);

#endif