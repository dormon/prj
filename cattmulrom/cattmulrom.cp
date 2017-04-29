#version 450 core

layout(vertices=1)out;

in vec2 vPos[];
in flat int vID[];
patch out vec2 cPos[4];

void main(){
  if(vID[0]==vID[1])cPos[0]=vPos[1]+(vPos[1]-vPos[2]);
  else cPos[0] = vPos[0];
  cPos[1] = vPos[1];
  cPos[2] = vPos[2];
  if(vID[2] == vID[3])cPos[3] = vPos[2]+(vPos[2]-vPos[1]);
  cPos[3] = vPos[3];
  gl_TessLevelInner[1]=64;
  gl_TessLevelInner[0]=1;
  gl_TessLevelOuter[1]=64;
  gl_TessLevelOuter[0]=1;
  gl_TessLevelOuter[2]=1;
  gl_TessLevelOuter[3]=1;
}
