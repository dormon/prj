#include"collision.h"

using namespace coll;

float Collision::collision(glm::vec3&n,BoxSymbol*box,glm::vec3 a,glm::vec3 b){
  //(a/u,b/v)*(x,y)
  //a/u*x+b/v*y
  //(a,b)/(u,v)*(x,y)
  //(a,b)*(x,y)/(u,v)*(x,y)
  //(ax+by)/(ux+vy)
  //
  //X(t)=a+t*(b-a)
  //X(t).n-c.n>=0
  //(a+t*(b-a)).n-c.n>=0
  //(a.n-c.n)+t*(b.n-a.n)>=0
  //(a-c).n+t*(b-a).n>=0
  //-(c-a).n+t*(b-a).n>=0
  //-e+t*d>=0
  //t*(b-a).n>=(c-a).n
  //t*d>=e
  //t>=e/d d>0
  //t<=e/d d<0
  //
  //t>=-(a.n-c.n)/(b.n-a.n)
  //t>=-(a-c).n/(b-a).n
  //t>=-(a-c).n/m.n
  //a+t*b>=0
  //p + t*p >= 0 -> t>= -p/p
  //n + t*n >= 0 -> t>= -n/n
  if(a==b){
    //std::cerr<<"same";
    return 1;
    //TODO box
  }
  float mint=std::numeric_limits<float>::min();
  float maxt=std::numeric_limits<float>::max();
  glm::vec3 m=b-a;
  glm::vec3 c0=box->c;
  glm::vec3 c1=c0+box->x+box->y+box->z;

  float d[6];
  d[0]=glm::dot(m,box->x);
  d[1]=glm::dot(m,box->y);
  d[2]=glm::dot(m,box->z);
  d[3]=-d[0];
  d[4]=-d[1];
  d[5]=-d[2];

  float e[6];
  e[0]=glm::dot((c0-a),box->x);
  e[1]=glm::dot((c0-a),box->y);
  e[2]=glm::dot((c0-a),box->z);
  e[3]=glm::dot((c1-a),-box->x);
  e[4]=glm::dot((c1-a),-box->y);
  e[5]=glm::dot((c1-a),-box->z);

  int collisionK=0;
  for(int k=0;k<6;++k){
    if(d[k]>0){//min up
      if(e[k]/d[k]>mint){
        collisionK=k;
        mint=e[k]/d[k];
      }
    }else if(d[k]<0){//max down
      if(e[k]/d[k]<maxt){
        maxt=e[k]/d[k];
      }
    }else{

    }
  }
  //std::cerr<<mint<<" - "<<maxt<<std::endl;
  if(mint>=maxt)return 1;
  switch(collisionK){
    case 0:n=-glm::normalize(box->x);break;
    case 1:n=-glm::normalize(box->y);break;
    case 2:n=-glm::normalize(box->z);break;
    case 3:n= glm::normalize(box->x);break;
    case 4:n= glm::normalize(box->y);break;
    case 5:n= glm::normalize(box->z);break;
  }
  return mint;
}

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define pvec(a)\
  std::cerr<< #a <<": "<<a.x<<" "<<a.y<<" "<<a.z<<" "<<std::endl;


float Collision::sphereQuadPlane(
    glm::vec3 c,
    glm::vec3 x,
    glm::vec3 y,
    glm::vec3 n,
    glm::vec3 a,
    glm::vec3 b,
    float r){
  //X(t)=a+t*(b-a)
  //(X(t).n-c.n)/sqrt(n.n)=r
  //((a-c).n+t*(b-a).n)/sqrt(n.n)=r
  //t=(r*sqrt(n.n)+(c-a).n)/(b-a).n
  //t=A/B
  float A=glm::length(n)*r+glm::dot(c-a,n);
  float B=glm::dot(b-a,n);
  if(B==0)return 1;
  float t=A/B;
  if(t<0||t>1)return 1;
  glm::vec3 X=a+t*(b-a)-glm::normalize(n)*r-c;
  //pvec(c);
  //pvec(x);
  //pvec(y);
  //pvec(X);
  //pvec(c+x+y);
  //pvec(a+t*(b-a)-glm::normalize(n)*r);
  float xl=glm::length(x);
  float yl=glm::length(y);
  float u=glm::dot(X,x)/(xl*xl);
  float v=glm::dot(X,y)/(yl*yl);
  //std::cerr<<"u: "<<u<<" v: "<<v<<std::endl; 
  //std::cerr<<"xl: "<<xl<<" yl: "<<yl<<std::endl;
  if(u<0||u>1||v<0||v>1)return 1;
  return A/B;
}

