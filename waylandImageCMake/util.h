#pragma once

//#define DEBUG

#ifdef DEBUG
  #define ___ std::cerr << __FILE__ << " : " << __LINE__ << std::endl
#else
  #define ___ [](){}
#endif

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

