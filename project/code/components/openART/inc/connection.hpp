#ifndef __CONNECTION_HPP
#define __CONNECTION_HPP

#define UART_GET_COORDINATE (0x01)
#define UART_GET_RECT_CENTER (0x02)
#define UART_CAPTURE_PHOTO (0x03)
#define UART_OPENART_CATEGORY (0X04)
#define UART_GET_VLIP_SIDE_LINE (0X05)
#define UART_GET_HORI_SIDE_LINE (0X11)
#define UART_ENTER_CARPORT (0X06)
#define UART_TRANSFER_SIDE (0X07)
#define UART_CAPTURE_DATASET (0X08)
#define UART_PHOTO_COORDINATE_START (0X09)
#define UART_PHOTO_COORDINATE_STOP (0X10)
#define UART_USE_DEFAULT_CATEGORY (0X20)


extern "C"
{
#include "zf_common_headfile.h"
#include "user_code.h"
}
#include "user_code_cpp.hpp"

extern uint8 point_index;
extern uint8 g_byOpenARTSlopSaveFifoData[168]; // 串口接收数据缓冲区
extern uint8 g_byOpenARTSlopReadFifoData[168]; // fifo 输出读出缓冲区
extern uint8 g_byOpenARTFlatSaveFifoData[168]; // 串口接收数据缓冲区
extern uint8 g_byOpenARTFlatReadFifoData[168]; // fifo 输出读出缓冲区
extern fifo_struct g_OpenARTSlopFifo;
extern uint8 g_byUseSideCommand[5]; // g_byUseSideCommand[2] = 0X01 则使用墙
extern uint8 g_bySetSideCommand[5]; // g_bySetSideCommand[2] = 0X01 则上下加墙
extern uint8 g_byOpenARTCommand[5]; // g_bySetSideCommand[2] = OpenART指令
extern uint8 g_byRectCenCommand[5]; // 直接调用
extern uint8 g_bySideCenCommand[5];
extern uint8 g_byPointClearCommand[5];
extern uint8 g_byALLCenCommand[5];
extern uint8 g_byGetPhotoCoordinateCommand[5];
extern uint8 g_byResultCenCommand[5];
extern uint8 g_byFlatOpenARTCommand[5];

extern uint8 g_bySlopOpenARTCommand[5];
extern uint8 g_byTableIndex;
extern void openart_flat_rx_interrupt_handler(void);


#endif