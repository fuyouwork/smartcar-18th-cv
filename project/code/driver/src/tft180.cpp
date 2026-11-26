#include "tft180.hpp"

uint8 g_byMenuSwitch = COMMON_MENU;
uint8 g_byOpenArtShowFlag = 0;
uint8 g_byLastMenuSwitch = COMMON_MENU;
extern uint8 g_fCategory;

/**
 *@brief 通用菜单显示函数
 *@parame 无
 *@retval 无
 */
void tft180_show_menu(void)
{
	char byTempString[50];
	tft180_set_color(RGB565_YELLOW, RGB565_BLACK); // 画笔颜色白色，背景色黑色
	// 第一行
	tft180_show_string(0, 0, "Flying Fish Team"); // 一个字母占水平8个像素点，竖直16个像素点
	// 第二行
	tft180_show_string(0, 16, "yaw:");
	tft180_show_float(48, 16, s_ICMresult.fYawAngle, 2, 4);
	// 第三行
	tft180_show_string(0, 32, "Absolute_X:");
	tft180_show_float(96, 32, g_fXPositionRealAbsoluteCoordinate, 4, 2);
	// 第四行
	tft180_show_string(0, 48, "Absolute_Y:");
	tft180_show_float(96, 48, g_fYPositionRealAbsoluteCoordinate, 4, 2);
	// 第六行
	sprintf(byTempString, "category:%2d", g_byCategoryIndex);
	tft180_show_string(0, 80, byTempString);

	// 第八行
	sprintf(byTempString, "Main:%d  Sub:%2d", g_CategoryMap[g_byCategoryIndex]->MainCategoryCode, g_CategoryMap[g_byCategoryIndex]->SubCategoryCode);
	tft180_show_string(0, 112, byTempString);
}
void tft180_show_speed(void)
{
	char byTempString[6][50];
	tft180_set_color(RGB565_YELLOW, RGB565_BLACK); // 画笔颜色白色，背景色黑色
	// 第一行
	tft180_show_string(0, 0, "Absolute_X:");
	tft180_show_float(96, 0, g_fXPositionRealAbsoluteCoordinate, 4, 2);
	// 第二行
	tft180_show_string(0, 16, "Absolute_Y:");
	tft180_show_float(96, 16, g_fYPositionRealAbsoluteCoordinate, 4, 2);
	// 第三行
	sprintf(byTempString[0], "Vx: %.0f", g_fXPositionRealInertialSpeed);
	tft180_show_string(0, 32, byTempString[0]);
	// 第四行
	sprintf(byTempString[1], "Vy: %.0f", g_fYPositionRealInertialSpeed);
	tft180_show_string(0, 48, byTempString[1]);
	// 第五行
	sprintf(byTempString[2], "Vx1: %.0f", g_fXPositionAbsoluteSpeed);
	tft180_show_string(0, 64, byTempString[2]);
	// 第六行
	sprintf(byTempString[3], "Vy1: %.0f", g_fYPositionAbsoluteSpeed);
	tft180_show_string(0, 80, byTempString[3]);
	// 第七行
	sprintf(byTempString[4], "write_finish");
	tft180_show_string(0, 96, byTempString[4]);
}
uint8 g_byShowLines;
/**
 *@brief 坐标显示函数
 *@parame 无
 *@retval 无
 */
void tft180_show_coordinate(void)
{
	uint8 i, j;
	char byTempString[50];

	g_byShowLines = 30;

	if (g_byPoint >= PARAM_LINES) // 如果指向的数据已经超过了显示的行数
	{
		g_byStart = g_byPoint - (PARAM_LINES - 1); // 数据出发下标下移超出量
	}
	else
	{
		g_byStart = 0; // 默认没有超出量
	}
	for (i = 0; i < PARAM_LINES; i++) // 刷新会有光标显示的所有行

	{
		if (i == g_byShowPoint)
		{
			tft180_show_string(144, i * 16, "<-"); // 指明现在在修改什么数据
		}
		else
			tft180_show_string(144, i * 16, "  ");
	}
	for (i = 0; i < PARAM_LINES; i++) // 刷新显示行现在的数据
	{
		//		sprintf(byTempString, "(%2d,%2d)",points[tsp.hamilton_path[i + g_byStart]][0], points[tsp.hamilton_path[i + g_byStart]][1]);
		sprintf(byTempString, "(%3.0f,%3.0f)", g_fPointPositionX[tsp.hamilton_path[i + g_byStart]], g_fPointPositionY[tsp.hamilton_path[i + g_byStart]]);
		tft180_show_string(0, i * 16, byTempString);
	}
}
void tft180_show_raw_coordinate(void)
{
	uint8 i, j;
	char byTempString[50];

	g_byShowLines = 30;

	if (g_byPoint >= PARAM_LINES) // 如果指向的数据已经超过了显示的行数
	{
		g_byStart = g_byPoint - (PARAM_LINES - 1); // 数据出发下标下移超出量
	}
	else
	{
		g_byStart = 0; // 默认没有超出量
	}
	for (i = 0; i < PARAM_LINES; i++) // 刷新会有光标显示的所有行

	{
		if (i == g_byShowPoint)
		{
			tft180_show_string(144, i * 16, "<-"); // 指明现在在修改什么数据
		}
		else
			tft180_show_string(144, i * 16, "  ");
	}
	for (i = 0; i < PARAM_LINES; i++) // 刷新显示行现在的数据
	{
		//		sprintf(byTempString, "(%2d,%2d)",points[tsp.hamilton_path[i + g_byStart]][0], points[tsp.hamilton_path[i + g_byStart]][1]);
		sprintf(byTempString, "(%3.0f,%3.0f)", g_fPointPositionX[i + g_byStart], g_fPointPositionY[i + g_byStart]);
		tft180_show_string(0, i * 16, byTempString);
	}
}
/**
 *@brief 参数显示函数
 *@parame 无
 *@retval 无
 */