//K(s,l,r)=C+s*X+l*Y+r*Z
//X(t)=a+t*(b-a)
//
//
//
//

float Collision::sphereSegment(
    glm::vec3 x,
    glm::vec3 y,
    glm::vec3 a,
    glm::vec3 b,
    float r){
  if(a==b)return 1;

  //X(t)=a+t*(b-a)
  //Y(l)=x+l*(y-x)
  //X(t)=a+t*u
  //Y(l)=x+l*v
  //(v,-v.X(t))
  //v.Y(l)-v.X(t)=0
  //v.x+l*v.v-v.a-v.u*t=0
  //v.(x-a)+v.v*l-v.u*t=0
  //t=(v.(x-a)+v.v*l)/v.u


  //X(t)=a+t*(b-a)
  //Y(l)=x+l*(y-x)
  //v=y-x
  //v.X(t)-v.Y(l)=0
  //v.a+t*v.(b-a)-v.Y(l)=0
  //v.(a-Y(l)) + t*v.(b-a) = 0
  //t=v.(Y(l)-a)/v.(b-a)
  //t=(v.Y(l)-v.a)/v.(b-a)
  //t=(v.x+l*v.(y-x)-v.a)/v.(b-a)
  //t=(v.(x-a)+l*v.(y-x))/v.(b-a)
  //t=(v.(x-a)+l*v.v)/v.(b-a)
  //t=(AA+l*BB)/CC
  //t=A+l*B
  //|X(A+l*B)Y(l)|=r
  //(a+(A+l*B)*(b-a)-x-l*(y-x))^2=r^2
  //((a-x)+A*(b-a)+l*(B*(b-a)-(y-x)))^2=r^2
  //(m+l*n)^2=r^2
  //m.m-r^2+2*m.n*l+n.n*l^2=0
  //cc+bb*l+aa*l^2=0
  //l=(-bb+-sqrt(bb^2-4*aa*cc))/2/aa
  glm::vec3 v=y-x;
  float AA=glm::dot(v,x-a);
  float BB=glm::dot(v,v  );
  float CC=glm::dot(v,b-a);
  if(glm::abs(CC)<0.0001){//lines are perpendicular
    //std::cerr<<"p";
    //(v,-v.a)
    //v.Y(l)-v.a=0
    //v.x+l*v.v-v.a=0
    //v.(x-a)+l*v.v=0
    //l=v.(a-x)/v.v
    //l=A/B
    float A=glm::dot(v,a-x);
    float B=glm::dot(v,v);

    float l=A/B;

    if(l>1)l=1;
    if(l<0)l=0;

    //|Y(l)X(t)|=r
    //(Y(l)-X(t))^2=r^2
    //(x+l*v-a-t*(b-a))^2=r^2
    //(x-a+l*v-t*(b-a))^2=r^2
    //(m-t*n)^2=r^2
    //n.n*t^2-2*m.n*t+m.m-r^2=0
    //aa*t^2+bb*t+cc=0

    if(glm::length(x+l*v-b)<r)return 0;
    glm::vec3 m=x-a+l*v;
    glm::vec3 n=b-a;
    float aa=glm::dot(n,n);
    float bb=-2*glm::dot(m,n);
    float cc=glm::dot(m,m)-r*r;
    float dd=bb*bb-4*aa*cc;
    if(aa==0){//not moving
      if(bb==0){
        //std::cerr<<"B";
        return 1;//TODO
      }else{
        float t=-cc/bb;
        if(t<0||t>1){std::cerr<<"C";return 1;}
        //std::cerr<<"k0";
        return t;
      }
    }else{
      //std::cerr<<"dd: "<<dd<<std::endl;
      //std::cerr<<glm::dot(v,b-a)<<std::endl;
      if(dd>-0.0001)dd=0;
      if(dd<0)return 1;
      float t0=(-bb-sqrt(dd))/2/aa;
      float t1=(-bb+sqrt(dd))/2/aa;
      //std::cerr<<"k1 t0: "<<t0<<" t1: "<<t1<<std::endl;
      if(t0<0||t0>1)t0=1;
      if(t1<0||t1>1)t1=1;
      return glm::min(t0,t1);
    }
  }
  float A=AA/CC;
  float B=BB/CC;
  glm::vec3 m=a-x+A*(b-a);
  glm::vec3 n=B*(b-a)-(y-x);
  float aa=glm::dot(n,n);
  float bb=glm::dot(m,n)*2.f;
  float cc=glm::dot(m,m)-r*r;
  float dd=bb*bb-4*aa*cc;
  if(aa==0){//lines are parallel
    if(bb==0){//lines do no cross
      /*std::cerr<<"D";*/
      return 1;//TODO
    }else{
      float l=-cc/bb;
      float t=A+l*B;
      if(t<0||t>1){/*std::cerr<<"E";*/return 1;}
      //std::cerr<<"k2";
      return t;
    }
  }else{
    if(dd<0){/*std::cerr<<"F";*/return 1;}
    float l0=(-bb-sqrt(dd))/2/aa;
    float l1=(-bb+sqrt(dd))/2/aa;
    float t0=A+l0*B;
    float t1=A+l1*B;
    if(t0<0||t0>1||l0<0||l0>1)t0=1;
    if(t1<0||t1>1||l1<0||l1>1)t1=1;
    //std::cerr<<"k3";
    return glm::min(t0,t1);
  }
}

