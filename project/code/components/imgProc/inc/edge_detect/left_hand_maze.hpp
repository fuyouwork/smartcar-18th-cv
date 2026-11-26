#ifndef _imgProc_edge_detect_left_hand_maze_hpp
#define _imgProc_edge_detect_left_hand_maze_hpp

#include "edge_detect/canny.hpp"

extern "C"
{
#include "zf_common_headfile.h"
}
#define UP_OFFSET (-MT9V03X_W)
#define DOWN_OFFSET (MT9V03X_W)
#define LEFT_OFFSET (-1)
#define RIGHT_OFFSET (1)
#define WALL (0)
#define ROAD (1)

namespace imgProc
{
  namespace edge_detect
  {
    using apriltag::staticBuffer;
    typedef struct forward_list
    {
      forward_list *p_iLast;
      int16_t iOffset;
      forward_list *p_iNext;
    } forward_list;
    void detect_angular_point(uint8_t *src);
    bool set_forward2(uint8 *point);
    bool set_forward(uint8 *point);
    typedef struct RoadPathIndex
    {
      uint16_t RoadPathIndex;
      Coordinate coordinate;
    } RoadPathIndex_t;
    class RoadPathStack_t
    {
      uint16_t cnt;

    public:
      RoadPathStack_t() { cnt = 0; }
      void push(RoadPathIndex_t RoadPathIndex)
      {
        *(RoadPathIndex_t *)(staticBuffer.allocate(sizeof(RoadPathIndex_t))) = RoadPathIndex;
        cnt++;
      }
      RoadPathIndex_t pop()
      {
        if (cnt > 0)
        {
          staticBuffer.pop(sizeof(RoadPathIndex_t));
          cnt--;
        }
        return *(RoadPathIndex_t *)staticBuffer.peek();
      }
      uint16_t empty()
      {
        return cnt;
      }
      void clear()
      {
        if (cnt != 0)
        {
          staticBuffer.pop(sizeof(RoadPathIndex_t) * cnt);
          cnt = 0;
        }
      }
    };

  }
}
#endif