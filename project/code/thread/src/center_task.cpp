#include "center_task.hpp"
bool g_CorrectWard1Flag = 1;
bool g_CorrectWard2Flag = 0;
bool g_CorrectWard3Flag = 0;
bool g_CorrectWard4Flag = 0;
// 等待凌瞳矩形中心点信号任务
void wait_correct_rect_task(uint8 times)
{
    static bool s_bStopSearchFlag = 0;
    static float fSearchRectXExp = 0;
    static float fSearchRectYExp = 0;
    g_byFindRectTimes += times;

    // 退出矩形搜索任务
    if (s_bStopSearchFlag)
    {
        openart_xy_speed(0, 0); // 清零速度
        g_byFindRectTimes = 0;  // 清零搜索时间
        g_bRequireCenterFlag = 0;           // 停止访问中心点
        g_bySetPointFlag = 1;               // 开始搜索下一个点

        // 复位标志位
        g_CorrectWard1Flag = 1;
        g_CorrectWard2Flag = 0;
        g_CorrectWard3Flag = 0;
        g_CorrectWard4Flag = 0;

        s_bStopSearchFlag = 0;              // 清零标志位
        g_byWaitOpenARTCorrectRectFlag = 0; // 结束搜索矩形任务
    }
    else if (g_byFindRectTimes >= 100)
    {
        if (g_CorrectWard1Flag)
        {
            fSearchRectXExp = g_fXExpectPosition - 20;
            fSearchRectYExp = g_fYExpectPosition - 20;
            g_fXGoalError = 20;
            g_fYGoalError = 20;

            if (float_abs(fSearchRectXExp - g_fXPositionRealAbsoluteCoordinate) < 10 &&
                float_abs(fSearchRectYExp - g_fYPositionRealAbsoluteCoordinate) < 10)
            {
                g_CorrectWard2Flag = 1;
                g_CorrectWard1Flag = 0;
            }
        }
        else if (g_CorrectWard2Flag) // 再后退
        {
            fSearchRectXExp = g_fXExpectPosition;
            fSearchRectYExp = g_fYExpectPosition - 30;
            g_fXGoalError = 20;
            g_fYGoalError = 30;
            
            
            if (float_abs(fSearchRectXExp - g_fXPositionRealAbsoluteCoordinate) < 10 &&
                float_abs(fSearchRectYExp - g_fYPositionRealAbsoluteCoordinate) < 10)
            {
                s_bStopSearchFlag = 1;
                // g_CorrectWard3Flag = 1;
                g_CorrectWard2Flag = 0;
            }
        }
        // else if (g_CorrectWard3Flag) // 左偏后看一眼
        // {
        //     fSearchRectXExp = g_fXExpectPosition;
        //     fSearchRectYExp = g_fYExpectPosition + 20;
        //     g_fXGoalError = 20;
        //     g_fYGoalError = 25;
        //     if (float_abs(fSearchRectXExp - g_fXPositionRealAbsoluteCoordinate) < 10 &&
        //         float_abs(fSearchRectYExp - g_fYPositionRealAbsoluteCoordinate) < 10)
        //     {
        //         g_CorrectWard4Flag = 1;
        //         g_CorrectWard3Flag = 0;
        //     }
        // }
        // else if (g_CorrectWard4Flag) // 左偏后看一眼
        // {
        //     fSearchRectXExp = g_fXExpectPosition + 20;
        //     fSearchRectYExp = g_fYExpectPosition - 5;
        //     g_fXGoalError = 20;
        //     g_fYGoalError = 15;
        //     if (float_abs(fSearchRectXExp - g_fXPositionRealAbsoluteCoordinate) < 10 &&
        //         float_abs(fSearchRectYExp - g_fYPositionRealAbsoluteCoordinate) < 10)
        //     {
        //         s_bStopSearchFlag = 1;
        //         g_CorrectWard4Flag = 0;
        //     }
        // }
        coordinate_omnidirect_move(fSearchRectXExp, fSearchRectYExp);
    }
    else if (g_byFindRectTimes >= 400) // 40ms未收到信号就停止
    {
        s_bStopSearchFlag = 0;
        openart_xy_speed(0, 0);
    }
}
