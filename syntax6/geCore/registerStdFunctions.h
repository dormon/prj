#pragma once

#include<memory>
#include"producer.h"

namespace ge{
  namespace core{
    void registerStdunctions(std::shared_ptr<Producer>const&tr);
  }
}
