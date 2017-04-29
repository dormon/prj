#include"BoundingVolumes.h"
#include<vector>
#include<memory>

int main(){
  ge::sg::BoundingSphere*asd=new ge::sg::BoundingSphere();
  std::vector<std::shared_ptr<ge::sg::BoundingSphere>>vec;
  asd->expand(vec);
  delete asd;
  return 0;
}





