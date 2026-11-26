#include "flash.hpp"

extern "C"
{
	Flash SFlashBuf = {
		.flash_buffer = flash_union_buffer,
	};
}
void flash_write_param(void)
{
	flash_buffer_clear(); // 请不要随便使用缓存区清除函数
	SFlashBuf.flash_buffer[V].float_type = g_fSpeedExpectValue;
	SFlashBuf.flash_buffer[XOP].float_type = XOuterPID.m_Spid.fKp;
	SFlashBuf.flash_buffer[YOP].float_type = YOuterPID.m_Spid.fKp;
	SFlashBuf.flash_buffer[XOD].float_type = XOuterPID.m_Spid.fKd;
	SFlashBuf.flash_buffer[YOD].float_type = YOuterPID.m_Spid.fKd;
	SFlashBuf.flash_buffer[AP].float_type = AnglePID.m_Spid.fKp;
	SFlashBuf.flash_buffer[AI].float_type = AnglePID.m_Spid.fKi;
	SFlashBuf.flash_buffer[AD].float_type = AnglePID.m_Spid.fKd;
	SFlashBuf.flash_buffer[ARTX_P].float_type = XOpenArtPID.m_Spid.fKp;
	SFlashBuf.flash_buffer[ARTY_P].float_type = YOpenArtPID.m_Spid.fKp;
	SFlashBuf.flash_buffer[FLP].float_type = FLMotorInnerPID.m_Spid.fKp;
	SFlashBuf.flash_buffer[FLI].float_type = FLMotorInnerPID.m_Spid.fKi;
	SFlashBuf.flash_buffer[FRP].float_type = FRMotorInnerPID.m_Spid.fKp;
	SFlashBuf.flash_buffer[FRI].float_type = FRMotorInnerPID.m_Spid.fKi;
	SFlashBuf.flash_buffer[BLP].float_type = BLMotorInnerPID.m_Spid.fKp;
	SFlashBuf.flash_buffer[BLI].float_type = BLMotorInnerPID.m_Spid.fKi;
	SFlashBuf.flash_buffer[BRP].float_type = BRMotorInnerPID.m_Spid.fKp;
	SFlashBuf.flash_buffer[BRI].float_type = BRMotorInnerPID.m_Spid.fKi;
	SFlashBuf.flash_buffer[XCO].float_type = g_fXPositionCoeffient;
	SFlashBuf.flash_buffer[YCO].float_type = g_fYPositionCoeffient;
	SFlashBuf.flash_buffer[X].float_type = g_fXExpectPosition;
	SFlashBuf.flash_buffer[Y].float_type = g_fYExpectPosition;
	SFlashBuf.flash_buffer[DATA_INDEX].int32_type = iCsvBuffer;
	SFlashBuf.flash_buffer[SERVO_10K_ANGLE].float_type = Servo10K.fAngleExpect;
	SFlashBuf.flash_buffer[SERVO_20K_ANGLE].float_type = Servo20K.fAngleExpect;
	SFlashBuf.flash_buffer[SERVO_35K_ANGLE].float_type = Servo35K.fAngleExpect;
	SFlashBuf.flash_buffer[LIGHT].float_type = g_lBrightnessLevel;
	flash_write_page_from_buffer(127, FLASH_PAGE_0); // 写入所有的参数
}
char g_cFlashBufName[FLASH_SAVE_NUM][NAME_COUNT];
float_t g_fChangeFactor[FLASH_SAVE_NUM];
void describe_flash_buff(void)
{
	strcpy(g_cFlashBufName[XOP], "XOP:");
	g_fChangeFactor[XOP] = 0.1;
	strcpy(g_cFlashBufName[YOP], "YOP:");
	g_fChangeFactor[YOP] = 0.1;
	strcpy(g_cFlashBufName[XOD], "XOD:");
	g_fChangeFactor[XOD] = 1;
	strcpy(g_cFlashBufName[YOD], "YOD:");
	g_fChangeFactor[YOD] = 1;
	strcpy(g_cFlashBufName[ARTX_P], "ARX:");
	g_fChangeFactor[ARTX_P] = 0.1;
	strcpy(g_cFlashBufName[ARTY_P], "ARY:");
	g_fChangeFactor[ARTY_P] = 0.1;
	strcpy(g_cFlashBufName[AP], "AP:");
	g_fChangeFactor[AP] = 0.1;
	strcpy(g_cFlashBufName[AI], "AI: ");
	g_fChangeFactor[AI] = 0.01;
	strcpy(g_cFlashBufName[AD], "AD:");
	g_fChangeFactor[AD] = 1;
	strcpy(g_cFlashBufName[FLP], "FLP:");
	g_fChangeFactor[FLP] = 0.25;
	strcpy(g_cFlashBufName[FLI], "FLI:");
	g_fChangeFactor[FLI] = 0.1;
	strcpy(g_cFlashBufName[FRP], "FRP:");
	g_fChangeFactor[FRP] = 0.25;
	strcpy(g_cFlashBufName[FRI], "FRI:");
	g_fChangeFactor[FRI] = 0.1;
	strcpy(g_cFlashBufName[BLP], "BLP:");
	g_fChangeFactor[BLP] = 0.25;
	strcpy(g_cFlashBufName[BLI], "BLI:");
	g_fChangeFactor[BLI] = 0.1;
	strcpy(g_cFlashBufName[BRP], "BRP:");
	g_fChangeFactor[BRP] = 0.25;
	strcpy(g_cFlashBufName[BRI], "BRI:");
	g_fChangeFactor[BRI] = 0.1;
	strcpy(g_cFlashBufName[XCO], "XCO:");
	g_fChangeFactor[XCO] = 0.01;
	strcpy(g_cFlashBufName[YCO], "YCO:");
	g_fChangeFactor[YCO] = 0.01;
	strcpy(g_cFlashBufName[X], "X:  ");
	g_fChangeFactor[X] = 100;
	strcpy(g_cFlashBufName[Y], "Y:  ");
	g_fChangeFactor[Y] = 100;
	strcpy(g_cFlashBufName[DATA_INDEX], "IND:");
	g_fChangeFactor[DATA_INDEX] = 1;
	strcpy(g_cFlashBufName[SERVO_10K_ANGLE], "10K:");
	g_fChangeFactor[SERVO_10K_ANGLE] = 1;
	strcpy(g_cFlashBufName[SERVO_20K_ANGLE], "20K:");
	g_fChangeFactor[SERVO_20K_ANGLE] = 1;
	strcpy(g_cFlashBufName[SERVO_35K_ANGLE], "35K:");
	g_fChangeFactor[SERVO_35K_ANGLE] = 1;
	strcpy(g_cFlashBufName[V], "v:");
	g_fChangeFactor[V] = 5;
	strcpy(g_cFlashBufName[LIGHT], "lig:");
	g_fChangeFactor[LIGHT] = 1;
}
void read_param_from_buffer(void)
{
	flash_read_page_to_buffer(127, FLASH_PAGE_0); // 从扇区中读数据出来
	XOuterPID.pid_set_param_from_buffer(SFlashBuf.flash_buffer[XOP].float_type, 0, 100);//SFlashBuf.flash_buffer[XOD].float_type);
	YOuterPID.pid_set_param_from_buffer(SFlashBuf.flash_buffer[YOP].float_type, 0, 100);//SFlashBuf.flash_buffer[YOD].float_type);
	AnglePID.pid_set_param_from_buffer(SFlashBuf.flash_buffer[AP].float_type, SFlashBuf.flash_buffer[AI].float_type, SFlashBuf.flash_buffer[AD].float_type);
	XOpenArtPID.pid_set_param_from_buffer(SFlashBuf.flash_buffer[ARTX_P].float_type);
	YOpenArtPID.pid_set_param_from_buffer(SFlashBuf.flash_buffer[ARTY_P].float_type);

	FLMotorInnerPID.pid_set_param_from_buffer(SFlashBuf.flash_buffer[FLP].float_type, SFlashBuf.flash_buffer[FLI].float_type);
	FRMotorInnerPID.pid_set_param_from_buffer(SFlashBuf.flash_buffer[FLP].float_type, SFlashBuf.flash_buffer[FLI].float_type);
	BLMotorInnerPID.pid_set_param_from_buffer(SFlashBuf.flash_buffer[FLP].float_type, SFlashBuf.flash_buffer[FLI].float_type);
	BRMotorInnerPID.pid_set_param_from_buffer(SFlashBuf.flash_buffer[FLP].float_type, SFlashBuf.flash_buffer[FLI].float_type);

	g_fXPositionCoeffient = SFlashBuf.flash_buffer[XCO].float_type;
	g_fYPositionCoeffient = SFlashBuf.flash_buffer[YCO].float_type;
	g_fXExpectPosition = SFlashBuf.flash_buffer[X].float_type;
	g_fYExpectPosition = SFlashBuf.flash_buffer[Y].float_type;
	Servo10K.fAngleExpect = SFlashBuf.flash_buffer[SERVO_10K_ANGLE].float_type;
	Servo20K.fAngleExpect = SFlashBuf.flash_buffer[SERVO_20K_ANGLE].float_type;
	Servo35K.fAngleExpect = SFlashBuf.flash_buffer[SERVO_35K_ANGLE].float_type;
	iCsvBuffer = SFlashBuf.flash_buffer[DATA_INDEX].int32_type;
	g_fSpeedExpectValue = SFlashBuf.flash_buffer[V].float_type;
	g_lBrightnessLevel = SFlashBuf.flash_buffer[LIGHT].float_type;
}