#include "edge_detect/canny.hpp"

#ifndef M_PI
#define M_PI 3.14159265f
#endif

namespace imgProc
{
    namespace edge_detect
    {
        using namespace apriltag;
        uint8_t src[MT9V03X_H * MT9V03X_W];
        gvec_t *canny(uint8_t *img, int low_thresh, int high_thresh)
        {
            for (uint16_t y = 0; y < MT9V03X_H; y++)
            {
                for (uint16_t x = 0; x < MT9V03X_W; x++)
                {
                    uint16_t i = y * MT9V03X_W + x;
                    src[i] = img[i];
                }
            }
            if (low_thresh > high_thresh)
                std::swap(low_thresh, high_thresh);

            // 1. Noise Reduction with a Gaussian filter
            sepconv3(src, kernel_gauss_3, 1.0f / 16.0f, 0.0f); // 3*3高斯滤波

            // 2. Finding Image Gradients
            gvec_t *gm = (gvec_t *)staticBuffer.allocate(N * M * sizeof(*gm));
            // 2. Finding Image Gradients
            // 采取非插值的方法进行Sobel算子计算，不对MT9V03视野边界像素点进行Sobel运算，因为Sobel算子是3*3起步的
            int vx = 0, vy = 0;
            for (int y = 1; y < N - 1; y++)
            {
                for (int x = 1; x < M - 1; x++)
                {
                    // sobel kernel in the horizontal direction，计算Y轴梯度
                    // TODO 梯度计算方式
                    vx = (int)src[(y - 1) * M + x - 1] - (int)src[(y - 1) * M + x + 1] + ((int)src[(y + 0) * M + x - 1] << 1) -
                         ((int)src[(y + 0) * M + x + 1] << 1) + (int)src[(y + 1) * M + x - 1] - (int)src[(y + 1) * M + x + 1];

                    // sobel kernel in the vertical direction，计算X轴梯度
                    vy = (int)src[(y - 1) * M + x - 1] + ((int)src[(y - 1) * M + x + 0] << 1) + (int)src[(y - 1) * M + x + 1] -
                         (int)src[(y + 1) * M + x - 1] - ((int)src[(y + 1) * M + x + 0] << 1) - (int)src[(y + 1) * M + x + 1];

                    // Find magnitude，计算梯度值
                    int g = (int)sqrtf(vx * vx + vy * vy);
                    // Find the direction and round angle to 0, 45, 90 or 135，计算梯度方向
                    // atan2 返回该点与X轴的夹角(弧度制),[-PI,PI],长度较小
                    // atan2 X负半轴是PI,X正半轴是0
                    // 第一象限(0,PI/2),第二象限(PI/2,PI),第三象限(-PI,-PI/2),第四象限(-PI/2,0)
                    float theta = atan2f(vy, vx);
                    // 假设有(0,0)点，与他相邻的点只有(1,0),(1,1),(0,1),(-1,1),(-1,0),(-1,-1),(0,-1),(1,-1)
                    // 即											angle 0,		PI/4,	PI/2,  3*PI/2,	PI, -3*PI/2,-PI/2, -PI/4
                    // 由于非极大值抑制实际上关注梯度方向上的“直线”而不是“射线”，即垂直，水平和两个对角线，所以可以把负方向零点对称至正方向
                    if (theta < 0)
                        theta += M_PI;
                    int t = (int)(theta * (180.0f / M_PI));

                    if (t < 22)
                    { // 45/2
                        t = 0;
                    }
                    else if (t < 67)
                    { // 45+(90-45)/2
                        t = 45;
                    }
                    else if (t < 112)
                    {
                        t = 90;
                    }
                    else if (t < 160)
                    {
                        t = 135;
                    }
                    else
                    {
                        t = 0;
                    }

                    gm[y * M + x].t = t; // 载入梯度方向(已经做过零点对称，全部对称至正度数，单位度)
                    gm[y * M + x].g = g; // 载入梯度值
                }
            }

            // 3. Non-maximum Suppression 非极大值抑制，双阈值边缘检测
            CoordStack stack;
            gvec_t *vc = &gm[0 * M + 0];
            int i;
            for (int y = 0; y < N; y++)
            {
                for (int x = 0; x < M; x++)
                {
                    i = y * M + x; // 定位至该坐标的数组位置，行优先存储
                    vc = &gm[i];
                    // Clear the borders，默认MT9V03视野边界像素点，默认他们是无边缘，因为他们是没有经过sobel运算的
                    if (y == (0) || y == (N - 1) || x == (0) || x == (M - 1))
                    {
                        src[i] = 0;
                        continue;
                    }

                    if (vc->g < low_thresh)
                    {
                        // Not an edge，无边缘
                        src[i] = 0;
                        continue;
                        // Strong edge，强边缘
                    }
                    else if (vc->g >= high_thresh)
                    {
                        src[i] = 255; // 强边缘进一步判断是否是极大值，再确定是否保留为边缘
                    }
                    else
                    { // Weak edge弱边缘
                        src[i] = 1;
                        continue;
                    }
// 非极大值抑制，如果该像素在其梯度方向上比他前后该像素点梯度值大时，该点为保留为边缘，否则该点将被抑制
// 每个强边缘像素点都会进行一次非极大值抑制，且该操作不会影响其他点非极大值抑制操作，应为该操作的结果是单独入栈
#define CANNY_CHECK_   \
    if (src[i] == 255) \
        ;              \
    else               \
        src[i] = 0; // 抑制该像素点
#define CANNY_CHECK1(dx, dy)                                                             \
    if (vc->g > gm[(y - dy) * M + (x - dx)].g && vc->g >= gm[(y + dy) * M + (x + dx)].g) \
    {                                                                                    \
        CANNY_CHECK_                                                                     \
    } // 该边缘在其梯度方向上比前后梯度大时，即为极大值时
#define CANNY_CHECK2(dx, dy)                                                            \
    if (vc->g > gm[(y - dy) * M + (x - dx)].g && vc->g > gm[(y + dy) * M + (x + dx)].g) \
    {                                                                                   \
        CANNY_CHECK_                                                                    \
    }

                    switch (vc->t)
                    {
                    case 0:
                        CANNY_CHECK1(1, 0);
                        break;
                    case 45:
                        CANNY_CHECK2(1, 1);
                        break;
                    case 90:
                        CANNY_CHECK1(0, 1);
                        break;
                    default:
                        CANNY_CHECK2(-1, 1);
                        break; // 其实就是135
                    }
                }
            }
            // 孤立边缘抑制
            // 将离散的强边缘点与其周边的弱边缘点连接起来
            while (!stack.empty())
            {
                Coordinate xy = stack.pop(); // 强边缘线的坐标就已经出来了
                int x = xy.x, y = xy.y;
                uint8_t *m = src + (y * M + x); // 确定边缘点在图像数组中的位置
#define CANNY_PUSH(dx, dy)           \
    if (*(m + ((dy)*M + (dx))) == 1) \
    *(m + ((dy)*M + (dx))) = 255
                if (y > 0)
                {
                    if (x > 0)
                        CANNY_PUSH(-1, -1);
                    CANNY_PUSH(0, -1);
                    if (x < M - 1)
                        CANNY_PUSH(1, -1);
                }
                if (x > 0)
                    CANNY_PUSH(-1, 0);
                if (x < M - 1)
                    CANNY_PUSH(1, 0);
                if (y < N - 1)
                {
                    if (x > 0)
                        CANNY_PUSH(-1, 1);
                    CANNY_PUSH(0, 1);
                    if (x < M - 1)
                        CANNY_PUSH(1, 1);
                }
            }
#undef CANNY_PUSH // 取消CANNY_PUSH的宏定义

            return gm;
        }

    } // namespace edge_detect
} // namespace imgProc