#include "sdcard.h"

FATFS g_fileSystem; // 文件系统结构体变量
FIL g_fileObject;   // 文件结构体变量
char cWriteBuffer[512];
char cDirBuffer[512];
int iCsvBuffer = 0;

FRESULT error;
DIR directory; // 目录结构体变量
FILINFO fileInformation;
UINT bytesWritten;
UINT bytesRead;
const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
volatile bool bFailedFlag = false;
char ch = '0';
BYTE work[FF_MAX_SS];

int sdcard_init(void)
{
    sdio_init(); // 初始化SDIO接口并等待SD卡插入
    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        // 挂载失败//
        return -1;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        // 改变驱动器失败//
        return -1;
    }
#endif

#if 0
    //创建文件系统中。。。这可能需要一个比较长的时间//
    // 当SD卡文件系统创建完毕之后，就不需要在每次都创建了
    // 因为每次创建都是会把SD卡格式化一遍的，每次都格式化容易导致卡报废
    if (f_mkfs(driverNumberBuffer, 0U, work, sizeof(work)))
    {

        //创建文件系统失败//
        return -1;
    }

#endif /* FF_USE_MKFS */

    // 创建目录中//
    error = f_mkdir(_T("/data"));
    if (error)
    {
        if (error == FR_EXIST)
        {
        }
        else
        {
            // 目录创建失败//
            return -1;
        }
    }
    // 创建一个文件在当前目录下//
    sprintf(cDirBuffer, "/data/%d.csv", iCsvBuffer++);
    error = f_open(&g_fileObject, cDirBuffer, (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
        }
        else
        {
            // 创建文件失败//
            return -1;
        }
    }
    f_lseek(&g_fileObject, 0U);
    // 写入数据到上面创建的文件//
    strcpy(cWriteBuffer, "I0,I1,I2,I3,I4\r\n");
    error = f_write(&g_fileObject, cWriteBuffer, strlen(cWriteBuffer), &bytesWritten);
    if ((error) || (bytesWritten != strlen(cWriteBuffer)))
    {
        // 写入文件失败//
        bFailedFlag = true;
        return -1;
    }

    return 0;
}
int sdcard_init_excel(void)
{
    sdio_init(); // 初始化SDIO接口并等待SD卡插入
    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        // 挂载失败//
        return -1;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        // 改变驱动器失败//
        return -1;
    }
#endif

#if 0
    //创建文件系统中。。。这可能需要一个比较长的时间//
    // 当SD卡文件系统创建完毕之后，就不需要在每次都创建了
    // 因为每次创建都是会把SD卡格式化一遍的，每次都格式化容易导致卡报废
    if (f_mkfs(driverNumberBuffer, 0U, work, sizeof(work)))
    {

        //创建文件系统失败//
        return -1;
    }

#endif /* FF_USE_MKFS */

    // 创建目录中//
    error = f_mkdir(_T("/data"));
    if (error)
    {
        if (error == FR_EXIST)
        {
        }
        else
        {
            // 目录创建失败//
            return -1;
        }
    }
    // 创建一个文件在当前目录下//
    sprintf(cDirBuffer, "/data/6.txt");
    if (f_open(&g_fileObject, cDirBuffer, (FA_WRITE | FA_READ | FA_OPEN_EXISTING)) == FR_OK)    // 尝试打开已存在的文件
    {
        f_lseek(&g_fileObject, f_size(&g_fileObject));
    }
    else
    {
        error = f_open(&g_fileObject, cDirBuffer, (FA_WRITE | FA_READ | FA_OPEN_ALWAYS));       // 尝试创建一个新的文件
        if (error)
        {
            // 创建文件失败//
            return -1;
        }
        else
        {
            f_lseek(&g_fileObject, 0U);
            // 写入数据到上面创建的文件//
            strcpy(cWriteBuffer, "Vx,Vy,Vx1,Vy1\r\n");
            error = f_write(&g_fileObject, cWriteBuffer, strlen(cWriteBuffer), &bytesWritten);
            if ((error) || (bytesWritten != strlen(cWriteBuffer)))
            {
                // 写入文件失败//
                bFailedFlag = true;
                return -1;
            }
        }
    }
    if (!f_close(&g_fileObject))
    {
        if (f_open(&g_fileObject, cDirBuffer, (FA_WRITE | FA_READ | FA_OPEN_EXISTING)) == FR_OK)
        {
            f_lseek(&g_fileObject, f_size(&g_fileObject));
        }
    }
    return 0;
}
/// @brief sdcard写入excel文件格式数据到记事本
/// @param cFloatBuffer 要写入的浮点数据
/// @param iLength 浮点数据的长度
/// @param byWriteFileFlag 是否保存
/// @return 成功return0
int sdcard_write_excel(float *cFloatBuffer, int iLength, uint8 byWriteFileFlag)
{
    int16 i;
    char wStringTemp[50];
    sprintf(cWriteBuffer, "");
    for (i = 0; i < iLength; i++)
    {
        if (i != iLength - 1)
        {
            sprintf(wStringTemp, "%f,", cFloatBuffer[i]);
        }
        else
        {
            sprintf(wStringTemp, "%f\r\n", cFloatBuffer[i]);
        }
        strcat(cWriteBuffer, wStringTemp);
    }
    if (iLength != 0)
    {
        error = f_write(&g_fileObject, cWriteBuffer, strlen(cWriteBuffer), &bytesWritten);
        if ((error) || (bytesWritten != strlen(cWriteBuffer)))
        {
            // 写入文件失败//
            bFailedFlag = true;
            return -1;
        }
    }
    if (byWriteFileFlag == 1)
    {
        if (f_close(&g_fileObject))
        {
            return -1;
        }
    }
    return 0;
}

int sdcard_write(float *cFloatBuffer, int iLength)
{
    if (iLength == 5)
    {
        sprintf(cWriteBuffer, "%f,%f,%f,%f,%f\r\n", cFloatBuffer[0], cFloatBuffer[1], cFloatBuffer[2], cFloatBuffer[3], cFloatBuffer[4]);
        error = f_write(&g_fileObject, cWriteBuffer, strlen(cWriteBuffer), &bytesWritten);
        if ((error) || (bytesWritten != strlen(cWriteBuffer)))
        {
            // 写入文件失败//
            bFailedFlag = true;
            return -1;
        }
    }
    else if (iLength == 17)
    {
        sprintf(cWriteBuffer, "%f, %f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", cFloatBuffer[0], cFloatBuffer[1], cFloatBuffer[2], cFloatBuffer[3], cFloatBuffer[4], cFloatBuffer[5], cFloatBuffer[6], cFloatBuffer[7], cFloatBuffer[8], cFloatBuffer[9], cFloatBuffer[10], cFloatBuffer[11], cFloatBuffer[12], cFloatBuffer[13], cFloatBuffer[14], cFloatBuffer[15], cFloatBuffer[16]);
        error = f_write(&g_fileObject, cWriteBuffer, strlen(cWriteBuffer), &bytesWritten);
        if ((error) || (bytesWritten != strlen(cWriteBuffer)))
        {
            // 写入文件失败//
            bFailedFlag = true;
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}
