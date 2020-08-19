#include <getTimeFormat.hpp>

#include<iomanip>

std::string getTimeFormat(size_t frameId,size_t fps){
  auto frame    = (frameId                ) % fps;
  auto second   = (frameId / fps          ) % 60;
  auto minute   = (frameId / fps / 60     ) % 60;
  auto hour     = (frameId / fps / 60 / 60);
  auto mili = (uint32_t)((float)frame / (float)fps * 1000);

  std::stringstream ss;

  ss << std::setfill('0') << std::setw(2) << hour   << ":";
  ss << std::setfill('0') << std::setw(2) << minute << ":";
  ss << std::setfill('0') << std::setw(2) << second << ":";
  ss << std::setfill('0') << std::setw(3) << mili;
  
  return ss.str();
}
