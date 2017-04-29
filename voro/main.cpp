#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<algorithm>
#include<glm/glm.hpp>

bool behindAll(glm::vec3 const& p,std::vector<glm::vec4>const&planes){
  for(auto x:planes)
    if(glm::dot(x,glm::vec4(p,1.f))>=0)return false;
  return true;
}

glm::vec4 newPlane(glm::vec3 s,glm::vec3 i){
  glm::vec3 n=glm::normalize(i-s);
  glm::vec3 half=(s+i)/2.f;
  return glm::vec4(n,-glm::dot(n,half));
}

std::vector<glm::vec4>computePlanes(std::vector<glm::vec3>const&points,unsigned id){
  std::vector<glm::vec4>planes;
  std::vector<glm::vec3>pp;
  glm::vec3 s=points[id];
  for(unsigned i=0;i<points.size();++i)
    if(i!=id)pp.push_back(points[i]);
  std::sort(pp.begin(),pp.end(),[s](glm::vec3 a,glm::vec3 b){return glm::distance(s,a)<glm::distance(s,b);});
  for(auto x:pp){
    glm::vec3 half=(x+s)/2.f;
    if(behindAll(half,planes))
      planes.push_back(newPlane(s,x));
  }
  return planes;
}

std::vector<std::vector<glm::vec4>>compute(std::vector<glm::vec3>const&points){
  std::vector<std::vector<glm::vec4>>result;
  for(unsigned i=0;i<points.size();++i)
    result.push_back(computePlanes(points,i));
  return result;
}

class Node{
  public:
    glm::vec3 p[2];
    float far[2]={0,0};
    float near[2]={10e10,1e10};
    Node*n[2]={nullptr,nullptr};
    Node(glm::vec3 p0,glm::vec3 p1){
      this->p[0]=p0;
      this->p[1]=p1;
    }
    ~Node(){
      for(int i=0;i<2;++i)if(this->n[i])delete this->n[i];
    }
    void insert(glm::vec3 b){
      float dd[2]={
        glm::distance(this->p[0],b),
        glm::distance(this->p[1],b)
      };
      int id=dd[0]>dd[1];
      if(this->far[id]<dd[id])
        this->far[id]=dd[id];
      if(this->near[id]>dd[id])
        this->near[id]=dd[id];
      if(!this->n[id])this->n[id]=new Node(this->p[id],b);
      else this->n[id]->insert(b);
    }
};

Node*buildTree(std::vector<glm::vec3>const&points){
  Node*root=new Node(points[0],points[1]);
  for(unsigned i=2;i<points.size();++i)
    root->insert(points[i]);
  return root;
}

glm::vec3 getNearestFartherThan(Node*root,glm::vec3 p,float minD,float maxD){
  /*
  if(!root)return p;
  glm::vec3 nn[2];
  for(int i=0;i<2;++i)
    if(root->n[i])nn[i]=getNearestFartherThan(root->n[i],p,minD,maxD);
    else{
      if(glm::distance(root->p[i],p)>minD)
        nn[i]=root->p[i];
      else
        nn[i]=p;
    }

  if(nn[0]==p){
    if(glm::distance(nn[1],p)>minD)
      return nn[1];
    else
      return p;
  }else{
    if(nn[1]==p){
      if(glm::distance(nn[0],p)>minD)
        return nn[0];
      else
        return p;
    }else{
      if(glm::distance(nn[0],p)<glm::distance(nn[1],p))
        if(glm::distance(nn[0],p)>minD)
          return nn[0];
        else
          return p;
      else
        if(glm::distance(nn[1],p)>minD)
          return nn[1];
        else
          return p;
    }
  }*/



  float dd[2]={
    glm::distance(root->p[0],p),
    glm::distance(root->p[1],p)
  };

  glm::vec3 nn[2];
  for(int i=0;i<2;++i)
    if(root->n[i]){
      if(dd[i]+maxD<root->near[i] || root->far[i]+maxD<dd[i] || dd[i]+root->far[i] < minD)
        nn[i]=p;
      else
        nn[i]=getNearestFartherThan(root->n[i],p,minD,maxD);
    }else{
      nn[i]=root->p[i];
    }


  if(nn[0]==p){
    if(nn[1]==p)
      return p;
    else
      if(glm::distance(nn[1],p)>minD)
        return nn[1];
      else
        return p;
  }else{
    if(nn[1]==p){
      if(glm::distance(nn[0],p)>minD)
        return nn[0];
      else
        return p;
    }else{
      if(glm::distance(nn[0],p)<glm::distance(nn[1],p)){
        if(glm::distance(nn[0],p)>minD)
          return nn[0];
        else
          return p;
      }else{
        if(glm::distance(nn[1],p)>minD)
          return nn[1];
        else
          return p;
      }
    }
  }
}

std::vector<glm::vec4>computePlanes1(glm::vec3 p,Node*tree){
  std::vector<glm::vec4>result;
  float minD=0;
  float maxD=10e10;
  do{
    std::cout<<minD<<std::endl;
    auto n=getNearestFartherThan(tree,p,minD,maxD);
    if(n==p)break;
    glm::vec3 half=(n+p)/2.f;
    if(behindAll(half,result))
      result.push_back(newPlane(p,n));
    minD=glm::distance(p,n);
    maxD=maxD;//TODO obalove teleso
    //std::cout<<"a"<<std::endl;
  }while(true);
  return result;
}

std::vector<std::vector<glm::vec4>>compute1(std::vector<glm::vec3>const&points){
  auto tree=buildTree(points);
  std::vector<std::vector<glm::vec4>>result;
  for(auto x:points)
    result.push_back(computePlanes1(x,tree));
  delete tree;
  return result;
}

float rnd(){
  return (float)rand()/(float)(RAND_MAX-1);
}

glm::vec3 randomPoint(glm::vec3 max){
  return glm::vec3(rnd(),rnd(),rnd())*max;
}

int main(){
  srand(1000);
  std::vector<glm::vec3>points;
  for(unsigned i=0;i<10;++i)
    points.push_back(randomPoint(glm::vec3(100.f,100.f,100.f)));
  auto r=compute(points);//2.8
  unsigned n=0;
  for(auto x:r){
    std::cout<<x.size()<<std::endl;
    n+=x.size();
  }
  std::cout<<n<<std::endl;
  return 0;
}





