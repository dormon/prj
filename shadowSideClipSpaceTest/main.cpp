#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <iomanip>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#define in 

vec4 movePointToSubfrustum(
    in vec4 point    ,
    in vec2 leftRight,
    in vec2 bottomTop,
    in vec2 nearFar  ){
  vec4 result;
  result.x = (point.x + -point.w * ( -1 + leftRight.y + leftRight.x)) / (leftRight.y - leftRight.x);
  result.y = (point.y + -point.w * ( -1 + bottomTop.y + bottomTop.x)) / (bottomTop.y - bottomTop.x);
  result.z = (point.w * (nearFar.y + nearFar.x) - 2 * nearFar.x * nearFar.y ) / (nearFar.y - nearFar.x);
  result.w = point.w;
  return result;
}

std::string toStr(glm::vec4 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << "," << a.z << "," << a.w << ")";
  return ss.str();
}

std::string toStr(glm::vec2 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << ")";
  return ss.str();
}

std::string toStr(glm::mat4 const&a){
  std::stringstream ss;
  for(int y=0;y<4;++y){
    for(int x=0;x<4;++x)
      ss << a[x][y] << " ";
    ss << std::endl;
  }
  return ss.str();
}

void testEq(float a,float b,float c,float d){
  float fovy =  glm::pi<float>();
  float aspect = 1.f;
  float near = 1.f;
  float far  = 1000.f;
  auto proj = glm::perspective(fovy,aspect,near,far);
  
  vec2 leftRight = vec2(0.f,1.f);
  vec2 bottomTop = vec2(0.f,1.f);
  vec2 nearFar   = vec2(near,far);

  vec4 point = proj*vec4(a,b,c,d);

  auto cc = movePointToSubfrustum(point,leftRight,bottomTop,nearFar);
  if(cc != point){
    std::cerr << toStr(point) << " != " << toStr(cc) << std::endl;
  }
}

bool inside(vec4 const&a){
  return 
    a.x >= -a.w && 
    a.x <= +a.w &&
    a.y >= -a.w && 
    a.y <= +a.w &&
    a.z >= -a.w && 
    a.z <= +a.w &&
    true;
}

bool diff(vec4 const&a,vec4 const&b){
  float const e = 0.001f;
  if(glm::abs(a.x-b.x) > glm::abs(a.x)*e)return true;
  if(glm::abs(a.y-b.y) > glm::abs(a.y)*e)return true;
  if(glm::abs(a.z-b.z) > glm::abs(a.z)*e)return true;
  return false;
}


void test(float a,float b,float c,float d,float ax,float bx,float ay,float by,float az,float bz){
  float fovy =  glm::half_pi<float>();
  float aspect = 1.f;
  float near = 1.f;
  float far  = 1000.f;
  auto proj = glm::perspective(fovy,aspect,near,far);
  
  //std::cerr << toStr(proj) << std::endl;
  vec2 leftRight = vec2(ax,bx);
  vec2 bottomTop = vec2(ay,by);
  vec2 nearFar   = vec2(az,bz);

  vec4 point = proj*vec4(a,b,c,d);

  //std::cerr << "point: " << toStr(point) << std::endl;
  auto cc = movePointToSubfrustum(point,leftRight,bottomTop,nearFar);


  std::cerr << toStr(vec4(a,b,c,d)) << " is ";
  if(inside(cc))std::cerr << "inside";
  else std::cerr << "outside";
  std::cerr << " - " << toStr(leftRight) << " " << toStr(bottomTop) << " " << toStr(nearFar);
  std::cerr << std::endl;
}

