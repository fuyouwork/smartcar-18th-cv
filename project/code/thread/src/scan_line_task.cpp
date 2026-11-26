#include "scan_line_task.hpp"

uint8 g_bySetScanLineFlag = 0;
uint8 g_byScanLineIndex = 0;
#define Y_OFFSET -40
// #define X_START 0
// #define X_END 700
#define X_START -20
#define X_END 700
LineXY_t g_ScanLine[10] =
    {
#if FLAT_SCAN_COORDINATE
        {X_END, 40 + Y_OFFSET},
        {X_END, 170 + Y_OFFSET},
        {X_END, 300 + Y_OFFSET},
        {X_END, 430 + Y_OFFSET},
#else
        {X_END, 20 + Y_OFFSET},
        {X_END, 110 + Y_OFFSET},
        {X_END, 200 + Y_OFFSET},
        {X_END, 290 + Y_OFFSET},
        {X_END, 380 + Y_OFFSET},
#endif
};
LineXY_t g_PrepareScan[10] =
    {
#if FLAT_SCAN_COORDINATE
        {0, 0},
        {X_START, 170 + Y_OFFSET},
        {X_START, 300 + Y_OFFSET},
        {X_START, 430 + Y_OFFSET},
#else
        {0, 0},
        {X_START, 110 + Y_OFFSET},
        {X_START, 200 + Y_OFFSET},
        {X_START, 290 + Y_OFFSET},
        {X_START, 380 + Y_OFFSET},
#endif
};
bool g_bSetLineGoalErrFag = 1;
// 换行任务
void set_scan_line_task(void)
{
// 扫4次
#if FLAT_SCAN_COORDINATE
    if (g_byScanLineIndex < 4)
#else
    if (g_byScanLineIndex < 5)
    
#endif
    {

        // 设定扫行起点
        g_fYExpectPosition = g_ScanLine[g_byScanLineIndex].y;
        if (g_bSetLineGoalErrFag)
        {
            calculate_goal_error();
            g_bSetLineGoalErrFag = 0;
        }
        coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);

        // 换行任务结束
        if (float_abs(g_fYExpectPosition - g_fYPositionRealAbsoluteCoordinate) < 5)
        {
            g_bGetSideFlag = 0;
            // 请求清除从核内的坐标点点数
            uart_write_buffer(SUB_CORE_UART_INDEX, g_byPointClearCommand, sizeof(g_byPointClearCommand));
#if COMMAND_REPEAT
            uart_write_buffer(SUB_CORE_UART_INDEX, g_byPointClearCommand, sizeof(g_byPointClearCommand));
#endif
            // 请求slop openart开启扫图
            g_bySlopOpenARTCommand[2] = UART_PHOTO_COORDINATE_START;
            uart_write_buffer(SUB_CORE_UART_INDEX, g_bySlopOpenARTCommand, sizeof(g_byOpenARTCommand));
#if COMMAND_REPEAT
            uart_write_buffer(SUB_CORE_UART_INDEX, g_bySlopOpenARTCommand, sizeof(g_byOpenARTCommand));
#endif
            g_bSetLineGoalErrFag = 1;  // 复位里程
            g_bSendCoordinateFlag = 1; // 开始向从核发送坐标位置
            g_byScanLineFlag = 1;      // 扫行任务开始
            g_bySetScanLineFlag = 0;   // 换行任务结束
        }
    }
    else
    {
        g_bySetSideFlag = 1;
        g_bySetScanLineFlag = 0;
        openart_xy_speed(0, 0); // 防止车失控
    }
}
uint8 g_byScanLineFlag = 0;
uint8 g_bySideCorrectScanFlag = 0;
uint8 g_byWaitScanCorrectFlag = 0;
uint8 g_byWaitSideCorrectScanTaskFlag = 0;
void scan_line_task(void)
{
    // 设定扫行终点
    g_fXExpectPosition = g_ScanLine[g_byScanLineIndex].x;
    static bool bGoalErrorSetFlag = 1;
    if (bGoalErrorSetFlag)
    {
        calculate_goal_error();
        bGoalErrorSetFlag = 0;
    }
    coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);

    static bool bRequireStopScanFlag = 1;
    // 到达该行末尾
    if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 40)
    {
        if (bRequireStopScanFlag)
        {
            // 先停止扫图
            g_bSendCoordinateFlag = 0;
            // 请求slop openart停止扫图
            g_bySlopOpenARTCommand[2] = UART_PHOTO_COORDINATE_STOP;
            uart_write_buffer(SUB_CORE_UART_INDEX, g_bySlopOpenARTCommand, sizeof(g_bySlopOpenARTCommand));
            bRequireStopScanFlag = 0; // 清除请求停止扫图标志位
        }
    }
    // 已经到达行末尾
    if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 15)
    {
        g_bRequireCoordinateFlag = 1; // 向从核请求坐标点
        g_bWaitCoordinateGetFlag = 1; // 等待坐标点获取
        bGoalErrorSetFlag = 1;        // 复位里程计
        bRequireStopScanFlag = 1;     // 复位请求停止扫图标志位
        g_byScanLineFlag = 0;         // 扫行任务结束
    }
}
uint8 g_byPrepareScanIndex = 0;
uint8 g_byWaitPrescanCorrectFlag = 0;
uint8 g_byPrepareScanFlag = 0;
bool g_bPrepareDetecSideFlag = 1;
bool g_bPreScanErrFlag = 1;
void prepare_scan_task(void)
{
#if FLAT_SCAN_COORDINATE
    if (g_byPrepareScanIndex < 4)
#else
    if (g_byPrepareScanIndex < 5)
#endif
    {
        g_fXExpectPosition = g_PrepareScan[g_byPrepareScanIndex].x;
        g_fYExpectPosition = g_PrepareScan[g_byPrepareScanIndex].y;
        if (g_bPreScanErrFlag)
        {
            calculate_goal_error();
            g_bPreScanErrFlag = 0;
        }

        coordinate_omnidirect_move(g_fXExpectPosition, g_fYExpectPosition);

        if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 100) // 到达该行末尾
        {
            if (g_bPrepareDetecSideFlag)
            {
                g_bGetSideFlag = 1;          // 请求边线中心
                g_byCorrectMode = CORRECT_X; // 矫正模式X
                g_bPrepareDetecSideFlag = 0;
            }
        }
        if (float_abs(g_fXExpectPosition - g_fXPositionRealAbsoluteCoordinate) < 10) // 到达该行末尾
        {
            g_byScanLineIndex++;     // 扫行索引增加
            g_bySetScanLineFlag = 1; // 开始换行任务
            g_byPrepareScanFlag = 0; // 结束准备任务
            g_bGetSideFlag = 0;      // 结束边线中心请求

            g_bPrepareDetecSideFlag = 1; // 复位
            g_bPreScanErrFlag = 1;
        }
    }
    else
    {
        g_bySetSideFlag = 1;
        g_byPrepareScanFlag = 0; // 结束准备任务
    }
}