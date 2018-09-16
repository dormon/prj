/**
 * @file 
 * @brief This file contains declarations for N dimensional index.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<cassert>
#include<vector>
#include<memory>
#include<numeric>
#include<functional>
#include<cmath>
#include<set>
#include<iostream>
#include<algorithm>

class NIndex{
  public:
    NIndex(
        std::vector<ssize_t>const&mins,
        std::vector<ssize_t>const&maxs);
    NIndex(std::vector<size_t>const&sizes);
    void setMin();
    void setMax();
    bool add(ssize_t value,std::vector<size_t>const&ordering = {});
    bool inc(std::vector<size_t>const&ordering = {});
    bool dec(std::vector<size_t>const&ordering = {});
    bool next(
        std::vector<ssize_t>const&stride   = {},
        std::vector<ssize_t>const&offset   = {},
        std::vector<size_t >const&ordering = {});
    size_t  getDimension()const;
    ssize_t getPart(size_t axis)const;
    size_t  getSize(size_t axis)const;
    ssize_t getMin (size_t axis)const;
    ssize_t getMax (size_t axis)const;
    ssize_t&getPart(size_t axis);
    ssize_t&getMin (size_t axis);
    ssize_t&getMax (size_t axis);
    size_t  getSize()const;
    size_t  get1DIndex(std::vector<bool>const&axisFlip = {})const;
    std::vector<ssize_t>const&getParts()const;
    std::vector<ssize_t>     &getParts();
    bool isValid()const;
    struct CompositionOperation{
      size_t thisAxis;
      size_t otherAxis;
      enum Operation{
        COPY_THIS ,
        COPY_OTHER,
        ADD       ,
        ADD_REPEAT,
        ADD_CLAMP ,
      }operation;
    };
    static CompositionOperation createCopyThis (
        size_t axis);
    static CompositionOperation createCopyOther(
        size_t axis);
    static CompositionOperation createAdd      (
        size_t thisAxis ,
        size_t otherAxis);
    static CompositionOperation createAddRepeat(
        size_t thisAxis ,
        size_t otherAxis);
    static CompositionOperation createAddClamp (
        size_t thisAxis ,
        size_t otherAxis);
    NIndex composite(
        NIndex                           const&other          ,
        std::vector<CompositionOperation>const&compositionType);
  protected:
    std::vector<ssize_t>parts;
    std::vector<ssize_t>mins;
    std::vector<ssize_t>maxs;
    size_t  getCurrentAxis  (
        size_t                    axisIndex     ,
        std::vector<size_t >const&ordering  = {});
    ssize_t getCurrentStride(
        size_t                    axisIndex     ,
        std::vector<ssize_t>const&stride    = {});
    ssize_t getCurrentOffset(
        size_t                    axisIndex     ,
        std::vector<ssize_t>const&offset    = {});
};


#define IS_SIGNED(type)typename std::enable_if_t<std::is_signed<type>::value,char>

template<typename TYPE,IS_SIGNED(TYPE) = 0>
inline TYPE mod(TYPE a,TYPE b){
  // (+7)/(+3) = (+2), (+7)%(+3) = (+1), (+7) = (+3)*(+2) + (+1)
  // (-7)/(+3) = (-2), (-7)%(+3) = (-1), (-7) = (+3)*(-2) + (-1)
  // (+7)/(-3) = (-2), (+7)%(-3) = (+1), (+7) = (-3)*(-2) + (+1)
  // (-7)/(-3) = (+2), (-7)%(-3) = (-1), (-7) = (-3)*(+2) + (-1)
  if(a >= 0 && b > 0)return +(         a  %          b );
  if(a <  0 && b > 0)return -(std::abs(a) %          b );
  if(a >= 0 && b < 0)return +(         a  % std::abs(b));
  if(a <  0 && b < 0)return -(std::abs(a) % std::abs(b));
  return 0;
}

template<typename TYPE,IS_SIGNED(TYPE) = 0>
inline TYPE divRoundDown(TYPE a,TYPE b){
  // b is assumed to be posive
  // a can be negative and positive
  //
  //    3 |                  *--
  // r  2 |               *--0
  // e  1 |            *--o
  // s  0 |         *--o
  // u -1 |      *--o
  // l -1 |   *--o
  // t -3 |*--o
  //      |_____________________
  //      -3 -2 -1  0  1  2  3
  //            value of a/b
  return static_cast<TYPE>(std::floor(static_cast<float>(a)/static_cast<float>(b)));
}

#undef IS_SIGNED

inline ssize_t wrap(ssize_t summedValue,ssize_t minVal,ssize_t maxVal){
  auto const size = maxVal - minVal + 1;
  return mod(mod(summedValue - minVal,size)+size,size) + minVal;
}

inline ssize_t clamp(
    ssize_t summedValue,
    ssize_t minVal     ,
    ssize_t maxVal     ){
  if(summedValue < minVal)return minVal;
  if(summedValue > maxVal)return maxVal;
  return summedValue;
}

inline std::vector<ssize_t>sizeToMax(
    std::vector<size_t>const&i){
  std::vector<ssize_t>result;
  for(auto const&x:i)result.emplace_back(static_cast<ssize_t>(x)-1);
  return result;
}


/**
 * @brief This function returns index part.
 *
 * @param axis selected axis
 *
 * @return the index part selected by axis parameter
 */
