#include<iostream>
#include<sstream>
#include<functional>
#include<map>
#include<vector>
#include<typeinfo>
#include<tuple>
#include<limits>
#include<type_traits>
#include<array>

template<typename TYPE>
class Interval{
  public:
    enum IntervalType{
      CLOSED  ,
      MIN_OPEN,
      MAX_OPEN,
      OPEN    ,
    };
    struct Methods{
      struct Contain0{
        using PARAM0 = std::vector<TYPE>const&;
        using RETURN = bool                   ;
      };
      struct Contain1{
        using PARAM0 = Interval<TYPE>   const&;
        using RETURN = bool                   ;
      };
    };

    using GetType = IntervalType(Interval::*)(size_t)const;

    Interval                 (
        );
    Interval                 (
        std::vector<TYPE        >const&mins          ,
        std::vector<TYPE        >const&maxs          ,
        std::vector<IntervalType>const&types     = {}
        );
    size_t       getDimension(
        )const;

    typename Methods::Contain0::RETURN 
      contain(
          typename Methods::Contain0::PARAM0 value         
          )const;

    typename Methods::Contain1::RETURN
      contain(
          typename Methods::Contain1::PARAM0 other
          )const;

    static bool  isSuccessor (
        TYPE                     const&value         ,                              
        TYPE                     const&nextValue     
        );
    IntervalType getType     (
        size_t                         axis          
        )const;
    TYPE         getMin      (
        size_t                         axis          
        )const;
    TYPE         getMax      (
        size_t                         axis          
        )const;
    void         setType     (
        size_t                         axis          ,
        IntervalType             const&type          
        );
    void         setMin      (
        size_t                         axis          ,
        TYPE                     const&value         
        );
    void         setMax      (
        size_t                         axis          ,
        TYPE                     const&value         
        );
    bool         empty       (
        )const;
  protected:
    struct Border{
      TYPE         minValue;
      TYPE         maxValue;
      IntervalType type    ;
    };
    std::vector<Border>borders;
};

template<typename TYPE>
inline bool isClosed(typename Interval<TYPE>::IntervalType a){
  using IntervalType = typename Interval<TYPE>::IntervalType;
  return a == IntervalType::CLOSED;
}

template<typename TYPE>
inline bool isOpened(typename Interval<TYPE>::IntervalType a){
  using IntervalType = typename Interval<TYPE>::IntervalType;
  return a == IntervalType::OPEN;
}

template<typename TYPE>
inline bool isMinClosed(typename Interval<TYPE>::IntervalType a){
  using IntervalType = typename Interval<TYPE>::IntervalType;
  return isClosed(a) || a == IntervalType::MAX_OPEN;
}

template<typename TYPE>
inline bool isMaxClosed(typename Interval<TYPE>::IntervalType a){
  using IntervalType = typename Interval<TYPE>::IntervalType;
  return isClosed(a) || a == IntervalType::MIN_OPEN;
}

template<typename TYPE>
inline bool isMinOpened(typename Interval<TYPE>::IntervalType a){
  using IntervalType = typename Interval<TYPE>::IntervalType;
  return isOpened(a) || a == IntervalType::MIN_OPEN;
}

template<typename TYPE>
inline bool isMaxOpened(typename Interval<TYPE>::IntervalType a){
  using IntervalType = typename Interval<TYPE>::IntervalType;
  return isOpened(a) || a == IntervalType::MAX_OPEN;
}

template<typename TYPE>
inline bool isMinClosed(
    typename Interval<TYPE>::IntervalType a,
    typename Interval<TYPE>::IntervalType b){
  return isMinClosed(a) || isMinClosed(b);
}

template<typename TYPE>
inline bool isMaxClosed(
    typename Interval<TYPE>::IntervalType a,
    typename Interval<TYPE>::IntervalType b){
  return isMaxClosed(a) || isMaxClosed(b);
}

template<typename TYPE>
inline typename Interval<TYPE>::IntervalType getIntervalType(
    bool minIsOpened,
    bool maxIsOpened){
  using IntervalType = typename Interval<TYPE>::IntervalType;
  if( minIsOpened &&  maxIsOpened)return IntervalType::OPEN    ;
  if( minIsOpened && !maxIsOpened)return IntervalType::MIN_OPEN;
  if(!minIsOpened &&  maxIsOpened)return IntervalType::MAX_OPEN;
  return IntervalType::CLOSED;
}


template<typename TYPE>
bool isSuccessor(TYPE const&a,TYPE const&b);

template<typename TYPE>
Interval<TYPE>::Interval(){}

