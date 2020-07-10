#include <fstream>

#include <saveTxtFile.hpp>

void saveTxtFile(std::string const&fileName,std::string const&txt){
  auto file = std::ofstream(fileName);
  if(!file.is_open())throw std::runtime_error("saveTxtFile - cannot open "+fileName);
  file.write(txt.data(),txt.size());
  file.close();
}
