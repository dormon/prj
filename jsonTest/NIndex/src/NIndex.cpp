/**
 * @file 
 * @brief This file contains definition for N dimensional index.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

/**
 * \class NIndex
 * This class represents N dimensional index.
 * It can be used to access N dimensional arrays.
 * It supports basic operation like addition, substraction and conversion to 1D index.
 * Index parts point to elements of N dimensional index (i,j for 2D array, i,j,k for 3D array, ...).
 * for(int j=0;...)
 *   for(int i=0;...)
 *     doStuff(...);
 *
 * Axis ordering reorders axes incrementation and decrementation.
 * Axes are incremented in standard order (0,1,2,...) if there is no axis ordering.
 * You can specify the fastes axis by inserting its number into axis ordering array to the first element.
 * You can specify the slowes axis by inserting its number into axis ordering array to the last element.
 * The fastes axis index part is changes the most frequently during incrementation.
 *
 * int indexParts[3] = {0,0,0};
 * int slowestAxisIndex = 2;
 * int fastestAxisIndex = 0;
 *
 * int slowestAxis = axisOrdering[slowestAxisIndex];
 * int fastestAxis = axisOrdering[fastestAxisIndex];
 * for(...; ++indexParts[slowestAxis])
 *   for(...; ++indexParts[axisOrdering[1])
 *     for(...; ++indexParts[fastestAxis])
 *       doStuff(...);
 *
 * Axis flipping flips orientation of axes.
 *
 * no flipping:
 * for(int i=0;i<size;++i)
 *
 * with flipping:
 * for(int i=size-1;i>=0;--i)
 */

#include<cassert>
#include<functional>

#include<NIndex/NIndex.h>

class NIndexImpl{
  public:
    NIndexImpl(std::vector<size_t>const&sizes);
    std::vector<size_t>parts;
    std::vector<size_t>sizes;
    size_t getCurrentAxis(size_t axisIndex,std::vector<size_t>const&ordering = {});
    size_t getDimension()const;
    bool update(
        size_t                                                         value          ,
        std::vector<size_t>                                      const&ordering       ,
        std::function<void(size_t&,size_t&,size_t,size_t,size_t)>const&overflowHandler,
        std::function<void(size_t&,size_t                      )>const&updateHandler  ,
        std::function<bool(size_t ,size_t ,size_t              )>const&isOverflow     );
    static void oveflowHandlerForAdd(
        size_t&newValue ,
        size_t&overflow ,
        size_t value    ,
        size_t increment,
        size_t size     );
    static void overflowHandlerForSub(
        size_t&newValue ,
        size_t&overflow ,
        size_t value    ,
        size_t decrement,
        size_t size     );
    static void updateForAdd(
        size_t&value    ,
        size_t increment);
    static void updateForSub(
        size_t&value    ,
        size_t decrement);
    static bool isOverflowForAdd(
        size_t value    ,
        size_t increment,
        size_t size     );
    static bool isOverflowForSub(
        size_t value    ,
        size_t decrement,
        size_t          );
};

NIndexImpl::NIndexImpl(std::vector<size_t>const&sizes):sizes(sizes){
  parts.resize(sizes.size(),0);
}

size_t NIndexImpl::getCurrentAxis(size_t axisIndex,std::vector<size_t>const&ordering){
  if(ordering.empty())return axisIndex;
  return ordering.at(axisIndex);
}

size_t NIndexImpl::getDimension()const{
  return sizes.size();
}

bool NIndexImpl::update(
    size_t                                                         value          ,
    std::vector<size_t>                                      const&ordering       ,
    std::function<void(size_t&,size_t&,size_t,size_t,size_t)>const&overflowHandler,
    std::function<void(size_t&,size_t                      )>const&updateHandler  ,
    std::function<bool(size_t ,size_t ,size_t              )>const&isOverflow     ){
  size_t currentAxisIndex = 0;
  size_t currentAxis = getCurrentAxis(currentAxisIndex,ordering);
  while(isOverflow(parts[currentAxis],value,sizes[currentAxis])){
    size_t newPart,newValue;
    overflowHandler(newPart,newValue,parts[currentAxis],value,sizes[currentAxis]);
    parts[currentAxis] = newPart ;
    value              = newValue;
    ++currentAxisIndex;
    if(currentAxisIndex == getDimension())return false;
    currentAxis = getCurrentAxis(currentAxisIndex,ordering);
  }
  updateHandler(parts[currentAxis],value);
  return true;
}

void NIndexImpl::oveflowHandlerForAdd(
    size_t&newValue ,
    size_t&overflow ,
    size_t value    ,
    size_t increment,
    size_t size     ){
  newValue = (value + increment) % size;
  overflow = (value + increment) / size;
}

