#include <mylib.hpp>
#include <ArgumentViewer/ArgumentViewer.h>

int main(int argc,char*argv[]){
  auto args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  AType a;
  //a.a=1;
  return 0;
}
