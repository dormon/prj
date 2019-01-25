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

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

class MeshData{
  public:
    MeshData(){}
    void load(aiMesh*mesh){
      for(size_t v=0;v<mesh->mNumVertices;++v){
        auto const vert = mesh->mVertices[v];
        auto const glmvec = glm::vec3(vert.x,vert.y,vert.z);
        vertices.push_back(glmvec);
      }
      for(size_t f=0;f<mesh->mNumFaces;++f){
        auto const face = mesh->mFaces[f];
        if(face.mNumIndices == 3){
          triangles.push_back(glm::uvec3(face.mIndices[0],face.mIndices[1],face.mIndices[2]));
        }
      }
    }
    MeshData(aiMesh*mesh){
      load(mesh);
    }
    std::vector<glm::vec3>vertices;
    std::vector<glm::uvec3>triangles;
};

class Node{
  public:
    Node(){}
    Node(aiNode*n,std::vector<std::shared_ptr<MeshData>>const&meshes){
      load(n,meshes);
    }
    void load(aiNode*n,std::vector<std::shared_ptr<MeshData>>const&ms){
      for(size_t m=0;m<n->mNumMeshes;++n)
        meshes.emplace_back(ms[n->mMeshes[m]]);
      for(size_t i=0;i<16;++i)
        transformation[i/4][i%4] = n->mTransformation[0][i];
      if(glm::mat4(0.f) == transformation)
        transformation = glm::mat4(1.f);
      for(size_t c=0;c<n->mNumChildren;++c)
        children.emplace_back(std::make_shared<Node>(n->mChildren[c],ms));
    }
    void visitor(std::function<void(std::shared_ptr<Node>const&,glm::mat4 const&)>const&f,glm::mat4 const&tt)const{
      for(auto const&c:children){
        f(c,tt);
        c->visitor(f,tt*transformation);
      }
    }

    glm::mat4 transformation;
    std::vector<std::shared_ptr<MeshData>>meshes;
    std::vector<std::shared_ptr<Node>>children;
};

class Scene{
  public:
    Scene(){}
    void load(aiScene const*scene){
      if(scene == nullptr)throw std::runtime_error("scene is nullptr");
      for(size_t i=0;i<scene->mNumMeshes;++i){
        auto const mesh = scene->mMeshes[i];
        meshes.emplace_back(std::make_shared<MeshData>(mesh));
      }
      root = std::make_shared<Node>(scene->mRootNode,meshes);
    }
    Scene(aiScene const*scene){
      load(scene);
    }
    void visitor(std::function<void(std::shared_ptr<Node>const&,glm::mat4 const&tt)>const&f,glm::mat4 const&tt)const{
      f(root,tt);
      root->visitor(f,tt);
    }
    std::shared_ptr<Node>root;
    std::vector<std::shared_ptr<MeshData>>meshes;
};

class Line{
  public:
    Line(glm::vec2 const&a,glm::vec2 const&b,glm::vec3 const&color = glm::vec3(1,0,0),float width = 1):a(a),b(b),color(color),width(width){}
    glm::vec2 a;
    glm::vec2 b;
    glm::vec3 color;
    float width;
};

class Circle{
  public:
    Circle(glm::vec2 const&p,float r,float w,glm::vec3 const&sC,glm::vec3 const&fC):pos(p),radius(r),width(w),sColor(sC),fColor(fC){}
    glm::vec2 pos;
    float radius;
    float width;
    glm::vec3 sColor;
    glm::vec3 fColor;
};

class SVG{
  public:
    glm::vec2 size;
    SVG(glm::vec2 const&size):size(size){}

    std::string attribf(std::string const&name,float value){
      std::stringstream ss;
      ss << name << "=" << "\"" << value <<"\"";
      return ss.str();
    }
    std::string attribs(std::string const&name,std::string const&value){
      std::stringstream ss;
      ss << name << "=" << "\"" << value <<"\"";
      return ss.str();
    }
    std::string width(float value){
      return attribf("width",value);
    }
    std::string height(float value){
      return attribf("height",value);
    }
    std::string rgb(glm::vec3 const&color){
      std::stringstream ss;
      ss << "rgb(";
      ss << uint32_t(color.x*255);
      ss << ",";
      ss << uint32_t(color.y*255);
      ss << ",";
      ss << uint32_t(color.z*255);
      ss << ")";
      return ss.str();
    }
    std::string stroke(glm::vec3 const&color){
      return attribs("stroke",rgb(color));
    }
    std::string fill(glm::vec3 const&color){
      return attribs("fill",rgb(color));
    }
    std::string strokeWidth(float value){
      return attribf("stroke-width",value);
    }

