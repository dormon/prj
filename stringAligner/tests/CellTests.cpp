#include <catch.hpp>

#include<StringAligner/Cell.h>
#include<StringAligner/Text.h>

using namespace stringAligner;

TEST_CASE("Cell constructor tests"){
  Cell c;
  REQUIRE(c.getAlignment() == Cell::TOP_LEFT);
  REQUIRE(c.getFiller()    == ' ');
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
