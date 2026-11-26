#include "cammera_callback.hpp"
uint8 g_byRectCenterIndex = 0;
uint8 g_byRectCorrectCounts = 0;
uint8 g_byRectCenterNum = 0;
void picture_rect_center_num_callback(void)
{
    g_byRectCenterIndex = 0;
    g_byRectCenterNum = g_byOpenARTFlatReadFifoData[2]; // 当前视野内坐标点中心总数
}
void picture_rect_center_callback(void)
{
    g_byGoPointFlag = 0;
    g_byFindRectTimes = 0; // 搜索矩形时间清零
    if (!g_byCollectPictureFlag)
    {
        g_fXOpenArtPosition = g_byOpenARTFlatReadFifoData[2] + g_byOpenARTFlatReadFifoData[3];
        g_fYOpenArtPosition = g_byOpenARTFlatReadFifoData[4];
        // OpenArt反馈的位置为期望位置
        if (float_abs(g_fXOpenArtPosition - g_fXOpenArtPositionExpect) < 70 &&
            float_abs(g_fYOpenArtPosition - g_fYOpenArtPositionExpect) < 70)
        {
            openart_xy_speed(0, 0); // 设定矫正期望速度为0

            g_bCorrectPointFlag = 1;

            g_byOpenArtCorrectFlag = 0; // 结束全向移动矫正任务
            g_byWaitOpenARTCorrectRectFlag = 0;

            g_fXPositionRealAbsoluteCoordinate = g_fPointPositionX[tsp.hamilton_path[g_byPointIndex]] * 20 - 0.21 * (g_fXOpenArtPosition - g_fXOpenArtPositionExpect);
            g_fYPositionRealAbsoluteCoordinate = g_fPointPositionY[tsp.hamilton_path[g_byPointIndex]] * 20 + 0.21 * (g_fYOpenArtPosition - g_fYOpenArtPositionExpect);
        }
        else
        {
            if (g_bCorrectPointFlag)
            {
                g_fXPositionRealAbsoluteCoordinate = g_fPointPositionX[tsp.hamilton_path[g_byPointIndex]] * 20 - 0.21 * (g_fXOpenArtPosition - g_fXOpenArtPositionExpect);
                g_fYPositionRealAbsoluteCoordinate = g_fPointPositionY[tsp.hamilton_path[g_byPointIndex]] * 20 + 0.21 * (g_fYOpenArtPosition - g_fYOpenArtPositionExpect);
            }
            else
            {
                g_CorrectWard1Flag = 1; // 复位修正程序
                g_CorrectWard2Flag = 0;
                g_CorrectWard3Flag = 0;
                g_CorrectWard4Flag = 0;
                g_byOpenArtCorrectFlag = 1; // 开始全向移动矫正任务
            }
        }
    }
    g_byCorrectMode = CORRECT_ALL; // 全向矫正模式
}
uint8 g_byCategoryIndex = 0;
void picture_category_callback(void)
{
    // 载入图像种类索引
    g_byCategoryIndex = g_byOpenARTFlatReadFifoData[2];

    g_byTableIndex = g_CategoryMap[g_byCategoryIndex]->PhoneTableIndex;

    if (g_byTableIndex > 5)
    {
        g_bGetCategoryFlag = 1;
    }
    else
    {
        if (g_byTableIndex == 0) // 给副车的
        {
            g_byPlaceCarFlag = 1;
            set_rotary_expect_angle_with_index(1);
            g_byRotaryPlatformFlag = 1;
        }
        else
        {
            g_byPlaceCarFlag = 0;
            set_rotary_expect_angle_with_index(g_byTableIndex);
            // 转动转台并拾取图片
            g_byRotaryPlatformFlag = 1;
        }
        g_byRingCounts = 2;
        g_byClassifyFinishFlag = 1;    // 开始分类完成任务
        g_bGetCategoryFlag = 0;        // 结束分类结果请求
        g_bUseDefaultCategoryFLag = 0; // 结束请求使用默认矩形框
        // 重新设计灯的亮度等级
        g_lBrightnessLevel = 100;
    }
}
uint8 g_bySideIndex = 0;
void side_center_num_callback(void)
{
    g_bySideIndex = 0;
    g_bySideNum = g_byOpenARTFlatReadFifoData[2];
}
void side_center_callback(void)
{
    if (g_bySideIndex < g_bySideNum - 1)
    {
        // TODO: 保存中心点
        g_fXScc8660SideArray[g_bySideIndex] = g_byOpenARTFlatReadFifoData[2];
        g_fYScc8660SideArray[g_bySideIndex] = g_byOpenARTFlatReadFifoData[3];
        g_bySideIndex++;
    }
    else
    {
        bool bCorrectSideFinishFlag = 0;
        g_fXScc8660SideArray[g_bySideIndex] = g_byOpenARTFlatReadFifoData[2];
        g_fYScc8660SideArray[g_bySideIndex] = g_byOpenARTFlatReadFifoData[3];
        g_bySideIndex++;
        g_fXScc8660SidePosition = 0;
        g_fYScc8660SidePosition = 0;
        for (uint8 index = 0; index < g_bySideIndex; index++)
        {
            g_fXScc8660SidePosition += g_fXScc8660SideArray[index];
            g_fYScc8660SidePosition += g_fYScc8660SideArray[index];
        }

        g_fXOpenArtPosition = (float)(g_fXScc8660SidePosition) / g_bySideNum;
        g_fYOpenArtPosition = (float)(g_fYScc8660SidePosition) / g_bySideNum;

        if (g_byCorrectMode == CORRECT_X)
        {
            if (sign(g_fXPositionRealAbsoluteCoordinate - 350) > 0)
                g_fXPositionRealAbsoluteCoordinate = 700 - (g_fXOpenArtPosition - g_fXOpenArtPositionExpect) * 0.21;
            else
                g_fXPositionRealAbsoluteCoordinate = 0 - (g_fXOpenArtPosition - g_fXOpenArtPositionExpect) * 0.21;
        }
        else if (g_byCorrectMode == CORRECT_Y)
        {
            if (sign(g_fYPositionRealAbsoluteCoordinate - 250) > 0)
                g_fYPositionRealAbsoluteCoordinate = 500 + (g_fYOpenArtPosition - g_fYOpenArtPositionExpect) * 0.21;
            else
                g_fYPositionRealAbsoluteCoordinate = 0 + (g_fYOpenArtPosition - g_fYOpenArtPositionExpect) * 0.21;
        }
        g_bRequireCenterFlag = 0;
    }
}
typedef struct OpenARTSideCen
{
    float fX;
    float fY;
} OpenARTSideCen_t;
OpenARTSideCen_t g_OpenARTSidePos, g_OpenARTSideExp;

