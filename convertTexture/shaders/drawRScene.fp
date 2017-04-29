#version 450 core

layout(location=0)out vec4 fColor;

layout(binding=0)uniform sampler2D tex;

void main(){
  fColor=vec4(texture(tex,vec2(0)));
}
