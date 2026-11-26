#include "path_plan.hpp"
#include <cmath>

using namespace std;
using namespace TSP;

float g_fPointPositionX[30];//, 5, 4, 8, 6, 12, 16, 19, 24, 28, 32, 32, 27, 22, 18, 18, 13, 9, 8};
float g_fPointPositionY[30];//, 5, 9, 18, 22, 23, 20, 23, 17, 18, 22, 5, 8, 6, 2, 12, 14, 7, 2};

static inline void calcDist()
{
    for (int i = 0; i < MAXN; ++i)
    {
        tsp.dist[i][i] = 0;
        for (int j = i + 1; j < MAXN; ++j)
        {
            int dx = g_fPointPositionX[i] - g_fPointPositionX[j], dy = g_fPointPositionY[i] - g_fPointPositionY[j];
            tsp.dist[i][j] = tsp.dist[j][i] = sqrt(dx * dx + dy * dy);
        }
    }
}

void path_plan()
{
    tsp.N = g_byPointNum;
    calcDist();
    tsp.solve();
}
// sorted(g_fPointPositionY, g_fPointPositionX, 16, tsp.hamilton_path);
void sorted(float* array_y, float* array_x, int count, int* index)
{
    for (int i = 0; i < count; i++)
    {
        index[i] = i;  //????????????????
    }

    for (int i = 0; i < count - 1; i++)
    {
        int flag = i;
        for (int j = flag + 1; j < count; j++)
        {
            if (array_x[index[flag]] < array_x[index[j]])
            {
                flag = j;
            }
            else if (array_x[index[flag]] == array_x[index[j]])
            {
                if (array_y[index[flag]] > array_y[index[j]])
                {
                    flag = j;
                }
            }
        }
        int temp = 0;
        temp = index[flag];
        index[flag] = index[i];
        index[i] = temp;
    }
}