void openart_side_callback(void)
{
    g_OpenARTSidePos.fX = g_byOpenARTFlatReadFifoData[2] - 127;
    g_OpenARTSidePos.fY = g_byOpenARTFlatReadFifoData[3] - 127;
    if (g_byCorrectMode == CORRECT_X)
    {
        g_byRingCounts = 2;
        if (sign(g_fXPositionRealAbsoluteCoordinate - 350) > 0)
            g_fXPositionRealAbsoluteCoordinate = 700 + g_OpenARTSidePos.fX;
        else
            g_fXPositionRealAbsoluteCoordinate = 0 + g_OpenARTSidePos.fX;
        if (g_byPrepareScanFlag)
        {
            g_byScanLineIndex++;
            g_bySetScanLineFlag = 1;
            g_byPrepareScanFlag = 0;
            g_bPreScanErrFlag = 1;
            g_bPrepareDetecSideFlag = 1;
        }
    }
    else if (g_byCorrectMode == CORRECT_Y)
    {
        g_byRingCounts = 2;
        if (sign(g_fYPositionRealAbsoluteCoordinate - 250) > 0)
        {
            g_fYPositionRealAbsoluteCoordinate = 500 + g_OpenARTSidePos.fY;
        }
        else
        {
            g_fYPositionRealAbsoluteCoordinate = 0 + g_OpenARTSidePos.fY;
        }
    }

    g_bGetSideFlag = 0;
}
uint8 g_byMapCoordinateGetCount;
void map_coordinate_num_callback(void)
{
    g_byMapCoordinateGetCount = 0;
    g_byPointNum = g_byOpenARTFlatReadFifoData[2];
}
void map_coordinate_get_callback(void)
{
    g_fPointPositionX[g_byMapCoordinateGetCount] = g_byOpenARTFlatReadFifoData[2]; // 坐标点载入
    g_fPointPositionY[g_byMapCoordinateGetCount] = g_byOpenARTFlatReadFifoData[3];
    g_byMapCoordinateGetCount++;
}
void map_coordinate_finish_callback(void)
{
    // 当实际获取到的坐标数量和OpenART发送的坐标总数相等时，坐标获取完成
    if (g_byPointNum == g_byMapCoordinateGetCount)
    {
        g_byGetCoordinateFinishFlag = 0;
        g_byMapCoordinateGetCount = 0;
        path_plan();
    }
}
void photo_coordinate_nums_callback(void)
{

    g_byMapCoordinateGetCount = 0;                     // 清零坐标点累计
    g_byPointNum = g_byOpenARTFlatReadFifoData[2];     // 扫图扫到的坐标点
    g_byPointNum = g_byOpenARTFlatReadFifoData[2] + 1; // 扫图坐标点数，囊括上起点

    // 坐标点总数为1,说明只有起点
    if (g_byPointNum == 1)
    {
        g_bySetPointFlag = 1;
        g_byPointNum = 0; // 清零PointNum
    }
    g_bRequireCoordinateFlag = 0; // 结束坐标点获取请求
}
CoordinataFrame_t g_CoordinateGet;
void photo_get_coordinate_callback(void)
{
    // 坐标点获取
    g_CoordinateGet.CoordianteX.bySend[0] = g_byOpenARTFlatReadFifoData[2];
    g_CoordinateGet.CoordianteX.bySend[1] = g_byOpenARTFlatReadFifoData[3];
    g_CoordinateGet.CoordianteY.bySend[0] = g_byOpenARTFlatReadFifoData[4];
    g_CoordinateGet.CoordianteY.bySend[1] = g_byOpenARTFlatReadFifoData[5];
    // 载入坐标点
    g_fPointPositionX[g_byMapCoordinateGetCount] = g_CoordinateGet.CoordianteX.iData;
    g_fPointPositionY[g_byMapCoordinateGetCount] = g_CoordinateGet.CoordianteY.iData;
    g_byMapCoordinateGetCount++;
}
bool bPathPlanFlag = 0;
void photo_get_coordinate_end_callback(void)
{
    // 如果载入的坐标点数和从核找到的点数一样说明获取成功
    if (g_byPointNum == g_byMapCoordinateGetCount)
    {
        // 载入起点
        g_fPointPositionX[0] = g_fXPositionRealAbsoluteCoordinate / 20;
        g_fPointPositionY[0] = g_fYPositionRealAbsoluteCoordinate / 20;
        g_byMapCoordinateGetCount = 0; // 清零坐标点累计
        bPathPlanFlag = 1;             // 开始路径规划
    }
    else
    {
        g_bRequireCoordinateFlag = 1; // 重新请求
    }
}