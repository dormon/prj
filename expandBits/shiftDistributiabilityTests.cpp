#include"catch.hpp"

uint32_t mulThenShift(uint32_t a,uint32_t b,uint32_t c){
  return (a*b) << c;
}

uint32_t shiftThenMul(uint32_t a,uint32_t b,uint32_t c){
  return a*(b<<c);
}

bool testShiftDistributiabilityOverMul(uint32_t a,uint32_t b,uint32_t c){
  return mulThenShift(a,b,c) == shiftThenMul(a,b,c);
}

uint32_t andThenShift(uint32_t a,uint32_t b,uint32_t c){
  return (a&b) << c;
}

uint32_t shiftThenAnd(uint32_t a,uint32_t b,uint32_t c){
  return (a<<c) & (b<<c);
}

bool testShiftDistributiabilityOverAnd(uint32_t a,uint32_t b,uint32_t c){
  return andThenShift(a,b,c) == shiftThenAnd(a,b,c);
}

TEST_CASE("shiftDistributiability")
{
  REQUIRE(testShiftDistributiabilityOverMul(0x33,0x76,3));
  REQUIRE(testShiftDistributiabilityOverMul(0x76,0x16,2));
  REQUIRE(testShiftDistributiabilityOverMul(0x189,0x346,1));
  REQUIRE(testShiftDistributiabilityOverAnd(0x189,0x346,1));
  REQUIRE(testShiftDistributiabilityOverAnd(0x189,0x346,2));
  REQUIRE(testShiftDistributiabilityOverAnd(0x189,0x346,3));
}
