#include <saveTxtFile.hpp>

#include <fstream>

#include <FunctionPrologue.h>

void saveFragment(vars::Vars&vars){
  FUNCTION_CALLER();
  auto fn = vars.getString("computeSourceFile");
  auto f = std::ofstream(fn);
  auto data = vars.getString("computeSource");
  f.write(data.data(),data.size());
  f.close();
}