    std::string headerBegin(glm::vec2 const&s){
      std::stringstream ss;
      ss << "<svg ";
      ss << width(s.x);
      ss << " ";
      ss << height(s.y);
      ss << ">" << std::endl;
      return ss.str();
    }
    std::string headerEnd(){
      std::stringstream ss;
      ss << "</svg>";
      return ss.str();
    }
    std::string line(glm::vec2 const&a,glm::vec2 const&b,glm::vec3 const&color = glm::vec3(1,0,0),float width = 1){
      std::stringstream ss;
      ss << "<line ";
      ss << attribf("x1",a.x);
      ss << " ";
      ss << attribf("y1",a.y);
      ss << " ";
      ss << attribf("x2",b.x);
      ss << " ";
      ss << attribf("y2",b.y);
      ss << " ";
      ss << stroke(color);
      ss << " ";
      ss << strokeWidth(width);
      ss << "/>";
      return ss.str();
    }
    std::string circle(glm::vec2 const&pos,float r,float w,glm::vec3 const&sColor,glm::vec3 const&fColor){
      std::stringstream ss;
      ss << "<circle ";
      ss << attribf("cx",pos.x);
      ss << " ";
      ss << attribf("cy",pos.y);
      ss << " ";
      ss << attribf("r",r);
      ss << " ";
      ss << stroke(sColor);
      ss << " ";
      ss << strokeWidth(w);
      ss << " ";
      ss << fill(fColor);
      ss << "/>";
      return ss.str();
    }

    void save(std::string const&n){
      std::ofstream ss;
      ss.open(n);//, std::ios_base::app);
      ss << headerBegin(size);
      for(auto const&l:lines)
        ss << line(l.a,l.b,l.color,l.width) << std::endl;
      for(auto const&c:circles)
        ss << circle(c.pos,c.radius,c.width,c.sColor,c.fColor) << std::endl;
      ss << headerEnd();
    }
    std::vector<Line>lines;
    std::vector<Circle>circles;
    void addLine(Line const&l){
      lines.push_back(l);
    }
    void addCircle(Circle const&c){
      circles.push_back(c);
    }
};

class Camera{
  public:
    Camera(glm::vec3 const&pos,glm::vec3 const&tar,glm::vec3 const&up,glm::uvec2 const&ws,float fovy,float n,float f){
      view = glm::lookAt(pos,tar,up);
      projection = glm::perspectiveRH(fovy,static_cast<float>(ws.x)/static_cast<float>(ws.y),n,f);
      windowSize = ws;
    }
    glm::mat4 view;
    glm::mat4 projection;
    glm::uvec2 windowSize;
};

bool line2NDC(glm::vec2&ao,glm::vec2&bo,glm::vec4 const&A,glm::vec4 const&B){
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
  ao = glm::vec2(abmin) / abmin.w;
  bo = glm::vec2(abmax) / abmax.w;
  return true;
}

glm::vec2 viewportTransform(glm::vec2 const&v,glm::uvec2 const&size){
  return ((v * 0.5f) + 0.5f) * glm::vec2(size);
}

bool isPointVisible(glm::vec4 const&p){
  for(size_t i=0;i<3;++i)
    if(p[i] < -p[3] || p[i] > p[3])return false;
  return true;
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

  auto adj = scene.adjacency;
  for(size_t e=0;e<adj->getNofEdges();++e){
    auto A = tVer[adj->getEdge(e,0)/3];
    auto B = tVer[adj->getEdge(e,1)/3];
    glm::vec2 aa,bb;
    if(line2NDC(aa,bb,A,B))svg.addLine(Line(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(1,0,0),1));
  }
  for(auto const&v:tVer){
    if(isPointVisible(v)){
      auto p = viewportTransform(v/v.w,camera.windowSize);
      svg.addCircle(Circle(p,2,1,glm::vec3(0,0,0),glm::vec3(0,0,0)));
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
        glm::vec2 aa,bb;
        if(line2NDC(aa,bb,A,B))svg.addLine(Line(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(1,0,0),1));
        if(line2NDC(aa,bb,B,C))svg.addLine(Line(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(1,0,0),1));
        if(line2NDC(aa,bb,C,A))svg.addLine(Line(viewportTransform(aa,camera.windowSize),viewportTransform(bb,camera.windowSize),glm::vec3(1,0,0),1));
      }
      for(auto const&v:tVer){
        if(isPointVisible(v)){
          auto p = viewportTransform(v/v.w,camera.windowSize);
          svg.addCircle(Circle(p,2,1,glm::vec3(0,0,0),glm::vec3(0,0,0)));
        }
      }
    }

  };
  scene.visitor(vis,camera.projection*camera.view);

}

int main(int argc,char*argv[]){
  auto args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  auto const sceneName = args->gets("--model","","model name");
  auto ascene = aiImportFile(sceneName.c_str(),aiProcess_Triangulate|aiProcess_GenNormals|aiProcess_SortByPType);
  auto scene = Scene(ascene);
  auto windowSize = glm::uvec2(1920,1080);
  auto svg = SVG(windowSize);
  auto camera = Camera(glm::vec3(0,0,3.f),glm::vec3(0,0,0),glm::vec3(0,1,0),windowSize,glm::half_pi<float>(),0.1f,1000.f);
  auto edges = SceneEdges(ascene);
  //project(svg,scene,camera);
  projectEdges(svg,edges,camera);
  svg.save("test.svg");
}