template<typename TYPE>
Interval<TYPE>::Interval(
    std::vector<TYPE        >const&mins ,
    std::vector<TYPE        >const&maxs ,
    std::vector<IntervalType>const&types){
  assert(mins.size() == maxs.size());
  auto types2 = types;
  if(types2.empty())
    for(size_t i=0;i<mins.size();++i)
      types2.emplace_back(CLOSED);
  else{
    assert(types.size() == mins.size());
  }
    
  for(size_t i=0;i<mins.size();++i)
    borders.emplace_back({mins.at(i),maxs.at(i),types2.at(i)});
}

template<typename TYPE>
size_t Interval<TYPE>::getDimension()const{
  return borders.size();
}

//template<typename TYPE>
//bool Interval<TYPE>::contain(
//    Interval<TYPE>const&other)const{
//  assert(this!=nullptr);
//  return contain(other.getLeft()) && contain(other.getRight());
//};
//
//template<typename TYPE>
//bool Interval<TYPE>::isSuccessor(
//    TYPE const&value,TYPE const&nextValue){
//  if(!std::is_integral<TYPE>::value)return false;
//  return value+1==nextValue;
//}

template<typename TYPE>
typename Interval<TYPE>::IntervalType Interval<TYPE>::getType(size_t axis)const{
  assert(this!=nullptr);
  assert(axis < borders.size());
  return borders.at(axis).type;
}

template<typename TYPE>
void Interval<TYPE>::setType(size_t axis,IntervalType const&type){
  assert(this!=nullptr);
  assert(axis < borders.size());
  borders.at(axis).type = type;
}

template<typename TYPE>
TYPE Interval<TYPE>::getMin(size_t axis)const{
  assert(this!=nullptr);
  assert(axis < borders.size());
  return borders.at(axis).minValue;
}

template<typename TYPE>
TYPE Interval<TYPE>::getMax(size_t axis)const{
  assert(this!=nullptr);
  assert(axis < borders.size());
  return borders.at(axis).maxValue;
}

template<typename TYPE>
void Interval<TYPE>::setMin(
    size_t       axis ,
    TYPE   const&value){
  assert(this!=nullptr);
  assert(axis < borders.size());
  borders.at(axis).minValue = value;
}

template<typename TYPE>
void Interval<TYPE>::setMax(
    size_t       axis , 
    TYPE   const&value){
  assert(this!=nullptr);
  assert(axis < borders.size());
  borders.at(axis).maxValue = value;
}

template<typename TYPE>
bool Interval<TYPE>::empty()const{
  assert(this!=nullptr);
  for(auto const&x:borders){
    if(x.minValue> x.maxValue)return true;
    if(x.minValue==x.maxValue)return !isClosed(x.type);
  }
  return false;
}

template<typename TYPE>
Interval<TYPE>intersectInterval(Interval<TYPE>const&a,Interval<TYPE>const&b){
  assert(a.getDimension() == b.getDimension());
  if(a.empty())return a;
  if(b.empty())return b;
  using IntervalType = typename Interval<TYPE>::IntervalType;
  std::vector<TYPE        >mins ;
  std::vector<TYPE        >maxs ;
  std::vector<IntervalType>types;
  for(size_t i=0;i<a.getDimension();++i){
    using GetBorder               = TYPE(Interval<TYPE>::*)(size_t);
    using IsBorderOpened          = bool(*)(IntervalType);
    using IsCompositeBorderOpened = bool(*)(IntervalType,IntervalType);
    using Comparison              = bool(*)(TYPE,TYPE);
    size_t const nofBorders = 2;
    std::array<GetBorder,nofBorders>const getBorders = {
      Interval<TYPE>::getMin,
      Interval<TYPE>::getMax,
    };
    std::array<IsBorderOpened,nofBorders>const isBorderOpeneds = {
      isMinOpened,
      isMaxOpened,
    };
    std::array<IsCompositeBorderOpened,nofBorders>const isCompositeBorderOpeneds = {
      isMinOpened,
      isMaxOpened,
    };
    std::array<Comparison,nofBorders>const aIsSupers = {
      [](TYPE a,TYPE b){return a<b;},
      [](TYPE a,TYPE b){return a>b;},
    };
    std::array<Comparison,nofBorders>const bIsSupers = {
      [](TYPE a,TYPE b){return a>b;},
      [](TYPE a,TYPE b){return a<b;},
    };

    std::array<TYPE,nofBorders>borders;
    std::array<bool,nofBorders>borderIsOpened;

    size_t const minBorder = 0;
    size_t const maxBorder = 1;
    for(size_t border=minBorder;border<=maxBorder;++border){
      auto const getBorder               = getBorders[border];
      auto const aIsSuper                = aIsSupers[border];
      auto const bIsSuper                = bIsSupers[border];
      auto const isBorderOpened          = isBorderOpeneds[border];
      auto const isCompositeBorderOpened = isCompositeBorderOpeneds[border];
      auto const aBorder           = (a.*getBorder)(i);
      auto const bBorder           = (b.*getBorder)(i);
      auto const aSuper            = aIsSuper(aBorder,bBorder);
      auto const bSuper            = bIsSuper(aBorder,bBorder);
      auto const aType             = a.getType(i);
      auto const bType             = b.getType(i);
      auto const aOpenness         = isBorderOpened(aType);
      auto const bOpenness         = isBorderOpened(bType);
      auto const compositeOpenness = isCompositeBorderOpened(aType,bType);
      if      (aSuper){
        borders       [border] = aBorder  ;
        borderIsOpened[border] = aOpenness;
      }else if(bSuper){
        borders       [border] = bBorder  ;
        borderIsOpened[border] = bOpenness;
      }else{
        borders       [border] = aBorder;
        borderIsOpened[border] = compositeOpenness;
      }
    }
    mins.emplace_back(borders[minBorder]);
    maxs.emplace_back(borders[maxBorder]);
    types.emplace_back(getIntervalType<TYPE>(
          borderIsOpened[minBorder],
          borderIsOpened[maxBorder]));
  }
  return Interval<TYPE>(mins,maxs,types);

}

