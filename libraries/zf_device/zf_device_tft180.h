/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library ����RT1064DVL6A ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
* 
* ���ļ��� RT1064DVL6A ��Դ���һ����
* 
* RT1064DVL6A ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
* 
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
* 
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
* 
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
* 
* �ļ�����          zf_device_tft180
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 8.32.4 or MDK 5.33
* ����ƽ̨          RT1064DVL6A
* ��������          https://seekfree.taobao.com/
* 
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�             ��Ƭ���ܽ�
*                   SCL                  �鿴 zf_device_tft180.h �� TFT180_SCL_PIN �궨��
*                   SDA                  �鿴 zf_device_tft180.h �� TFT180_SDA_PIN �궨��
*                   RES                  �鿴 zf_device_tft180.h �� TFT180_RES_PIN �궨��
*                   DC                   �鿴 zf_device_tft180.h �� TFT180_DC_PIN �궨��
*                   CS                   �鿴 zf_device_tft180.h �� TFT180_CS_PIN �궨��
*                   BL                   �鿴 zf_device_tft180.h �� TFT180_BL_PIN �궨��
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ���ֱ���160*128
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_tft180_h_
#define _zf_device_tft180_h_

#include "zf_common_typedef.h"

#define TFT180_USE_SOFT_SPI             (0)                                     // Ĭ��ʹ��Ӳ�� SPI ��ʽ���� ����ʹ��Ӳ�� SPI ��ʽ����
#if TFT180_USE_SOFT_SPI                                                         // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================���� SPI ����====================================================
#define TFT180_SOFT_SPI_DELAY           (1 )                                    // ���� SPI ��ʱ����ʱ���� ��ֵԽС SPI ͨ������Խ��
#define TFT180_SCL_PIN                  (B0)                                    // ���� SPI SCK ����
#define TFT180_SDA_PIN                  (B1)                                    // ���� SPI MOSI ����
//====================================================���� SPI ����====================================================
#else
//====================================================Ӳ�� SPI ����====================================================
#define TFT180_SPI_SPEED                (40 * 1000 * 1000)                      // Ӳ�� SPI ����
#define TFT180_SPI                      (SPI_3)                                 // Ӳ�� SPI ��
#define TFT180_SCL_PIN                  (SPI3_SCK_B0 )                          // Ӳ�� SPI SCK ����
#define TFT180_SDA_PIN                  (SPI3_MOSI_B1)                          // Ӳ�� SPI MOSI ����
//====================================================Ӳ�� SPI ����====================================================
#endif

#define TFT180_RES_PIN                  (B2 )                                   // Һ����λ���Ŷ���
#define TFT180_DC_PIN                   (C19)                                   // Һ������λ���Ŷ���
#define TFT180_CS_PIN                   (B3 )                                   // CS Ƭѡ����
#define TFT180_BL_PIN                   (C18)                                   // Һ���������Ŷ���

#define TFT180_DEFAULT_DISPLAY_DIR      (TFT180_PORTAIT)                        // Ĭ�ϵ���ʾ����
#define TFT180_DEFAULT_PENCOLOR         (RGB565_RED)                            // Ĭ�ϵĻ�����ɫ
#define TFT180_DEFAULT_BGCOLOR          (RGB565_WHITE)                          // Ĭ�ϵı�����ɫ
#define TFT180_DEFAULT_DISPLAY_FONT     (TFT180_8X16_FONT)                      // Ĭ�ϵ�����ģʽ

#define TFT180_DC(x)                    ((x) ? (gpio_high(TFT180_DC_PIN)) : (gpio_low(TFT180_DC_PIN)))
#define TFT180_RST(x)                   ((x) ? (gpio_high(TFT180_RES_PIN)) : (gpio_low(TFT180_RES_PIN)))
#define TFT180_CS(x)                    ((x) ? (gpio_high(TFT180_CS_PIN)) : (gpio_low(TFT180_CS_PIN)))
#define TFT180_BLK(x)                   ((x) ? (gpio_high(TFT180_BL_PIN)) : (gpio_low(TFT180_BL_PIN)))

