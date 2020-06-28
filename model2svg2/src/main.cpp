#include<memory>
#include<sstream>
#include<fstream>
#include<ArgumentViewer/ArgumentViewer.h>
#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>
#include<functional>
#include<FastAdjacency.h>
#include<Camera.h>
#include<Scene.h>
#include<SVG.h>
#include<plane.h>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl


void clipLine(float&tmin,float&tmax,glm::vec4 const&a,glm::vec4 const&b,int axis,float s){
  //+ai+t(bi-ai) >= -aw-t(bw-aw)
  //+ai+t(bi-ai) <= +aw+t(bw-aw)
  //
  //+ai+t(bi-ai) >= -aw-t(bw-aw)
  //-ai-t(bi-ai) >= -aw-t(bw-aw)
  //
  //s(ai+t(bi-ai)) >= -aw-t(bw-aw)
  //sai+aw+ts(bi-ai) >= t(aw-bw)
  //sai+aw >= ts(ai-bi) + t(aw-bw)
  //M >= t(s(ai-bi)+(aw-bw))
  //M >= tN
  //tN <= M
  //t <= M/N
  //t <= O   N>=0
  //t >= O   N< 0
  //
  float M = s*a[axis]+a[3];
  float N = s*(a[axis]-b[axis])+a[3]-b[3];
  if(N==0.f){
    if(M<0.f)tmax=-1.f;
    return;
  }
  float O = M / N;
  if(N>=0.f){
    tmax = glm::min(tmax,O);
  }else{
    tmin = glm::max(tmin,O);
  }
  return;
}

enum class ClipPlaneParts: uint32_t{
  x        = 0   ,
  y        = 1   ,
  z        = 2   ,
  negative = 0   ,
  positive = 1<<2,
};

enum class ClipPlane: uint32_t{
  LEFT   = static_cast<uint32_t>(ClipPlaneParts::x) | static_cast<uint32_t>(ClipPlaneParts::negative), 
  RIGHT  = static_cast<uint32_t>(ClipPlaneParts::x) | static_cast<uint32_t>(ClipPlaneParts::positive),
  BOTTOM = static_cast<uint32_t>(ClipPlaneParts::y) | static_cast<uint32_t>(ClipPlaneParts::negative),
  TOP    = static_cast<uint32_t>(ClipPlaneParts::y) | static_cast<uint32_t>(ClipPlaneParts::positive),
  FAR    = static_cast<uint32_t>(ClipPlaneParts::z) | static_cast<uint32_t>(ClipPlaneParts::negative),
  NEAR   = static_cast<uint32_t>(ClipPlaneParts::z) | static_cast<uint32_t>(ClipPlaneParts::positive),
};

uint32_t axis(ClipPlane const&cp){
  return static_cast<uint32_t>(cp)&3;
}

float sign(ClipPlane const&cp){
  return -1.f+2.f*(static_cast<uint32_t>(cp)>>2);
}

//void clipLine(float&tmin,float&tmax,glm::vec4 const&a,glm::vec4 const&b,ClipPlane p){
//  int axis;
//  float s;
//  if(p == LEFT || p == RIGHT || )
//
//  clipPlane(tmin,tmax,a,b,)
//}

//bool clip(glm::vec4 const&a,glm::vec4 const&b,glm::vec4 &c,glm::vec4&o0,glm::vec4&o1){
//  
//  // ai+t(bi-ai) > -aw-t(bw-aw)
//  // ai+t(bi-ai) < +aw+t(bw-aw)
//}

float linearizeDepth(float n,float f,float d){
  //d = (-(f+n)/(f-n)*z + -2*f*n/(f-n)*w) / -z
  //d = (f+n)/(f-n) + 2*f*n/(f-n) / z
  //d - (f+n)/(f-n) = 2*f*n/(f-n) / z
  //z = 2fn/(f-n) / (d-(f+n)/(f-n))
  //z = 2fn/(f-n) / ((d(f-n)-(f+n))/(f-n))
  //z = 2fn / (d(f-n)-(f+n))
  //z = 2fn / (df-dn-f-n)
  float lin = (2*f*n) / (d*f-d*n-f-n);
  return (lin+n)/(n-f);
}


bool line2NDC(glm::vec3&ao,glm::vec3&bo,glm::vec4 const&A,glm::vec4 const&B){
  //Ax/Aw(1-t)+Bx/Bwt
  //Ax/Aw+(Bx/Bw-Ax/Aw)t
  //Ax/Aw+(BxAw-AxBw)/(BwAw)t
  //(AxBw)/(AwBw)+(AwBx-AxBw)/(AwBw)t
  //(AxBw+AwBxt-AxBwt)/(AwBw)
  //(AxBw(1-t)+AwBxt)/(AwBw)
  //
  //(AxBw)/(AwBw)(1-t) + (AwBx)/(AwBw)(t)
  //Ax/Aw(1-t) + Bx/Bwt
  //Ax/Aw - Ax/Awt + Bx/Bwt
  //Ax/Aw + (Bx/Bw - Ax/Aw)t
  //
  //(Ax - tAx + Bxt) / (Aw - tAw + Bwt)
  //(Ax + (Bx - Ax)t) / (Aw + (Bw - Aw)t)
  //
  //(Ax + (Bx - Ax)t) / (Aw + (Bw - Aw)t) * (1/Aw)/(1/Aw)
  //(Ax/Aw + (Bx/Aw - Ax/Aw)t) / (1 + (Bw/Aw - 1)t)
  //
  //
  //
  //X(t) = A(1-t) + Bt
  //X(t)i >= -X(t)w
  //X(t)i <= +X(t)w
  //
  //X(t)i + X(t)w >= 0
  //X(t)i - X(t)w <= 0
  //
  //+X(t)i + X(t)w >= 0
  //-X(t)i + X(t)w >= 0
  //
  //sX(t)i + X(t)w >= 0
  //
  //sAi(1-t) + sBit + Aw(1-t) + Bwt >= 0
  //
  //(sAi+Aw)(1-t) + (sBi+Bw)t >= 0
  //(-sAi-Aw+Sbi+Bw)t + (sAi+Aw) >= 0
  //(-sAi-Aw+Sbi+Bw)t >= - (sAi+Aw)
  //N*t >= M
  //
  //N >  0           -> t >= M/N
  //N <  0           -> t <= M/N
  //N == 0 && M <= 0 -> ok
  //N == 0 && M >  0 -> bad
  //
  
  float tMin = 0;
  float tMax = 1;
  for(size_t ss=0;ss<2;++ss){
    float s=-1.f + ss*2.f;
    for(size_t i=0;i<3;++i){
      float N = -s*A[i]+s*B[i]-A[3]+B[3];
      float M = -(s*A[i]+A[3]);
      if(N <  0.f            )tMax = glm::min(tMax,M/N);
      if(N == 0.f && M <  0.f)continue;
      if(N == 0.f && M == 0.f)continue;
      if(N == 0.f && M >  0.f)return false;
      if(N >  0.f            )tMin = glm::max(tMin,M/N);
    }

  }
  if(tMin >= tMax)return false;
  auto abmin = A*(1.f-tMin) + B*tMin;
  auto abmax = A*(1.f-tMax) + B*tMax;
  ao = glm::vec3(abmin) / abmin.w;
  bo = glm::vec3(abmax) / abmax.w;
  return true;
}

