#version 460

layout(local_size_x=64)in;
layout(set=0,binding=0)buffer Data {uint data [];};
layout(set=0,binding=2)buffer Data2{uint data2[];};

void main(){
  data [gl_GlobalInvocationID.x] = gl_GlobalInvocationID.x     ;
  data2[gl_GlobalInvocationID.x] = gl_GlobalInvocationID.x+1000;

}
