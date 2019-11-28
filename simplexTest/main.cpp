#include <iostream>
#include <vector>
#include "create.h"


int main()
{
  std::vector<GPUEdgeData<2>>dst;
  create(dst,100000);
  if(dst.at(0).edge.vertices[0].elements[0] == 0){
    for(size_t i=0;i<100000;++i){
      dst[i].edge.clear();
      dst[i].oppositeVertices[0].clear();
      dst[i].oppositeVertices[1].clear();
    }
  }
  return 0;
}
