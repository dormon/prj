#pragma once

#include<memory>
#include<functional>

namespace fileWatcher{
  class FileWatcher;
  class FileWatcherImpl;
}

class fileWatcher::FileWatcher{
  public:
    FileWatcher(bool block = false);
    ~FileWatcher();
    void watch      (std::string const&name,std::function<void()>const&onModify = nullptr);
    void setOnModify(std::string const&name,std::function<void()>const&on = nullptr);
    void removeWatch(std::string const&name);
    void operator()();
  private:
    friend class FileWatcherImpl;
    FileWatcherImpl*impl = nullptr;
    //std::unique_ptr<FileWatcherImpl>impl;
};