inline ssize_t&NIndex::getPart(size_t axis){
  assert(axis < getDimension());
  return parts.at(axis);
}

/**
 * @brief This function returns axis minimum.
 *
 * @param axis selected axis
 *
 * @return the minimum of selected axis
 */
inline ssize_t NIndex::getMin(size_t axis)const{
  assert(axis < getDimension());
  return mins.at(axis);
}

/**
 * @brief This function returns axis minimum.
 *
 * @param axis selected axis
 *
 * @return the minimum of selected axis
 */
inline ssize_t&NIndex::getMin(size_t axis){
  assert(axis < getDimension());
  return mins.at(axis);
}

/**
 * @brief This function returns axis maximum.
 *
 * @param axis selected axis
 *
 * @return the maximum of selected axis
 */
inline ssize_t NIndex::getMax(size_t axis)const{
  assert(axis < getDimension());
  return maxs.at(axis);
}

/**
 * @brief This function returns axis maximum.
 *
 * @param axis selected axis
 *
 * @return the maximum of selected axis
 */
inline ssize_t&NIndex::getMax(size_t axis){
  assert(axis < getDimension());
  return maxs.at(axis);
}

/**
 * @brief This function returns overall 1D index size.
 * All axis sizes are multiplied together to form 1D index size.
 *
 * @return 1D index size
 */
inline size_t NIndex::getSize()const{
  size_t result = 1;
  for(size_t i=0;i<getDimension();++i)result*=getSize(i);
  return result;
}

/**
 * @brief This function returns 1D index.
 *
 * @param axisFlip optional axes flipping. 
 *
 * @return 1D index
 */
inline size_t NIndex::get1DIndex(std::vector<bool>const&axisFlip)const{
  assert(axisFlip.size() == 0 || axisFlip.size() == getDimension());
  size_t result = 0;
  //min <= val     <= max
  //0   <= val-min <= max-min
  for(size_t i=getDimension()-1;;--i){
    result *= getSize(i);
    if(axisFlip.empty() || !axisFlip.at(i))
      result += std::abs(getPart(i)-getMin(i));
    else
      result += std::abs(getPart(i)-getMax(i));
    if(i==0)break;
  }
  return result;
}

/**
 * @brief This function returns parts of the index
 *
 * @return parts of the index
 */
inline std::vector<ssize_t>const&NIndex::getParts()const{
  return parts;
}

/**
 * @brief This function returns parts of the index
 *
 * @return parts of the index
 */
inline std::vector<ssize_t>     &NIndex::getParts(){
  return parts;
}

inline ssize_t NIndex::getPart(
    size_t axis)const{
  assert(axis < parts.size());
  return parts.at(axis);
}

/**
 * @brief This function checks if the index is valid (i.e. in range).
 *
 * @return true if valid
 */
