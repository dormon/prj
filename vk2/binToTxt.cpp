#include<iostream>
#include<fstream>
#include<vector>

std::vector<uint8_t>loadBinary(std::string const&fileName){
  auto file = std::ifstream(fileName,std::ios::binary);
  if(!file.is_open()){
    std::cerr << "cannot open file: " << fileName << std::endl;
    return{};
  }
  auto ff = std::istreambuf_iterator<char>(file);
  auto ee = std::istreambuf_iterator<char>();
  return std::vector<uint8_t>(ff,ee);
}

int main(int argc,char*argv[]){
  if(argc<2)return EXIT_SUCCESS;
  auto data = loadBinary(argv[1]);
  auto ptr = (uint32_t*)data.data();
  size_t const lineLen=20;
  size_t lineCounter=0;
  for(size_t i=0;i*4<data.size();++i){
    std::cout << "0x" << std::hex << ptr[i] << ",";
    lineCounter++;
    if(lineCounter>=lineLen){
      lineCounter=0;
      std::cout << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
