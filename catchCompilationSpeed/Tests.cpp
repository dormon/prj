#include<catch.hpp>

unsigned factorial(unsigned i){
  if(i<1)return 1;
  return factorial(i-1) * i;
}

TEST_CASE("asa"){
  REQUIRE(factorial(0) == 1 );
  REQUIRE(factorial(1) == 1 );
  REQUIRE(factorial(2) == 2 );
  REQUIRE(factorial(3) == 6 );
  REQUIRE(factorial(4) == 24);
}
