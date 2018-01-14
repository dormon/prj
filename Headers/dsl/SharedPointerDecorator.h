#pragma once

#include<memory>

namespace dsl{

  template<typename TYPE>
    struct PointerDecorator{
      using type = std::shared_ptr<TYPE>;
    };

}
