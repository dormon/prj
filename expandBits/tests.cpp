#include"catch.hpp"

#include "morton.h"

#define LINE_ULOG2(x,i) x<=(1u<<i)?i:

#define ULOG2(x) (\
    LINE_ULOG2(x,0u )\
    LINE_ULOG2(x,1u )\
    LINE_ULOG2(x,2u )\
    LINE_ULOG2(x,3u )\
    LINE_ULOG2(x,4u )\
    LINE_ULOG2(x,5u )\
    LINE_ULOG2(x,6u )\
    LINE_ULOG2(x,7u )\
    LINE_ULOG2(x,8u )\
    LINE_ULOG2(x,9u )\
    LINE_ULOG2(x,10u)\
    LINE_ULOG2(x,11u)\
    LINE_ULOG2(x,12u)\
    LINE_ULOG2(x,13u)\
    LINE_ULOG2(x,14u)\
    LINE_ULOG2(x,15u)\
    LINE_ULOG2(x,16u)\
    LINE_ULOG2(x,17u)\
    LINE_ULOG2(x,18u)\
    LINE_ULOG2(x,19u)\
    LINE_ULOG2(x,20u)\
    LINE_ULOG2(x,21u)\
    LINE_ULOG2(x,22u)\
    LINE_ULOG2(x,23u)\
    LINE_ULOG2(x,24u)\
    LINE_ULOG2(x,25u)\
    LINE_ULOG2(x,26u)\
    LINE_ULOG2(x,27u)\
    LINE_ULOG2(x,28u)\
    LINE_ULOG2(x,29u)\
    LINE_ULOG2(x,30u)\
    LINE_ULOG2(x,31u)\
    32\
    )

TEST_CASE("morton")
{

  for(uint32_t i=0;i<32;++i)
    std::cerr << i << " - " << ULOG2(i) << std::endl;
  REQUIRE(ULOG2(1) == 0);
  REQUIRE(ULOG2(2) == 1);

  REQUIRE(ULOG2(3) == 2);
  REQUIRE(ULOG2(4) == 2);

  REQUIRE(ULOG2(5) == 3);
  REQUIRE(ULOG2(8) == 3);

  REQUIRE(ULOG2(9 ) == 4);
  REQUIRE(ULOG2(16) == 4);

  REQUIRE(ULOG2(17 ) == 5);
  REQUIRE(ULOG2(32 ) == 5);

  REQUIRE(ULOG2(33 ) == 6);
  REQUIRE(ULOG2(64 ) == 6);

  REQUIRE(ULOG2(65 ) == 7);
  REQUIRE(ULOG2(128) == 7);
}
