/*
 * Copyright (C), 1988-1999, Xxxxxx Tech. Co., Ltd.
 * FileName: ZwPlus_lib.c
 * Description: 驺吾图传PLUS库函数
 * Change Logs:
  |Date          |Author       |Notes               |version
  |2022-10-29    |满心欢喜     |Initial build       |1.0.0
 */

/* @include */
#include "zw_device/ZwPlus_lib.h"

/************************************************************
                                                 驺吾图传

【函数名称】ZW_Send_Image
【功    能】图像发送函数
【参    数】ZW_Image二值化图像数组（非0及1）的二值化图像
【返 回 值】无
【实    例】ZW_Send_Image(mt9v03x_csi_image);
【注意事项】①只能传入二值化图像（非0及1）
                        ②.h文件中宏定义尺寸必须与上位机所填写的尺寸对应否则无法使用
                        ③该函数将会对图像进行重新编码再向上位机发送

************************************************************/
void Zw_SendImage(Zw_uint16* Zw_Image)
{
    Zw_uint16 dat;
    Zw_uint16 i, j;
    if(Zw_Image == Zw_NULL) return;
    for(i = 0; i < 120 * 188; i += 8)
    {
        dat = 0;
        for(j = 0; j < 8; j++)
        {
            if(*(Zw_Image + i + j))    dat |= 0x01 << (7-j);
        }
        Zw_Putchar(dat);
    }
}
