// #include "edge_detect/left_hand_maze.hpp"

// namespace imgProc
// {
//     namespace edge_detect
//     {

//         forward_list ForwardStaticBuffer[4] = {
//             {NULL, UP_OFFSET, NULL},
//             {NULL, DOWN_OFFSET, NULL},
//             {NULL, LEFT_OFFSET, NULL},
//             {NULL, RIGHT_OFFSET, NULL}};
//         forward_list *Head = ForwardStaticBuffer;
//         forward_list *Back = &ForwardStaticBuffer[1];
//         forward_list *Left = &ForwardStaticBuffer[2];
//         forward_list *Right = &ForwardStaticBuffer[3];
//         forward_list *Temp = Head;

//         uint16_t g_wFindCount[MT9V03X_H * MT9V03X_W];
//         uint16_t RoadPath[MT9V03X_H * MT9V03X_W];
//         uint16_t g_byStaticSpace[MT9V03X_H * MT9V03X_W]; // 最多也就1/3会是连通
//         // Coordinate g_CenterPoint[MT9V03X_H * MT9V03X_W];
//         Coordinate g_CenterPoint;
//         Coordinate g_MaxCenterPoint;
//         uint32_t PointDistanceMax;
//         Coordinate g_Point[MT9V03X_H * MT9V03X_W];
//         uint8_t g_MaxImage[MT9V03X_H * MT9V03X_W];
//         void detect_angular_point(uint8_t *src)
//         {

//             uint16_t RoadIndex = 0;
//             uint16_t temp = 0;
//             uint16_t PixelLowThresh = 500; // 最少像素点设置

//             RoadPathStack_t RoadPathStackLeft;
//             Head->p_iLast = Left;
//             Left->p_iLast = Back;
//             Back->p_iLast = Right;
//             Right->p_iLast = Head;

//             Head->p_iNext = Right;
//             Right->p_iNext = Back;
//             Back->p_iNext = Left;
//             Left->p_iNext = Head;

//             for (uint16_t y = 0; y < MT9V03X_H; y++)
//             {
//                 for (uint16_t x = 0; x < MT9V03X_W; x++)
//                 {
//                     uint16_t i = y * MT9V03X_W + x;
//                     RoadPath[i] = WALL;
//                     g_wFindCount[i] = 0;
//                     g_byStaticSpace[i] = 0;
//                     // g_CenterPoint[i].x = 0;
//                     // g_CenterPoint[i].y = 0;

//                     g_MaxCenterPoint.x = 0;
//                     g_MaxCenterPoint.y = 0;
//                     PointDistanceMax = 0;
//                 }
//             }
//             for (uint16_t y = 0; y < MT9V03X_H; y++)
//             {
//                 for (uint16_t x = 0; x < MT9V03X_W; x++)
//                 {
//                     uint16_t i = y * MT9V03X_W + x;
//                     g_CenterPoint.x = 0;
//                     g_CenterPoint.y = 0;

//                     if (y == (0) || y == (MT9V03X_H - 1) || x == (0) || x == (MT9V03X_W - 1)) // 将视野边缘视为墙
//                     {
//                         continue;
//                     }

//                     if (src[i] == WALL) // 起点必须是强边缘点
//                     {
//                         continue;
//                     }
//                     if (!set_forward(&src[i])) // 起点能保证前方有路，左手扶墙
//                     {

