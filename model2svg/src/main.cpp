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

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

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

class Element{
  public:
    virtual ~Element(){}
    enum Type{
      CIRCLE,
      LINE  ,
    }type;
    Element(Type t):type(t){}
    Circle const&toCircle()const;
    Line   const&toLine()const;

};

class Circle: public Element{
  public:
    Circle(glm::vec3 const&c,float r,float w,glm::vec3 const&sC,glm::vec3 const&fC):Element(CIRCLE),center(c),radius(r),width(w),sColor(sC),fColor(fC){}
    glm::vec3 center;
    float     radius;
    float     width;
    glm::vec3 sColor;
    glm::vec3 fColor;
};

class Line: public Element{
  public:
    Line(glm::vec3 const&a,glm::vec3 const&b,float w,glm::vec3 const&c):Element(LINE),a(a),b(b),width(w),color(c){}
    glm::vec3 a;
    glm::vec3 b;
    float     width;
    glm::vec3 color;
};

Circle const&Element::toCircle()const{
  return *(Circle*)this;
}

Line   const&Element::toLine()const{
  return *(Line*)this;
}

bool operator<(Circle const&a,Circle const&b){
  return a.center.z < b.center.z;
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

CompareResult compare(std::unique_ptr<Element>const&a,std::unique_ptr<Element>const&b){
  if(a->type == Element::CIRCLE){
    if(b->type == Element::CIRCLE)
      return compareCircleCircle(a->toCircle(),b->toCircle());
    if(b->type == Element::LINE){
      auto r = compareLineCircle(b->toLine(),a->toCircle(),0.005f);
      if(r == LESS)return GREATER;
      if(r == GREATER)return LESS;
      return r;
    }
    return NOT_COMPARABLE;
  }
  if(a->type == Element::LINE){
    if(b->type == Element::CIRCLE)
      return compareLineCircle(a->toLine(),b->toCircle(),0.001f);
    if(b->type == Element::LINE)
      return compareLineLine(a->toLine(),b->toLine());
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
};

void project(VectorScene&out,VectorScene const&in,Camera const&camera){
  auto mvp = camera.projection*camera.view;
  for(auto const&e:in.elements){
    if(e->type == Element::CIRCLE){
      auto cir = e->toCircle();
      auto center = mvp * glm::vec4(cir.center,1);
      if(!isPointVisible(center))continue;
      auto newCen = glm::vec3(center)/center.w;
      auto newRad = mvp * glm::vec4(cir.radius,0,cir.center.z,1);
      auto newWid = mvp * glm::vec4(cir.width,0,cir.center.z,1);
      newCen.z = linearizeDepth(newCen.z,camera.near,camera.far);
      out.elements.emplace_back(std::make_unique<Circle>(newCen,newRad.x/newRad.w,newWid.x/newWid.w,cir.sColor,cir.fColor));
    }
    if(e->type == Element::LINE){
      auto line = e->toLine();
      auto A = mvp * glm::vec4(line.a,1);
      auto B = mvp * glm::vec4(line.b,1);
      glm::vec3 aa,bb;
      if(!line2NDC(aa,bb,A,B))continue;
      auto newWid = mvp * glm::vec4(line.width,0,(line.a.z+line.b.z)/2,1);
      aa.z = linearizeDepth(aa.z,camera.near,camera.far);
      bb.z = linearizeDepth(bb.z,camera.near,camera.far);
      out.elements.emplace_back(std::make_unique<Line>(aa,bb,newWid.x/newWid.w,line.color));
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

  for(size_t i=0;i<edges.vertices.size();i+=3){
    auto v = edges.vertices.data();
    aSpace.addCircle(Circle(glm::vec3(v[i+0],v[i+1],v[i+2]),6,1,glm::vec3(0,0,0),glm::vec3(0,0,0)));
  }
  if(
      l.x < minAABB.x || l.y < minAABB.y || l.z < minAABB.z ||
      l.x > maxAABB.x || l.y > maxAABB.y || l.z > maxAABB.z
      ){
    std::cerr << "SVETLO MIMO" << std::endl;
    exit(0);
  }

  for(size_t i=0;i<7;++i)
    for(size_t j=i+1;j<8;++j){
      size_t c = i^j;
      if(c == 3 || c == 5 || c == 6 || c == 7)continue;
      std::cerr << "AABB" << std::endl;
      aSpace.addLine(Line(corners[i],corners[j],3,glm::vec3(1,1,0)));
    }
  for(size_t i=0;i<8;++i)
    std::cerr << corners[i].x << " "<< corners[i].y << " "<< corners[i].z << std::endl;



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


    //aSpace.addLine(Line(a,b,3,glm::vec3(0,0,0)));
    //*
    if(inCollision){
      if(mult == 0){
        aSpace.addLine(Line(a,b,9,glm::vec3(1,0,0)));
      }else{
        aSpace.addLine(Line(a,b,9,glm::vec3(1,1,0)));
      }
    }else{
      if(mult == 0){
        aSpace.addLine(Line(a,b,3,glm::vec3(0,0,0)));
      }else{
        aSpace.addLine(Line(a,b,3,glm::vec3(0,1,0)));
      }
    }
    // */
  }



  VectorScene ndc;
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
  saveEdgesAsSVG(ascene);
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