uint8 g_byPoint = 0;	 // 数据修改位下标, 0到FLASH_SAVE_NUM-1
uint8 g_byShowPoint = 0; // 光标指向位行数, 0到PARAM_LINES-1
uint8 g_byStart = 0;
void tft180_show_param(void)
{
	uint8 i, j;
	g_byShowLines = FLASH_SAVE_NUM;
	if (g_byPoint >= PARAM_LINES) // 如果指向的数据已经超过了显示的行数
	{
		g_byStart = g_byPoint - (PARAM_LINES - 1); // 数据出发下标下移超出量
	}
	else
	{
		g_byStart = 0; // 默认没有超出量
	}
	for (i = 0; i < PARAM_LINES; i++) // 刷新会有光标显示的所有行

	{
		if (i == g_byShowPoint)
		{
			tft180_show_string(144, i * 16, "<-"); // 指明现在在修改什么数据
		}
		else
			tft180_show_string(144, i * 16, "  ");
	}
	for (i = 0; i < PARAM_LINES && i < FLASH_SAVE_NUM; i++) // 刷新显示行现在的数据
	{
		tft180_show_string(0, i * 16, g_cFlashBufName[i + g_byStart]);
		tft180_show_float(80, i * 16, SFlashBuf.flash_buffer[i + g_byStart].float_type, 3, 3);
	}
}
void tft_show_encoder_count(void)
{
	char byTempString[50];
	sprintf(byTempString, "FL %15d", g_SEncoder.SEncoderFL.sCountSum);
	tft180_show_string(0, 0, byTempString);
	sprintf(byTempString, "FR %15d", g_SEncoder.SEncoderFR.sCountSum);
	tft180_show_string(0, 16, byTempString);
	sprintf(byTempString, "BL %15d", g_SEncoder.SEncoderBL.sCountSum);
	tft180_show_string(0, 32, byTempString);
	sprintf(byTempString, "BR %15d", g_SEncoder.SEncoderBR.sCountSum);
	tft180_show_string(0, 48, byTempString);
}
/**
 *@brief 菜单显示程序，所有菜单都会经过这个函数显示，长按KEY1可切换菜单
 *@parame 无
 *@retval 无
 */
void menu_show(void)
{
	if (g_bWriteTestFlag == 1)
	{
		flash_write_param();
		g_bWriteTestFlag = 0;
	}
	if (g_byLastMenuSwitch != g_byMenuSwitch)
	{
		tft180_clear();
		g_byShowPoint = 0;
		g_byPoint = 0;
	}
	g_byLastMenuSwitch = g_byMenuSwitch;
	switch (g_byMenuSwitch)
	{
	case COMMON_MENU:
		tft180_show_menu();
		break;

	case PARAM_MENU:
		tft180_show_param();
		if (g_bWriteFlag)
		{
			tft180_show_string(1, 7 * 16, "Wait write");
			while (flash_write_page_from_buffer(127, FLASH_PAGE_0))
			{
				tft180_show_string(1, 7 * 16, "Write fail ");
			}
			read_param_from_buffer(); // 从扇区中读数据出来
			tft180_show_string(1, 7 * 16, "Write OK  ");
			g_bWriteFlag = 0;
		}
		break;

	case COORDINATE_MENU:
		tft180_show_coordinate();
		break;
	case SPEED_MENU:
		tft180_show_speed();
		break;
	case RAW_COORDINATE_MENU:
		tft180_show_raw_coordinate();
		break;
	case ENCODER_MENU:
		tft_show_encoder_count();
	default:
		break;
	}
}
void test_coordinate_menu()
{
	tft180_show_int(0, 16, iCsvBuffer, 2); // 指明现在在修改什么数据
}