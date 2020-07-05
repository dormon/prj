/*
 * copyright (c) 2001 Fabrice Bellard
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <algorithm>
		
#include<iomanip>
#include<iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define ___ std::cerr << __LINE__ << std::endl

#define CHECK_ERR(ERR) {if ((ERR)<0) return -1; }

#include "CachedVideo.hpp"


int main(int argc, char **argv) {
  if(argc<2)return 0;
  auto video = cachedVideo::Video(argv[1]);
  //for(size_t i=0;i<10;++i)
  //  std::cerr << video.frameInfo.at(i).str() << std::endl;

  size_t frameCounter = 0;
  for(int i=0;i<500;++i){
    std::stringstream ss;
    std::cerr << "saving frame: " << frameCounter << std::endl;
    ss << "frame" << std::setfill('0') << std::setw(7) << frameCounter << ".jpg";
    auto d = video.getFrame(i).data();
    stbi_write_jpg(ss.str().c_str(),video.width,video.height,3,d,100);
    frameCounter++;
  }
  for(int i=0;i<10;++i){
    std::stringstream ss;
    std::cerr << "saving frame: " << frameCounter << std::endl;
    ss << "frame" << std::setfill('0') << std::setw(7) << frameCounter << ".jpg";
    auto d = video.getFrame(400+i).data();
    stbi_write_jpg(ss.str().c_str(),video.width,video.height,3,d,100);
    frameCounter++;
  }


  return 0;
}

