#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<utility>
#include<cassert>
#include<limits>
#include<algorithm>
#include<array>

#include<FreeImage.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>


int main(int32_t argc,char*argv[]){

  std::string fileName = std::string(argv[1]);

  FreeImage_Initialise(TRUE);

  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  FIBITMAP *dib(0);
  BYTE* bits(0);
  unsigned int width(0), height(0);

  fif = FreeImage_GetFileType(fileName.c_str(), 0);
  if(fif == FIF_UNKNOWN) 
    fif = FreeImage_GetFIFFromFilename(fileName.c_str());
  if(fif == FIF_UNKNOWN)
    return 0;
  if(FreeImage_FIFSupportsReading(fif))
    dib = FreeImage_Load(fif, fileName.c_str());
  if(!dib)
    return 0;
  auto bpp = FreeImage_GetBPP(dib);

  bits   = FreeImage_GetBits(dib);
  width  = FreeImage_GetWidth(dib);
  height = FreeImage_GetHeight(dib);

  std::vector<glm::vec3>normalMap;
  normalMap.resize(width*height);
  for(size_t y = 0;y<height;++y){
    for(size_t x = 0;x<width;++x){
      glm::vec3 n;
      //for(int32_t k=0;k<3;++k)
      //  n[k] = (bits[(y*width+x)*bpp/8+k]/255.f)*2.0f-1.0f;
      n[0] = (bits[(y*width+x)*bpp/8+2]/255.f)*2.0f-1.0f;
      n[1] = (bits[(y*width+x)*bpp/8+1]/255.f)*2.0f-1.0f;
      n[2] = (bits[(y*width+x)*bpp/8+0]/255.f)*2.0f-1.0f;
      n = glm::normalize(n);
      normalMap[y*width+x] = n;
    }
  }

  FreeImage_Unload(dib);

  auto normalize = [](std::vector<float>&h,uint32_t size){
    glm::vec2 minMax=glm::vec2(10e10f,-10e10f);
    for(uint32_t i=0;i<size;++i){
      minMax.x=glm::min(h[i],minMax.x);
      minMax.y=glm::max(h[i],minMax.y);
    }
    for(uint32_t i=0;i<size;++i){
      h[i]=(h[i]-minMax.x)/(minMax.y-minMax.x);
    }
  };

  std::vector<float>heightMapX;
  heightMapX.resize(width*height);

  float counter = 0;
  for(size_t y=0;y<height;++y){
    counter = 0;
    heightMapX[y*width] = counter;
    for(size_t x=1;x<width;++x){
      float nd = 1.f/glm::tan(glm::acos(glm::dot(glm::vec3(1.f,0.f,0.f),normalMap[y*width+x])));
      counter -= nd;
      heightMapX[y*width+x]=counter;
    }
  }

  normalize(heightMapX,width*height);

  std::vector<float>heightMapY;
  heightMapY.resize(width*height);


  for(size_t x=0;x<width;++x){
    counter = 0;
    heightMapY[x] = counter;
    for(size_t y=1;y<height;++y){
      float nd = glm::dot(glm::vec3(0.f,1.f,0.f),normalMap[(y-1)*width+x]);
      counter += nd;
      heightMapY[y*width+x] = counter;
    }
  }

  normalize(heightMapY,width*height);

  std::vector<float>heightMap;
  heightMap.resize(width*height);

  //*
  heightMap[0]=0;

  for(size_t x=1;x<width;++x)
    heightMap[x]=heightMap[x-1]-glm::dot(glm::vec3(1.f,0.f,0.f),normalMap[x-1]);

  for(size_t y=1;y<height;++y)
    heightMap[y*width] = heightMap[(y-1)*width]-glm::dot(glm::vec3(0.f,1.f,0.f),normalMap[(y-1)*width]);


  
  for(size_t y=1;y<height;++y){
    for(size_t x=1;x<width;++x){
      float nx  = glm::dot(glm::vec3(1.f,0.f,0.f),normalMap[(y-0)*width+(x-1)]);
      float ny  = glm::dot(glm::vec3(0.f,1.f,0.f),normalMap[(y-1)*width+(x-0)]);
      float nxy = glm::dot(glm::vec3(glm::sqrt(2.f)/2.f,glm::sqrt(2.f)/2.f,0.f),normalMap[(y-1)*width+(x-1)]);
      float anx  = glm::abs(nx );
      float any  = glm::abs(ny );
      float anxy = glm::abs(nxy);
      float sw = anx+any+anxy;
      anx /=sw;
      any /=sw;
      anxy/=sw;

      heightMap[y*width+x] = 
        (heightMap[(y-0)*width+(x-1)]-nx )*(anx ) + 
        (heightMap[(y-1)*width+(x-0)]-ny )*(any ) +
        (heightMap[(y-1)*width+(x-1)]-nxy)*(anxy);

      //if(glm::abs(nx)<glm::abs(ny))
      //  heightMap[y*width+x] = heightMap[(y-0)*width+(x-1)]-nx;
      //else
      //  heightMap[y*width+x] = heightMap[(y-1)*width+(x-0)]-ny;
    }
  }
  // */

  /*
  for(size_t i=0;i<width*height;++i)
    heightMap[i]=glm::min(heightMapX[i],heightMapY[i]);
    // */

  normalize(heightMap,width*height);



  FIBITMAP*bitmap = FreeImage_Allocate(width,height,24);
  RGBQUAD color;
  for(uint32_t y=0;y<height;++y){
    for(uint32_t x=0;x<width;++x){
      color.rgbRed   = uint8_t(heightMap[y*width+x]*255);
      color.rgbGreen = uint8_t(heightMap[y*width+x]*255);
      color.rgbBlue  = uint8_t(heightMap[y*width+x]*255);
      FreeImage_SetPixelColor(bitmap,x,y,&color);
    }
  }
  FreeImage_Save(FIF_PNG,bitmap,"out.png",0);

  FreeImage_DeInitialise();
  return 0;
}
