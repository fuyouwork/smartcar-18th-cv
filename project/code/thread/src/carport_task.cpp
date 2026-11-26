#include "carport_task.hpp"

uint8 g_byOutCarPorFlag = 0;

// 出库任务
void out_carport_task(void)
{
    // 压线触发计时
    g_fXExpectPosition = 20;
    g_fYExpectPosition = 70;
    // 计算总的里程，以便计算车起跑偏移量
    static bool bGoalErrorSetFlag = 1;
    if (bGoalErrorSetFlag)
    {
        calculate_goal_error();
        bGoalErrorSetFlag = 0; // 清除里程标志防止反复运算
    }
    // 全向移动至期望位置
    coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);
    // 压线成功
    if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 20 && float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 20)
    {
        bGoalErrorSetFlag = 1;   // 复位里程
        g_byOutCarPorFlag = 0;   // 结束出库任务
        g_bySetScanLineFlag = 1; // 开始换行任务
    }
}

// 回库任务
void go_car_task(void)
{
    static uint8 byFirstOpenElectricFlag = 1;
    static bool bCorrectXFinishFlag = 0;
    static bool bDetectSideFlag = 1;
    g_fXExpectPosition = 30;

    if (!bCorrectXFinishFlag)
    {
        static bool bGoalErrorSetFlag = 1;
        if (bGoalErrorSetFlag)
        {
            calculate_goal_error();
            bGoalErrorSetFlag = 0;
        }
        coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);

        if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 30 &&
            float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 30)
        {

            if (bDetectSideFlag)
            {
#if SIDE_DETECT_BY_OPENART
                g_bGetSideFlag = 1;
                g_byCorrectMode = CORRECT_X;
                bDetectSideFlag = 0;
#endif
            }
        }
        if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 10 &&
            float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 10)
        {
            bCorrectXFinishFlag = 1;
            bGoalErrorSetFlag = 1;
        }
    }

    if (bCorrectXFinishFlag)
    {
        g_fYExpectPosition = -40;
        static bool bGoalErrorSetFlag1 = 1;
        static bool bDetectSideFlag = 1;
        if (bGoalErrorSetFlag1)
        {
            calculate_goal_error();
            bGoalErrorSetFlag1 = 0;
        }
        coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);

        if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 30 &&
            float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 30)
        {
            g_bGetSideFlag = 1;
            g_byCorrectMode = CORRECT_Y;
            bDetectSideFlag = 0;
        }
        if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 5 &&
            float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 5)
        {
            g_bGetSideFlag = 0;
        }
    }
}