template<typename TYPE>
Interval<TYPE>unionInterval(Interval<TYPE>const&a,Interval<TYPE>const&b){
  assert(a.getDimension() == b.getDimension());
  if(a.empty())return b;
  if(b.empty())return a;
  using IntervalType = typename Interval<TYPE>::IntervalType;
  std::vector<TYPE        >mins ;
  std::vector<TYPE        >maxs ;
  std::vector<IntervalType>types;
  for(size_t i=0;i<a.getDimension();++i){
    using GetBorder               = TYPE(Interval<TYPE>::*)(size_t);
    using IsBorderOpened          = bool(*)(IntervalType);
    using IsCompositeBorderOpened = bool(*)(IntervalType,IntervalType);
    using Comparison              = bool(*)(TYPE,TYPE);
    size_t const nofBorders = 2;
    std::array<GetBorder,nofBorders>const getBorders = {
      Interval<TYPE>::getMin,
      Interval<TYPE>::getMax,
    };
    std::array<IsBorderOpened,nofBorders>const isBorderOpeneds = {
      isMinOpened,
      isMaxOpened,
    };
    std::array<IsCompositeBorderOpened,nofBorders>const isCompositeBorderOpeneds = {
      isMinOpened,
      isMaxOpened,
    };
    std::array<Comparison,nofBorders>const aIsSupers = {
      [](TYPE a,TYPE b){return a<b;},
      [](TYPE a,TYPE b){return a>b;},
    };
    std::array<Comparison,nofBorders>const bIsSupers = {
      [](TYPE a,TYPE b){return a>b;},
      [](TYPE a,TYPE b){return a<b;},
    };

    std::array<TYPE,nofBorders>borders;
    std::array<bool,nofBorders>borderIsOpened;

    size_t const minBorder = 0;
    size_t const maxBorder = 1;
    for(size_t border=minBorder;border<=maxBorder;++border){
      auto const getBorder               = getBorders[border];
      auto const aIsSuper                = aIsSupers[border];
      auto const bIsSuper                = bIsSupers[border];
      auto const isBorderOpened          = isBorderOpeneds[border];
      auto const isCompositeBorderOpened = isCompositeBorderOpeneds[border];
      auto const aBorder           = (a.*getBorder)(i);
      auto const bBorder           = (b.*getBorder)(i);
      auto const aSuper            = aIsSuper(aBorder,bBorder);
      auto const bSuper            = bIsSuper(aBorder,bBorder);
      auto const aType             = a.getType(i);
      auto const bType             = b.getType(i);
      auto const aOpenness         = isBorderOpened(aType);
      auto const bOpenness         = isBorderOpened(bType);
      auto const compositeOpenness = isCompositeBorderOpened(aType,bType);
      if      (aSuper){
        borders       [border] = aBorder  ;
        borderIsOpened[border] = aOpenness;
      }else if(bSuper){
        borders       [border] = bBorder  ;
        borderIsOpened[border] = bOpenness;
      }else{
        borders       [border] = aBorder;
        borderIsOpened[border] = compositeOpenness;
      }
    }
    mins.emplace_back(borders[minBorder]);
    maxs.emplace_back(borders[maxBorder]);
    types.emplace_back(getIntervalType<TYPE>(
          borderIsOpened[minBorder],
          borderIsOpened[maxBorder]));
  }
  return Interval<TYPE>(mins,maxs,types);
}

int main(){
  return 0;
}
