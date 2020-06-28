#version 450

layout(binding=0)uniform usampler2D tex;
out vec3 fColor;

uvec3 thresholdRGB(uvec3 c,uint t){
  return uvec3(c.x>t,c.y>t,c.z>t)*255;
}
uvec3 filterNonWhite(uvec3 c){
  return uvec3(all(equal(c,uvec3(255))))*255;
}

in vec2 vCoord;

uniform uint drawMode = 0;
uniform uint threshold = 230;

void main(){

  ivec2 size = textureSize(tex,0).xy;
  vec2 coord = vec2(vCoord.x,1-vCoord.y);

  uvec3 color = texture(tex,coord).rgb;
  if(drawMode == 0)fColor = vec3(color) / vec3(255);
  if(drawMode == 1){
    color = filterNonWhite(thresholdRGB(color,threshold));
    fColor = vec3(color) / vec3(255);
  }
  
}
