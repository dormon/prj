/**
 * @file 
 * @brief This file contains definition for N dimensional index.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

/**
 * \class NIndex
 * This class represents N dimensional index.
 * It can be used to access N dimensional arrays.
 * It supports basic operation like addition, substraction and conversion to 1D index.
 * Index parts point to elements of N dimensional index (i,j for 2D array, i,j,k for 3D array, ...).
 * for(int j=0;...)
 *   for(int i=0;...)
 *     doStuff(...);
 *
 * Axis ordering reorders axes incrementation and decrementation.
 * Axes are incremented in standard order (0,1,2,...) if there is no axis ordering.
 * You can specify the fastes axis by inserting its number into axis ordering array to the first element.
 * You can specify the slowes axis by inserting its number into axis ordering array to the last element.
 * The fastes axis index part is changes the most frequently during incrementation.
 *
 * int indexParts[3] = {0,0,0};
 * int slowestAxisIndex = 2;
 * int fastestAxisIndex = 0;
 *
 * int slowestAxis = axisOrdering[slowestAxisIndex];
 * int fastestAxis = axisOrdering[fastestAxisIndex];
 * for(...; ++indexParts[slowestAxis])
 *   for(...; ++indexParts[axisOrdering[1])
 *     for(...; ++indexParts[fastestAxis])
 *       doStuff(...);
 *
 * Axis flipping flips orientation of axes.
 *
 * no flipping:
 * for(int i=0;i<size;++i)
 *
 * with flipping:
 * for(int i=size-1;i>=0;--i)
 */

#include<cassert>
#include<functional>
#include<cmath>
#include<set>
#include<iostream>
#include<algorithm>
#include<numeric>

#include<NIndex/NIndex.h>

/*
class NIndexImpl{
  public:
    NIndexImpl(std::vector<ssize_t>const&mins,std::vector<ssize_t>const&maxs);
    void setMin();
    void setMax();
};

*/
