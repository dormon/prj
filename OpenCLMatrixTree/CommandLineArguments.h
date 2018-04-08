#pragma once

#include<map>
#include<sstream>

using Arguments = std::map<std::string, std::string>;

template<typename T>
T convertArgument(char*argv);

template<>size_t convertArgument<size_t>(char*argv){
  return static_cast<size_t>(atoi(argv));
}

template<>std::string convertArgument<std::string>(char*argv){
  return std::string(argv);
}

template<typename T>
T defaultValue();

template<>size_t defaultValue<size_t>(){return 0;}
template<>std::string defaultValue<std::string>(){return "";}

template<typename T>
T getArgument(Arguments& args, int argc, char* argv[],
                   std::string const& arg, T const& def = defaultValue<T>(),std::string const&help = "") {
  std::stringstream ss;
  ss << arg << " " << def << " " << help;
  args[arg] = ss.str();
  for (int i = 0; i < argc; ++i)
    if (std::string(argv[i]) == arg)
      if (i + 1 < argc) return convertArgument<T>(argv[i+1]);
  return def;
}

bool isArgumentPresent(Arguments& args, int argc, char* argv[],
                       std::string const& arg,std::string const&help = "") {
  std::stringstream ss;
  ss << arg << " " << help;
  args[arg] = ss.str();
  for (int i = 0; i < argc; ++i)
    if (std::string(argv[i]) == arg) return true;
  return false;
}

