#include<NetworkConfiguration/Input.h>

/**
 * @brief Constructor of input layer
 *
 * @param w width of input image
 * @param h height of input image
 * @param c number of layer of input image - number of channels
 */
cnnConf::Input::Input(
    size_t w,
    size_t h,
    size_t c):
  Layer(Layer::INPUT),
  width(w),
  height(h),
  channels(c)
{
}

/**
 * @brief This funciton returns number of layers - channels of input image.
 *
 * @return number of layers (channels) of input image
 */
size_t cnnConf::Input::getNofChannels()const{
  return channels;
}

/**
 * @brief This function returns width of input image.
 *
 * @return width of input image
 */
size_t cnnConf::Input::getWidth()const{
  return width;
}

/**
 * @brief This function returns height of input image.
 *
 * @return height of input image
 */
size_t cnnConf::Input::getHeight()const{
  return height;
}