bool isPointVisible(glm::vec4 const&p){
  for(size_t i=0;i<3;++i)
    if(p[i] < -p[3] || p[i] > p[3])return false;
  return true;
}



class Circle;
class Line;
class Triangle;

class Element{
  public:
    virtual ~Element(){}
    enum Type{
      CIRCLE  ,
      LINE    ,
      TRIANGLE,
    }type;
    Element(Type t):type(t){}
    Circle   const&toCircle()const;
    Line     const&toLine()const;
    Triangle const&toTriangle()const;
};

class Circle: public Element{
  public:
    Circle():Element(CIRCLE){}
    Circle(glm::vec3 const&c,float r,float w,glm::vec3 const&sC,glm::vec3 const&fC):Circle(glm::vec4(c,1.f),r,w,sC,fC){}
    Circle(glm::vec4 const&c,float r,float w,glm::vec3 const&sC,glm::vec3 const&fC):Element(CIRCLE),center(c),radius(r),width(w),sColor(sC),fColor(fC){}
    glm::vec4 center = glm::vec4(0.f,0.f,0.f,1.f);
    float     radius = 1.f                       ;
    float     width  = 1.f                       ;
    glm::vec3 sColor = glm::vec3(0.f)            ;
    glm::vec3 fColor = glm::vec3(0.f)            ;
};

class Line: public Element{
  public:
    Line():Element(LINE){}
    Line(glm::vec3 const&a,glm::vec3 const&b,float w,glm::vec3 const&c):Line(glm::vec4(a,1.f),glm::vec4(b,1.f),w,c){}
    Line(glm::vec4 const&a,glm::vec4 const&b,float w,glm::vec3 const&c):Element(LINE),a(a),b(b),width(w),color(c){}
    glm::vec4 a     = glm::vec4(0.f,0.f,0.f,1.f);
    glm::vec4 b     = glm::vec4(0.f,0.f,0.f,1.f);
    float     width = 1.f                       ;
    glm::vec3 color = glm::vec3(0.f)            ;
};

class Triangle: public Element{
  public:
    Triangle():Element(TRIANGLE){}
    Triangle(
        glm::vec3 const&a,
        glm::vec3 const&b,
        glm::vec3 const&c,
        glm::vec4 const&ac = glm::vec4(1,0,0,1),
        glm::vec4 const&bc = glm::vec4(0,1,0,1),
        glm::vec4 const&cc = glm::vec4(0,0,1,1)
        ):Triangle(glm::vec4(a,1.f),glm::vec4(b,1.f),glm::vec4(c,1.f),ac,bc,cc){}
    Triangle(
        glm::vec4 const&a,
        glm::vec4 const&b,
        glm::vec4 const&c,
        glm::vec4 const&ac = glm::vec4(1,0,0,1),
        glm::vec4 const&bc = glm::vec4(0,1,0,1),
        glm::vec4 const&cc = glm::vec4(0,0,1,1)
        ):Element(TRIANGLE),a(a),b(b),c(c),ac(ac),bc(bc),cc(cc){}
    glm::vec4 a  = glm::vec4(0.f,0.f,0.f,1.f);
    glm::vec4 b  = glm::vec4(0.f,0.f,0.f,1.f);
    glm::vec4 c  = glm::vec4(0.f,0.f,0.f,1.f);
    glm::vec4 ac = glm::vec4(0.f,0.f,0.f,1.f);
    glm::vec4 bc = glm::vec4(0.f,0.f,0.f,1.f);
    glm::vec4 cc = glm::vec4(0.f,0.f,0.f,1.f);
};

Circle const&Element::toCircle()const{
  return *(Circle*)this;
}

Line   const&Element::toLine()const{
  return *(Line*)this;
}

Triangle const&Element::toTriangle()const{
  return *(Triangle*)this;
}

bool operator<(Circle const&a,Circle const&b){
  return a.center.z < b.center.z;
}

void project(std::vector<Element>&out,std::vector<Element>const&in,glm::mat4 const&m){
  for(auto const&e:in){
    if(e.type == Element::CIRCLE){
      auto c = e.toCircle();
      c.center = m * c.center;
      out.push_back(c);
    }
    if(e.type == Element::LINE){
      auto c = e.toLine();
      c.a = m * c.a;
      c.b = m * c.b;
      out.push_back(c);
    }
    if(e.type == Element::TRIANGLE){
      auto c = e.toTriangle();
      c.a = m * c.a;
      c.b = m * c.b;
      c.c = m * c.c;
      out.push_back(c);
    }
  }
}

using Range = std::tuple<float,float>;

Range clipLine(glm::vec4 const&a,glm::vec4 const&b,ClipPlane const&cp){
  //X(t) = A + t*(B-A)
  //-X(t)w < +X(t)i
  //-X(t)w < -X(t)i
  //
  //-X(t)w-X(t)i < 0
  //-X(t)w+X(t)i < 0
  //
  //-X(t)w+s*X(t)i < 0
  //
  //-Aw-t*(Bw-Aw) + s*(Ai+t*(Bi-Ai)) < 0
  //t*(-Bw+Aw+sBi-sAi) < Aw-sAi
  //t*M < N
  //M > 0 -> t < N/M
  //M < 0 -> t > N/M
  //M = 0 N > 0 -> any t
  //M = 0 N <= 0 -> non t
  int i = axis(cp);
  float s = sign(cp);
  float M = -b.w+a.w+s*b[i]-s*a[i];
  float N = a.w - s*a[i];
  if(M>0)return Range(0  ,glm::min(1.f,N/M));
  if(M>0)return Range(glm::max(0.f,N/M),1.f);
  if(M==0 && N > 0)return Range(0,1);
  return Range(1,0);
}

float interpolate(float a,float b,float l0,float l1,float h0,float h1){
  return (a*l0/h0+b*l1/h1) / (l0/h0 + l1/h1);
}


bool clip(Line&out,Line const&line,ClipPlane const&cp){
  auto range = clipLine(line.a,line.b,cp);
  auto const&t0 = std::get<0>(range);
  auto const&t1 = std::get<1>(range);
  if(t0 >= t1)return false;
  auto a = line.a + t0*(line.b-line.a);
  auto b = line.a + t1*(line.b-line.a);
    
  out = Line{a,b,line.width,line.color};
  return true;
}

bool clip(Line&out,Line line){
  if(!clip(line,out,ClipPlane::NEAR  ))return false;line=out;
  if(!clip(line,out,ClipPlane::FAR   ))return false;line=out;
  if(!clip(line,out,ClipPlane::LEFT  ))return false;line=out;
  if(!clip(line,out,ClipPlane::RIGHT ))return false;line=out;
  if(!clip(line,out,ClipPlane::BOTTOM))return false;line=out;
  if(!clip(line,out,ClipPlane::TOP   ))return false;line=out;
  return true;
}