inline bool NIndex::isValid()const{
  for(size_t i=0;i<getDimension();++i)
    if(getPart(i) < getMin(i) || getPart(i) > getMax(i))return false;
  return true;
}

/**
 * @brief This function creates composition operation (copy from this).
 *
 * @param axis id of axis from this NIndex
 *
 * @return composition operation
 */
inline NIndex::CompositionOperation NIndex::createCopyThis(
    size_t axis){
  CompositionOperation result;
  result.operation = NIndex::CompositionOperation::COPY_THIS;
  result.thisAxis = axis;
  return result;
}

/**
 * @brief This function creates composition operation (copy from other).
 *
 * @param axis id of axis from other NIndex
 *
 * @return composition operation
 */
inline NIndex::CompositionOperation NIndex::createCopyOther(
    size_t axis){
  CompositionOperation result;
  result.operation = NIndex::CompositionOperation::COPY_OTHER;
  result.otherAxis = axis;
  return result;
}

/**
 * @brief This function creates composition operation (add).
 *
 * @param thisAxis id of axis from this NIndex
 * @param otherAxis id of axis from other NIndex
 *
 * @return composition operation
 */
inline NIndex::CompositionOperation NIndex::createAdd(
    size_t thisAxis ,
    size_t otherAxis){
  CompositionOperation result;
  result.operation = NIndex::CompositionOperation::ADD;
  result.thisAxis  = thisAxis;
  result.otherAxis = otherAxis;
  return result;
}

/**
 * @brief This function creates composition operation (add with repeat).
 *
 * @param thisAxis id of axis from this NIndex
 * @param otherAxis id of axis from other NIndex
 *
 * @return composition operation
 */
inline NIndex::CompositionOperation NIndex::createAddRepeat(
    size_t thisAxis ,
    size_t otherAxis){
  CompositionOperation result;
  result.operation = NIndex::CompositionOperation::ADD_REPEAT;
  result.thisAxis  = thisAxis;
  result.otherAxis = otherAxis;
  return result;
}

/**
 * @brief This function creates composition operation (add with clamping).
 *
 * @param thisAxis id of axis from this NIndex
 * @param otherAxis id of axis from other NIndex
 *
 * @return composition operation
 */
inline NIndex::CompositionOperation NIndex::createAddClamp(
    size_t thisAxis ,
    size_t otherAxis){
  CompositionOperation result;
  result.operation = NIndex::CompositionOperation::ADD_CLAMP;
  result.thisAxis  = thisAxis;
  result.otherAxis = otherAxis;
  return result;
}

/**
 * This is constructor of NIndex.
 * @param mins min sizes of axes
 * @param maxs max sizes of axes
 */
inline NIndex::NIndex(
    std::vector<ssize_t>const&mins,
    std::vector<ssize_t>const&maxs):
  mins(mins),
  maxs(maxs)
{
  parts.resize(mins.size());
  setMin();
}

inline size_t NIndex::getCurrentAxis(
    size_t                   axisIndex,
    std::vector<size_t>const&ordering ){
  if(ordering.empty())return axisIndex;
  assert(axisIndex < ordering.size());
  return ordering.at(axisIndex);
}

inline ssize_t NIndex::getCurrentStride(
    size_t                    axisIndex,
    std::vector<ssize_t>const&stride   ){
  if(stride.empty())return 1;
  assert(axisIndex < stride.size());
  return stride.at(axisIndex);
}

inline ssize_t NIndex::getCurrentOffset(
    size_t                    axisIndex,
    std::vector<ssize_t>const&offset   ){
  assert(axisIndex < mins.size());
  if(offset.empty())return mins[axisIndex];
  assert(axisIndex < offset.size());
  return offset.at(axisIndex);
}

/**
 * @brief This function returns dimension of index.
 *
 * @return dimension of index
 */
inline size_t NIndex::getDimension()const{
  return mins.size();
}

inline void overflowHandler(
    ssize_t&newValue,
    ssize_t&overflow,
    ssize_t value   ,
    ssize_t minVal  ,
    ssize_t maxVal  ){
  auto const size = std::abs(maxVal - minVal) + 1;
  newValue = wrap(value,minVal,maxVal);
  overflow = divRoundDown(static_cast<ssize_t>(value - minVal),static_cast<ssize_t>(size));
}

