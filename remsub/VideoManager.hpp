#pragma once

#include<cstdint>
#include<memory>
#include<thread>
#include<keyframes.hpp>
#include<geGL/geGL.h>
#include<geGL/StaticCalls.h>
#include<imguiDormon/imgui.h>

#if defined(USE_OPENCV)
#include <video.hpp>
#else
#include <CachedVideo.hpp>
#endif

class VideoManager{
  public:
    struct Stream{
      float   fps         = 25.f     ;
      float   offset      = 0        ;
      int32_t start       = 0        ;
      int32_t end         = 0        ;
      int32_t nofFrames   = 0        ;
      int32_t loadedFrame = -10000000;
#if defined(USE_OPENCV)
      std::shared_ptr<Video>video;
#else
      std::shared_ptr<cachedVideo::Video>video;
#endif
      std::shared_ptr<ge::gl::Texture>tex;
      std::string file;
      keyframe::Keyframes keys;
      Stream(std::string const&name):file(name){
#if defined(USE_OPENCV)
        video = std::make_shared<Video>(name.c_str());
#else
        video = std::make_shared<cachedVideo::Video>(name.c_str());
#endif
        tex = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGB8UI,1,video->width,video->height);
        tex->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        tex->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        //tex->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        //tex->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        nofFrames = video->nofFrames;
        end = video->nofFrames;
        fps = video->fps;
        //keys.set<int>(0,"offset",0);
        //keys.set<float>(0,"contrast",1);
      }
      float getOffset(int f)const{
        return keys.get<float>(f,"offset",0);
      }
      float getContrast(int f)const{
        return keys.get<float>(f,"contrast",1);
      }
      glm::vec4 getOffsetScale(int f)const{
        return keys.get<glm::vec4>(f,"offsetScale",glm::vec4(0.f,0.f,1.f,1.f));
      }
    };
    ~VideoManager(){
      for(auto&t:threads)
        if(t.joinable())t.join();
    }
    std::shared_ptr<ge::gl::Texture>finalFrame;
    int32_t frame;
    std::vector<Stream>streams;
    uint32_t activeClip = 0;

    void setContrast(ge::gl::Program*prg){
      std::vector<float>c;
      for(auto const&s:streams)
        c.push_back(s.getContrast(frame));
      prg->set1fv("contrast",c.data(),c.size());
    }
    void setOffsetScale(ge::gl::Program*prg){
      std::vector<glm::vec4>c;
      for(auto const&s:streams)
        c.push_back(s.getOffsetScale(frame));
      prg->set4fv("offsetScale",(float*)c.data(),c.size());
    }
    void setSubBox(ge::gl::Program*prg){
      if(streams.empty())return;
      auto c = streams.at(0).keys.get<glm::vec4>(frame,"subBox",glm::vec4(0.f,1.f,0.f,1.f));
      prg->set4fv("subBox",(float*)&c);
    }

    int32_t getNofFrames()const{
      if(streams.empty())return 0;
      return streams.at(0).nofFrames;
    }

    void readAndUploadIfNecessary(){
      activeClip = 0;
      int clipCounter = 0;
      for(size_t clipCounter=0;clipCounter<streams.size();++clipCounter){
        auto&s=streams.at(clipCounter);
        if(!s.video)return;


        if(frame   >= s.end        )continue;
        if(frame   <  s.start      )continue;


        activeClip |= 1<<clipCounter;



        int frameInClip;
        if(clipCounter == 0){
          frameInClip = frame;
        }else{
          float mainClipTime = frame/getFPS();
          auto timeInClip = mainClipTime-s.getOffset(frame);
          //std::cerr << "timeInClip: " << timeInClip << std::endl;
          int ss = (int)(timeInClip*s.fps);
          int se = ss+1;
          float sss = (float)ss / s.fps;
          float see = (float)se / s.fps;
          if(fabs(sss-timeInClip) < fabs(see-timeInClip))
            frameInClip = ss;
          else
            frameInClip = se;

          //frameInClip = (int)roundf(timeInClip*s.fps);
        }
        //if(clipCounter != 0){
        //  std::cerr << "frameTime: " << s.video->getFrameTime() << std::endl;
        //}
        //if(clipCounter == 0){
        //  std::cerr << "main: " << s.video->getFrameId() << " " << s.video->getFrameTime() << std::endl;
        //}

        if(frameInClip <  0            )continue;
        if(frameInClip >= s.nofFrames  )continue;
        if(frameInClip == s.video->getFrameId()-1)continue;
        s.loadedFrame = frameInClip;
#if defined(USE_OPENCV)
        if(frameInClip != s.video->getFrameId()){
          s.video->moveToFrame(frameInClip);
        }

        s.video->readFrame();
        if(!s.video->frame.empty())
          s.tex->setData2D(s.video->frame.data,GL_BGR_INTEGER);
#else
        s.tex->setData2D(s.video->getFrame(frameInClip).data(),GL_BGR_INTEGER);
#endif
      }

    }
    float getFPS(){
      if(streams.empty())return 25.f;
      return streams.at(0).fps;
    }
    void bind(){
      for(size_t i=0;i<streams.size();++i){
        auto&s = streams.at(i);

        if(!s.tex)continue;
        s.tex->bind(i);
      }
    }
    void clampFrame(){
      int32_t n = getNofFrames();
      if(frame >= n)frame = n-1;
      if(frame < 0 )frame = 0;
    }
    void nextFrame(){
      frame++;
      clampFrame();
    }
    void prevFrame(){
      frame--;
      clampFrame();
    }
    void gotoFrame(int32_t f){
      frame = f;
      clampFrame();
    }

    struct CPUFrame{
      std::vector<uint8_t>data;
      int frame;
      CPUFrame(int s=1){
        data.resize(s);
      }
    };

    struct FrameBuffer{
      std::vector<CPUFrame>frames   ;
      std::vector<bool>    freeToUse;
      int w;
      int h;
      FrameBuffer(int w=1,int h=1,int n=1):w(w),h(h){
        freeToUse   = std::vector<bool>(n,true);
        frames      = std::vector<CPUFrame>(n,w*h*3);
      }
    };
    FrameBuffer frameBuffer;
    size_t nofThreads = 8;
    std::vector<std::thread>threads;


    void createFinalFrame(){
      if(streams.size() != 1)return;
      auto const&v = streams.at(0).video;
      finalFrame = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGBA8UI,1,v->width,v->height);
      finalFrame->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
      finalFrame->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
      //finalFrame->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      //finalFrame->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      int h = streams.at(0).video->height;
      int w = streams.at(0).video->width;
      frameBuffer = FrameBuffer(w,h,nofThreads);
      threads.resize(nofThreads);
    }


    void addVideo(std::string const&n){
      streams.emplace_back(n);
      createFinalFrame();
    }

    void setKeyValueGUI(std::string const&var,int&value,int step,int mmin,int mmax){
      ImGui::InputInt(var.c_str(),&value,step,mmin,mmax);
    }
    void setKeyValueGUI(std::string const&var,float&value,float step,float mmin,float mmax){
      ImGui::DragFloat(var.c_str(),&value,step,mmin,mmax,"%.7f");
    }
    void setKeyValueGUI(std::string const&var,glm::vec4&value,glm::vec4 const& step,glm::vec4 const& mmin,glm::vec4 const& mmax){
      ImGui::DragFloat4(var.c_str(),(float*)&value,step.x,mmin.x,mmax.x,"%.7f");
    }

    template<typename ValueType>
    void showVarGUI(Stream&st,std::string const&var,ValueType step,ValueType mmin,ValueType mmax){
      std::stringstream addName;
      addName << "add " << var; 
      if(ImGui::Button(addName.str().c_str())){
        st.keys.set<ValueType>(frame,var.c_str(),ValueType(0));
      }
      int counter = 0;

      std::vector<std::tuple<int,ValueType>>toInsert;
      std::vector<int>toErase;
      for(auto&key:st.keys.keyframes){
        if(!key.second.has(var))continue;
        ValueType defValue = ValueType(0);
        auto&value = key.second.get(var,defValue);
        int f = key.first;


        std::stringstream ss;
        ss << var << counter;

        if(ImGui::TreeNode(ss.str().c_str())){

          ImGui::InputInt("frame",&f);
          if(f != key.first){
            toErase.push_back(key.first);
            toInsert.push_back(std::tuple<int,ValueType>(f,value));
          }
     
          setKeyValueGUI(var,value,step,mmin,mmax);
          if(ImGui::Button("delete"))
            toErase.push_back(key.first);

          ImGui::TreePop();
        }
        counter++;

      }
      for(auto const&t:toErase)
        st.keys.erase(t,var.c_str());
      for(auto const&oo:toInsert)
        st.keys.set(std::get<0>(oo),var,std::get<1>(oo));
    }
    std::string outputExtension = "jpg";
    std::string outputName = "/media/paposud/hodhod/01/01_";
    void downloadFrame(std::vector<uint8_t>&data){
      ge::gl::glGetTextureImage(finalFrame->getId(),0,GL_RGB_INTEGER,GL_UNSIGNED_BYTE,data.size(),data.data());
    }
    void saveFrame(int frame,int w,int h,std::vector<uint8_t>&data);

    int getWidth()const{
      return streams.at(0).video->width;
    }
    int getHeight()const{
      return streams.at(0).video->height;
    }



    void saveFrame(){
      downloadFrame(frameBuffer.frames.at(0).data);
      saveFrame(frame,getWidth(),getHeight(),frameBuffer.frames.at(0).data);
    }

    size_t findFreeThread(){
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

    void saveFrameParallel(){
      auto i = findFreeThread();

      downloadFrame(frameBuffer.frames.at(i).data);

      threads.at(i) = std::thread([i,this](){
        saveFrame(this->frame,getWidth(),getHeight(),frameBuffer.frames.at(i).data);
        frameBuffer.freeToUse.at(i) = true;
      });
    }

    void guiTextInput(std::string const&name,std::string&txt){
      size_t const bufSize = 256;
      char buf[bufSize];
      strcpy(buf, txt.c_str());
      ImGui::InputText(name.c_str(),buf,bufSize);
      txt = std::string(buf);
    }

    void showGUI(){
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
    void setFrame(){
      ImGui::InputInt("goto",&frame);
      clampFrame();
    }
};