bool valid(Range const&r){
  return std::get<0>(r) <= std::get<1>(r);
}

bool isVisible(glm::vec4 const&a){
  if(a.x < -a.w)return false;
  if(a.y < -a.w)return false;
  if(a.z < -a.w)return false;
  if(a.x > +a.w)return false;
  if(a.y > +a.w)return false;
  if(a.z > +a.w)return false;
  return true;
}


void clip(std::vector<Element>&out,std::vector<Element>const&in){
  for(auto const&e:in){
    if(e.type == Element::CIRCLE){
      auto c = e.toCircle();
      if(!isVisible(c.center))return;
      out.push_back(c);
    }
    if(e.type == Element::LINE){
      auto c = e.toLine();
      if(!clip(c,c))return;
      out.push_back(c);
    }
    if(e.type == Element::TRIANGLE){
    }
  }
}

enum CompareResult{
  LESS          ,
  EQUAL         ,
  GREATER       ,
  NOT_COMPARABLE,
};

CompareResult compareCircleCircle(
    Circle const&a,
    Circle const&b
    ){
  if(glm::distance(glm::vec2(a.center),glm::vec2(b.center)) > (a.radius + b.radius))return NOT_COMPARABLE;
  if(a.center.z < b.center.z)return LESS;
  if(a.center.z > b.center.z)return GREATER;
  return EQUAL;
}

float getParameterForClosestPointOnLineToPoint(
    glm::vec2 const&A,
    glm::vec2 const&B,
    glm::vec2 const&C){
  // L(t) = A(1-t)+Bt
  // C
  //
  // MIN[t]{(L(t)x - Cx)^2 + (L(t)y - Cy)^2}
  //
  // 2(L(t)x-Cx)(-Ax+Bx) + 2(L(t)y-Cy)(-Ay+By) = 0
  // 2(Bx-Ax)L(t)x + 2(By-Ay)L(t)y = 2Cx(Bx-Ax) + 2Cy(By-Ay)
  // (Bx-Ax)L(t)x + (By-Ay)L(t)y = Cx(Bx-Ax) + Cy(By-Ay)
  // (Bx-Ax)(Ax(1-t)+Bxt) + (By-Ay)(Ay(1-t)+Byt) = Cx(Bx-Ax) + Cy(By-Ay)
  // (Bx-Ax)Ax-(Bx-Ax)Axt+(Bx-Ax)Bxt + (By-Ay)Ay-(By-Ay)Ayt+(By-Ay)Byt = Cx(Bx-Ax) + Cy(By-Ay)
  // -(Bx-Ax)Axt + (Bx-Ax)Bxt - (By-Ay)Ayt + (By-Ay)Byt = Cx(Bx-Ax) + Cy(By-Ay) - (Bx-Ax)Ax - (By-Ay)Ay
  // (Ax-Bx)Axt + (Bx-Ax)Bxt + (Ay-By)Ayt + (By-Ay)Byt = (Bx-Ax)Cx + (By-Ay)Cy + (Ax-Bx)Ax + (Ay-By)Ay
  // t((Ax-Bx)Ax + (Bx-Ax)Bx + (Ay-By)Ay + (By-Ay)By) = (Bx-Ax)Cx + (By-Ay)Cy + (Ax-Bx)Ax + (Ay-By)Ay
  // t(AxAx - BxAx + BxBx - AxBx + AyAy - ByAy + ByBy - AyBy) = BxCx - AxCx + ByCy - AyCy + AxAx - BxAx + AyAy - ByAy
  // t(AxAx - 2AxBx + BxBx + AyAy - 2AyBy + ByBy) = BxCx - AxCx + ByCy - AyCy + AxAx - AxBx + AyAy - ByAy
  // t((Ax-Bx)^2 + (Ay-By)^2) = BxCx - AxCx + ByCy - AyCy + AxAx - AxBx + AyAy - ByAy
  // t((Ax-Bx)^2 + (Ay-By)^2) = (Bx-Ax)Cx + (By-Ay)*Cy + AxAx - AxBx + AyAy - ByAy
  // t((Ax-Bx)^2 + (Ay-By)^2) = (B-A)C + AxAx - AxBx + AyAy - ByAy
  // t((Ax-Bx)^2 + (Ay-By)^2) = (B-A)C + (Ax-Bx)Ax + (Ay-By)Ay
  // t((Ax-Bx)^2 + (Ay-By)^2) = (B-A)C + (A-B)A
  // t((A-B)*(A-B)) = (B-A)C + (A-B)A
  // t(A-B)^2 = (B-A)C + (A-B)A
  // t(A-B)^2 = (B-A)C - (B-A)A
  // t(A-B)^2 = (B-A)(C-A)
  // tM = N
  auto const M = glm::dot(A-B,A-B);
  auto const N = glm::dot(B-A,C-A);
  if(M == 0)return std::numeric_limits<float>::infinity();
  return N/M;
}

CompareResult compareLineCircle(Line const&line,Circle const&circle,float bias = 0.f){
  auto const&A = line.a;
  auto const&B = line.b;
  auto const&C = circle.center;
  auto const radius = circle.radius;
  auto const t = getParameterForClosestPointOnLineToPoint(A,B,C);
  if(t == std::numeric_limits<float>::infinity())return NOT_COMPARABLE;
  if(t < 0.f){
    if(glm::distance(glm::vec2(C),glm::vec2(A)) > radius)return NOT_COMPARABLE;
    if(A.z < C.z + bias)return LESS;
    if(A.z > C.z)return GREATER;
    return EQUAL;
  }
  if(t > 1.f){
    if(glm::distance(glm::vec2(C),glm::vec2(B)) > radius)return NOT_COMPARABLE;
    if(B.z < C.z + bias)return LESS;
    if(B.z > C.z)return GREATER;
    return EQUAL;
  }
  auto L = A*(1-t) + B*t;
  if(glm::distance(glm::vec2(L),glm::vec2(C)) > radius)return NOT_COMPARABLE;
  if(L.z < C.z + bias)return LESS;
  if(L.z > C.z)return GREATER;
  return EQUAL;
}

CompareResult operator!(CompareResult const&r){
  if(r == LESS)return GREATER;
  if(r == GREATER)return LESS;
  return r;
}

CompareResult compareCircleLine(Circle const&circle,Line const&line,float bias = 0.f){
  return !compareLineCircle(line,circle,bias);
}

bool operator<(Circle const&cir,Line const&line){
  auto const res = compareLineCircle(line,cir);
  if(res == NOT_COMPARABLE)return true;
  if(res == GREATER)return true;
  return false;
}

bool operator<(Line const&line,Circle const&cir){
  auto const res = compareLineCircle(line,cir);
  if(res == NOT_COMPARABLE)return true;
  if(res == LESS)return true;
  return false;
}

