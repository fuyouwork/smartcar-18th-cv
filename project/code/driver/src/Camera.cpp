#include "Camera.hpp"

extern "C" {
	#include "zf_common_headfile.h"
	#include "zf_device_mt9v03x.h"
}

//需要配置到摄像头的数据
int16 MT9V03X_CFG_CSI[MT9V03X_CONFIG_FINISH][2] = {
    {MT9V03X_AUTO_EXP,
     63},  //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启
           // EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
           //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    {MT9V03X_EXP_TIME, 150},  //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    {MT9V03X_FPS, 1000},  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    {MT9V03X_SET_COL, Camera::W},  //图像列数量        范围1-752     K60采集不允许超过188
    {MT9V03X_SET_ROW, Camera::H},  //图像行数量        范围1-480
    {MT9V03X_LR_OFFSET, 0},        //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移
                     //摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {MT9V03X_UD_OFFSET, 0},  //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移
                     //摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {MT9V03X_GAIN, 32},      //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度

    {MT9V03X_INIT, 0}  //摄像头开始初始化
};

void Camera::set_exposure_time(int16_t value) { MT9V03X_CFG_CSI[1][1] = value; }

void Camera::set_exposure_time_fast(uint16_t value) { ::mt9v03x_set_exposure_time(value); }

Camera camera;
