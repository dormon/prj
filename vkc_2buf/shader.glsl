#version 460

layout(local_size_x=64)in;
layout(set=0,binding=0)buffer Data {uint data [];};
layout(set=0,binding=3)buffer Data2{uint data2[];};
layout(set=5,binding=0)buffer Data3{uint data3[];};

void main(){
  data [gl_GlobalInvocationID.x] = gl_GlobalInvocationID.x         ;
  data2[gl_GlobalInvocationID.x] = gl_GlobalInvocationID.x+1000    ;
  data3[gl_GlobalInvocationID.x] = gl_GlobalInvocationID.x+10000000;

}
