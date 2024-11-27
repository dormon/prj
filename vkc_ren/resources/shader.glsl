#version 460

layout(local_size_x=64)in;
layout(set=0,binding=0)buffer Data {uint data [];};

void main(){
  data [gl_GlobalInvocationID.x] = gl_GlobalInvocationID.x         ;
}