glm::vec2 perpendicular(glm::vec2 const&a){
  return glm::vec2(a.y,-a.x);
}

glm::vec3 points2Line(glm::vec2 const&a,glm::vec2 const&b){
  auto const n = perpendicular(b-a);
  return glm::vec3(n,-glm::dot(n,a));
}

float pointLineDistance(glm::vec3 const&l,glm::vec2 const&a){
  return glm::abs(glm::dot(glm::vec2(l),a)+l.z) / glm::length(glm::vec2(l));
}


float lineLineDistance(
    float&za,
    float&zb,
    glm::vec3 const&A,
    glm::vec3 const&B,
    glm::vec3 const&C,
    glm::vec3 const&D){
  auto const lab = points2Line(A,B);
  auto const lcd = points2Line(C,D);
  float distances[8];
  distances[0] = glm::length(glm::vec2(A-C));
  distances[1] = glm::length(glm::vec2(A-D));
  distances[2] = glm::length(glm::vec2(B-C));
  distances[3] = glm::length(glm::vec2(B-D));
  distances[4] = pointLineDistance(lab,C);
  distances[5] = pointLineDistance(lab,D);
  distances[6] = pointLineDistance(lcd,A);
  distances[7] = pointLineDistance(lcd,B);
  size_t minIndex = 0;
  for(size_t i=1;i<8;++i)
    if(distances[minIndex] > distances[i])
      minIndex = i;
  if(minIndex == 0){
    za = A.z;
    zb = C.z;
  }
  if(minIndex == 1){
    za = A.z;
    zb = D.z;
  }
  if(minIndex == 2){
    za = B.z;
    zb = C.z;
  }
  if(minIndex == 3){
    za = B.z;
    zb = D.z;
  }
  if(minIndex == 4){
    auto const t = getParameterForClosestPointOnLineToPoint(A,B,C);
    za = A.z + t*(B.z-A.z);
    zb = C.z;
  }
  if(minIndex == 5){
    auto const t = getParameterForClosestPointOnLineToPoint(A,B,D);
    za = A.z + t*(B.z-A.z);
    zb = D.z;
  }
  if(minIndex == 6){
    za = A.z;
    auto const t = getParameterForClosestPointOnLineToPoint(C,D,A);
    zb = C.z + t*(D.z-C.z);
  }
  if(minIndex == 7){
    za = B.z;
    auto const t = getParameterForClosestPointOnLineToPoint(C,D,B);
    zb = C.z + t*(D.z-C.z);
  }
  return distances[minIndex];
}

CompareResult compareLineLine(Line const&a,Line const&b){
  float za,zb;
  auto dis = lineLineDistance(za,zb,a.a,a.b,b.a,b.b);

  if(dis > (a.width+b.width))return NOT_COMPARABLE;
  if(za<zb)return LESS;
  if(za>zb)return GREATER;
  return EQUAL;
}

CompareResult compareCircleTriangle(Circle const&a,Triangle const&b){
  //TODO
  return NOT_COMPARABLE;
}

CompareResult compareLineTriangle(Line const&a,Triangle const&b){
  //TODO
  return NOT_COMPARABLE;
}

CompareResult compareTriangleCircle(Triangle const&a,Circle const&b){
  return !compareCircleTriangle(b,a);
}

CompareResult compareTriangleLine(Triangle const&a,Line const&b){
  return !compareLineTriangle(b,a);
}

CompareResult compareTriangleTriangle(Triangle const&a,Triangle const&b){
  //TODO
  return NOT_COMPARABLE;
}

CompareResult compare(std::unique_ptr<Element>const&a,std::unique_ptr<Element>const&b){
  if(a->type == Element::CIRCLE){
    if(b->type == Element::CIRCLE)
      return compareCircleCircle(a->toCircle(),b->toCircle());
    if(b->type == Element::LINE)
      return compareCircleLine(a->toCircle(),b->toLine(),0.005f);
    if(b->type == Element::TRIANGLE)
      return compareCircleTriangle(a->toCircle(),b->toTriangle());
    return NOT_COMPARABLE;
  }
  if(a->type == Element::LINE){
    if(b->type == Element::CIRCLE)
      return compareLineCircle(a->toLine(),b->toCircle(),0.001f);
    if(b->type == Element::LINE)
      return compareLineLine(a->toLine(),b->toLine());
    if(b->type == Element::TRIANGLE)
      return compareLineTriangle(a->toLine(),b->toTriangle());
    return NOT_COMPARABLE;
  }
  if(a->type == Element::TRIANGLE){
    if(b->type == Element::CIRCLE)
      return compareTriangleCircle(a->toTriangle(),b->toCircle());
    if(b->type == Element::LINE)
      return compareTriangleLine(a->toTriangle(),b->toLine());
    if(b->type == Element::TRIANGLE)
      return compareTriangleTriangle(a->toTriangle(),b->toTriangle());
    return NOT_COMPARABLE;
  }
  return NOT_COMPARABLE;
}

bool operator<(Line const&a,Line const&b){
  return false;
}

bool operator<(std::unique_ptr<Element>const&a,std::unique_ptr<Element>const&b){
  if(a->type == Element::CIRCLE){
    if(b->type == Element::CIRCLE)
      return a->toCircle() < b->toCircle();
    if(b->type == Element::LINE)
      return a->toCircle() < b->toLine();
    return false;
  }
  if(a->type == Element::LINE){
    if(b->type == Element::CIRCLE)
      return a->toLine() < b->toCircle();
    if(b->type == Element::LINE)
      return a->toLine() < b->toLine();
    return false;
  }
  return false;
}


class VectorScene{
  public:
    std::vector<std::unique_ptr<Element>>elements;
    void addCircle(Circle const&c){
      elements.emplace_back(std::make_unique<Circle>(c));
    }
    void addLine(Line const&l){
      elements.emplace_back(std::make_unique<Line>(l));
    }
    void addTriangle(Triangle const&t){
      elements.emplace_back(std::make_unique<Triangle>(t));
    }
};

