#include "buzzer.hpp"

void ring(uint8 i)//叫几下子
{
    while(i--)
    {
        gpio_toggle_level(BUZZER_PIN);//铃响寻车
        system_delay_ms(50);
        gpio_toggle_level(BUZZER_PIN);
        system_delay_ms(50);
    }
}
