#ifndef __FLASH_H
#define __FLASH_H

// 宏定义请放在头文件最开始处
#define NAME_COUNT 10

extern "C"
{
#include "zf_common_headfile.h"
}
#include "user_code_cpp.hpp"

typedef enum
{
	V,
	FLP,
	FLI,
	X,
	Y,
	XOP,
	YOP,
	XOD,
	YOD,
	ARTX_P,
	ARTY_P,
	AP,
	AI,
	AD,
	FRP,
	FRI,
	BLP,
	BLI,
	BRP,
	BRI,
	XCO,
	YCO,
	DATA_INDEX,
	SERVO_10K_ANGLE,
	SERVO_20K_ANGLE,
	SERVO_35K_ANGLE,
	LIGHT,
	FLASH_SAVE_NUM
} FLASH_SAVE;

extern "C"
{
	
	// 结构体计算要遵循字节对齐原则，一般满足三个准则：
	// 1) 结构体变量的首地址能够被其最宽基本类型成员的大小所整除；
	// 2) 结构体每个成员相对于结构体首地址的偏移量（offset）都是成员大小的整数倍，如有需要编译器会在成员之间加上填充字节；
	// 3) 结构体的总大小为结构体最宽基本类型成员大小的整数倍，如有需要编译器会在最末一个成员之后加上填充字节；
	typedef struct
	{
		flash_data_union *flash_buffer;
		
	} Flash;
	extern char g_cFlashBufName[FLASH_SAVE_NUM][NAME_COUNT];
	extern Flash SFlashBuf;
	extern float_t g_fChangeFactor[FLASH_SAVE_NUM];
}

extern void flash_write_param(void);
extern void describe_flash_buff(void);
extern void read_param_from_buffer(void);

#endif