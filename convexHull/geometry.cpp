#include"geometry.h"

AdjacentPoint::AdjacentPoint(AdjacentPlane a,AdjacentPlane b,AdjacentPlane c){
  if(b<a){AdjacentPlane z=b;b=a;a=z;}
  if(c<b){AdjacentPlane z=c;c=b;b=z;}
  if(b<a){AdjacentPlane z=b;b=a;a=z;}
  /*
  glm::mat3 matrix;
  glm::vec3 result;
  matrix[0]=glm::vec3(a);
  matrix[1]=glm::vec3(b);
  matrix[2]=glm::vec3(c);
  float dt=glm::determinant(matrix);
  matrix[0]=glm::vec3(-a.data.w,a.data.y,a.data.z);
  matrix[1]=glm::vec3(-b.data.w,b.data.y,b.data.z);
  matrix[2]=glm::vec3(-c.data.w,c.data.y,c.data.z);
  result.x=glm::determinant(matrix);
  matrix[0]=glm::vec3(a.data.x,-a.data.w,a.data.z);
  matrix[1]=glm::vec3(b.data.x,-b.data.w,b.data.z);
  matrix[2]=glm::vec3(c.data.x,-c.data.w,c.data.z);
  result.y=glm::determinant(matrix);
  matrix[0]=glm::vec3(a.data.x,a.data.y,-a.data.w);
  matrix[1]=glm::vec3(b.data.x,b.data.y,-b.data.w);
  matrix[2]=glm::vec3(c.data.x,c.data.y,-c.data.w);
  result.z=glm::determinant(matrix);
  if(dt==0)return glm::vec3(std::numeric_limits<float>::quiet_NaN());
  return result/dt;
  */
}
//bool operator==(AdjacentPoint const&other)const;
bool AdjacentPoint::operator< (AdjacentPoint const&other)const{
  for(unsigned i=0;i<3;++i)if((*this)[i]<other[i])return true;
  return false;
}

bool AdjacentPoint::operator==(AdjacentPoint const&other)const{
  for(unsigned i=0;i<3;++i)if((*this)[i]!=other[i])return false;
  return true;
}

