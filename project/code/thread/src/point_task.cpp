#include "point_task.hpp"

uint8 g_bySetPointFlag = 0;
uint8 g_byPointIndex = 0;
void set_point_task(void)
{
    // 遍历所有扫图得到的坐标点
    if (g_byPointIndex < g_byPointNum - 1)
    {
        g_bGoPointGoalErrFlag = 1; // 复位里程
        // 设定坐标点
        g_fXExpectPosition = g_fPointPositionX[tsp.hamilton_path[g_byPointIndex + 1]] * 20;
        g_fYExpectPosition = g_fPointPositionY[tsp.hamilton_path[g_byPointIndex + 1]] * 20;

        g_byPointIndex++;    // 遍历索引增加
        g_byGoPointFlag = 1; // 开始前往坐标点任务

        g_bySetPointFlag = 0; // 设定坐标点任务结束
        g_byDetectRectFlag = 1;
    }
    else
    {
        // 捡完所有扫图图片后，请等待机械臂的回收以避免误识别边线
        // if (g_bServoNoInROIFlag)
        // {
        g_byPointNum = 0;        // 坐标点数量清空
        g_byPointIndex = 0;      // 坐标点索引清零
        g_byPrepareScanFlag = 1; // 开始扫行校准Y轴任务
        g_byPrepareScanIndex++;  // 校准X轴任务索引增加
        g_bySetPointFlag = 0;    // 设定坐标点任务结束
        // }
    }
}
float g_fXTempExpection;
float g_fYTempExpection;
bool g_bChangeAngleFlag = 0;
void change_angle_task(void)
{
    coordinate_omnidirect_move(g_fXTempExpection, g_fYTempExpection);
    g_byGoPointFlag = 1;
    if (float_abs(s_ICMresult.fYawAngle - g_fAngleExpect) < ANGLE_DEADZONE) // 转至指定角度
    {
        g_byGoPointFlag = 1;
        g_bChangeAngleFlag = 0;
    }
}
uint8 g_byGoPointFlag = 0;
uint8 g_byDetectRectFlag = 1;
uint8 g_byClassifyResetFlag = 0;
bool g_bGoPointGoalErrFlag = 1;
void go_point_task(void)
{
    if (g_bGoPointGoalErrFlag)
    {
        calculate_goal_error();
        g_bGoPointGoalErrFlag = 0;
    }

    coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition); // 开始前往指定目标板
    // 接近目标板时，开始检索目标板位置
    if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 30 &&
        float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 30)
    {
        if (g_byDetectRectFlag)
        {
            // if (g_bServoNoInROIFlag)
            // {
            g_byFlatOpenARTCommand[2] = UART_GET_RECT_CENTER;
            g_bRequireCenterFlag = 1; // 向从核请求中心点
            g_byDetectRectFlag = 0;
            g_byClassifyResetFlag = 1; // 分类信号复位
            // }
        }
    }
    // 如果惯导外环PID进入死区说明达到指定位置
    if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 5 &&
        float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 5)
    {
        // if (g_bServoNoInROIFlag)
        // {
        g_byWaitOpenARTCorrectRectFlag = 1;
        g_byFindRectTimes = 0;
        g_byGoPointFlag = 0;
        // }
    }
}

bool g_bCorrectPointFlag = 0;

void correct_point_task(uint8 times)
{
    static uint32 s_dwDelayTimes = 0;
    static bool s_bGetCategoryFlag = 1;
    s_dwDelayTimes += times;
    if (s_dwDelayTimes >= 300)
    {
        coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);
        g_bRequireCenterFlag = 0; // 结束中心点请求任务
        if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 3 &&
            float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 3)
        {
            if (s_bGetCategoryFlag)
            {
                g_bGetCategoryFlag = 1;     // 开始向flat openart请求分类结果
                g_byCollectPictureFlag = 1; // 开始拾取任务
                s_bGetCategoryFlag = 0;
            }
        }
        if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 1 &&
            float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 1)
        {
            s_dwDelayTimes = 0;
            s_bGetCategoryFlag = 1;
            g_bCorrectPointFlag = 0;
        }
    }
}