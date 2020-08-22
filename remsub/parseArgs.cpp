#include <parseArgs.hpp>

#include <Vars/Vars.h>
#include <ArgumentViewer/ArgumentViewer.h>

void parseArgs(vars::Vars&vars,int argc,char*argv[]){
  auto args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  vars.addString("args.projectName")  = args->gets("--proj","","project file");
  auto printHelp  = args->isPresent("--help");
  if(printHelp||!args->validate()){
    std::cerr <<args->toStr() << std::endl;
  }
}
