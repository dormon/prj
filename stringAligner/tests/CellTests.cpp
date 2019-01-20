#include <catch.hpp>

#include<StringAligner/Cell.h>
#include<StringAligner/Text.h>

using namespace stringAligner;

TEST_CASE("Cell constructor tests"){
  Cell c;
  REQUIRE(c.getAlignment() == Cell::TOP_LEFT);
  REQUIRE(c.getFiller()    == ' ');
}

struct Padding{
  size_t left = 0;
  size_t right = 0;
  size_t bottom = 0;
  size_t top = 0;
};
Padding getPadding(Cell::Alignment a,size_t maxWidth,size_t maxHeight,size_t blockWidth,size_t blockHeight);

TEST_CASE("Cell_padding"){
  REQUIRE(getPadding(Cell::BOTTOM_LEFT ,10,14,5,7).left   == 0);
  REQUIRE(getPadding(Cell::BOTTOM_LEFT ,10,14,5,7).right  == 5);
  REQUIRE(getPadding(Cell::BOTTOM_LEFT ,10,14,5,7).bottom == 0);
  REQUIRE(getPadding(Cell::BOTTOM_LEFT ,10,14,5,7).top    == 7);
  REQUIRE(getPadding(Cell::BOTTOM      ,10,14,5,7).left   == 2);
  REQUIRE(getPadding(Cell::BOTTOM      ,10,14,5,7).right  == 3);
  REQUIRE(getPadding(Cell::BOTTOM      ,10,14,5,7).bottom == 0);
  REQUIRE(getPadding(Cell::BOTTOM      ,10,14,5,7).top    == 7);
  REQUIRE(getPadding(Cell::BOTTOM_RIGHT,10,14,5,7).left   == 5);
  REQUIRE(getPadding(Cell::BOTTOM_RIGHT,10,14,5,7).right  == 0);
  REQUIRE(getPadding(Cell::BOTTOM_RIGHT,10,14,5,7).bottom == 0);
  REQUIRE(getPadding(Cell::BOTTOM_RIGHT,10,14,5,7).top    == 7);
  REQUIRE(getPadding(Cell::LEFT        ,10,14,5,7).left   == 0);
  REQUIRE(getPadding(Cell::LEFT        ,10,14,5,7).right  == 5);
  REQUIRE(getPadding(Cell::LEFT        ,10,14,5,7).bottom == 4);
  REQUIRE(getPadding(Cell::LEFT        ,10,14,5,7).top    == 3);
  REQUIRE(getPadding(Cell::CENTER      ,10,14,5,7).left   == 2);
  REQUIRE(getPadding(Cell::CENTER      ,10,14,5,7).right  == 3);
  REQUIRE(getPadding(Cell::CENTER      ,10,14,5,7).bottom == 4);
  REQUIRE(getPadding(Cell::CENTER      ,10,14,5,7).top    == 3);
  REQUIRE(getPadding(Cell::RIGHT       ,10,14,5,7).left   == 5);
  REQUIRE(getPadding(Cell::RIGHT       ,10,14,5,7).right  == 0);
  REQUIRE(getPadding(Cell::RIGHT       ,10,14,5,7).bottom == 4);
  REQUIRE(getPadding(Cell::RIGHT       ,10,14,5,7).top    == 3);
  REQUIRE(getPadding(Cell::TOP_LEFT    ,10,14,5,7).left   == 0);
  REQUIRE(getPadding(Cell::TOP_LEFT    ,10,14,5,7).right  == 5);
  REQUIRE(getPadding(Cell::TOP_LEFT    ,10,14,5,7).bottom == 7);
  REQUIRE(getPadding(Cell::TOP_LEFT    ,10,14,5,7).top    == 0);
  REQUIRE(getPadding(Cell::TOP         ,10,14,5,7).left   == 2);
  REQUIRE(getPadding(Cell::TOP         ,10,14,5,7).right  == 3);
  REQUIRE(getPadding(Cell::TOP         ,10,14,5,7).bottom == 7);
  REQUIRE(getPadding(Cell::TOP         ,10,14,5,7).top    == 0);
  REQUIRE(getPadding(Cell::TOP_RIGHT   ,10,14,5,7).left   == 5);
  REQUIRE(getPadding(Cell::TOP_RIGHT   ,10,14,5,7).right  == 0);
  REQUIRE(getPadding(Cell::TOP_RIGHT   ,10,14,5,7).bottom == 7);
  REQUIRE(getPadding(Cell::TOP_RIGHT   ,10,14,5,7).top    == 0);

  REQUIRE(getPadding(Cell::BOTTOM_LEFT ,5,6,5,6).left   == 0);
  REQUIRE(getPadding(Cell::BOTTOM_LEFT ,5,6,5,6).right  == 0);
  REQUIRE(getPadding(Cell::BOTTOM_LEFT ,5,6,5,6).bottom == 0);
  REQUIRE(getPadding(Cell::BOTTOM_LEFT ,5,6,5,6).top    == 0);
  REQUIRE(getPadding(Cell::BOTTOM      ,5,6,5,6).left   == 0);
  REQUIRE(getPadding(Cell::BOTTOM      ,5,6,5,6).right  == 0);
  REQUIRE(getPadding(Cell::BOTTOM      ,5,6,5,6).bottom == 0);
  REQUIRE(getPadding(Cell::BOTTOM      ,5,6,5,6).top    == 0);
  REQUIRE(getPadding(Cell::BOTTOM_RIGHT,5,6,5,6).left   == 0);
  REQUIRE(getPadding(Cell::BOTTOM_RIGHT,5,6,5,6).right  == 0);
  REQUIRE(getPadding(Cell::BOTTOM_RIGHT,5,6,5,6).bottom == 0);
  REQUIRE(getPadding(Cell::BOTTOM_RIGHT,5,6,5,6).top    == 0);
  REQUIRE(getPadding(Cell::LEFT        ,5,6,5,6).left   == 0);
  REQUIRE(getPadding(Cell::LEFT        ,5,6,5,6).right  == 0);
  REQUIRE(getPadding(Cell::LEFT        ,5,6,5,6).bottom == 0);
  REQUIRE(getPadding(Cell::LEFT        ,5,6,5,6).top    == 0);
  REQUIRE(getPadding(Cell::CENTER      ,5,6,5,6).left   == 0);
  REQUIRE(getPadding(Cell::CENTER      ,5,6,5,6).right  == 0);
  REQUIRE(getPadding(Cell::CENTER      ,5,6,5,6).bottom == 0);
  REQUIRE(getPadding(Cell::CENTER      ,5,6,5,6).top    == 0);
  REQUIRE(getPadding(Cell::RIGHT       ,5,6,5,6).left   == 0);
  REQUIRE(getPadding(Cell::RIGHT       ,5,6,5,6).right  == 0);
  REQUIRE(getPadding(Cell::RIGHT       ,5,6,5,6).bottom == 0);
  REQUIRE(getPadding(Cell::RIGHT       ,5,6,5,6).top    == 0);
  REQUIRE(getPadding(Cell::TOP_LEFT    ,5,6,5,6).left   == 0);
  REQUIRE(getPadding(Cell::TOP_LEFT    ,5,6,5,6).right  == 0);
  REQUIRE(getPadding(Cell::TOP_LEFT    ,5,6,5,6).bottom == 0);
  REQUIRE(getPadding(Cell::TOP_LEFT    ,5,6,5,6).top    == 0);
  REQUIRE(getPadding(Cell::TOP         ,5,6,5,6).left   == 0);
  REQUIRE(getPadding(Cell::TOP         ,5,6,5,6).right  == 0);
  REQUIRE(getPadding(Cell::TOP         ,5,6,5,6).bottom == 0);
  REQUIRE(getPadding(Cell::TOP         ,5,6,5,6).top    == 0);
  REQUIRE(getPadding(Cell::TOP_RIGHT   ,5,6,5,6).left   == 0);
  REQUIRE(getPadding(Cell::TOP_RIGHT   ,5,6,5,6).right  == 0);
  REQUIRE(getPadding(Cell::TOP_RIGHT   ,5,6,5,6).bottom == 0);
  REQUIRE(getPadding(Cell::TOP_RIGHT   ,5,6,5,6).top    == 0);
}

