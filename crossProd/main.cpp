#include<iostream>
#include<memory>
#include<functional>
#include<type_traits>
#include<cassert>
#include<vector>
#include<map>
#include<set>
#include<tuple>
#include<functional>
#include<sstream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

std::string val2str(glm::vec4 const&v){
  std::stringstream ss;
  ss<<"vec4("<<v.x<<","<<v.y<<","<<v.z<<","<<v.w<<")";
  return ss.str();
}

std::string val2str(glm::vec3 const&v){
  std::stringstream ss;
  ss<<"vec3("<<v.x<<","<<v.y<<","<<v.z<<")";
  return ss.str();
}

std::string val2str(float const&v){
  std::stringstream ss;
  ss<<v;
  return ss.str();
}

#define PRINT(x) std::cout<<#x<<": "<<val2str(x)<<std::endl

int main(){
  auto mvp = glm::perspective(glm::radians(90.f),1.f,1.f,1000.f);
  auto itmvp = glm::inverse(glm::transpose(mvp));
  auto a=glm::vec4(1.f,2.f,3.f,1.f);
  auto b=glm::vec4(-3.2f,2.3f,3.1f,1.f);
  auto l=glm::vec4(100.f,100.f,100.f,1.f);

  //A = (ax,ay,az,1)
  //B = (ax,ay,az,1)
  //L = (lx,ly,lz,1)
  //M = projection matrix
  //
  //M*A
  //
  //n = ((Bxyz-Axyz)x(Lxyz-Axyz),0)
  //P = (nxyz,-n*A)
  //
  //P*A = 0
  //P*B = 0
  //P*L = 0
  //
  //(it(M)*P)*(M*A) = 0
  //(it(M)*P)*(M*B) = 0
  //(it(M)*P)*(M*L) = 0
  //
  //

  auto clip_a = mvp*a;
  auto clip_b = mvp*b;
  auto clip_l = mvp*l;

  auto n = glm::cross(glm::vec3(b)-glm::vec3(a),glm::vec3(l)-glm::vec3(a));
  auto lp = glm::vec4(n,-glm::dot(n,glm::vec3(a)));
  auto clip_lp = itmvp*lp;
  auto clip_n = itmvp*glm::vec4(n,0.f);
  auto clip_n2 = mvp*glm::vec4(n,0.f);
  auto clip_zero = mvp*glm::vec4(0.f,0.f,0.f,1.f);
//    glm::cross(glm::vec3(clip_b-clip_a),glm::vec3(clip_l-clip_a));

  PRINT(clip_n);
  PRINT(clip_lp);

  PRINT(clip_n2);
  PRINT(clip_zero);

	return 0;
}
