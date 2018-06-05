#pragma once

#include <vector>
#include <TaskData.h>

class CPUTaskData: public TaskData{
  public:
    std::vector<float>matrix;
    std::vector<float>input ;
    std::vector<float>output;
};

