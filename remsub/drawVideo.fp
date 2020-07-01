#version 450

layout(local_size_x=16,local_size_y=16)in;

#define MAX_CLIPS 10

layout(        binding=0)uniform usampler2D tex       [MAX_CLIPS];
layout(rgba32f,binding=0)uniform image2D    finalFrame           ;

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
  ivec2 frameSize = imageSize(finalFrame);
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
  imageStore(finalFrame,coord,vec4(color,1));
}

uvec3 thresholdRGB(uvec3 c,uint t){
  return uvec3(c.x>t,c.y>t,c.z>t)*255;
}
uvec3 filterNonWhite(uvec3 c){
  return uvec3(all(equal(c,uvec3(255))))*255;
}


const float scaleBase = 10000;
const float tranBase  = 10000;
const float colorDistanceBase = 1000;

uniform uint activeClip = 1u;

uniform uint  drawMode            = 0;
uniform bool  drawDiff            = false;
uniform uint  threshold           = 230;
uniform vec2  help0offset         = vec2(-4,46);
uniform vec2  help0scale          = vec2(10052,9856);
uniform float colorDistance       = float(64);
uniform float contrast[MAX_CLIPS] = {1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f};

//bool isSub(vec2 pixel){
//  uvec3 color = texture(tex[0],pixel).rgb;
//  return color.x > threshold && color.y > threshold && color.z > threshold;
//}

void applyContract(inout vec3 color,float c){
  color = clamp((color-.5f)*c,-.5f,.5f)+.5;
}

vec3 readStream(uint id,vec2 coord){
  vec2 help0offsetf = vec2(help0offset) / vec2(tranBase);
  uvec3 help0Color = texture(tex[id],coord*help0scale/vec2(scaleBase)+help0offsetf).rgb;
  vec3 hColor = vec3(help0Color) / vec3(255);
  applyContract(hColor,contrast[id]);
  return hColor;
}

void compute(uint job){
  ivec2 outCoord = getThreadCoord(job);
  
  ivec2 size = imageSize(finalFrame);
  vec2 coord = vec2(outCoord) / vec2(size-1);

  coord = vec2(coord.x,1-coord.y);


  uvec3 color      = texture(tex[0],coord).rgb;

  vec3 bColor = vec3(color)      / vec3(255);


  vec3 hColor;
  if((activeClip & (1<<1)) != 0)
    hColor = readStream(1,coord);
  if((activeClip & (1<<2)) != 0)
    hColor = readStream(2,coord);


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



  if(drawMode == 10){
    color = filterNonWhite(thresholdRGB(color,threshold));
    writeColor(outCoord,vec3(color) / vec3(255));
  }

  bool shouldReplace = false;
  shouldReplace = coord.y > 0.5 && coord.y < 0.95 && coord.x > 0.1 && coord.x < 0.9;
  //shouldReplace = shouldReplace && (color.x > threshold) && (color.y > threshold) && (color.z > threshold);
  //shouldReplace = shouldReplace && isSub(coord);
  shouldReplace = shouldReplace && (length(bColor-hColor)>colorDistance/colorDistanceBase);
  if(drawMode == 4){
    if(shouldReplace){
      writeColor(outCoord,vec3(1,0,0));
    }
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
