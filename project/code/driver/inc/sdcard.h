#ifndef _SDCARD_h
#define _SDCARD_h

#include "zf_common_headfile.h"
#define BUFFER_SIZE (512U)
#ifdef __cplusplus
extern "C"
{
#endif
extern FATFS g_fileSystem; // 文件系统结构体变量
extern FIL g_fileObject;   // 文件结构体变量
extern char cWriteBuffer[512];
extern char cDirBuffer[512];
extern int iCsvBuffer;

extern FRESULT error;
extern DIR directory;                  // 目录结构体变量
extern FILINFO fileInformation;
extern UINT bytesWritten;
extern UINT bytesRead;
extern const TCHAR driverNumberBuffer[3U];
extern volatile bool bFailedFlag;
extern char ch;
extern BYTE work[FF_MAX_SS];

extern int sdcard_init(void);
extern int sdcard_init_excel(void);
extern int sdcard_write(float *cFloatBuffer, int iLength);
extern int sdcard_write_excel(float *cFloatBuffer, int iLength, uint8 byWriteFileFlag);
#ifdef __cplusplus
}
#endif

#endif