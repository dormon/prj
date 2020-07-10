#include <iomanip>

#include <VideoManager.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void VideoManager::downloadFrame(std::vector<uint8_t>&data){
  ge::gl::glGetTextureImage(finalFrame->getId(),0,GL_RGB_INTEGER,GL_UNSIGNED_BYTE,data.size(),data.data());
}

void VideoManager::saveFrame(int frame,int w,int h,std::vector<uint8_t>&data){
  std::stringstream ss;
  ss << outputName << std::setfill('0') << std::setw(7) << frame << "."<<outputExtension;
  if(outputExtension == "jpg"){
    stbi_write_jpg(ss.str().c_str(),w,h,3,data.data(),100);
  }
  if(outputExtension == "png"){
    stbi_write_png(ss.str().c_str(),w,h,3,data.data(),3);
  }
}

int VideoManager::getWidth()const{
  return streams.at(0).video->width;
}
int VideoManager::getHeight()const{
  return streams.at(0).video->height;
}

void VideoManager::saveFrame(){
  downloadFrame(frameBuffer.frames.at(0).data);
  saveFrame(frame,getWidth(),getHeight(),frameBuffer.frames.at(0).data);
}

size_t VideoManager::findFreeThread(){
  size_t i = 0;

  while(true){
    i = 0;
    for(i=0;i<nofThreads;++i){
      if(frameBuffer.freeToUse.at(i)){
        if(threads.at(i).joinable())
          threads.at(i).join();
        frameBuffer.freeToUse.at(i) = false;
        break;
      }
    }
    if(i<frameBuffer.frames.size())
      break;
  }
  return i;
}

void VideoManager::saveFrameParallel(){
  auto i = findFreeThread();

  downloadFrame(frameBuffer.frames.at(i).data);

  threads.at(i) = std::thread([i,this](){
    saveFrame(this->frame,getWidth(),getHeight(),frameBuffer.frames.at(i).data);
    frameBuffer.freeToUse.at(i) = true;
  });
}


void VideoManager::guiTextInput(std::string const&name,std::string&txt){
  size_t const bufSize = 256;
  char buf[bufSize];
  strcpy(buf, txt.c_str());
  ImGui::InputText(name.c_str(),buf,bufSize);
  txt = std::string(buf);
}

void VideoManager::showGUI(){
  if(ImGui::Button("screen")){
    saveFrame();
  }
  guiTextInput("ext",outputExtension);
  guiTextInput("outputName",outputName);

  size_t streamCounter=0;
  for(auto&st:streams){
    std::stringstream ss;
    ss << st.file;
    if(ImGui::TreeNode(ss.str().c_str())){
      if(activeClip&(1<<streamCounter))
        ImGui::Text("active");
      else
        ImGui::Text("inactive");

      ImGui::InputInt("start",&st.start);
      ImGui::InputInt("end"  ,&st.end  );

      if(ImGui::Button("print keys")){
        st.keys.print();
      }

      showVarGUI<float    >(st,"offset"     ,0.0001f           ,-1e10           ,+1e10           );
      showVarGUI<float    >(st,"contrast"   ,0.001f            ,0.001           ,10.f            );
      showVarGUI<glm::vec4>(st,"offsetScale",glm::vec4(0.0001f),glm::vec4(-2.f) ,glm::vec4(2.f)  );
      if(streamCounter == 0)
        showVarGUI<glm::vec4>(st,"subBox",glm::vec4(0.01f),glm::vec4(0.f) ,glm::vec4(1.f)  );


      ImGui::TreePop();
    }
    streamCounter++;
  }
}

void VideoManager::setFrame(){
  ImGui::InputInt("goto",&frame);
  clampFrame();
}