inline bool isInRange(
    ssize_t value   ,
    ssize_t minVal  ,
    ssize_t maxVal  ){
  return value >= minVal && value <= maxVal;
}

/**
 * @brief This function adds value to index.
 *
 * @param value additional value (can be negative).
 * @param ordering optional axis ordering. See ordering in class description.
 *
 * @return It returns true if addition was successful (no overflow).
 */
inline bool NIndex::add(
    ssize_t                  value   ,
    std::vector<size_t>const&ordering){
  size_t currentAxisIndex = 0;
  size_t currentAxis = getCurrentAxis(currentAxisIndex,ordering);
  assert(currentAxis < parts.size());

  while(!isInRange(parts[currentAxis]+value,mins[currentAxis],maxs[currentAxis])){
    ssize_t newPart,newValue;
    overflowHandler(newPart,newValue,parts[currentAxis]+value,mins[currentAxis],maxs[currentAxis]);
    parts[currentAxis] = newPart ;
    value              = newValue;
    ++currentAxisIndex;
    if(currentAxisIndex == getDimension())return false;
    currentAxis = getCurrentAxis(currentAxisIndex,ordering);
    assert(currentAxis < parts.size());
  }
  parts[currentAxis] += value;
  return true;
}

inline bool NIndex::next(
    std::vector<ssize_t>const&stride  ,
    std::vector<ssize_t>const&offset  ,
    std::vector<size_t >const&ordering){
  size_t currentAxisIndex = 0;
  size_t currentAxis = getCurrentAxis(currentAxisIndex,ordering);
  assert(currentAxis < parts.size());

  while(!isInRange(parts[currentAxis]+getCurrentStride(currentAxis,stride),mins[currentAxis],maxs[currentAxis])){
    parts[currentAxis] = getCurrentOffset(currentAxis,offset);
    ++currentAxisIndex;
    if(currentAxisIndex == getDimension())return false;
    currentAxis = getCurrentAxis(currentAxisIndex,ordering);
    assert(currentAxis < parts.size());
  }
  parts[currentAxis] += getCurrentStride(currentAxis,stride);
  return true;
}

inline bool allUniq(std::vector<size_t>const&v){
  auto v2 = v;
  std::sort(std::begin(v2),std::end(v2));
  for(size_t i=1;i<v2.size();++i)
    if(v2.at(i-1) == v2.at(i))return false;
  return true;
}

inline bool allLessThan(std::vector<size_t>const&v,size_t val){
  return std::accumulate(std::begin(v),std::end(v),true,[&](bool a,size_t v){return a&&(v<val);});
}

inline bool checkComposition(
    NIndex                                   const&a              ,
    NIndex                                   const&b              ,
    std::vector<NIndex::CompositionOperation>const&compositionType){
  std::vector<size_t>allThisAxes;
  std::vector<size_t>allOtherAxes;

  for(auto const&x:compositionType){
    switch(x.operation){
      case NIndex::CompositionOperation::ADD       :
      case NIndex::CompositionOperation::ADD_REPEAT:
      case NIndex::CompositionOperation::ADD_CLAMP :
        allThisAxes  .emplace_back(x.thisAxis );
        allOtherAxes .emplace_back(x.otherAxis);
        break;
      case NIndex::CompositionOperation::COPY_THIS :
        allThisAxes  .emplace_back(x.thisAxis );
        break;
      case NIndex::CompositionOperation::COPY_OTHER:
        allOtherAxes .emplace_back(x.otherAxis);
        break;
    }
  }
  return 
    allUniq(allThisAxes ) && 
    allUniq(allOtherAxes) && 
    allLessThan(allThisAxes ,a.getDimension()) && 
    allLessThan(allOtherAxes,b.getDimension());
}