//                         continue;
//                     }
//                     if (RoadPath[i] != WALL) // 这个可以保证只遍历一次该数组,不重复走已经走过的路
//                         continue;
//                     temp = i;    // 从当前点开始寻路
//                     RoadIndex++; // 路线索引增加
//                     uint8_t RoadIndexTemp = RoadIndex;
//                     uint16_t Counts = 0;
//                     do
//                     {
//                         if (src[temp + Left->iOffset] == ROAD) // 若左手无墙则往左走
//                         {
//                             RoadPath[temp] = RoadIndex;
//                             g_byStaticSpace[RoadIndex] += 1;
//                             // g_CenterPoint[RoadIndex].x += (uint16_t)temp % (uint16_t)MT9V03X_W;
//                             // g_CenterPoint[RoadIndex].y += (uint16_t)temp / (uint16_t)MT9V03X_W;
//                             g_Point[Counts].x = (uint16_t)temp % (uint16_t)MT9V03X_W;
//                             g_Point[Counts].y = (uint16_t)temp / (uint16_t)MT9V03X_W;
//                             g_CenterPoint.x += g_Point[Counts].x;
//                             g_CenterPoint.y += g_Point[Counts].y;
//                             Counts++;
//                             temp = temp + Left->iOffset;
//                             set_forward2(&src[temp]);
//                         }
//                         else if (src[temp + Head->iOffset] == ROAD) // 若左手扶墙，前方无墙，则往前走
//                         {
//                             RoadPath[temp] = RoadIndex;
//                             g_byStaticSpace[RoadIndex] = 1;
//                             // g_CenterPoint[RoadIndex].x += (uint16_t)temp % (uint16_t)MT9V03X_W;
//                             // g_CenterPoint[RoadIndex].y += (uint16_t)temp / (uint16_t)MT9V03X_W;
//                             g_Point[Counts].x = (uint16_t)temp % (uint16_t)MT9V03X_W;
//                             g_Point[Counts].y = (uint16_t)temp / (uint16_t)MT9V03X_W;
//                             g_CenterPoint.x += g_Point[Counts].x;
//                             g_CenterPoint.y += g_Point[Counts].y;
//                             Counts++;
//                             temp = temp + Head->iOffset;
//                         }
//                         else // 若左手有墙，前方有墙，遇到角点
//                         {
//                             if (src[temp + Right->iOffset] == WALL) // 若右手有墙，则封死该路，往后走
//                             {
//                                 RoadPath[temp] = WALL;
//                                 src[temp] = WALL;
//                                 g_byStaticSpace[RoadIndex] -= 1;
//                                 // g_CenterPoint[RoadIndex].x -= (uint16_t)temp % (uint16_t)MT9V03X_W;
//                                 // g_CenterPoint[RoadIndex].y -= (uint16_t)temp / (uint16_t)MT9V03X_W;
//                                 g_Point[Counts].x = (uint16_t)temp % (uint16_t)MT9V03X_W;
//                                 g_Point[Counts].y = (uint16_t)temp / (uint16_t)MT9V03X_W;
//                                 g_CenterPoint.x -= g_Point[Counts].x;
//                                 g_CenterPoint.y -= g_Point[Counts].y;
//                                 Counts--;
//                                 temp = temp + Back->iOffset;
//                                 if (temp == i) // 如果回到原点，说明这是一条线，封死起点
//                                 {
//                                     RoadPath[temp] = WALL;
//                                     g_byStaticSpace[RoadIndex] -= 1;
//                                     // g_CenterPoint[RoadIndex].x -= (uint16_t)temp % (uint16_t)MT9V03X_W;
//                                     // g_CenterPoint[RoadIndex].y -= (uint16_t)temp / (uint16_t)MT9V03X_W;
//                                     g_Point[Counts].x = (uint16_t)temp % (uint16_t)MT9V03X_W;
//                                     g_Point[Counts].y = (uint16_t)temp / (uint16_t)MT9V03X_W;
//                                     g_CenterPoint.x -= g_Point[Counts].x;
//                                     g_CenterPoint.y -= g_Point[Counts].y;
//                                     Counts--;
//                                     RoadIndex--; // 取消该路线
//                                     break;       // 非闭合回路
//                                 }
//                                 set_forward(&src[temp]);
//                             }
//                             else // 若右手有路
//                             {
//                                 set_forward(&src[temp]);
//                             }
//                         }