void frustumTest(float a,float b,float c,float d,float ax,float bx,float ay,float by,float az,float bz){
  float fovy =  glm::half_pi<float>();
  float aspect = 1.f;
  float near = 1.f;
  float far  = 1000.f;
  float L = -1.f;
  float R = +1.f;
  float B = -1.f;
  float T = +1.f;

  float ll = L + ax*(R-L);
  float rr = L + bx*(R-L);
  float bb = B + ay*(T-B);
  float tt = B + by*(T-B);
  auto  proj = glm::frustum(L ,R ,B ,T ,near,far);
  auto mproj = glm::frustum(ll*az/near,rr*az/near,bb*az/near,tt*az/near,az  ,bz );

  
  vec4 point = vec4(a,b,c,d);
  
  vec2 leftRight = vec2(ax,bx);
  vec2 bottomTop = vec2(ay,by);
  vec2 nearFar   = vec2(az,bz);

  vec4 q     =  proj*point;
  auto mq    = mproj*point;

  auto cq = movePointToSubfrustum(q,leftRight,bottomTop,nearFar);

  if(diff(mq,cq)){
    std::cerr << "ft: " << toStr(point) << " : " << toStr(cq) << " != " << toStr(mq) << " for " << toStr(leftRight) << " " << toStr(bottomTop) << " " << toStr(nearFar) << std::endl;
  }
}

float mr(float mmin = 0,float mmax = 1){
  float t = (float)rand() / (float)RAND_MAX;
  return mmin + t*(mmax-mmin);
}

float clamp(float a,float b = 0,float c = 1){
  if(a<b)return b;
  if(a>c)return c;
  return a;
}

int main()
{

  testEq(1,2,3,1);
  testEq(0,0,0,1);
  testEq(-9,3,-1,1);

  //Z test
  test(0,0,0    ,1,0,1,0,1,1,1000);
  test(0,0,-1   ,1,0,1,0,1,1,1000);
  test(0,0,-1000,1,0,1,0,1,1,1000);
  test(0,0,-1001,1,0,1,0,1,1,1000);
  test(0,0,-1   ,1,0,1,0,1,2,1000);
  test(0,0,-2   ,1,0,1,0,1,2,1000);
  test(0,0,-100 ,1,0,1,0,1,2,100 );
  test(0,0,-101 ,1,0,1,0,1,2,100 );
  test(0,0,-101 ,1,0,1,0,1,2,1000);
  //X test
  test(0,0,-1   ,1,0,1,0,1,1,1000);
  test(1,0,-1   ,1,0,1,0,1,1,1000);
  test(-1,0,-1   ,1,0,1,0,1,1,1000);
  test(1.1,0,-1   ,1,0,1,0,1,1,1000);
  test(11,0,-1   ,1,0,1,0,1,1,1000);
  test(-1.1,0,-1   ,1,0,1,0,1,1,1000);
  test(0,0,-1   ,1,0.75,1,0,1,1,1000);
  test(.6,0,-1   ,1,0.75,1,0,1,1,1000);

  frustumTest(0,0,-1   ,1,0,1,0,1,1,1000);
  frustumTest(1,1,-1   ,1,0,1,0,1,1,1000);
  frustumTest(1,1,-10  ,1,0,1,0,1,1,1000);
  frustumTest(1,2,-10  ,1,0.5,.75,.5,.75,1,1000);
  frustumTest(1,2,-10  ,1,0.5,.75,.5,.75,4,100);
  frustumTest(-3,7,-13 ,1,0.3,.8,.2,.9,7,40);

  for(size_t i=0;i<1000;++i)
    frustumTest(mr(-100,100),mr(-100,100),mr(-100,100),1,0.3,0.8,.2,.9,7,40);

  for(size_t i=0;i<10000;++i){
    auto ll = mr();
    auto rr = clamp(ll + mr());
    auto bb = mr();
    auto tt = clamp(bb + mr());
    float near = 1.f;
    float far = 1000.f;
    auto nn = mr(near,far);
    auto ff = clamp(nn + mr(far-near),near,far);
    frustumTest(mr(-100,100),mr(-100,100),mr(-100,100),1,ll,rr,bb,tt,nn,ff);
  }

  return 0;
}