void project(VectorScene&out,VectorScene const&in,Camera const&camera){
  auto mvp = camera.projection*camera.view;
  for(auto const&e:in.elements){
    if(e->type == Element::CIRCLE){
      auto cir = e->toCircle();
      auto center = mvp * cir.center;
      if(!isPointVisible(center))continue;
      auto newCen = glm::vec3(center)/center.w;
      auto newRad = mvp * glm::vec4(cir.radius,0,cir.center.z,1);
      auto newWid = mvp * glm::vec4(cir.width,0,cir.center.z,1);
      newCen.z = linearizeDepth(newCen.z,camera.near,camera.far);
      out.elements.emplace_back(std::make_unique<Circle>(newCen,newRad.x/newRad.w,newWid.x/newWid.w,cir.sColor,cir.fColor));
    }
    if(e->type == Element::LINE){
      auto line = e->toLine();
      auto A = mvp * line.a;
      auto B = mvp * line.b;
      glm::vec3 aa,bb;
      if(!line2NDC(aa,bb,A,B))continue;
      auto newWid = mvp * glm::vec4(line.width,0,(line.a.z+line.b.z)/2,1);
      aa.z = linearizeDepth(aa.z,camera.near,camera.far);
      bb.z = linearizeDepth(bb.z,camera.near,camera.far);
      out.elements.emplace_back(std::make_unique<Line>(aa,bb,newWid.x/newWid.w,line.color));
    }
    if(e->type == Element::TRIANGLE){
      auto triangle = e->toTriangle();
      auto A = mvp * triangle.a;
      auto B = mvp * triangle.b;
      auto C = mvp * triangle.c;
      //TODO do clipping
      glm::vec3 aa = glm::vec3(A) / A.w;
      glm::vec3 bb = glm::vec3(B) / B.w;
      glm::vec3 cc = glm::vec3(C) / C.w;

      aa.z = linearizeDepth(aa.z,camera.near,camera.far);
      bb.z = linearizeDepth(bb.z,camera.near,camera.far);
      bb.z = linearizeDepth(bb.z,camera.near,camera.far);
      cc.z = linearizeDepth(cc.z,camera.near,camera.far);
      out.elements.emplace_back(std::make_unique<Triangle>(aa,bb,cc,triangle.ac,triangle.bc,triangle.cc));
    }

  }
}

void sort(VectorScene&s){
  std::sort(s.elements.begin(),s.elements.end());
}

glm::vec2 viewportTransform(glm::vec2 const&v,glm::uvec2 const&size){
  return ((v* glm::vec2(1,-1) * 0.5f) + 0.5f) * glm::vec2(size) ;
}


std::vector<float>getSceneTriangles(aiScene const*const scene){
  std::vector<float>t;
  size_t nofVertices = 0;
  for(size_t i=0;i<scene->mNumMeshes;++i)
    nofVertices+=scene->mMeshes[i]->mNumFaces*3;
  t.reserve(nofVertices*3);
  for(size_t i=0;i<scene->mNumMeshes;++i){
    auto mesh = scene->mMeshes[i];
    for(size_t j=0;j<mesh->mNumFaces;++j)
      for(size_t k=0;k<3;++k)
        for(size_t l=0;l<3;++l)
          t.push_back(mesh->mVertices[mesh->mFaces[(uint32_t)j].mIndices[(uint32_t)k]][(uint32_t)l]);
  }
  return t;
}

class SceneEdges{
  public:
    SceneEdges(aiScene const*const scene){
      vertices = getSceneTriangles(scene);
      adjacency = std::make_shared<Adjacency const>(vertices,2);
    }
    std::vector<float>vertices;
    std::shared_ptr<Adjacency const>adjacency;
};

void projectEdges(SVG&svg,SceneEdges const&scene,Camera const&camera){
  auto mvp = camera.projection*camera.view;
  std::vector<glm::vec4>tVer;
  auto v = scene.vertices.data();
  for(size_t i=0;i<scene.vertices.size();i+=3)
    tVer.push_back(mvp * glm::vec4(v[i+0],v[i+1],v[i+2],1.f));

  auto const l = glm::vec3(0,10,0);
  auto adj = scene.adjacency;
  for(size_t e=0;e<adj->getNofEdges();++e){
    auto A = tVer[adj->getEdge(e,0)/3];
    auto B = tVer[adj->getEdge(e,1)/3];
    glm::vec3 aa,bb;
    int mult = 0;
    for(size_t i=0;i<adj->getNofOpposite(e);++i){
      auto a = glm::vec3(
          v[adj->getEdge(e,0)+0],
          v[adj->getEdge(e,0)+1],
          v[adj->getEdge(e,0)+2]);
      auto b = glm::vec3(
          v[adj->getEdge(e,1)+0],
          v[adj->getEdge(e,1)+1],
          v[adj->getEdge(e,1)+2]);
      auto o = glm::vec3(
          v[adj->getOpposite(e,i)+0],
          v[adj->getOpposite(e,i)+1],
          v[adj->getOpposite(e,i)+2]);
      auto n = glm::normalize(glm::cross(b-a,o-a));
      mult += glm::sign(glm::dot(n,l) - glm::dot(n,a));
    }
    if(mult == 0){
      if(line2NDC(aa,bb,A,B))svg.addLine(SVGLine(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(1,0,0),1));
    }else{
      if(line2NDC(aa,bb,A,B))svg.addLine(SVGLine(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(0,0,1),3));
    }
  }
  for(auto const&v:tVer){
    if(isPointVisible(v)){
      auto p = viewportTransform(v/v.w,camera.windowSize);
      svg.addCircle(SVGCircle(p,2,1,glm::vec3(0,0,0),glm::vec3(0,0,0)));
    }
  }
}

void project(SVG&svg,Scene const&scene,Camera const&camera){
  auto vis = [&](std::shared_ptr<Node>const&n,glm::mat4 const&transformation){
    auto mvp = transformation * n->transformation;
    for(auto const&m:n->meshes){
      std::vector<glm::vec4>tVer;
      for(auto const&v:m->vertices)
        tVer.push_back(mvp * glm::vec4(v,1.f));
      for(auto const&t:m->triangles){
        auto const A = tVer[t.x];
        auto const B = tVer[t.y];
        auto const C = tVer[t.z];
        glm::vec3 aa,bb;
        if(line2NDC(aa,bb,A,B))svg.addLine(SVGLine(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(1,0,0),1));
        if(line2NDC(aa,bb,B,C))svg.addLine(SVGLine(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(1,0,0),1));
        if(line2NDC(aa,bb,C,A))svg.addLine(SVGLine(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(1,0,0),1));
      }
      for(auto const&v:tVer){
        if(isPointVisible(v)){
          auto p = viewportTransform(v/v.w,camera.windowSize);
          svg.addCircle(SVGCircle(p,2,1,glm::vec3(0,0,0),glm::vec3(0,0,0)));
        }
      }
    }

  };
  scene.visitor(vis,camera.projection*camera.view);

}

class Voxel{
  public:
    glm::vec3 corners[8];
    bool isPlaneCollision(glm::vec4 const&p){
      bool inFront = false;
      bool behind = false;
      for(size_t i=0;i<8;++i){
        if(glm::dot(p,glm::vec4(corners[i],1.f))<0.f)behind = true;
        else inFront = true;
      }
      return inFront && behind;
    }
};

void computeCorners(glm::vec3*corners,glm::vec3 const&minAABB,glm::vec3 const&maxAABB){
  auto const size = (maxAABB - minAABB);
  for(size_t i=0;i<8;++i)
    corners[i] = minAABB + size*glm::vec3((i>>2)&1,(i>>1)&1,(i>>0)&1);
}

