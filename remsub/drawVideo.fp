#version 450

#define WGS 16

layout(local_size_x=WGS,local_size_y=WGS)in;

#define MAX_CLIPS 8

layout(        binding=0)uniform usampler2D tex       [8];
layout(rgba8ui,binding=0)uniform uimage2D   finalFrame   ;

layout(std430,binding=0)volatile buffer AuxBuffer{
  uint  jobCounter;
};

uniform ivec2 tileSize = ivec2(16,16);

int divRoundUp(in int x,in int y){
  return (x/y) + int((x%y)!=0);
}

ivec2 divRoundUp(in ivec2 x,in ivec2 y){
  return ivec2(divRoundUp(x.x,y.x),divRoundUp(x.y,y.y));
}

ivec2 getGridSize(){
  ivec2 frameSize = ivec2(720,480);//imageSize(finalFrame);
  return divRoundUp(frameSize,tileSize);
}

uint getNofJobs(){
  ivec2 count = getGridSize();
  return uint(count.x*count.y);
}

ivec2 getJobCoord(in uint job){
  ivec2 count = getGridSize();
  return ivec2((job%count.x)*tileSize.x,(job/count.x)*tileSize.y);
}

ivec2 getThreadCoord(in uint job){
  ivec2 jobCoord = getJobCoord(job);
  return jobCoord + ivec2(gl_LocalInvocationID.xy) - (ivec2(gl_WorkGroupSize.xy) - tileSize)/2;
}

void writeColor(ivec2 coord,in vec3 color){
  imageStore(finalFrame,coord,uvec4(clamp(vec4(color,1),vec4(0),vec4(1))*255));
}

uvec3 thresholdRGB(uvec3 c,uint t){
  return uvec3(c.x>t,c.y>t,c.z>t)*255;
}
uvec3 filterNonWhite(uvec3 c){
  return uvec3(all(equal(c,uvec3(255))))*255;
}

#define COPY2(x) x,x
#define COPY4(x) COPY2(x),COPY2(x)
#define COPY8(x) COPY4(x),COPY4(x)

const float colorDistanceBase = 1000;

uniform uint activeClip = 1u;

uniform uint  drawMode            = 7;
uniform bool  drawDiff            = false;
uniform uint  threshold           = 230;
uniform float colorDistance       = float(557);

uniform vec4  subBox                 = vec4(.1,.9,.5,.95);
uniform vec4  offsetScale[MAX_CLIPS] = {COPY8(vec4(0.f,0.f,1.f,1.f))};
uniform float contrast   [MAX_CLIPS] = {COPY8(1.f)};

//bool isSub(vec2 pixel){
//  uvec3 color = texture(tex[0],pixel).rgb;
//  return color.x > threshold && color.y > threshold && color.z > threshold;
//}

void applyContract(inout vec3 color,float c){
  color = clamp((color-.5f)*c,-.5f,.5f)+.5;
}

vec3 readStream(uint id,vec2 coord){
  vec2 offset = offsetScale[id].xy;
  vec2 scale  = offsetScale[id].zw;
  uvec3 uColor = texture(tex[id],coord*scale+offset).rgb;
  vec3 color = vec3(uColor) / vec3(255);
  applyContract(color,contrast[id]);
  return color;
}

vec3 loadBase(vec2 coord){
  return readStream(0,coord);
}

vec3 loadHelp(vec2 coord){
  if((activeClip & (1u<<1u)) != 0)
    return readStream(1,coord);
  if((activeClip & (1u<<2u)) != 0)
    return readStream(2,coord);
  return vec3(0);
}

float intensity(in vec3 color){
  return (color.x+color.y+color.z)/3.f;
}

#define FULL_TILE 22
#define PADDING ((FULL_TILE-WGS)/2)

shared float isPixelSub[FULL_TILE][FULL_TILE];
uniform float subIntensity = 0.8;

uniform int dilatation=2;

