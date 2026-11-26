#include "connection.hpp"
uint8 g_byTableIndex = 0;
uint8 g_byRectCenCommand[5] = {0x4F, 0X10, 0X01, 0X76, 0X76}; // 直接调用
uint8 g_bySideCenCommand[5] = {0x4F, 0X10, 0X02, 0X76, 0X76};
uint8 g_byALLCenCommand[5] = {0x4F, 0X10, 0X03, 0X76, 0X76};
uint8 g_byResultCenCommand[5] = {0x4F, 0X10, 0x04, 0X76, 0X76};
uint8 g_byUseSideCommand[5] = {0x4F, 0X11, 0X00, 0X76, 0X76};     // g_byUseSideCommand[2] = 0X01 则使用墙
uint8 g_bySetSideCommand[5] = {0x4F, 0X12, 0X00, 0X76, 0X76};     // g_bySetSideCommand[2] = 0X00 则左右加墙， 0X01则上下加墙
uint8 g_byFlatOpenARTCommand[5] = {0x4F, 0x16, 0X00, 0X76, 0X76};
uint8 g_bySlopOpenARTCommand[5] = {0x4F, 0x17, 0X00, 0X76, 0X76};
uint8 g_byOpenARTCommand[5] = {0x4F, 0X18, 0X00, 0X76, 0X76};
uint8 g_byGetPhotoCoordinateCommand[5] = {0x4F, 0X21, 0X01, 0X76, 0X76};
uint8 g_byPointClearCommand[5] = {0x4F, 0x22, 0X01, 0X76, 0X76};

uint8 g_byOpenARTFlatSaveFifoData[168]; // 串口接收数据缓冲区
uint8 g_byOpenARTFlatReadFifoData[168]; // fifo 输出读出缓冲区

uint8 PhoenixTurntableCount = 0;
uint8 g_byOpenARTFlatDataTemp = 0;   // 接收数据变量
uint32 g_dwOpenARTFlatFifoCount = 0; // fifo 数据个数

fifo_struct g_OpenARTFlatFifo;

