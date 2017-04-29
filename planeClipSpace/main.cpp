#include<iostream>
#include<iomanip>
#include<sstream>
#include<cstdint>
#include<cstdlib>
#include<cassert>
#include<vector>
#include<map>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

std::string toStr(glm::vec3 const&a){
  std::stringstream ss;
  ss<<std::scientific<<std::setprecision(10)<<std::showpos<<"("<<a.x<<","<<a.y<<","<<a.z<<")";
  return ss.str();
}

std::string toStr(glm::vec4 const&a){
  std::stringstream ss;
  ss<<std::scientific<<std::setprecision(10)<<std::showpos<<"("<<a.x<<","<<a.y<<","<<a.z<<","<<a.w<<")";
  return ss.str();
}

std::string toStr(float const&a){
  std::stringstream ss;
  ss<<std::scientific<<std::setprecision(10)<<std::showpos<<a;
  return ss.str();
}

int main(){
  auto A = glm::vec4(+1.3f,+2.3f,-3.3f,+1.4f);
  auto B = glm::vec4(-4.3f,-0.3f,+5.3f,-1.3f);
  auto C = glm::vec4(+9.3f,+2.3f,-3.3f,+1.1f);
  
  auto AA = glm::vec3(A);
  auto BB = glm::vec3(B);
  auto CC = glm::vec3(C);

  auto M  = glm::transpose(glm::mat3(AA,BB,CC));
  //auto M  = glm::transpose(glm::mat3(glm::vec3(+A.x,+A.y,+A.z),glm::vec3(+B.x,+B.y,+B.z),glm::vec3(+C.x,+C.y,+C.z)));
  auto Nx = glm::transpose(glm::mat3(glm::vec3(-A.w,+A.y,+A.z),glm::vec3(-B.w,+B.y,+B.z),glm::vec3(-C.w,+C.y,+C.z)));
  auto Ny = glm::transpose(glm::mat3(glm::vec3(+A.x,-A.w,+A.z),glm::vec3(+B.x,-B.w,+B.z),glm::vec3(+C.x,-C.w,+C.z)));
  auto Nz = glm::transpose(glm::mat3(glm::vec3(+A.x,+A.y,-A.w),glm::vec3(+B.x,+B.y,-B.w),glm::vec3(+C.x,+C.y,-C.w)));

  auto P = glm::vec4(
      glm::determinant(Nx)/glm::determinant(M),
      glm::determinant(Ny)/glm::determinant(M),
      glm::determinant(Nz)/glm::determinant(M),
      1.f);

  std::cout << toStr(glm::dot(P,A)) << std::endl;
  std::cout << toStr(glm::dot(P,B)) << std::endl;
  std::cout << toStr(glm::dot(P,C)) << std::endl;

  auto PP = glm::cross(BB/B.w-AA/A.w,CC/C.w-AA/A.w);
  std::cout << toStr(glm::vec3(P))            << std::endl;
  std::cout << toStr(glm::vec3(P.x/PP.x,P.y/PP.y,P.z/PP.z)) << std::endl;

  return 0;
}
