#include"catch.hpp"

#include "morton.h"

TEST_CASE("nofLevels")
{
  REQUIRE(NOF_LEVELS(32,512   ,512   ,9) == (6+7 +9 )/5 + uint32_t((6+7 +9 )%5>0));
  REQUIRE(NOF_LEVELS(32,1920  ,1080  ,9) == (8+9 +9 )/5 + uint32_t((8+9 +9 )%5>0));
  REQUIRE(NOF_LEVELS(32,1024  ,768   ,9) == (7+8 +9 )/5 + uint32_t((7+8 +9 )%5>0));
  REQUIRE(NOF_LEVELS(32,1920*2,1080*2,9) == (9+10+10)/5 + uint32_t((9+10+10)%5>0));

  REQUIRE(NOF_LEVELS(64,512   ,512   ,9) == (6+6 +9 )/6 + uint32_t((6+6 +9 )%6>0));
  REQUIRE(NOF_LEVELS(64,1920  ,1080  ,9) == (8+8 +9 )/6 + uint32_t((8+8 +9 )%6>0));
  REQUIRE(NOF_LEVELS(64,1024  ,768   ,9) == (7+7 +9 )/6 + uint32_t((7+7 +9 )%6>0));
  REQUIRE(NOF_LEVELS(64,1920*2,1080*2,9) == (9+9 +9 )/6 + uint32_t((9+9 +9 )%6>0));
}
