#include "fileWatcher.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <iostream>
#include <map>
#include <memory>
#include <functional>
#include <experimental/filesystem>
#include <set>

class Callback{
  public:
    std::function<void()>onModify;
};


class fileWatcher::FileWatcherImpl{
  public:

    FileWatcherImpl(bool block = false){
      int f = inotify_init1(block?0:IN_NONBLOCK); 
      if ( f < 0 ) {
        perror( "inotify_init" );
      }

      fd = std::shared_ptr<int>(new int,[&](int*ptr){close(*ptr);delete ptr;});
      *fd = f;
    }
    
    void watch(std::string name,std::function<void()>const&onModify = nullptr){
      //auto fileName = std::string(std::experimental::filesystem::path(name).filename());
      //auto dirName  = std::string(std::experimental::filesystem::path(name).parent_path());
      //if(dirName == "")dirName = ".";
      //name = dirName + "/" + fileName;

      auto it = nameToFileWD.find(name);
      if(it == std::end(nameToFileWD)){
        uint32_t mask = IN_DELETE | IN_ATTRIB | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE | IN_CREATE | IN_DELETE  | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO | IN_OPEN; 
        auto wd = inotify_add_watch( *fd, name.c_str(),mask);
        //std::cerr << "inotify_add_wath: " << wd << std::endl;
        if(wd < 0)
          perror("inotify_add_watch");
        fileCallbacks[wd].onModify = onModify;
        nameToFileWD[name] = wd;
        wdToName[wd] = name;

      }else{
        auto wd = it->second;
        fileCallbacks.at(wd).onModify = onModify;
      }
    }
  
    bool getCallback(Callback**clb,std::string const&name){
      auto nwit = nameToFileWD.find(name);
      if(nwit == std::end(nameToFileWD))return false;
      auto it = fileCallbacks.find(nwit->second);
      if(it == std::end(fileCallbacks))return false;
      *clb = &it->second;
      return true;
    }

    void setOnModify(std::string const&name,std::function<void()>const&on = nullptr){
      Callback*clb;
      if(!getCallback(&clb,name))return;
      clb->onModify = on;
    }

    void removeWatch(std::string name){
      //auto fileName = std::string(std::experimental::filesystem::path(name).filename());
      //auto dirName  = std::string(std::experimental::filesystem::path(name).parent_path());
      //if(dirName == "")dirName = ".";
      //name = dirName + "/" + fileName;
      auto it = nameToFileWD.find(name);
      if(it == std::end(nameToFileWD))return;

      auto wd = it->second;
      nameToFileWD.erase(name);
      wdToName.erase(wd);
      //std::cerr << "inotify_rm_wath: " << wd << std::endl;
      inotify_rm_watch(*fd,wd);

      fileCallbacks.erase(wd);
    }

    void operator()(){
      constexpr size_t EVENT_SIZE = ( sizeof (struct inotify_event) );
      constexpr size_t BUF_LEN    = ( 1024 * ( EVENT_SIZE + 16 ) );


      char buffer[BUF_LEN] __attribute__ ((aligned(__alignof__(struct inotify_event))));
      const struct inotify_event *event;

      for(;;){
        int length = read( *fd, buffer, BUF_LEN );  

        if (length == -1 && errno != EAGAIN) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (length <= 0)break;

        char *ptr;
        for (ptr = buffer; ptr < buffer + length;
                ptr += sizeof(struct inotify_event) + event->len) {

            event = (const struct inotify_event *) ptr;
            //std::cerr << "#" << event->name << "#" << std::endl;
            //std::cerr << "wd: " << event->wd << std::endl;
            std::map<uint32_t,std::string>maskToName;
            maskToName[IN_CLOSE_NOWRITE] = "IN_CLOSE_NOWRITE";
            maskToName[IN_ACCESS       ] = "IN_ACCESS"       ;
            maskToName[IN_ATTRIB       ] = "IN_ATTRIB"       ;
            maskToName[IN_CLOSE_WRITE  ] = "IN_CLOSE_WRITE"  ;
            maskToName[IN_CLOSE_NOWRITE] = "IN_CLOSE_NOWRITE";
            maskToName[IN_CREATE       ] = "IN_CREATE"       ;
            maskToName[IN_DELETE       ] = "IN_DELETE"       ;
            maskToName[IN_DELETE_SELF  ] = "IN_DELETE_SELF"  ;
            maskToName[IN_MODIFY       ] = "IN_MODIFY"       ;
            maskToName[IN_MOVE_SELF    ] = "IN_MOVE_SELF"    ;
            maskToName[IN_MOVED_FROM   ] = "IN_MOVED_FROM"   ;
            maskToName[IN_MOVED_TO     ] = "IN_MOVED_TO"     ;
            maskToName[IN_OPEN         ] = "IN_OPEN"         ;
            maskToName[IN_DONT_FOLLOW  ] = "IN_DONT_FOLLOW"  ;
            maskToName[IN_EXCL_UNLINK  ] = "IN_EXCL_UNLINK"  ;
            maskToName[IN_MASK_ADD     ] = "IN_MASK_ADD"     ;
            maskToName[IN_ONESHOT      ] = "IN_ONESHOT"      ;
            maskToName[IN_ONLYDIR      ] = "IN_ONLYDIR"      ;
            maskToName[IN_IGNORED      ] = "IN_IGNORED"      ;
            maskToName[IN_ISDIR        ] = "IN_ISDIR"        ;
            maskToName[IN_Q_OVERFLOW   ] = "IN_Q_OVERFLOW"   ;
            maskToName[IN_UNMOUNT      ] = "IN_UNMOUNT"      ;


            //std::cerr << "mask: ";
            //for(auto const&x:maskToName)
            //  if(event->mask & x.first)
            //    std::cerr << x.second << " ";
            //std::cerr << std::endl;

            if( event->mask & IN_ATTRIB ) {
              if ( event->mask & IN_ISDIR ){}else
              {
                callOnModify(event->wd);
              }
            }
            if(event->mask & IN_IGNORED){
              auto oldwd = event->wd;
              auto name = wdToName.at(oldwd);
              Callback*clb;
              getCallback(&clb,name);
              Callback clbCopy = *clb;
              removeWatch(name);
              watch(name,clbCopy.onModify);
            }
        }

      }

    }
    ~FileWatcherImpl(){
      std::vector<std::string>names;
      for(auto const&x:nameToFileWD)names.push_back(x.first);
      for(auto const&x:names)
        removeWatch(x);
    }
    void callOnModify(int wd){
      auto it = fileCallbacks.find(wd);
      if(it == std::end(fileCallbacks))return;
      if(!it->second.onModify)return;
      it->second.onModify();
    }
  protected:
    std::map<std::string,int>nameToFileWD;
    std::map<int,Callback>fileCallbacks;
    std::map<int,std::string>wdToName;
    std::shared_ptr<int>fd;
};


fileWatcher::FileWatcher::FileWatcher(bool block){
  //impl = std::make_unique<fileWatcher::FileWatcherImpl>(block);
  impl = new fileWatcher::FileWatcherImpl(block);
}

fileWatcher::FileWatcher::~FileWatcher(){
  delete impl;
}

void fileWatcher::FileWatcher::watch      (std::string const&name,std::function<void()>const&onModify){
  impl->watch(name,onModify);
}

void fileWatcher::FileWatcher::setOnModify(std::string const&name,std::function<void()>const&on){
  impl->setOnModify(name,on);
}

void fileWatcher::FileWatcher::removeWatch(std::string const&name){
  impl->removeWatch(name);
}

void fileWatcher::FileWatcher::operator()(){
  (*impl)();
}
