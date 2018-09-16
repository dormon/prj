/*!
 * @file 
 * @brief This file contains the declaration of PoolingType enum.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

namespace cnnConf{
  /**
   * @brief This enum represents pooling type.
   */
  enum class PoolingType{
    MAX    ,
    AVERAGE,
  };
}