void NIndexImpl::overflowHandlerForSub(
    size_t&newValue ,
    size_t&overflow ,
    size_t value    ,
    size_t decrement,
    size_t size     ){
  newValue = (value + (size - (decrement % size))) % size;
  overflow = (decrement / size) + static_cast<size_t>((decrement % size) > value);
}

void NIndexImpl::updateForAdd(
    size_t&value    ,
    size_t increment){
  value += increment;
}

void NIndexImpl::updateForSub(
    size_t&value    ,
    size_t decrement){
  value -= decrement;
}

bool NIndexImpl::isOverflowForAdd(
    size_t value    ,
    size_t increment,
    size_t size     ){
  return value + increment >= size;
}

bool NIndexImpl::isOverflowForSub(
    size_t value    ,
    size_t decrement,
    size_t          ){
  return decrement > value;
}




/**
 * This is constructor of NIndex.
 * @param sizes sizes of axes
 */
NIndex::NIndex(std::vector<size_t>const&sizes){
  pimpl = std::make_shared<NIndexImpl>(sizes);
}

/**
 * @brief This functiono sets all index parts to zero.
 */
void NIndex::zero(){
  for(auto&x:pimpl->parts)x=0;
}

/**
 * @brief This function adds value to index.
 *
 * @param value additional value.
 * @param ordering optional axis ordering. See ordering in class description.
 *
 * @return It returns true if addition was successful (no overflow).
 */
bool NIndex::add(size_t value,std::vector<size_t>const&ordering){
  return pimpl->update(value,ordering,pimpl->oveflowHandlerForAdd,pimpl->updateForAdd,pimpl->isOverflowForAdd);
}

/**
 * @brief This function substracts value from index.
 *
 * @param value value that is substracted
 * @param ordering optional axis ordering. See ordering in class description.
 *
 * @return It returns true if substracion was successful (no underflow).
 */
bool NIndex::sub(size_t value,std::vector<size_t>const&ordering){
  return pimpl->update(value,ordering,pimpl->overflowHandlerForSub,pimpl->updateForSub,pimpl->isOverflowForSub);
}

/**
 * @brief This function increments index by 1.
 *
 * @param ordering optional axis ordering. See ordering in class description.
 *
 * @return It returns true if incrementation was successful (no overflow).
 */
bool NIndex::inc(std::vector<size_t>const&ordering){
  return add(1,ordering);
}

/**
 * @brief This function decrements index by 1.
 *
 * @param ordering optional axis ordering. See ordering in class description.
 *
 * @return It returns true if decrementation was successful (no underflow).
 */
bool NIndex::dec(std::vector<size_t>const&ordering){
  return sub(1,ordering);
}

/**
 * @brief This function returns dimension of index.
 *
 * @return dimension of index
 */
size_t NIndex::getDimension()const{
  return pimpl->getDimension();
}

/**
 * @brief This function returns index part.
 *
 * @param axis index axis
 *
 * @return index part pointed by axis
 */
size_t NIndex::getPart(size_t axis)const{
  return pimpl->parts.at(axis);
}

/**
 * @brief This function retruns axis size.
 *
 * @param axis selected axis
 *
 * @return the size of selected axis
 */
size_t NIndex::getSize(size_t axis)const{
  return pimpl->sizes.at(axis);
}

/**
 * @brief This function returns index part.
 *
 * @param axis selected axis
 *
 * @return the index part selected by axis parameter
 */
size_t&NIndex::getPart(size_t axis){
  return pimpl->parts.at(axis);
}

/**
 * @brief This function returns axis size.
 *
 * @param axis selected axis
 *
 * @return the size of selected axis
 */
size_t&NIndex::getSize(size_t axis){
  return pimpl->sizes.at(axis);
}

/**
 * @brief This function returns overall 1D index size.
 * All axis sizes are multiplied together to form 1D index size.
 *
 * @return 1D index size
 */
size_t NIndex::getSize()const{
  size_t result = 1;
  for(auto const&x:pimpl->sizes)result*=x;
  return result;
}

/**
 * @brief This function returns 1D index.
 *
 * @param axisFlip optional axis flipping. See axis flipping in class description.
 *
 * @return 1D index
 */
size_t NIndex::get1DIndex(std::vector<bool>const&axisFlip)const{
  assert(axisFlip.size() == 0 || axisFlip.size() == getDimension());
  size_t result = 0;
  for(size_t i=getDimension()-1;;--i){
    result *= getSize(i);
    if(axisFlip.empty() || !axisFlip.at(i))
      result += getPart(i);
    else
      result += getSize(i) - getPart(i) - 1;
    if(i==0)break;
  }
  return result;
}

std::vector<size_t>const&NIndex::getParts()const{
  return pimpl->parts;
}