Voxel getVoxel(glm::vec3 const&minAABB,glm::vec3 const&maxAABB,size_t voxFac,glm::vec3 const&l){
  auto const size = (maxAABB - minAABB);
  auto const vSize = size / static_cast<float>(voxFac);
  auto const index = glm::floor(((l-minAABB)/size)*static_cast<float>(voxFac));
  Voxel result;
  for(size_t i=0;i<8;++i)
    result.corners[i] = vSize*index+vSize*glm::vec3((i>>2)&1,(i>>1)&1,(i>>0)&1);
  return result;
}

void saveTo(std::string const&name,VectorScene&aSpace,glm::uvec2 const&windowSize = glm::uvec2(1920,1080)){
  VectorScene ndc;
  auto svg = SVG(windowSize);
  auto camera = Camera(glm::vec3(5,5,5),glm::vec3(0,0,0),glm::vec3(0,1,0),windowSize,glm::half_pi<float>(),0.1f,1000.f);
  project(ndc,aSpace,camera);

  std::sort(ndc.elements.begin(),ndc.elements.end(),[](std::unique_ptr<Element>const&a,std::unique_ptr<Element>const&b)->bool{
    auto r = compare(a,b);
    if(r == LESS)return true;
    if(r == GREATER)return false;
    if(r == EQUAL){
      if(a->type == Element::CIRCLE && b->type == Element::LINE)return true;
      return false;
    }
    return false;
  });

  for(auto const&e:ndc.elements){
    if(e->type == Element::CIRCLE){
      auto const circle = e->toCircle();
      auto p = viewportTransform(circle.center,camera.windowSize);
      svg.addCircle(SVGCircle(p,circle.radius,circle.width,circle.sColor,circle.fColor));
    }
    if(e->type == Element::LINE){
      auto const line = e->toLine();
      auto a = viewportTransform(line.a,camera.windowSize);
      auto b = viewportTransform(line.b,camera.windowSize);
      svg.addLine(SVGLine(a,b,line.color,line.width));
    }
    if(e->type == Element::TRIANGLE){
      auto const triangle = e->toTriangle();
      auto a = viewportTransform(triangle.a,camera.windowSize);
      auto b = viewportTransform(triangle.b,camera.windowSize);
      auto c = viewportTransform(triangle.c,camera.windowSize);
      svg.addTriangle(SVGTriangle(a,b,c,triangle.ac,triangle.bc,triangle.cc));
    }
  }
  
  svg.save(name);
}

void saveModelsAsEdges(std::string const&name,aiScene const*const ascene){
  auto edges = SceneEdges(ascene);
  VectorScene aSpace;

  auto adj = edges.adjacency;
  for(size_t e=0;e<adj->getNofEdges();++e){
    auto v = edges.vertices.data();
    glm::vec3 aa,bb;
    int mult = 0;
    auto a = glm::vec4(
        v[adj->getEdge(e,0)+0],
        v[adj->getEdge(e,0)+1],
        v[adj->getEdge(e,0)+2],1);
    auto b = glm::vec4(
        v[adj->getEdge(e,1)+0],
        v[adj->getEdge(e,1)+1],
        v[adj->getEdge(e,1)+2],1);
    aSpace.addLine(Line(a,b,10,glm::vec3(0,0,0)));
  }

  saveTo(name,aSpace);
}

void saveModelsAsSilhouettes(std::string const&name,aiScene const*const ascene){
  auto edges = SceneEdges(ascene);
  VectorScene aSpace;

  auto const l = glm::vec3(0.f,10.f,0.f);
  auto adj = edges.adjacency;
  for(size_t e=0;e<adj->getNofEdges();++e){
    auto v = edges.vertices.data();
    glm::vec3 aa,bb;
    int mult = 0;
    auto a = glm::vec3(
        v[adj->getEdge(e,0)+0],
        v[adj->getEdge(e,0)+1],
        v[adj->getEdge(e,0)+2]);
    auto b = glm::vec3(
        v[adj->getEdge(e,1)+0],
        v[adj->getEdge(e,1)+1],
        v[adj->getEdge(e,1)+2]);
    for(size_t i=0;i<adj->getNofOpposite(e);++i){
      auto o = glm::vec3(
          v[adj->getOpposite(e,i)+0],
          v[adj->getOpposite(e,i)+1],
          v[adj->getOpposite(e,i)+2]);
      auto n = glm::normalize(glm::cross(b-a,o-a));
      mult += glm::sign(glm::dot(n,l) - glm::dot(n,a));
    }

    bool inCollision = false;
    for(size_t i=0;i<adj->getNofOpposite(e);++i){
      auto o = glm::vec3(
          v[adj->getOpposite(e,i)+0],
          v[adj->getOpposite(e,i)+1],
          v[adj->getOpposite(e,i)+2]);
      auto n = glm::normalize(glm::cross(b-a,o-a));
      auto p = glm::vec4(n,-glm::dot(n,a));
    }


      if(mult != 0)
        aSpace.addLine(Line(glm::vec4(a,1),glm::vec4(b,1),10,glm::vec3(1,0,0)));
  }
  saveTo(name,aSpace);
}

glm::vec3 toVec3(aiVector3D const&a){
  return glm::vec3(a.x,a.y,a.z);
}

void saveModel(std::string const&name,aiScene const*const ascene){
  VectorScene aSpace;

  glm::vec3 light = glm::vec3(10,10,10);
  for(size_t mm=0;mm<ascene->mNumMeshes;++mm){
    auto mesh = ascene->mMeshes[mm];
    auto material = ascene->mMaterials[mesh->mMaterialIndex];
    aiColor3D color (0.f,0.f,0.f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE,color);
    float opacity = 0.f;
    material->Get(AI_MATKEY_OPACITY,opacity);
    glm::vec3 clr = glm::vec3(color.r,color.g,color.b);

    for(size_t ff=0;ff<mesh->mNumFaces;++ff){
      auto&face = mesh->mFaces[ff];
      if(face.mNumIndices != 3)continue;
      auto a=toVec3(mesh->mVertices[face.mIndices[0]]);
      auto b=toVec3(mesh->mVertices[face.mIndices[1]]);
      auto c=toVec3(mesh->mVertices[face.mIndices[2]]);
      auto n = glm::normalize(glm::cross(b-a,c-a));
      auto l = glm::normalize(light-((a+b+c)/3.f));

      float df = glm::abs(glm::dot(n,l));
      auto cc = glm::vec4(df*clr,opacity);
      aSpace.addTriangle(Triangle(glm::vec4(a,1),glm::vec4(b,1),glm::vec4(c,1),cc));
    }
  }

  saveTo(name,aSpace);

}