inline NIndex createCompositionIndex(
    NIndex                                   const&a              ,
    NIndex                                   const&b              ,
    std::vector<NIndex::CompositionOperation>const&compositionType){
  std::vector<ssize_t>resultMins;
  std::vector<ssize_t>resultMaxs;
  for(auto const&x:compositionType){
    switch(x.operation){
      case NIndex::CompositionOperation::COPY_OTHER:
        resultMins.emplace_back(b.getMin(x.otherAxis));
        resultMaxs.emplace_back(b.getMax(x.otherAxis));
        break;
      case NIndex::CompositionOperation::COPY_THIS :
      case NIndex::CompositionOperation::ADD       :
      case NIndex::CompositionOperation::ADD_REPEAT:
      case NIndex::CompositionOperation::ADD_CLAMP :
        resultMins.emplace_back(a.getMin(x.thisAxis));
        resultMaxs.emplace_back(a.getMax(x.thisAxis));
        break;
    }
  }
  return NIndex(resultMins,resultMaxs);
}

inline void computeCompositionParts(
    NIndex                                        &result         ,
    NIndex                                   const&a              ,
    NIndex                                   const&b              ,
    std::vector<NIndex::CompositionOperation>const&compositionType){
  size_t axis = 0;
  for(auto const&x:compositionType){
    switch(x.operation){
      case NIndex::CompositionOperation::COPY_OTHER:
        result.getPart(axis) = b.getPart(x.otherAxis);
        break;
      case NIndex::CompositionOperation::COPY_THIS :
        result.getPart(axis) = a.getPart(x.thisAxis);
        break;
      case NIndex::CompositionOperation::ADD       :
        result.getPart(axis) = b.getPart(x.otherAxis) + a.getPart(x.thisAxis);
        break;
      case NIndex::CompositionOperation::ADD_REPEAT:
        result.getPart(axis) = b.getPart(x.otherAxis) + a.getPart(x.thisAxis);
        result.getPart(axis) = wrap(result.getPart(axis),result.getMin(axis),result.getMax(axis));
        break;
      case NIndex::CompositionOperation::ADD_CLAMP :
        result.getPart(axis) = b.getPart(x.otherAxis) + a.getPart(x.thisAxis);
        result.getPart(axis) = clamp(result.getPart(axis),result.getMin(axis),result.getMax(axis));
        break;
    }
    ++axis;
  }
}

/**
 * @brief This function combines two NIndex (this and other).
 *
 * @param other Other NIndex
 * @param compositionType composition type - instructions, how to build composite index
 *
 * @return composite NIndex
 */
inline NIndex NIndex::composite(
    NIndex                           const&other          ,
    std::vector<CompositionOperation>const&compositionType){
  assert(checkComposition(*this,other,compositionType));
  auto result = createCompositionIndex(*this,other,compositionType);
  computeCompositionParts(result,*this,other,compositionType);
  return result;
}


/**
 * @brief This functiono sets all index parts to minimum.
 */
inline void NIndex::setMin(){
  parts = mins;
}

/**
 * @brief This functiono sets all index parts to maximum.
 */
inline void NIndex::setMax(){
  parts = maxs;
}

/**
 * This is constructor of NIndex.
 * @param sizes sizes of axes
 */
inline NIndex::NIndex(std::vector<size_t>const&sizes):NIndex(
      std::vector<ssize_t>(sizes.size()),
      sizeToMax(sizes))
{
}

/**
 * @brief This function increments index by 1.
 *
 * @param ordering optional axis ordering.
 * See ordering in class description.
 *
 * @return It returns true if incrementation was successful (no overflow).
 */
inline bool NIndex::inc(std::vector<size_t>const&ordering){
  return add(1,ordering);
}

/**
 * @brief This function decrements index by 1.
 *
 * @param ordering optional axis ordering.
 * See ordering in class description.
 *
 * @return It returns true if decrementation was successful (no underflow).
 */
inline bool NIndex::dec(std::vector<size_t>const&ordering){
  return add(-1,ordering);
}

/**
 * @brief This function retruns axis size.
 *
 * @param axis selected axis
 *
 * @return the size of selected axis
 */
inline size_t NIndex::getSize(size_t axis)const{
  assert(axis < getDimension());
  return static_cast<size_t>(
      std::abs(maxs.at(axis) - mins.at(axis))) + 1;
}

