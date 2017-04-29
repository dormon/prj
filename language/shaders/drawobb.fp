#version 440 core

layout(location=0)out vec4 fColor;

uniform vec4 lightPosition=vec4(10,100,10,1);
in vec3 gNormal;

void main(){
  
  fColor=vec4(gNormal*.5+.5,1);
}
