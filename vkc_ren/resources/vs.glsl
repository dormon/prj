#version 460

void main(){
  gl_Position = vec4(0,0,0,1);
  if(gl_VertexIndex==0)gl_Position = vec4(0,0,0.5,1);
  if(gl_VertexIndex==1)gl_Position = vec4(1,0,0.5,1);
  if(gl_VertexIndex==2)gl_Position = vec4(0,1,0.5,1);
}
