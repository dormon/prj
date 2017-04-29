#version 440 core

layout(location=0)out vec4 fColor;

#define FOVY  0.7853981633974483
#define WIDTH 512

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

vec3 noise_bump(in uvec2 x,in uint M,in uint N,in float p){
  vec3 n;
  n.x=noise(x+uvec2(+1,+0),M,N,p)-noise(x+uvec2(-1,+0),M,N,p);
  n.y=noise(x+uvec2(+0,+1),M,N,p)-noise(x+uvec2(+0,-1),M,N,p);
  n.z=.1;
  return normalize(n);
}

float distanceToSource(in uvec2 x,in uint M,in uint N,in float p,in uint it,in uvec2 sourcePos){
  vec2 curPos=x;
  uint i=0;
  do{
    if(noise(uvec2(curPos)+sourcePos,4u,4u,1.7)>0.52)return float(i)/float(it);
    i++;
    curPos+=normalize(noise_bump(uvec2(curPos),M,N,p).xy);
  }while(i<it);
  return 1.;
}

#define BEGINGRADIENT(name)\
vec4 name(float t){\
  t=clamp(t,0,1);\
  const vec4 colors[]={

#define ENDGRADIENT\
  };\
  t*=colors.length()-1;\
  uint i=uint(floor(t));\
  uint j=i+1;\
  if(j>=colors.length()){\
    i=colors.length()-1;\
    j=i;\
  }\
  return mix(colors[i],colors[j],fract(t));\
}

BEGINGRADIENT(skyColors)
  vec4(0),
  vec4(.4,.4,1,1),
  vec4(.4,.4,1,1),
  vec4(.4,.4,1,1),
  vec4(.4,.4,1,1),
  vec4(.4,.4,1,1),
  vec4(.5,.3,0,1),
  vec4(.5,0,0,1),
  vec4(0),
ENDGRADIENT

BEGINGRADIENT(horizonColors)
  vec4(0,0,0,1),
  vec4(.5,.5,.5,1),
  vec4(1),
  vec4(1),
  vec4(1),
  vec4(1),
  vec4(1,1,.5,1),
  vec4(1,1,0,1),
  vec4(.5,0,0,1),
  vec4(0,0,0,1)
ENDGRADIENT

BEGINGRADIENT(sunColors)
  vec4(0,0,0,1),
  vec4(1),
  vec4(1),
  vec4(1),
  vec4(1),
  vec4(1,1,0,1),
  vec4(1,.7,0,1),
  vec4(0,0,0,1)
ENDGRADIENT

BEGINGRADIENT(fireColors)
  vec4(0,0,0,1),
  vec4(1,0,0,1),
  vec4(1,1,0,1),
  vec4(1,1,1,1)
ENDGRADIENT

#line 199

uniform uvec2 WindowSize;
uniform uint Depth;
uniform mat4 InverseProjectionViewMatrix;
uniform float Time;
uniform vec3 CameraPos;
uniform int mode=0;



const float pi=3.141592653589793;

vec4 fire(vec2 coord,float time){
  float rr=0;
  rr=(noise(vec3(coord*3000,time)+vec3(0,-time,0),8u)*.5+.5)* 
  (1-coord.y)*
  (1-pow(abs(-1+coord.x*2),2));
  return vec4(fireColors(rr));
}

vec4 lightning(vec2 coord,float time){
  float r=0;
  //r=sin(coord.x*100+noise(coord*1000)*30);
  r=pow(1-abs(coord.x-noise(vec3(coord*1000,time),8u)*.2-.5),200);
  return vec4(r);
}

vec2 warp(vec2 coord){
  return mod(coord,1);
}

float square(float x,float f){
  return step(f,mod(x,2*f));
}

vec2 repeat(vec2 coord,vec2 f){
  //coord.x+=square(coord.y,1/f.y)*.5;
  //coord=warp(coord);

  vec2 c=mod(coord,1/f)*f;
  return c;
}

