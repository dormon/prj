#pragma once

#include <string>
#include <VideoManager.hpp>

class Project{
  public:
    void load(std::string const&file);
    void save(std::string const&file);
    VideoManager        videoManager;
};
