#include"catch.hpp"

#include "morton.h"

TEST_CASE("middle")
{
  REQUIRE(MIDDLE(32,1920,1080,10) == 9);
  REQUIRE(MIDDLE(32,8   ,4   ,10) == 0);
  REQUIRE(MIDDLE(32,16  ,8   ,10) == 1);
  REQUIRE(MIDDLE(32,32  ,1080,10) == 9);
  REQUIRE(MIDDLE(32,512 ,512 ,2 ) == 7);

  REQUIRE(MIDDLE(64,1920,1080,10) == 8);
  REQUIRE(MIDDLE(64,8   ,4   ,10) == 0);
  REQUIRE(MIDDLE(64,16  ,8   ,10) == 1);
  REQUIRE(MIDDLE(64,32  ,1080,10) == 8);
  REQUIRE(MIDDLE(64,512 ,512 ,2 ) == 6);
}
