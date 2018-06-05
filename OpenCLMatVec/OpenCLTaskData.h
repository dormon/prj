#pragma once

#include <vector>
#include <TaskData.h>

#include<CLQuick.h>

class OpenCLTaskData: public TaskData{
  public:
    OpenCLTaskData(CLBuffer const&i,CLBuffer const&o,CLBuffer const&m):
      input(i),output(o),matrix(m){}
    CLBuffer input ;
    CLBuffer output;
    CLBuffer matrix;
};
