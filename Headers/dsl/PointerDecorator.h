#pragma once

namespace dsl{

  template<typename TYPE>
    struct PointerDecorator{
      using type = TYPE*;
    };

}
