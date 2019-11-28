#pragma once

#include "Simplex.h"
#include <vector>

template<size_t N>
struct GPUEdgeData{
  Line4Df   edge;
  Vertex4Df oppositeVertices[N];
};

void create(std::vector<GPUEdgeData<2>>&data,size_t n);
