#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <iomanip>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

using namespace glm;

std::string toStr(glm::dvec4 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << "," << a.z << "," << a.w << ")";
  return ss.str();
}

std::string toStr(glm::dvec3 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << "," << a.z << ")";
  return ss.str();
}

std::string toStr(glm::dvec2 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << ")";
  return ss.str();
}

std::string toStr(glm::dmat4 const&a){
  std::stringstream ss;
  for(int y=0;y<4;++y){
    for(int x=0;x<4;++x)
      ss << a[x][y] << " ";
    ss << std::endl;
  }
  return ss.str();
}


bool inside(dvec4 const&a){
  return 
    a.x >= -a.w && 
    a.x <= +a.w &&
    a.y >= -a.w && 
    a.y <= +a.w &&
    a.z >= -a.w && 
    a.z <= +a.w &&
    true;
}

bool diff(dvec4 const&a,dvec4 const&b){
  double const e = 0.001f;
  if(glm::abs(a.x-b.x) > glm::abs(a.x)*e)return true;
  if(glm::abs(a.y-b.y) > glm::abs(a.y)*e)return true;
  if(glm::abs(a.z-b.z) > glm::abs(a.z)*e)return true;
  return false;
}


double mr(double mmin = 0,double mmax = 1){
  double t = (double)rand() / (double)RAND_MAX;
  return mmin + t*(mmax-mmin);
}

double clamp(double a,double b = 0,double c = 1){
  if(a<b)return b;
  if(a>c)return c;
  return a;
}

double rrr(double mmin=-1,double mmax=1){
  auto t = (double)rand() / (double)RAND_MAX; 
  return mmin + t*(mmax-mmin);
}

glm::dvec3 r3(double mmin=-1,double mmax=1){
  return glm::vec3(rrr(mmin,mmax),rrr(mmin,mmax),rrr(mmin,mmax));
}

double depthToZ(double d,double NN,double FF){
  return -2*NN*FF / (d*(NN-FF) + FF + NN);
}

dvec3 getSample3D(dvec3 c,double LL,double RR,double BB,double TT,double NN,double FF){
  return vec3(
    LL+(.5+.5*c.x)*(RR-LL),
    BB+(.5+.5*c.y)*(TT-BB),
    depthToZ(c.z,NN,FF)
    );
}

dmat4 invProj(double l,double r,double b,double t,double n,double f){
  dmat4 res;
  res[0][0] = (r-l)/(2*n);
  res[0][1] = 0;
  res[0][2] = 0;
  res[0][3] = 0;

  res[1][0] = 0;
  res[1][1] = (t-b)/(2*n);
  res[1][2] = 0;
  res[1][3] = 0;

  res[2][0] = 0;
  res[2][1] = 0;
  res[2][2] = 0;
  res[2][3] = -(f-n)/(2*f*n);

  res[3][0] = (r+l)/(2*n);
  res[3][1] = (t+b)/(2*n);
  res[3][2] = -1;
  res[3][3] = -(-n-f)/(2*f*n);

  return res;
}


glm::dvec4 cartezPlane(glm::dvec3 const&a,glm::dvec3 const&b,glm::dvec3 const&l,double LL,double RR,double BB,double TT,double NN,double FF){
  auto A = getSample3D(a,LL,RR,BB,TT,NN,FF);
  auto B = getSample3D(b,LL,RR,BB,TT,NN,FF);
  auto n = glm::normalize(glm::cross(B-A,l-A));

  auto itproj = glm::transpose(invProj(LL,RR,BB,TT,NN,FF));
  //auto itproj = glm::transpose(glm::inverse(glm::frustum(LL,RR,BB,TT,NN,FF)));

  auto proj = frustum(LL,RR,BB,TT,NN,FF);

  auto plane = glm::dvec4(n,glm::dot(n,-glm::dvec3(A)));

  std::cerr << "APLANE: " << dot(dvec4(A,1.),plane) << " BPLANE: " << dot(dvec4(B,1.),plane) << " LPLANE: " << dot(dvec4(l,1.),plane) << std::endl;
  auto ppppp = itproj*plane;
  ppppp = normalize(ppppp);
  std::cerr << "APLANE: " << dot(dvec4(A,1.),plane) << " BPLANE: " << dot(dvec4(B,1.),plane) << " LPLANE: " << dot(dvec4(l,1.),plane) << std::endl;
  std::cerr << "AAAAAA: " << dot(dvec4(a,1.),ppppp) << " BBBBBB: " << dot(dvec4(b,1.),ppppp) << std::endl;

  std::cerr << "APPPPP: " << dot(proj*dvec4(A,1.),ppppp) << " BPPPPP: " << dot(proj*dvec4(B,1.),ppppp) << " LPPPPP: " << dot(proj*dvec4(l,1.),ppppp) << std::endl;
  
  return ppppp;
}

glm::dvec4 projPlane(glm::dvec3 const&a,glm::dvec3 const&b,glm::dvec3 const&l,glm::dmat4 const&p){
  auto L = p*glm::dvec4(l,1.f);
  auto n = normalize(cross(dvec3(b)-dvec3(a),dvec3(L)-dvec3(a)*L.w));
  return dvec4(n,-dot(n,a));
}

glm::dvec4 projPlane_(glm::dvec3 const&a,glm::dvec3 const&b,glm::dvec3 const&l,glm::dmat4 const&p){
  auto L = p*glm::dvec4(l,1.f);
  
  auto sig = -1.+2.*(L.w < 0.);
  auto LL = glm::dvec3(L)/L.w;
  

  auto n = sig*glm::normalize(glm::cross(b-a,LL-a));

  return glm::vec4(n,-glm::dot(n,a));
}

int main()
{

  auto test = [&](){
    

    auto left = rrr();
    auto right = rrr(left,1-left);
    auto bottom = rrr();
    auto top = rrr(bottom,1-bottom);
    auto near = rrr(1,10);
    auto far = rrr(near,1000);

    auto proj = glm::frustum(left,right,bottom,top,near,far);

    auto A = r3();
    auto B = r3();
    auto L = r3(-100,100);

    auto LL = proj*glm::vec4(L,1.f);

    auto iproj = invProj(left,right,bottom,top,near,far);
    
    auto cp = cartezPlane(A,B,L,left,right,bottom,top,near,far);
    auto pp = projPlane(A,B,L,proj);

    auto norCor = dot(dvec3(cp),dvec3(pp));

    std::cerr << glm::length(cp-pp) << " - cp,pp ";
    std::cerr << std::endl << "                     ";
    std::cerr << "CP" << toStr(cp) << " - ";
    std::cerr << "PP" << toStr(pp) << " - ";
    std::cerr << std::endl << "                     ";
    std::cerr << "A"<<toStr(A) << " B" << toStr(B) << " L" << toStr(LL) << " ";
    std::cerr << std::endl << "                     ";
    std::cerr << "norcor: " << norCor;
    std::cerr << std::endl << "                     ";

    std::cerr << glm::dot(cp,glm::dvec4(A,1.f)) << " , ";
    std::cerr << glm::dot(cp,glm::dvec4(B,1.f)) << " , ";
    std::cerr << glm::dot(cp,glm::dvec4(LL   )) << " , ";
    std::cerr << glm::dot(pp,glm::dvec4(A,1.f)) << " | ";
    std::cerr << glm::dot(pp,glm::dvec4(B,1.f)) << " | ";
    std::cerr << glm::dot(pp,glm::dvec4(LL   )) << " | ";
    
    std::cerr << std::endl;
  };

  for(int i=0;i<1000;++i)
  test();


  return 0;
}
