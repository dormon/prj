#include <CL/cl.hpp>
#include <iostream>
#include <sstream>
#include <cassert>

#include<CLQuick.h>

std::string static const kernel_code =
R".(
void kernel simple_add(
  global const int*A,
  global const int*B,
  global       int*C){
  uint gid = get_global_id(0);
  if(gid>10)return;
  C[gid] = A[gid] + B[gid] + 10000;
}
).";

int main() {
  auto clb     = createContext();
  auto program = createProgram(clb,kernel_code);

  auto bufferA = createBuffer<int>(clb, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  auto bufferB = createBuffer<int>(clb, {0, 1, 2, 0, 1, 2, 0, 1, 2, 0});
  auto bufferC = createBuffer(clb, sizeof(int) * 10);

  auto kernel_add = createKernel(program,"simple_add");
  setArgs(kernel_add,bufferA,bufferB,bufferC);

  Timer<float>t;
  enqueueKernel(kernel_add,cl::NDRange(64),cl::NDRange(64));
  finish(clb);
  std::cout << "time: " << t.elapsedFromStart() << std::endl;

  std::vector<int>C(10);
  readBuffer(bufferC,C);

  std::cout << "result: \n";
  for(auto const&x:C)
    std::cout << x << " ";

  return 0;
}
