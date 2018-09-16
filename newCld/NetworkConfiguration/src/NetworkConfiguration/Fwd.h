/** 
 * @file
 * @brief This file contains forward declarations of classes.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

namespace cnnConf{
  class Layer             ;
  class Input             ;
  class Pooling           ;
  class GlobalMaxPooling  ;
  class Activation        ;
  class Network           ;
  class Concatenate       ;
  template<typename>class Conv2D            ;
  template<typename>class Bias              ;
  template<typename>class BatchNormalization;
  template<typename>class Dense             ;
}

