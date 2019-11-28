#include "create.h"

void create(std::vector<GPUEdgeData<2>>&data,size_t n){
  data.resize(n);
  for(size_t i=0;i<n;++i){
    data[i].edge.clear();
    data[i].oppositeVertices[0].clear();
    data[i].oppositeVertices[1].clear();
  }
}
