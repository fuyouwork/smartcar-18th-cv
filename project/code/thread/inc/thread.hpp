#ifndef _THREAD_H
#define _THREAD_H

extern "C"
{
#include "zf_common_headfile.h"
#include "user_code.h"
}
#include "user_code_cpp.hpp"
#include "isr.h" // 实际上是C++头文件

#define SERVO_PROTECT 1
#define SIDE_DETECT_BY_OPENART 1
#define SIDE_WITH_AUDIO 0
#define COMMAND_REPEAT 0
#define OMINIDIRECT_MODE 1
#define SIDE_BY_CENTER_MODE 0
#define RECT_BY_ART 1
#define GO_SIDE_WITHOUT_PREPARE 1
#define FLAT_SCAN_COORDINATE 1

extern void task_2ms(void);
extern void task_4ms(void);
extern void task_8ms(void);
extern void task_10ms(void);
extern void task_12ms(void);
extern void task_30ms(void);
extern void task_20ms(void);
extern void task_50ms();
extern void task_300ms(void);
extern void task_100ms(void);

extern bool g_bSendErrorFlag, g_bAudioGoalErrorSetFlag, g_bSendCoordinateFlag, g_bWaitCoordinateGetFlag, g_bRequireCoordinateFlag, g_bRequireCenterFlag, g_bGetSideFlag, g_bGetCategoryFlag, g_bUseDefaultCategoryFLag;
extern bool g_bSetSideByAudioFlag, g_bGoSideByAudioFlag, g_bWaitAudioSignalFlag;
extern uint32 g_dwTimeCountUs;
extern uint8 g_bySendSDFlag;

extern bool g_bSendErrorFlag, g_bGoCenterTaskFlag, g_bAudioGoalErrorSetFlag, g_bRequireCenterFlag, g_bWaitClassifyFlag, g_bGetSideFlag, bPathPlanFlag, g_bWaitCorrectFinishFlag, g_bGetCategoryFlag, g_bUseDefaultCategoryFLag;
extern uint32 g_dwTimeCountUs,g_byWaitAudioSignalTimes;
extern uint8 g_bySendSDFlag, g_byScanLineIndex, g_byPrepareScanIndex;
extern uint8 g_byOpenARTOpenFlag, g_byDetectSideFlag;
extern uint8 g_bySetPointFlag, g_byWaitOpenARTCorrectRectFlag, g_byPrepareSetPointFlag, g_byWaitSubCarFlag;
extern uint8 g_byGetCoordinateFinishFlag, g_byOpenArtSearchFlag;
extern uint8 g_byOpenArtCorrectFinishFlag, g_byOpenArtReadyFlag;
extern uint8 g_byMotorStartFlag;
extern uint8 g_byCollectPictureFlag, g_byDetectRectFlag, g_byWaitSideCorrectFlag;
extern uint8 g_byPlaceCarFlag, g_byRotaryPlatformFlag, g_byClassifyFinishFlag;
extern uint8 g_byOpenArtCorrectFlag, g_byOpenArtClassifyFlag, g_byGoPointFlag;
extern uint32 g_byFindRectTimes, g_byWaitSubCarTimes, g_byWaitSideTimes;
extern uint8 g_byWriteSpeedFlag, g_byDetectRectFinishFlag, g_byGoSideFlag, g_byOpenElectricLockFlag, g_bySetSideFlag, g_byOpenArtCorrectSideFlag;
extern uint8 g_byCoordinateMoveFlag, g_byClassifyResetFlag;
extern uint8 g_byAudioIndex;
extern uint8 g_byGoCarPortFlag;
extern uint8 g_bySideCorrectPositionFlag, g_byScanLineFlag, g_bySetScanLineFlag, g_byOutCarPorFlag;
extern uint8 g_byPointIndex, g_byServoPlatformSpeedMax, g_byBasicError, g_byPrepareScanFlag;
#endif