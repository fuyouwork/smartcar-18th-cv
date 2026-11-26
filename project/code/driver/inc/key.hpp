#ifndef __KEY_HPP
#define __KEY_HPP

extern "C"
{
	#include "zf_common_headfile.h"
	#include "user_code.h"
}
#include "user_code_cpp.hpp"

// 按键开关引脚定义
#define SW1_PIN 	(C26)
#define SW2_PIN 	(B27)
#define SW3_PIN 	(B21)
#define SW4_PIN 	(B11)
#define SW5_PIN		(B10)

// 按键开关引脚电压读取
#define SW1_LEVEL			(gpio_get_level(SW1_PIN))
#define SW2_LEVEL			(gpio_get_level(SW2_PIN))
#define SW3_LEVEL			(gpio_get_level(SW3_PIN))
#define SW4_LEVEL			(gpio_get_level(SW4_PIN))
#define SW5_LEVEL			(gpio_get_level(SW5_PIN))

// 按键状态宏定义
#define KEY_RELEASE   		(0)                                                     // 按键释放状态
#define KEY_SHORT_PRESS 	(1)                                                     // 按键短按状态
#define KEY_LONG_PRESS		(2)                                                     // 按键长按状态

// 按键状态是否清除
#define KEY_CLEAR_STATE	(1)
#define KEY_SAVE_STATE (0)

// 按键导通IO口电压
#define KEY_ON	(0)					// 导通电压
#define KEY_OFF	(1)

// 长按增长延时判定,非延时函数
#define KEY_DELAY (1)				// 延时中
#define KEY_EXECUTE	(0)
#define KEY_DELAY_MS	(1)		// 增加延时时间设置，1 * 20ms

// 按键开关枚举
typedef enum
{
    BKEY1 = 0,
    BKEY2 = 1,
    BKEY3 = 2,
    BKEY4 = 3,
	BKEY5 = 4,
	BKEYNUM
}KEYn_e;

// 请注意由于所有IO口初始化都应在单片机时钟初始化之后
// 所以请不要将按键IO口初始化放在构造函数中除非你从不将按键类视为全局变量，否则程序将在所有程序开始前初始化按键IO口
// 在需要修改按键数时，请访问按键KEYn_e枚举，并添加相应的按键IO口宏定义
// 在需要修改按键功能时，请访问void read_key_command(bool bClearFlag);
class key_t
{
	private:
		bool m_bKeyNowState;
		bool m_bKeyLastState;
		bool m_bFirstJudgeKeyState;
		gpio_pin_enum m_KEY_PIN;
		uint32 m_dwKeyPressTimes;
	public:
		uint8 m_byChangeDelayFlag;
		uint8 m_byKeyState;
		key_t(gpio_pin_enum KEY_PIN);
		void judge_key_state(void);
		void read_key_state_with_clear(bool bKeyState);
		void clear_key_state(void);
};

namespace bluetooth
{
	enum KEYINDEX
	{
		MOTOR_KEY = 0XD1,
		SERVO_KEY = 0XD2,
		KEY1 = 0X11,
		KEY1_LONG = 0X01,
		KEY2 = 0X22,
		KEY3 = 0X33,
		KEY3_LONG = 0X03,
		KEY4 = 0X44,
		KEY5 = 0X55,
		KEY6 = 0X66,
		KEY6_LONG = 0X06,
		KEY7 = 0X77,
		KEY8 = 0X88
	};
}
extern void read_key_command(bool bClearFlag);
extern void clear_key_all_state(void);	
extern void read_bluetooth_key(uint8 KeyIndex);

extern bool g_bWriteFlag;
extern uint8 g_byStart;

#endif

