#pragma once

#include <glm/glm.hpp>

inline bool operator<=(glm::vec3 const&a,glm::vec3 const&b){
  for(int i=0;i<3;++i){
    if(a[i] < b[i])return true;
    if(a[i] > b[i])return false;
  }
  return true;
}

inline bool operator<(glm::vec3 const&a,glm::vec3 const&b){
  for(int i=0;i<3;++i){
    if(a[i] < b[i])return true;
    if(a[i] > b[i])return false;
  }
  return false;
}


inline glm::vec4 planeSorted(glm::vec3 const&a,glm::vec3 const&b,glm::vec3 const&c,float swap = 1.f){
  auto n = glm::normalize(glm::cross(b-a,c-a)) * swap;
  return glm::vec4(n,-glm::dot(a,n));
}


inline glm::vec4 plane(glm::vec3 const&a,glm::vec3 const&b,glm::vec3 const&c){
  if(a<=b)
    if(b<=c)
      return planeSorted(a,b,c);
    else
      if(c<=a)
        return planeSorted(c,a,b);
      else
        return planeSorted(a,c,b,-1.f);
  else
    if(a<=c)
      return planeSorted(b,a,c,-1.f);
    else
      if(c<=b)
        return planeSorted(c,b,a,-1.f);
      else
        return planeSorted(b,c,a);
}

void swap(glm::vec3&a,glm::vec3&b){
  glm::vec3 c = a;
  a = b;
  b = c;
}

inline glm::vec4 plane2(glm::vec3 a,glm::vec3 b,glm::vec3 c){
  float swapped = 1.f;
  if(b<a){swap(a,b);swapped *= -1;}
  if(c<b){swap(b,c);swapped *= -1;}
  if(b<a){swap(a,b);swapped *= -1;}
  return planeSorted(a,b,c,swapped);
}
