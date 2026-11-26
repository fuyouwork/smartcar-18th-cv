#include "category_task.hpp"

void classify_finish_task(void)
{
    if (g_byOpenArtCorrectFlag == 0)
    {
        openart_xy_speed(0, 0);

        g_bGetCategoryFlag = 0;     // 结束分类结果请求
        
        g_byClassifyFinishFlag = 0;
    }
}