//                     } while (i != temp);
//                     if (RoadIndex == RoadIndexTemp)
//                     {
//                         uint32 PointDistance = 0;
//                         for (uint16 j = 0; j < Counts; j++)
//                         {
//                             PointDistance += pow((g_CenterPoint.x - g_Point[Counts].x), 2) + pow((g_CenterPoint.y - g_Point[Counts].y), 2);
//                         }
//                         if (PointDistance > PointDistanceMax)
//                         {
//                             PointDistanceMax = PointDistance;
//                             g_MaxCenterPoint.x = g_CenterPoint.x / Counts;
//                             g_MaxCenterPoint.y = g_CenterPoint.y / Counts;
//                         }
//                     }
//                 }
//             }

//             if (PointDistanceMax >= 500000)
//             {
//                 uint32 width_index = g_MaxCenterPoint.x * MT9V03X_H / 128;
//                 uint32 height_index = g_MaxCenterPoint.y * MT9V03X_W / 160;
//                 tft180_draw_point(width_index, height_index, RGB565_RED);
//             }
//             // for (uint16_t i = 0; i < MT9V03X_W * MT9V03X_H; i++)
//             // {
//             //     if (RoadPath[i] != WALL)
//             //     {
//             //         if (g_byStaticSpace[RoadPath[i]] < PixelLowThresh) // 限制最少像素点
//             //             RoadPath[i] = WALL;
//             //     }
//             // }
//             // for (uint16_t y = 0; y < MT9V03X_H; y++)
//             // {
//             //     for (uint16_t x = 0; x < MT9V03X_W; x++)
//             //     {
//             //         uint16_t i = y * MT9V03X_W + x;
//             //         if (RoadPath[i] == WALL)
//             //             src[i] = 0;
//             //         else
//             //         {
//             //             src[i] = 128;
//             //         }
//             //     }
//             // }
//             // tft180_show_gray_image(0, 0, src, MT9V03X_W, MT9V03X_H, 160, 128, 0);
//             // 由于像素点密度分布均匀，每点重力加速度相同，故形心、质心、重心处于同一位置
//             // for (uint16_t j = 1; j <= RoadIndex; j++)
//             // {
//             //     // 求得形心
//             //     if (g_byStaticSpace[j] >= PixelLowThresh)
//             //     {
//             //         g_CenterPoint[j].x /= g_byStaticSpace[j];
//             //         g_CenterPoint[j].y /= g_byStaticSpace[j];
//             //         RoadPath[g_CenterPoint[j].x + g_CenterPoint[j].y * MT9V03X_W] = ROAD;
//             //         uint32 width_index = g_CenterPoint[j].x * MT9V03X_H / 128;
//             //         uint32 height_index = g_CenterPoint[j].y * MT9V03X_W / 160;
//             //         // tft180_draw_point(width_index , height_index, RGB565_RED);
//             //     }
//             // }
//         }

//         bool set_forward(uint8 *point)
//         {
//             for (uint8_t i = 0; i < 3; i++)
//             {
//                 if ((*(point + Left->iOffset) == WALL) && (*(point + Head->iOffset) == ROAD)) // 左手无墙，前方无墙
//                     return 1;
//                 else
//                 {
//                     Temp = Head;
//                     Head = Head->p_iNext;
//                     Right = Right->p_iNext;
//                     Back = Back->p_iNext;
//                     Left = Temp;
//                 }
//             }
//             return 0;
//         }
//         bool set_forward2(uint8 *point)
//         {
//             for (uint8_t i = 0; i < 3; i++)
//             {
//                 if ((*(point + Left->iOffset) == WALL) && (*(point + Head->iOffset) == ROAD)) // 左手无墙，前方无墙
//                     return 1;
//                 else
//                 {
//                     Temp = Head;
//                     Head = Head->p_iLast;
//                     Left = Left->p_iLast;
//                     Back = Back->p_iLast;
//                     Right = Temp;
//                 }
//             }
//             return 0;
//         }
//         void describe_plane()
//         {
//         }

//     }
// }