float wee(vec2 c){
  return pow(clamp(1-abs(abs(c.y)-4*abs(c.x-.5)),0,1),40);
}

float sphereGrad(vec2 c){
  return clamp(1-2*length(c-.5),0,1);
}

vec4 lightning2(vec2 coord,float time){
  float r=0;
  coord+=vec2(noise(vec2(coord.x*1000,time*.1),8u),noise(vec2(coord.y*1000+100,time*.1),8u));
  vec2 c=repeat(coord,vec2(1,3));
  c=repeat(c,vec2(1<<int(coord.y*3),1));
  return vec4(sphereGrad(c));
  //return vec4(wee(c));
  c-=vec2(.5,.5);
  float n=noise(vec3(coord*1000,time*0),8u)*.5+.5;

  float rv=abs(.5-length(c));
  float n2=noise(vec3(coord*1000+100,time),8u)*.5+.5;
  r=pow(1-rv,10);
  r=(noise(c*1000,8u)*.5+.5);
  r=1-length(c);

  return vec4(r);
}

BEGINGRADIENT(marbleColors)
  vec4(0.5,.4,.4,1),
  vec4(0.6,.4,.4,1),
  vec4(.2,.1,.1,1)
ENDGRADIENT


vec4 lightningBall(vec2 coord,float time){
  float r=0;
  vec2 c=coord-.5;
  float angle=20*atan(c.x,c.y);
  r=pow(sin(angle+noise(vec3(coord*1000,time*10),8u)*10)*.5+.5,10+20*length(c));
  r+=pow(clamp(1-length(c),0,1),10);
  return vec4(r);
}

void main(){
  gl_FragDepth=1;
  vec2 coord=gl_FragCoord.xy/vec2(WindowSize);
  vec3 view = normalize((InverseProjectionViewMatrix*vec4(-1+2*coord,-1,1)).xyz);
  if(view.y<0){
    vec2 worldPos2=CameraPos.y/-view.y*view.xz+CameraPos.xz;
    vec2 pos2=mod(worldPos2,2);
    pos2=step(pos2,vec2(1));
    float circleSize=20;
    float circleMask=clamp(pow(clamp(circleSize-length(worldPos2),0,circleSize)/circleSize,1),0,1);
    float chessMask=float(pos2.x!=pos2.y);
    fColor=vec4(vec3(0,chessMask*.5,0)*circleMask,1);
  }
  else fColor=vec4(0,0,1,1);

  //CameraPos.y+t*view.y = 0
  float t=-CameraPos.y/view.y;
  if(t<0)fColor = vec4(0,0,1,1);
  else{
    vec2 worldPos2=CameraPos.xz+view.xz*t;
    vec2 pos2=mod(worldPos2,2);
    pos2=step(pos2,vec2(1));
    float circleSize=20;
    float circleMask=clamp(pow(clamp(circleSize-length(worldPos2),0,circleSize)/circleSize,1),0,1);
    float chessMask=float(pos2.x!=pos2.y);
    fColor=vec4(vec3(0,chessMask*.5,0)*circleMask,1);
  }
  return;
  

  float n=noise(uvec2(gl_FragCoord.xy)+0xfffff00u,8u,8u,1.7);
  fColor=vec4(sin(n*Time));
  //return;
  //float n=noise(vec3(gl_FragCoord.xy,Time)*0.1+1000.f,10u);
  //fColor=marbleColors(abs(sin(n*50)));
  //fColor=vec4(n>.49);
  //fColor=vec4(1-distanceToSource(uvec2(gl_FragCoord.xy),8u,4u,2.5,uint(mod(Time,100)),uvec2(1000)));
  fColor=vec4(noise_bump(uvec2(gl_FragCoord.xy)+0xfffff00u,8u,8u,1.7)*.5+.5,1);
  float time=Time*10;

  fColor=lightning(coord,time);
  //fColor=vec4(wee(coord));
  //fColor=lightningBall(coord,time);
  return;

}

