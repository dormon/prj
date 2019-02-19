#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>
#include <cstdlib>
#include <memory>

#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

#include <sstream>

#include "vars.h"
#include "sink.h"

int main(int argc,char*[]) {

  sink();
  std::cerr << "##############" << std::endl;
  Vars moje;
  moje.data.resize(10);
  moje.d = "ahoj svete";
  std::cerr << moje.data.size() << std::endl;
  std::cerr << moje.d << std::endl;
  sink(std::move(moje));
  std::cerr << "##############" << std::endl;
  std::cerr << moje.data.size() << std::endl;
  std::cerr << moje.d << std::endl;
  return 0;
}
