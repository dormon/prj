#version 450

layout(binding=0)uniform usampler2D tex;
layout(binding=1)uniform usampler2D help0;
out vec3 fColor;

uvec3 thresholdRGB(uvec3 c,uint t){
  return uvec3(c.x>t,c.y>t,c.z>t)*255;
}
uvec3 filterNonWhite(uvec3 c){
  return uvec3(all(equal(c,uvec3(255))))*255;
}

in vec2 vCoord;
in flat int help0Inside;

const float scaleBase = 10000;
const float tranBase  = 10000;
const float colorDistanceBase = 1000;

uniform uint  drawMode    = 0;
uniform uint  threshold   = 230;
uniform vec2  help0offset = vec2(-12,15);
uniform vec2  help0scale  = vec2(10060,9900);
uniform float colorDistance = float(64);

bool isSub(vec2 pixel){
  uvec3 color = texture(tex,pixel).rgb;
  return color.x > threshold && color.y > threshold && color.z > threshold;
}

void main(){

  ivec2 size = textureSize(tex,0).xy;
  vec2 coord = vec2(vCoord.x,1-vCoord.y);

  vec2 help0offsetf = vec2(help0offset) / vec2(tranBase);

  uvec3 color      = texture(tex,coord).rgb;
  uvec3 help0Color = texture(help0,coord*help0scale/vec2(scaleBase)+help0offsetf).rgb;

  vec3 bColor = vec3(color)      / vec3(255);
  vec3 hColor = vec3(help0Color) / vec3(255);


  if(drawMode == 0)fColor = vec3(color) / vec3(255);
  if(drawMode == 10){
    color = filterNonWhite(thresholdRGB(color,threshold));
    fColor = vec3(color) / vec3(255);
  }
  if(drawMode == 1){
    fColor = vec3(help0Color) / vec3(255);
  }
  if(drawMode == 2){
    fColor = vec3(length(bColor-hColor));
  }
  bool shouldReplace = false;
  shouldReplace = coord.y > 0.7 && coord.y < 0.95 && coord.x > 0.1 && coord.x < 0.9;
  //shouldReplace = shouldReplace && (color.x > threshold) && (color.y > threshold) && (color.z > threshold);
  //shouldReplace = shouldReplace && isSub(coord);
  shouldReplace = shouldReplace && (length(bColor-hColor)>colorDistance/colorDistanceBase);
  if(drawMode == 3){
    if(shouldReplace){
      fColor = vec3(1,0,0);
    }
  }
  if(drawMode == 4){
    if(shouldReplace)fColor = hColor;
    else fColor = bColor;
  }
  
  
}
