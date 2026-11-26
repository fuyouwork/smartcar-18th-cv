#ifndef _imgProc_edge_detect_canny_hpp
#define _imgProc_edge_detect_canny_hpp

#include <cstdint>
#include <utility>

#include "apriltag/config.hpp"
#include "apriltag/fmath.hpp"
#include "apriltag/internal/StaticBuffer.hpp"
#include "edge_detect/conv.hpp"
#include "imgProc/CoordStack.hpp"
#include "imgProc/common.hpp"

extern "C"{
    #include "zw_device/ZwPlus_lib.h"
}
namespace imgProc {
namespace edge_detect {

typedef struct gvec {
    uint16_t t;	// t 是Angle
    uint16_t g; // g 是梯度值
} gvec_t;
extern uint8_t src[MT9V03X_H * MT9V03X_W];
gvec_t* canny(uint8_t* img, int low_thresh, int high_thresh);

}  // namespace edge_detect
}  // namespace imgProc

#endif  // _imgProc_edge_detect_canny_hpp