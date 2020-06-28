#include <iostream>
#include <map>
#include <memory>
#include <functional>
#include <set>

#include "fileWatcher.hpp"


int main( int argc, char **argv ) 
{
  auto watcher = fileWatcher::FileWatcher();
  watcher.watch("dormon.txt",[](){std::cerr << "dormon.txt was changed!" << std::endl;});
  //watcher.watch("/home/dormon/a.txt",[](){std::cerr << "a.txt was changed!" << std::endl;});
  //watcher.onModify("magor.txt" ,[](){std::cerr << "magor.txt was changed!" << std::endl;});

  while(true){
    //std::cerr << "new iteration" << std::endl;
    watcher();
  }


  exit( 0 );
}