void saveModelsAsPot(std::string const&name,aiScene const*const ascene){
  auto edges = SceneEdges(ascene);
  VectorScene aSpace;

  auto const l = glm::vec3(0.f,10.f,0.f);
  auto adj = edges.adjacency;

  glm::vec3 minAABB = glm::vec3(+1e10f);
  glm::vec3 maxAABB = glm::vec3(-1e10f);
  for(size_t i=0;i<edges.vertices.size();i+=3){
    auto v = edges.vertices.data();
    auto p = glm::vec3(v[i+0],v[i+1],v[i+2]);
    minAABB = glm::min(minAABB,p);
    maxAABB = glm::max(maxAABB,p);
  }

  glm::vec3 corners[8];
  computeCorners(corners,minAABB,maxAABB);

  float scale = 20.f;
  auto centerAABB = (minAABB + maxAABB)/2.f;
  auto halfAABB = (maxAABB-minAABB)/2.f;
  minAABB = centerAABB - halfAABB*scale;
  maxAABB = centerAABB + halfAABB*scale;
  size_t voxFac = 8;

  auto vox = getVoxel(minAABB,maxAABB,voxFac,l);

  if(
      l.x < minAABB.x || l.y < minAABB.y || l.z < minAABB.z ||
      l.x > maxAABB.x || l.y > maxAABB.y || l.z > maxAABB.z
      ){
    std::cerr << "SVETLO MIMO" << std::endl;
    exit(0);
  }

  for(size_t e=0;e<adj->getNofEdges();++e){
    auto v = edges.vertices.data();
    glm::vec3 aa,bb;
    int mult = 0;
    auto a = glm::vec3(
        v[adj->getEdge(e,0)+0],
        v[adj->getEdge(e,0)+1],
        v[adj->getEdge(e,0)+2]);
    auto b = glm::vec3(
        v[adj->getEdge(e,1)+0],
        v[adj->getEdge(e,1)+1],
        v[adj->getEdge(e,1)+2]);
    for(size_t i=0;i<adj->getNofOpposite(e);++i){
      auto o = glm::vec3(
          v[adj->getOpposite(e,i)+0],
          v[adj->getOpposite(e,i)+1],
          v[adj->getOpposite(e,i)+2]);
      auto n = glm::normalize(glm::cross(b-a,o-a));
      mult += glm::sign(glm::dot(n,l) - glm::dot(n,a));
    }

    bool inCollision = false;
    for(size_t i=0;i<adj->getNofOpposite(e);++i){
      auto o = glm::vec3(
          v[adj->getOpposite(e,i)+0],
          v[adj->getOpposite(e,i)+1],
          v[adj->getOpposite(e,i)+2]);
      auto n = glm::normalize(glm::cross(b-a,o-a));
      auto p = glm::vec4(n,-glm::dot(n,a));
      if(vox.isPlaneCollision(p))
        inCollision = true;
    }


    if(inCollision)
        aSpace.addLine(Line(glm::vec4(a,1),glm::vec4(b,1),8,glm::vec3(0,0,0)));
  }
  saveTo(name,aSpace);
}

