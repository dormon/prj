#version 450

layout(local_size_x=16,local_size_y=16)in;


layout(        binding=0)uniform usampler2D tex       [10];
layout(rgba32f,binding=0)uniform image2D    finalFrame    ;

layout(std430,binding=0)buffer AuxBuffer{
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
const float contrastBase = 1000;

uniform uint  drawMode    = 2;
uniform uint  threshold   = 230;
uniform vec2  help0offset = vec2(-12,15);
uniform vec2  help0scale  = vec2(10060,9900);
uniform float colorDistance = float(64);
uniform float contrast      = float(contrastBase);

//bool isSub(vec2 pixel){
//  uvec3 color = texture(tex[0],pixel).rgb;
//  return color.x > threshold && color.y > threshold && color.z > threshold;
//}

void compute(uint job){
  ivec2 outCoord = getThreadCoord(job);
  
  ivec2 size = imageSize(finalFrame);
  vec2 coord = vec2(outCoord) / vec2(size);

  coord = vec2(coord.x,1-coord.y);

  vec2 help0offsetf = vec2(help0offset) / vec2(tranBase);

  uvec3 color      = texture(tex[0],coord).rgb;
  uvec3 help0Color = texture(tex[1],coord*help0scale/vec2(scaleBase)+help0offsetf).rgb;

  vec3 bColor = vec3(color)      / vec3(255);
  vec3 hColor = vec3(help0Color) / vec3(255);
  hColor = pow(hColor,vec3(contrast/contrastBase));


  if(drawMode == 0)writeColor(outCoord,bColor);
  if(drawMode == 10){
    color = filterNonWhite(thresholdRGB(color,threshold));
    writeColor(outCoord,vec3(color) / vec3(255));
  }
  if(drawMode == 1){
    writeColor(outCoord,hColor);
  }
  if(drawMode == 2){
    writeColor(outCoord,vec3(length(bColor-hColor)));
  }
  bool shouldReplace = false;
  shouldReplace = coord.y > 0.7 && coord.y < 0.95 && coord.x > 0.1 && coord.x < 0.9;
  //shouldReplace = shouldReplace && (color.x > threshold) && (color.y > threshold) && (color.z > threshold);
  //shouldReplace = shouldReplace && isSub(coord);
  shouldReplace = shouldReplace && (length(bColor-hColor)>colorDistance/colorDistanceBase);
  if(drawMode == 3){
    if(shouldReplace){
      writeColor(outCoord,vec3(1,0,0));
    }
  }
  if(drawMode == 4){
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
