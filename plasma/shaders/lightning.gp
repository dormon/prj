#version 440

#define JOIN1(x,y) x##y
#define JOIN0(x,y) JOIN1(x,y)
#define JOIN(x,y)  JOIN0(x,y)

#define VEC1 float
#define VEC2 vec2
#define VEC3 vec3
#define VEC4 vec4

#define IVEC1 int
#define IVEC2 ivec2
#define IVEC3 ivec3
#define IVEC4 ivec4

#define UVEC1 uint
#define UVEC2 uvec2
#define UVEC3 uvec3
#define UVEC4 uvec4

#define VECXI(x,m,i) JOIN(x,JOIN(VECI,m(i)))
#define VECI1(i) 
#define VECI2(i) [i]
#define VECI3(i) [i]
#define VECI4(i) [i]

const uint UINT_NULA                   = 0u;
const uint UINT_JEDNA                  = 1u;
const uint UINT_DVA                    = 2u;
const uint UINT_DESET                  = 10u;
const uint UINT_DVACETTISICDVACETCTYRI = 20024u;
const uint UINT_MAXDELENODVEMA         = 0x7fffffffu;
const uint UINT_MAX                    = 0xffffffffu;

uint poly(in uint x,in uint c){
  //return x*(x*(x+c)+c);
  return x*(x*(x*(x*(x+c)+c)+c)+c);
}

#define BASE(DIM) \
float bnoise(in JOIN(UVEC,DIM) x){\
  uint last=UINT_DESET;\
  for(uint i=UINT_NULA;i<DIM##u;++i)\
    last=poly(VECXI(x,DIM,i)+(UINT_DVACETTISICDVACETCTYRI<<i),last);\
  return -1.+float(last)/float(UINT_MAXDELENODVEMA);\
}


#define SMOOTH(DIM)\
float snoise(in uint d,in JOIN(UVEC,DIM) x){\
  if(d==UINT_NULA)return bnoise(x);\
  uint dd=UINT_JEDNA<<d;\
  uint mm=(UINT_MAX>>d)+UINT_JEDNA;\
  JOIN(UVEC,DIM) xx=x>>d;\
  JOIN(VEC,DIM) t=JOIN(VEC,DIM)(x%dd)/JOIN(VEC,DIM)(dd);\
  float ret=0.;\
  for(uint i=UINT_NULA;i<(UINT_JEDNA<<DIM);++i){\
    float coef=1.;\
    JOIN(UVEC,DIM) o;\
    for(uint j=UINT_NULA;j<DIM##u;++j){\
      VECXI(o,DIM,j)=(i>>j)&UINT_JEDNA;\
      coef*=smoothstep(0,1,float(UINT_JEDNA-((i>>j)&UINT_JEDNA))*(1.-2.*VECXI(t,DIM,j))+VECXI(t,DIM,j));\
    }\
    ret+=bnoise((xx+o)%mm)*(coef);\
  }\
  return ret;\
}

#define OCTAVE(DIM)\
float noise(in JOIN(UVEC,DIM) x,in uint M,in uint N,float p){\
  float ret=0.;\
  float sum=0.;\
  for(uint k=UINT_NULA;k<=N;++k){\
    sum*=p;\
    sum+=1;\
    ret*=p;\
    ret+=snoise(M-k,x);\
  }\
  return ret/sum;\
}

#define OCTAVE_SIMPLIFIED(DIM)\
float noise(in JOIN(UVEC,DIM) x,in uint M){\
  return noise(x,M,M,2.);\
}

#define FNOISE(DIM)\
float noise(in JOIN(VEC,DIM) x,in uint M){\
  return noise(JOIN(UVEC,DIM)(JOIN(IVEC,DIM)(x)),M);\
}

#define INOISE(DIM)\
float noise(in JOIN(IVEC,DIM) x,in uint M,in uint N,float p){\
  return noise(JOIN(UVEC,DIM)(x),M,N,p);\
}

BASE(1)
BASE(2)
BASE(3)
BASE(4)

SMOOTH(1)
SMOOTH(2)
SMOOTH(3)
SMOOTH(4)

OCTAVE(1)
OCTAVE(2)
OCTAVE(3)
OCTAVE(4)

OCTAVE_SIMPLIFIED(1)
OCTAVE_SIMPLIFIED(2)
OCTAVE_SIMPLIFIED(3)
OCTAVE_SIMPLIFIED(4)

FNOISE(1)
FNOISE(2)
FNOISE(3)
FNOISE(4)

INOISE(1)
INOISE(2)
INOISE(3)
INOISE(4)


layout(points)in;
layout(triangle_strip,max_vertices=4)out;


uniform uint VERTICES=100;
flat in uint vID[];

uniform mat4 v;
uniform mat4 p;
uniform float Time;

const float PI = 3.1415925;

out vec2 gCoord;
out vec3 gWorldPosition;
out vec3 gA;
out vec3 gB;


vec3 computeOffset(float t,vec3 lv,vec3 tv){
  float r     = noise(uvec2(t*100,Time*3),8u,8u,1.7)*4;
  float angle = noise(uvec2(t*100,Time*3)+uvec2(1000),8u,8u,1.7)*PI;
  return (lv*cos(angle)+tv*sin(angle))*r;
}

vec3 computePosition(vec3 start,vec3 end,float t){
  vec3 sv=normalize(end-start);
  vec3 lv=normalize(cross(sv,sv+vec3(1,1,1)));
  vec3 tv=cross(sv,lv);
  return mix(start,end,t)+computeOffset(t,lv,tv);
}

void generateLightning(vec3 start,vec3 end,float t,float tPrev,float tNext){
  vec3 centerPosition     = computePosition(start,end,t    );
  vec3 centerPositionPrev = computePosition(start,end,tPrev);
  vec3 centerPositionNext = computePosition(start,end,tNext);

  vec4 viewPosition     = v*vec4(centerPosition    ,1);
  for(uint i=0;i<4u;++i){
    float size=1;
    vec4 viewPos = vec4(viewPosition.xyz+size*vec3((-1.f+2.f*(i%2)),(-1.f+2.f*(i/2)),0),viewPosition.w);
    gl_Position=p*viewPos;
    gWorldPosition=vec3(inverse(v)*viewPos);
    gA=centerPositionPrev;
    gB=centerPositionNext;
    
    gCoord=vec2(i%2,i/2);
    EmitVertex();
  }
  EndPrimitive();
}



void main(){


  float t    =float(vID[0]  )/float(VERTICES);
  float tPrev=float(vID[0]-1)/float(VERTICES);
  float tNext=float(vID[0]+1)/float(VERTICES);

  vec3 start=vec3(0,2,0);
  vec3 end  =vec3(0,0,0);

  generateLightning(start,end,t,tPrev,tNext);

}
