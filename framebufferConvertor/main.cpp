#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdexcept>
#include <string.h>
#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include<fstream>

int main(){
  auto f = std::ifstream("/dev/fb0",std::ios::binary);
  std::vector<char>data(1920*1080*4);
  f.read(data.data(),data.size());
  stbi_write_jpg("frame.jpg",1920,1080,1,data.data(),100);
  return 0;
}