TEST_CASE("Cell tests"){
  Cell c;
  c.setData(std::make_shared<Text>("hi",5,'.'));
  REQUIRE(c.getLine(10,10,0) == "hi...     ");
  REQUIRE(c.getLine(10,10,1) == "          ");
  REQUIRE(c.getLine(10,10,2) == "          ");
  c.setFiller('x');
  REQUIRE(c.getLine(8,4,0) == "hi...xxx");
  REQUIRE(c.getLine(8,4,1) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,2) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,3) == "xxxxxxxx");
  c.setAlignment(Cell::TOP);
  REQUIRE(c.getLine(8,4,0) == "xhi...xx");
  REQUIRE(c.getLine(8,4,1) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,2) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,3) == "xxxxxxxx");
  c.setAlignment(Cell::TOP_RIGHT);
  REQUIRE(c.getLine(8,4,0) == "xxxhi...");
  REQUIRE(c.getLine(8,4,1) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,2) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,3) == "xxxxxxxx");
  c.setAlignment(Cell::LEFT);
  REQUIRE(c.getLine(8,4,0) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,1) == "hi...xxx");
  REQUIRE(c.getLine(8,4,2) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,3) == "xxxxxxxx");
  c.setAlignment(Cell::CENTER);
  REQUIRE(c.getLine(8,4,0) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,1) == "xhi...xx");
  REQUIRE(c.getLine(8,4,2) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,3) == "xxxxxxxx");
  c.setAlignment(Cell::RIGHT);
  REQUIRE(c.getLine(8,4,0) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,1) == "xxxhi...");
  REQUIRE(c.getLine(8,4,2) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,3) == "xxxxxxxx");
  c.setAlignment(Cell::BOTTOM_LEFT);
  REQUIRE(c.getLine(8,4,0) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,1) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,2) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,3) == "hi...xxx");
  c.setAlignment(Cell::BOTTOM);
  REQUIRE(c.getLine(8,4,0) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,1) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,2) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,3) == "xhi...xx");
  c.setAlignment(Cell::BOTTOM_RIGHT);
  REQUIRE(c.getLine(8,4,0) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,1) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,2) == "xxxxxxxx");
  REQUIRE(c.getLine(8,4,3) == "xxxhi...");
}