float Collision::spherePoint(
    glm::vec3 x,
    glm::vec3 a,
    glm::vec3 b,
    float r){

  //X(t)=a+t*(b-a)
  //|X(t)x|=r
  //(X(t)-x)^2=r^2
  //(a-x+t*(b-a))^2=r^2
  //(m+t*n)^2=r^2
  //n.n*t^2+2*m.n*t+m.m-r^2=0
  //aa*t^2+bb*t+cc=0
  glm::vec3 m=a-x;
  glm::vec3 n=b-a;
  float aa=glm::dot(n,n);
  float bb=glm::dot(m,n)*2;
  float cc=glm::dot(m,m)-r*r;
  float dd=bb*bb-4*aa*cc;
  if(aa==0){
    return 1;
  }else{
    float t0=(-bb-sqrt(dd))/2/aa;
    float t1=(-bb+sqrt(dd))/2/aa;
    if(t0<0||t0>1)t0=1;
    if(t1<0||t1>1)t1=1;
    return glm::min(t0,t1);
  }
}


float Collision::collision(glm::vec3&n,BoxSymbol*box,glm::vec3 a,glm::vec3 b,float r){
  //glm::vec3 half=(box->x+box->y+box->z)/2.f;
  //glm::vec3 center=box->c+half;
  float minqp=1;
  
  glm::vec3 c1=box->c+box->x+box->y+box->z;
  float qp[6];
  for(int k=0;k<6;++k)qp[k]=1;
  qp[0]=this->sphereQuadPlane(box->c,box->y,box->x,-box->z,a,b,r);
  qp[1]=this->sphereQuadPlane(box->c,box->x,box->z,-box->y,a,b,r);
  qp[2]=this->sphereQuadPlane(box->c,box->z,box->y,-box->x,a,b,r);
  qp[3]=this->sphereQuadPlane(c1,-box->x,-box->y,box->z,a,b,r);
  qp[4]=this->sphereQuadPlane(c1,-box->z,-box->x,box->y,a,b,r);
  qp[5]=this->sphereQuadPlane(c1,-box->y,-box->z,box->x,a,b,r);
  for(int k=0;k<6;++k){
    if(qp[k]<minqp){
      minqp=qp[k];
      switch(k){
        case 0:n=-glm::normalize(box->x);break;
        case 1:n=-glm::normalize(box->y);break;
        case 2:n=-glm::normalize(box->z);break;
        case 3:n= glm::normalize(box->x);break;
        case 4:n= glm::normalize(box->y);break;
        case 5:n= glm::normalize(box->z);break;
      }
    }
  }
  if(minqp<1)return minqp;
  
  float ls[12]={1};
  for(int k=0;k<12;++k)ls[k]=1;
  ls[0]=this->sphereSegment(box->c,box->c+box->x,a,b,r);
  ls[1]=this->sphereSegment(box->c,box->c+box->y,a,b,r);
  ls[2]=this->sphereSegment(box->c,box->c+box->z,a,b,r);
  ls[3]=this->sphereSegment(box->c+box->x,box->c+box->x+box->y,a,b,r);
  ls[4]=this->sphereSegment(box->c+box->x,box->c+box->x+box->z,a,b,r);
  ls[5]=this->sphereSegment(box->c+box->y,box->c+box->y+box->x,a,b,r);
  ls[6]=this->sphereSegment(box->c+box->y,box->c+box->y+box->z,a,b,r);
  ls[7]=this->sphereSegment(box->c+box->x+box->y,box->c+box->x+box->y+box->z,a,b,r);
  ls[8]=this->sphereSegment(box->c+box->z,box->c+box->z+box->x,a,b,r);
  ls[9]=this->sphereSegment(box->c+box->z,box->c+box->z+box->y,a,b,r);
  ls[10]=this->sphereSegment(box->c+box->z+box->x,box->c+box->z+box->x+box->y,a,b,r);
  ls[11]=this->sphereSegment(box->c+box->z+box->y,box->c+box->z+box->x+box->y,a,b,r);
  for(int k=0;k<12;++k){
    if(ls[k]<minqp){
      minqp=ls[k];
      switch(k){
        case  0:n=glm::normalize(-glm::normalize(box->z)-glm::normalize(box->y));break;
        case  1:n=glm::normalize(-glm::normalize(box->z)-glm::normalize(box->x));break;
        case  2:n=glm::normalize(-glm::normalize(box->x)-glm::normalize(box->y));break;
        case  3:n=glm::normalize( glm::normalize(box->x)-glm::normalize(box->z));break;
        case  4:n=glm::normalize( glm::normalize(box->x)-glm::normalize(box->y));break;
        case  5:n=glm::normalize(-glm::normalize(box->z)+glm::normalize(box->y));break;
        case  6:n=glm::normalize(-glm::normalize(box->x)+glm::normalize(box->y));break;
        case  7:n=glm::normalize( glm::normalize(box->x)+glm::normalize(box->y));break;
        case  8:n=glm::normalize( glm::normalize(box->z)-glm::normalize(box->y));break;
        case  9:n=glm::normalize( glm::normalize(box->z)-glm::normalize(box->x));break;
        case 10:n=glm::normalize( glm::normalize(box->z)+glm::normalize(box->x));break;
        case 11:n=glm::normalize( glm::normalize(box->z)+glm::normalize(box->y));break;
      }
    }
  }
  if(minqp<1)return minqp;
  
  float pp[8];
  for(int k=0;k<8;++k)pp[k]=1;
  pp[0]=this->spherePoint(box->c,a,b,r);
  pp[1]=this->spherePoint(box->c+box->x,a,b,r);
  pp[2]=this->spherePoint(box->c+box->y,a,b,r);
  pp[3]=this->spherePoint(box->c+box->x+box->y,a,b,r);
  pp[4]=this->spherePoint(box->c+box->z,a,b,r);
  pp[5]=this->spherePoint(box->c+box->z+box->x,a,b,r);
  pp[6]=this->spherePoint(box->c+box->z+box->y,a,b,r);
  pp[7]=this->spherePoint(box->c+box->z+box->x+box->y,a,b,r);
  for(int k=0;k<8;++k){
    if(pp[k]<minqp){
      minqp=pp[k];
      switch(k){
        case 0:n=glm::normalize(-glm::normalize(box->x)-glm::normalize(box->y)-glm::normalize(box->z));
        case 1:n=glm::normalize( glm::normalize(box->x)-glm::normalize(box->y)-glm::normalize(box->z));
        case 2:n=glm::normalize(-glm::normalize(box->x)+glm::normalize(box->y)-glm::normalize(box->z));
        case 3:n=glm::normalize( glm::normalize(box->x)+glm::normalize(box->y)-glm::normalize(box->z));
        case 4:n=glm::normalize(-glm::normalize(box->x)-glm::normalize(box->y)+glm::normalize(box->z));
        case 5:n=glm::normalize( glm::normalize(box->x)-glm::normalize(box->y)+glm::normalize(box->z));
        case 6:n=glm::normalize(-glm::normalize(box->x)+glm::normalize(box->y)+glm::normalize(box->z));
        case 7:n=glm::normalize( glm::normalize(box->x)+glm::normalize(box->y)+glm::normalize(box->z));
      }
    }
  }
  return minqp;

}

float Collision::collision(glm::vec3&n,glm::vec3 a,glm::vec3 b,float r){
  glm::vec3 normal;
  float t=1;
  for(unsigned i=0;i<this->boxes.size();++i){
    float tNew=this->collision(normal,this->boxes[i],a,b,r);
    if(tNew<t){
      t=tNew;
      n=normal;
    }
  }
  return t;
}


float Collision::collision(glm::vec3&n,glm::vec3 a,glm::vec3 b){
  glm::vec3 normal;
  float t=1;
  for(unsigned i=0;i<this->boxes.size();++i){
    float tNew=this->collision(normal,this->boxes[i],a,b);
    if(tNew<t){
      t=tNew;
      n=normal;
    }
  }
  return t;
}

