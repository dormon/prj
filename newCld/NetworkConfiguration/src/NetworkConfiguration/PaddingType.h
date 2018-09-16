/*!
 * @file 
 * @brief This file contains the declaration of PaddingType enum.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

namespace cnnConf{
  /**
   * @brief This enum represents padding for pooling, conv and other layers
   */
  enum class PaddingType{
    /**
     * @brief Use layer operator only on parts of input tensor where it fits.
     * Example:
     * input tensor size [7,9]
     * operator size     [2,6]
     * output tensor size [3,1]
     */
    VALID,
    /**
     * @brief Use layer operator everywhere, even on borders of input tensor.
     * Example:
     * input tensor size [7,9]
     * operator size     [2,6]
     * output tensor size [4,2]
     */
    SAME ,
  };
}