void loadIsSubToShared(float isSub,uint job){
  ivec2 loc = ivec2(gl_LocalInvocationID.xy);
  isPixelSub[PADDING+loc.x][PADDING+loc.y] = isSub;

#if PADDING != 0

  ivec2 tileCoord = getJobCoord(job);

  int newlii;
  if(gl_LocalInvocationIndex >= (FULL_TILE*FULL_TILE - WGS*WGS) ){
    newlii = -1;
  }else if(int(gl_LocalInvocationIndex)<PADDING*FULL_TILE){
    newlii = int(gl_LocalInvocationIndex);
  }else if(int(gl_LocalInvocationIndex)>=(PADDING*FULL_TILE+2*PADDING*WGS)){
    newlii = int(gl_LocalInvocationIndex)+(PADDING+WGS)*FULL_TILE-PADDING*FULL_TILE-2*PADDING*WGS;
  }else{
    int ww = int(gl_LocalInvocationIndex)-PADDING*FULL_TILE;
    newlii = FULL_TILE*PADDING + ((ww)/(PADDING*2))*FULL_TILE + int((ww%(PADDING*2))>=PADDING)*(PADDING+WGS) + (ww%PADDING);
  }

  if(newlii >= 0){
    ivec2 lll = ivec2(newlii%FULL_TILE,newlii/FULL_TILE);
    vec2 coord = vec2(tileCoord-ivec2(PADDING)+lll)/vec2(/*imageSize(finalFrame)*/ivec2(720,480)-1);
    vec3 base = loadBase(coord);
    vec3 help = loadHelp(coord);
    float dist = length(base-help);

    isPixelSub[lll.x][lll.y] = float(dist > colorDistance/colorDistanceBase);
  }
#endif

  barrier();
}

float computeIsPixelSub(in vec3 base,in vec3 help,uint job){
  ivec2 loc = ivec2(gl_LocalInvocationID.xy)+ivec2(PADDING);
  float dist = length(base-help);
  float isSub = float(dist > colorDistance/colorDistanceBase);
  loadIsSubToShared(isSub,job);

  barrier();

  for(int i=-dilatation;i<=dilatation;++i){
    if(i==0)continue;
    if(isPixelSub[clamp(loc.x+i,0,FULL_TILE-1)][clamp(loc.y  ,0,FULL_TILE-1)]>0.f)isSub = 1.f;
    if(isPixelSub[clamp(loc.x  ,0,FULL_TILE-1)][clamp(loc.y+i,0,FULL_TILE-1)]>0.f)isSub = 1.f;
    if(isPixelSub[clamp(loc.x+i,0,FULL_TILE-1)][clamp(loc.y+i,0,FULL_TILE-1)]>0.f)isSub = 1.f;
    if(isPixelSub[clamp(loc.x-i,0,FULL_TILE-1)][clamp(loc.y+i,0,FULL_TILE-1)]>0.f)isSub = 1.f;
  }
  return isSub;
}

bool shouldRep(in vec3 base,in vec3 help,in vec2 coord,uint job){
  bool isInBox = coord.y > subBox.z && coord.y < subBox.w && coord.x > subBox.x && coord.x < subBox.y;

  bool isSub = computeIsPixelSub(base,help,job)>0;

  //if(!isInBox)return false;
  if(isInBox && isSub)return true;
  else return false;


  bool baseBrighter = dot(base-help,vec3(1))>0;
  
  bool baseIsTooDark = intensity(base) < 0.2;
  bool helpIsTooBright = intensity(help) > 0.7;

  float dist = length(base-help);

  bool tooFar = dist > colorDistance/colorDistanceBase;

  if(tooFar){
    //if(baseIsTooDark && !helpIsTooBright)return true;
    //return baseBrighter;
    return true;
  }
  return false;

}

void compute(uint job){
  ivec2 outCoord = getThreadCoord(job);
  
  ivec2 size = ivec2(720,480);//imageSize(finalFrame);
  vec2 coord = vec2(outCoord) / vec2(size-1);

  coord = vec2(coord.x,coord.y);
writeColor(outCoord,vec3(1));

  vec3 bColor = loadBase(coord);
  vec3 hColor = loadHelp(coord);
  if(drawMode == 7){
    if(coord.x<0.5)writeColor(outCoord,bColor);
    else           writeColor(outCoord,hColor);
    return;
  }


  //draw english stream
  if(drawMode == 0){
    writeColor(outCoord,bColor);
    return;
  }

  //draw kurdis stream
  if(drawMode > 0 && drawMode < 4){
    vec3 hc = readStream(drawMode,coord);
    if(drawDiff){
      writeColor(outCoord,vec3(length(bColor-hc)));
    }else{
      writeColor(outCoord,hc);
    }
    return;
  }



  if(activeClip == 1u){
    writeColor(outCoord,bColor);
    return;
  }


  bool shouldReplace = shouldRep(bColor,hColor,coord,job);
  if(drawMode == 4){
    if(shouldReplace){
      writeColor(outCoord,vec3(1,0,0));
    }else
      writeColor(outCoord,vec3(0,0,0));
    return;
  }
  if(drawMode == 5){
    if(shouldReplace)writeColor(outCoord,hColor);
    else writeColor(outCoord,bColor);
  }
  
}

shared uint sharedJob;
void main(){
  for(uint i=0;i<10000;++i){//safety measure
    if(gl_LocalInvocationIndex == 0){
      sharedJob = atomicAdd(jobCounter,1);
    }
    barrier();
    uint job = sharedJob;
    if(job >= getNofJobs())return;
    compute(job);

  }
}