void saveEdgesAsSVG(aiScene const*const ascene){
  auto edges = SceneEdges(ascene);
  auto windowSize = glm::uvec2(1920,1080);
  auto svg = SVG(windowSize);
  auto camera = Camera(glm::vec3(0,0,3.f),glm::vec3(0,0,0),glm::vec3(0,1,0),windowSize,glm::half_pi<float>(),0.1f,1000.f);
  VectorScene aSpace;


  glm::vec3 minAABB = glm::vec3(+1e10f);
  glm::vec3 maxAABB = glm::vec3(-1e10f);
  for(size_t i=0;i<edges.vertices.size();i+=3){
    auto v = edges.vertices.data();
    auto p = glm::vec3(v[i+0],v[i+1],v[i+2]);
    minAABB = glm::min(minAABB,p);
    maxAABB = glm::max(maxAABB,p);
  }


  glm::vec3 corners[8];
  computeCorners(corners,minAABB,maxAABB);

  float scale = 20.f;
  auto centerAABB = (minAABB + maxAABB)/2.f;
  auto halfAABB = (maxAABB-minAABB)/2.f;
  minAABB = centerAABB - halfAABB*scale;
  maxAABB = centerAABB + halfAABB*scale;
  size_t voxFac = 8;

  auto const l = glm::vec3(0.f,10.f,0.f);

  auto vox = getVoxel(minAABB,maxAABB,voxFac,l);

  /*
  for(size_t i=0;i<edges.vertices.size();i+=3){
    auto v = edges.vertices.data();
    aSpace.addCircle(Circle(glm::vec3(v[i+0],v[i+1],v[i+2]),3,1,glm::vec3(0,0,0),glm::vec3(0,0,0)));
  }
  // */
  if(
      l.x < minAABB.x || l.y < minAABB.y || l.z < minAABB.z ||
      l.x > maxAABB.x || l.y > maxAABB.y || l.z > maxAABB.z
      ){
    std::cerr << "SVETLO MIMO" << std::endl;
    exit(0);
  }

  //AABB
  /*
  for(size_t i=0;i<7;++i)
    for(size_t j=i+1;j<8;++j){
      size_t c = i^j;
      if(c == 3 || c == 5 || c == 6 || c == 7)continue;
      //std::cerr << "AABB" << std::endl;
      aSpace.addLine(Line(corners[i],corners[j],10,glm::vec3(0,0,1)));
    }
  // */
  /*
  auto cen = (corners[0]+corners[7])/2.f;
  for(auto&c:corners)c = (c-cen)*2.f + cen;
  for(size_t a=0;a<3;++a){
    //0 010 100
    //1 001 100
    //2 001 010
    //
    //((~(1<<a))>>0)&1
    glm::vec3 const line[3] = {
      glm::vec3(corners[7].x-corners[0].x,0,0),
      glm::vec3(0,corners[7].y-corners[0].y,0),
      glm::vec3(0,0,corners[7].z-corners[0].z),
      //(corners[7]-corners[0])*glm::vec3(1,0,0),
      //(corners[7]-corners[0])*glm::vec3(0,1,0),
      //(corners[7]-corners[0])*glm::vec3(0,0,1),
    };
    glm::vec3 const diffK[3] = {
      (corners[7]-corners[0])/static_cast<float>(voxFac)*glm::vec3(0,1,0),
      (corners[7]-corners[0])/static_cast<float>(voxFac)*glm::vec3(1,0,0),
      (corners[7]-corners[0])/static_cast<float>(voxFac)*glm::vec3(1,0,0),
    };
    glm::vec3 const diffL[3] = {
      (corners[7]-corners[0])/static_cast<float>(voxFac)*glm::vec3(0,0,1),
      (corners[7]-corners[0])/static_cast<float>(voxFac)*glm::vec3(0,0,1),
      (corners[7]-corners[0])/static_cast<float>(voxFac)*glm::vec3(0,1,0),
    };

    //std::cerr << "line[0] - " << line[0].x << " "<< line[0].y << " "<< line[0].z << std::endl;
    //std::cerr << "line[1] - " << line[1].x << " "<< line[1].y << " "<< line[1].z << std::endl;
    //std::cerr << "line[2] - " << line[2].x << " "<< line[2].y << " "<< line[2].z << std::endl;

    for(size_t k=0;k<=voxFac;++k){
      for(size_t l=0;l<=voxFac;++l){
        auto st = corners[0]+(diffK[a]*(float)k)+(diffL[a]*(float)l);
        auto en = st + line[a];
        aSpace.addLine(Line(st,en,8,glm::vec3(0,0,1)));
        //std::cerr << "k=" << k << " l=" << l << " - " << st.x << " " << st.y << " " << st.z << std::endl;

        //aSpace.addCircle(Circle(st,10,1,glm::vec3(0,0,1),glm::vec3(0,0,1)));

        //aSpace.addCircle(Circle(
        //      corners[0]+diffK[a]*(float)k+diffL[a]*(float)l+line[a],
        //      10,1,glm::vec3(0,0,1),glm::vec3(0,0,1)));
      }
    }
  }
  // */

  /*
  for(size_t i=0;i<8;++i)
    std::cerr << corners[i].x << " "<< corners[i].y << " "<< corners[i].z << std::endl;
  */



  auto adj = edges.adjacency;
  for(size_t e=0;e<adj->getNofEdges();++e){
    auto v = edges.vertices.data();
    glm::vec3 aa,bb;
    int mult = 0;
    auto a = glm::vec3(
        v[adj->getEdge(e,0)+0],
        v[adj->getEdge(e,0)+1],
        v[adj->getEdge(e,0)+2]);
    auto b = glm::vec3(
        v[adj->getEdge(e,1)+0],
        v[adj->getEdge(e,1)+1],
        v[adj->getEdge(e,1)+2]);
    for(size_t i=0;i<adj->getNofOpposite(e);++i){
      auto o = glm::vec3(
          v[adj->getOpposite(e,i)+0],
          v[adj->getOpposite(e,i)+1],
          v[adj->getOpposite(e,i)+2]);
      auto n = glm::normalize(glm::cross(b-a,o-a));
      mult += glm::sign(glm::dot(n,l) - glm::dot(n,a));
    }

    bool inCollision = false;
    for(size_t i=0;i<adj->getNofOpposite(e);++i){
      auto o = glm::vec3(
          v[adj->getOpposite(e,i)+0],
          v[adj->getOpposite(e,i)+1],
          v[adj->getOpposite(e,i)+2]);
      auto n = glm::normalize(glm::cross(b-a,o-a));
      auto p = glm::vec4(n,-glm::dot(n,a));
      if(vox.isPlaneCollision(p))
        inCollision = true;
    }


    aSpace.addLine(Line(a,b,10,glm::vec3(0,0,0)));
    /*
    if(inCollision){
      if(mult == 0){
        aSpace.addLine(Line(a,b,3,glm::vec3(0,0,0)));
      }else{
        aSpace.addLine(Line(a,b,8,glm::vec3(1,0,0)));
      }
    }else{
      //if(mult == 0){
      //  aSpace.addLine(Line(a,b,3,glm::vec3(0,0,0)));
      //}else{
      //  aSpace.addLine(Line(a,b,3,glm::vec3(0,1,0)));
      //}
    }
    // */
  }



  VectorScene ndc;
  project(ndc,aSpace,camera);

  //*
  std::sort(ndc.elements.begin(),ndc.elements.end(),[](std::unique_ptr<Element>const&a,std::unique_ptr<Element>const&b)->bool{
    auto r = compare(a,b);
    if(r == LESS)return true;
    if(r == GREATER)return false;
    if(r == EQUAL){
      if(a->type == Element::CIRCLE && b->type == Element::LINE)return true;
      return false;
    }
    return false;
  });
  // */
  //std::reverse(ndc.elements.begin(),ndc.elements.end());


  for(auto const&e:ndc.elements){
    if(e->type == Element::CIRCLE){
      auto const circle = e->toCircle();
      auto p = viewportTransform(circle.center,camera.windowSize);
      svg.addCircle(SVGCircle(p,circle.radius,circle.width,circle.sColor,circle.fColor));
    }
    if(e->type == Element::LINE){
      auto const line = e->toLine();
      auto a = viewportTransform(line.a,camera.windowSize);
      auto b = viewportTransform(line.b,camera.windowSize);
      svg.addLine(SVGLine(a,b,line.color,line.width));
    }
  }
  

  /*
  auto mvp = camera.projection*camera.view;
  std::vector<glm::vec4>tVer;
  auto v = edges.vertices.data();
  for(size_t i=0;i<edges.vertices.size();i+=3)
    tVer.push_back(mvp * glm::vec4(v[i+0],v[i+1],v[i+2],1.f));

  auto const l = glm::vec3(0,10,0);
  auto adj = edges.adjacency;
  for(size_t e=0;e<adj->getNofEdges();++e){
    auto A = tVer[adj->getEdge(e,0)/3];
    auto B = tVer[adj->getEdge(e,1)/3];
    glm::vec3 aa,bb;
    int mult = 0;
    for(size_t i=0;i<adj->getNofOpposite(e);++i){
      auto a = glm::vec3(
          v[adj->getEdge(e,0)+0],
          v[adj->getEdge(e,0)+1],
          v[adj->getEdge(e,0)+2]);
      auto b = glm::vec3(
          v[adj->getEdge(e,1)+0],
          v[adj->getEdge(e,1)+1],
          v[adj->getEdge(e,1)+2]);
      auto o = glm::vec3(
          v[adj->getOpposite(e,i)+0],
          v[adj->getOpposite(e,i)+1],
          v[adj->getOpposite(e,i)+2]);
      auto n = glm::normalize(glm::cross(b-a,o-a));
      mult += glm::sign(glm::dot(n,l) - glm::dot(n,a));
    }
    if(mult == 0){
      if(line2NDC(aa,bb,A,B))svg.addLine(SVGLine(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(1,0,0),1));
    }else{
      if(line2NDC(aa,bb,A,B))svg.addLine(SVGLine(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(0,0,1),3));
    }
  }
  for(auto const&v:tVer){
    if(isPointVisible(v)){
      auto p = viewportTransform(v/v.w,camera.windowSize);
      svg.addCircle(SVGCircle(p,2,1,glm::vec3(0,0,0),glm::vec3(0,0,0)));
    }
  }*/
  svg.save("test.svg");
}

int main(int argc,char*argv[]){
  auto args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  auto const sceneName = args->gets("--model","","model name");
  auto ascene = aiImportFile(sceneName.c_str(),aiProcess_Triangulate|aiProcess_GenNormals|aiProcess_SortByPType);
  saveModelsAsEdges("edges.svg",ascene);
  //saveModelsAsSilhouettes("silhouettes.svg",ascene);
  //saveModelsAsPot("potential.svg",ascene);
  //saveModel("model.svg",ascene);
  //saveEdgesAsSVG(ascene);
  /*
  auto scene = Scene(ascene);
  auto windowSize = glm::uvec2(1920,1080);
  auto svg = SVG(windowSize);
  auto camera = Camera(glm::vec3(0,0,3.f),glm::vec3(0,0,0),glm::vec3(0,1,0),windowSize,glm::half_pi<float>(),0.1f,1000.f);
  auto edges = SceneEdges(ascene);
  //project(svg,scene,camera);
  projectEdges(svg,edges,camera);
  svg.save("test.svg");
  */
}
