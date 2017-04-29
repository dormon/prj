#version 440 core

layout(points)in;
layout(triangle_strip,max_vertices=24)out;

uniform vec3 corner=vec3(0);
uniform vec3 xAxis=vec3(1,0,0);
uniform vec3 yAxis=vec3(0,1,0);
uniform vec3 zAxis=vec3(0,0,1);

uniform mat4 mvp;

out vec3 gNormal;

vec3 getPoint(uint id){
  vec3 t=vec3((id>>0)&1,(id>>1)&1,(id>>2)&1);
  return corner+xAxis*t.x+yAxis*t.y+zAxis*t.z;
}

vec3 getNormal(uint a,uint b,uint c){
  return normalize(cross(getPoint(b)-getPoint(a),getPoint(c)-getPoint(a)));
}

void main(){
  gl_Position=mvp*vec4(getPoint(1),1);gNormal=getNormal(1,0,3);EmitVertex();
  gl_Position=mvp*vec4(getPoint(0),1);gNormal=getNormal(1,0,3);EmitVertex();
  gl_Position=mvp*vec4(getPoint(3),1);gNormal=getNormal(1,0,3);EmitVertex();
  gl_Position=mvp*vec4(getPoint(2),1);gNormal=getNormal(1,0,3);EmitVertex();
  EndPrimitive();
  gl_Position=mvp*vec4(getPoint(4),1);gNormal=getNormal(4,5,6);EmitVertex();
  gl_Position=mvp*vec4(getPoint(5),1);gNormal=getNormal(4,5,6);EmitVertex();
  gl_Position=mvp*vec4(getPoint(6),1);gNormal=getNormal(4,5,6);EmitVertex();
  gl_Position=mvp*vec4(getPoint(7),1);gNormal=getNormal(4,5,6);EmitVertex();
  EndPrimitive();
  gl_Position=mvp*vec4(getPoint(0),1);gNormal=getNormal(0,4,2);EmitVertex();
  gl_Position=mvp*vec4(getPoint(4),1);gNormal=getNormal(0,4,2);EmitVertex();
  gl_Position=mvp*vec4(getPoint(2),1);gNormal=getNormal(0,4,2);EmitVertex();
  gl_Position=mvp*vec4(getPoint(6),1);gNormal=getNormal(0,4,2);EmitVertex();
  EndPrimitive();
  gl_Position=mvp*vec4(getPoint(5),1);gNormal=getNormal(5,1,7);EmitVertex();
  gl_Position=mvp*vec4(getPoint(1),1);gNormal=getNormal(5,1,7);EmitVertex();
  gl_Position=mvp*vec4(getPoint(7),1);gNormal=getNormal(5,1,7);EmitVertex();
  gl_Position=mvp*vec4(getPoint(3),1);gNormal=getNormal(5,1,7);EmitVertex();
  EndPrimitive();
  gl_Position=mvp*vec4(getPoint(6),1);gNormal=getNormal(6,7,2);EmitVertex();
  gl_Position=mvp*vec4(getPoint(7),1);gNormal=getNormal(6,7,2);EmitVertex();
  gl_Position=mvp*vec4(getPoint(2),1);gNormal=getNormal(6,7,2);EmitVertex();
  gl_Position=mvp*vec4(getPoint(3),1);gNormal=getNormal(6,7,2);EmitVertex();
  EndPrimitive();
  gl_Position=mvp*vec4(getPoint(0),1);gNormal=getNormal(0,1,4);EmitVertex();
  gl_Position=mvp*vec4(getPoint(1),1);gNormal=getNormal(0,1,4);EmitVertex();
  gl_Position=mvp*vec4(getPoint(4),1);gNormal=getNormal(0,1,4);EmitVertex();
  gl_Position=mvp*vec4(getPoint(5),1);gNormal=getNormal(0,1,4);EmitVertex();
  EndPrimitive();
}
