#include<loadFragment.hpp>

#include<FunctionPrologue.h>

#include<loadTxtFile.hpp>

void loadFragment(vars::Vars&vars){
  FUNCTION_CALLER();
  *vars.reCreate<std::string>("computeSource") = loadTxtFile(vars.getString("computeSourceFile"));
}