/// @brief Openart平放摄像头串口接收处理函数
/// @param  无
void openart_flat_rx_interrupt_handler(void)
{
    static uint8 byCount = 0;

    uart_query_byte(SUB_CORE_UART_INDEX, &g_byOpenARTFlatDataTemp);     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
    fifo_write_buffer(&g_OpenARTFlatFifo, &g_byOpenARTFlatDataTemp, 1); // 将数据写入 fifo 中
    g_dwOpenARTFlatFifoCount = fifo_used(&g_OpenARTFlatFifo);
    fifo_read_buffer(&g_OpenARTFlatFifo, g_byOpenARTFlatReadFifoData, &g_dwOpenARTFlatFifoCount, FIFO_READ_ONLY);
    // 帧头必有0x2c
    if (g_byOpenARTFlatReadFifoData[0] != 0x2c)
    {
        fifo_clear(&g_OpenARTFlatFifo);
    }

    if (g_dwOpenARTFlatFifoCount >= 4) // 2个帧头和2个帧尾
    {

        if (g_byOpenARTFlatReadFifoData[0] == 0x2C && g_byOpenARTFlatReadFifoData[1] == 0xFF &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B)
        {
            g_byOpenARTOpenFlag = 1;
        }
        // 坐标点发送开始标志，附带坐标点总数
        if (g_byOpenARTFlatReadFifoData[0] == 0x2C && g_byOpenARTFlatReadFifoData[1] == 0x00 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B) // 获取坐标点数
        {
            map_coordinate_num_callback();
        }

        // 坐标点接收
        if (g_byOpenARTFlatReadFifoData[0] == 0x2C && g_byOpenARTFlatReadFifoData[1] == 0x01 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B) // 获取坐标值
        {
            map_coordinate_get_callback();
        }

        // 坐标点接收完成标志
        if (g_byOpenARTFlatReadFifoData[0] == 0x2C && g_byOpenARTFlatReadFifoData[1] == 0x02 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B) // 坐标获取完成
        {
            map_coordinate_finish_callback();
        }
        // 收到当前视野内图片中心点总数
        if (g_byOpenARTFlatReadFifoData[0] == 0x2c && g_byOpenARTFlatReadFifoData[1] == 0x10 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B)
        {
            // pass
            picture_rect_center_num_callback();
        }
        // 图片中心矫正
        if (g_byOpenARTFlatReadFifoData[0] == 0x2c && g_byOpenARTFlatReadFifoData[1] == 0x11 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B)
        {
            picture_rect_center_callback();
        }
        // 图片种类获取
        if (g_byOpenARTFlatReadFifoData[0] == 0x2C && g_byOpenARTFlatReadFifoData[1] == 0x12 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B) // 坐标获取完成
        {
            picture_category_callback();
        }
        // 边线数量获取
        if (g_byOpenARTFlatReadFifoData[0] == 0x2c && g_byOpenARTFlatReadFifoData[1] == 0x20 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B) // 收到边线中心点总数
        {
            side_center_num_callback();
        }
        // 边线矫正
        if (g_byOpenARTFlatReadFifoData[0] == 0x2c && g_byOpenARTFlatReadFifoData[1] == 0x21 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B) // 获取边线中心位置
        {
            side_center_callback();
        }
        if (g_byOpenARTFlatReadFifoData[0] == 0x2c && g_byOpenARTFlatReadFifoData[1] == 0x22 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B) // 获取OpenART边线中心位置
        {
            // 横线
            openart_side_callback();
        }
        if (g_byOpenARTFlatReadFifoData[0] == 0x2c && g_byOpenARTFlatReadFifoData[1] == 0x23 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B) // 获取OpenART边线中心位置
        {
            // 竖线
            openart_side_callback();
        }
        if (g_byOpenARTFlatReadFifoData[0] == 0x2c && g_byOpenARTFlatReadFifoData[1] == 0x30 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B)
        {
            photo_coordinate_nums_callback();
        }
        if (g_byOpenARTFlatReadFifoData[0] == 0x2c && g_byOpenARTFlatReadFifoData[1] == 0x31 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B)
        {
            photo_get_coordinate_callback();
        }
        if (g_byOpenARTFlatReadFifoData[0] == 0x2c && g_byOpenARTFlatReadFifoData[1] == 0x32 &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
            g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B)
        {
            photo_get_coordinate_end_callback();
        }
    }
    if ((g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 2] == 0x5B &&
         g_byOpenARTFlatReadFifoData[g_dwOpenARTFlatFifoCount - 1] == 0x5B) ||
        g_dwOpenARTFlatFifoCount > 8)
    {
        fifo_clear(&g_OpenARTFlatFifo);
    }
}

uint8 g_byOpenARTSlopSaveFifoData[168]; // 串口接收数据缓冲区
uint8 g_byOpenARTSlopReadFifoData[168]; // fifo 输出读出缓冲区

uint8 g_byOpenARTSLopDataTemp = 0;   // 接收数据变量
uint32 g_dwOpenARTSlopFifoCount = 0; // fifo 数据个数
fifo_struct g_OpenARTSlopFifo;
void openart_slop_rx_interrupt_handler(void)
{
    static uint8 byCount = 0;
    uart_query_byte(OPENART_SLOP_UART_INDEX, &g_byOpenARTSLopDataTemp); // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
    fifo_write_buffer(&g_OpenARTSlopFifo, &g_byOpenARTSLopDataTemp, 1); // 将数据写入 fifo 中
    g_dwOpenARTSlopFifoCount = fifo_used(&g_OpenARTSlopFifo);

    if (g_dwOpenARTSlopFifoCount >= 3) // 2个帧头和1个帧尾
    {
        fifo_read_buffer(&g_OpenARTSlopFifo, g_byOpenARTSlopReadFifoData, &g_dwOpenARTSlopFifoCount, FIFO_READ_ONLY);

        // 图片中心点反馈
        if (g_byOpenARTSlopReadFifoData[0] == 0x2c && g_byOpenARTSlopReadFifoData[1] == 0x22 && g_byOpenARTSlopReadFifoData[g_dwOpenARTSlopFifoCount - 1] == 0x5B) // 获取图片中心位置
        {
            fifo_clear(&g_OpenARTSlopFifo); // 清除UART的FIFO区
        }
    }
}
