#include<geCore/registerStdFunctions.h>
#include<geCore/stdFunctions.h>

using namespace ge::core;

void registerStdFunctions(std::shared_ptr<Producer>const&pro){
  /*
#define REG_STD(NAME,TYPE)\
  pro->registerFunction(ge::core::NAME<TYPE>::name(),ge::core::NAME<TYPE>::sharedInstance)

#define REG_STD_INTEGER(NAME)\
  REG_STD(NAME,int8_t  );\
  REG_STD(NAME,int16_t );\
  REG_STD(NAME,int32_t );\
  REG_STD(NAME,int64_t );\
  REG_STD(NAME,uint8_t );\
  REG_STD(NAME,uint16_t);\
  REG_STD(NAME,uint32_t);\
  REG_STD(NAME,uint64_t);

#define REG_STD_ALL(NAME)\
  REG_STD_INTEGER(NAME);\
  REG_STD(NAME,float );\
  REG_STD(NAME,double)

  REG_STD_ALL(Add );
  REG_STD_ALL(Sub );
  REG_STD_ALL(Mul );
  REG_STD_ALL(Div );
  REG_STD_ALL(Ass );
  REG_STD_ALL(Adds);
  REG_STD_ALL(Subs);
  REG_STD_ALL(Muls);
  REG_STD_ALL(Divs);
  REG_STD_ALL(Less);
  REG_STD_ALL(Leq);
  REG_STD_ALL(Greater);
  REG_STD_ALL(Geq);
  REG_STD_ALL(Eq);
  REG_STD_ALL(Neq);
  REG_STD_INTEGER(Mod);
  REG_STD_INTEGER(LShift);
  REG_STD_INTEGER(RShift);
  REG_STD_INTEGER(BAnd);
  REG_STD_INTEGER(BOr);
  REG_STD_INTEGER(BXor);
  REG_STD_INTEGER(Mods);
  REG_STD_INTEGER(LShifts);
  REG_STD_INTEGER(RShifts);
  REG_STD_INTEGER(BAnds);
  REG_STD_INTEGER(BOrs);
  REG_STD_INTEGER(BXors);
  REG_STD_INTEGER(IncrPre);
  REG_STD_INTEGER(IncrPost);
  REG_STD_INTEGER(DecrPre);
  REG_STD_INTEGER(DecrPost);
// */
/*
#define REG_CAST(FROM,TO)\
  pro->registerFunction(ge::core::Cast<FROM,TO>::name(),ge::core::Cast<FROM,TO>::sharedFactory())

#define REG_CAST_FROM_ALL(TO)\
  REG_CAST(int8_t,TO);\
  REG_CAST(int16_t,TO);\
  REG_CAST(int32_t,TO);\
  REG_CAST(int64_t,TO);\
  REG_CAST(uint8_t,TO);\
  REG_CAST(uint16_t,TO);\
  REG_CAST(uint32_t,TO);\
  REG_CAST(uint64_t,TO);\
  REG_CAST(float,TO);\
  REG_CAST(double,TO)

  REG_CAST_FROM_ALL(int8_t);
  REG_CAST_FROM_ALL(int16_t);
  REG_CAST_FROM_ALL(int32_t);
  REG_CAST_FROM_ALL(int64_t);
  REG_CAST_FROM_ALL(uint8_t);
  REG_CAST_FROM_ALL(uint16_t);
  REG_CAST_FROM_ALL(uint32_t);
  REG_CAST_FROM_ALL(uint64_t);
  REG_CAST_FROM_ALL(float);
  REG_CAST_FROM_ALL(double);
  // */
}
