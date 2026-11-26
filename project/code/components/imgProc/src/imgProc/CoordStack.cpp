#include "imgProc/CoordStack.hpp"

#include "apriltag/internal/StaticBuffer.hpp"

namespace imgProc
{
    using apriltag::staticBuffer;

    void CoordStack::push(Coordinate x)
    {
        *(Coordinate *)(staticBuffer.allocate(sizeof(Coordinate))) = x;
        ++cnt;
    }
    Coordinate CoordStack::pop()
    {
        if (cnt > 0)
        {
            staticBuffer.pop(sizeof(Coordinate));
            --cnt;
        }

        return *(Coordinate *)staticBuffer.peek();
    }
    bool CoordStack::empty() { return cnt; }
    void CoordStack::clear()
    {
        if (cnt != 0)
        {
            staticBuffer.pop(sizeof(Coordinate) * cnt);
            cnt = 0;
        }
    }
} // namespace imgProc
// namespace imgProc
// {
//     using apriltag::staticBuffer;

//     void CoordStack::push(Coordinate x)
//     {
//         *(Coordinate *)(staticBuffer.allocate(sizeof(Coordinate))) = x;
//         ++cnt;
//     }
//     Coordinate CoordStack::pop()
//     {
//         if (cnt > 0)
//         {
//             staticBuffer.pop(sizeof(Coordinate));
//             --cnt;
//         }

//         return *(Coordinate *)staticBuffer.peek();
//     }
//     bool CoordStack::empty() { return cnt; }
//     void CoordStack::clear()
//     {
//         if (cnt != 0)
//         {
//             staticBuffer.pop(sizeof(Coordinate) * cnt);
//             cnt = 0;
//         }
//     }
// } // namespace imgProc