typedef enum
{
    TFT180_PORTAIT                      = 0,                                    // ����ģʽ
    TFT180_PORTAIT_180                  = 1,                                    // ����ģʽ  ��ת180
    TFT180_CROSSWISE                    = 2,                                    // ����ģʽ
    TFT180_CROSSWISE_180                = 3,                                    // ����ģʽ  ��ת180
}tft180_dir_enum;

typedef enum
{
    TFT180_6X8_FONT                     = 0,                                    // 6x8      ����
    TFT180_8X16_FONT                    = 1,                                    // 8x16     ����
    TFT180_16X16_FONT                   = 2,                                    // 16x16    ���� Ŀǰ��֧��
}tft180_font_size_enum;

void    tft180_clear                    (void);
void    tft180_full                     (const uint16 color);
void    tft180_set_dir                  (tft180_dir_enum dir);
void    tft180_set_font                 (tft180_font_size_enum font);
void    tft180_set_color                (const uint16 pen, const  uint16 bgcolor);
void    tft180_draw_point               (uint16 x, uint16 y, const uint16 color);
void    tft180_draw_line                (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);

void    tft180_show_char                (uint16 x, uint16 y, const char dat);
void    tft180_show_string              (uint16 x, uint16 y, const char dat[]);
void    tft180_show_int                 (uint16 x,uint16 y, const int32 dat, uint8 num);
void    tft180_show_uint                (uint16 x,uint16 y, const uint32 dat, uint8 num);
void    tft180_show_float               (uint16 x,uint16 y, const float dat, uint8 num, uint8 pointnum);

void    tft180_show_binary_image        (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void    tft180_show_gray_image          (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold);
void    tft180_show_rgb565_image        (uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode);

void    tft180_show_wave                (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);
void    tft180_show_chinese             (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color);

//-------------------------------------------------------------------------------------------------------------------
// �������     TFT180 ��ʾС���ͼ��
// ����˵��     p               ͼ������
// ����˵��     width           ��ʾ����
// ����˵��     height          ��ʾ�߶�
// ���ز���     void
// ʹ��ʾ��     tft180_displayimage7725(ov7725_image_binary[0], 80, 60);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
#define tft180_displayimage7725(p, width, height)       (tft180_show_binary_image(0, 0, (p), OV7725_W, OV7725_H, (width), (height)))

//-------------------------------------------------------------------------------------------------------------------
// �������     TFT180 ��ʾ�����ͼ�� ������ֵ�� ��ʾ�Ҷ�ͼ��
// ����˵��     p               ͼ������
// ����˵��     width           ��ʾ����
// ����˵��     height          ��ʾ�߶�
// ���ز���     void
// ʹ��ʾ��     tft180_displayimage03x(mt9v03x_image[0], 94, 60);
// ��ע��Ϣ     ���Ҫ��ʾ��ֵ��ͼ���ȥ���� tft180_show_gray_image ����
//-------------------------------------------------------------------------------------------------------------------
#define tft180_displayimage03x(p, width, height)        (tft180_show_gray_image(0, 0, (p), MT9V03X_W, MT9V03X_H, (width), (height), 0))

//-------------------------------------------------------------------------------------------------------------------
// �������     TFT180 ��ʾ��ͫͼ��
// ����˵��     p               ͼ������
// ����˵��     width           ��ʾ����
// ����˵��     height          ��ʾ�߶�
// ���ز���     void
// ʹ��ʾ��     tft180_displayimage8660(scc8660_image[0], 80, 60);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
#define tft180_displayimage8660(p, width, height)       (tft180_show_rgb565_image(0, 0, (p), SCC8660_W, SCC8660_H, (width), (height), 1))

void    tft180_init                     (void);

#endif

