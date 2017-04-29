#version 440

layout(location=0)out vec4 fColor;

in vec2 gCoord;

uniform bool alphaOn=true;

void main(){
  float alpha=pow(1-abs(gCoord.x-.5),10);
  if(alphaOn)
    fColor=vec4(alpha);
  else
    fColor=vec4(1);
}
