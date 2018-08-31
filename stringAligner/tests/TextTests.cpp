#include<catch.hpp>
#include<StringAligner/Text.h>

using namespace stringAligner;

TEST_CASE("Text default constructor"){
  Text txt{};
  REQUIRE(txt.getHeight() == 0);
  REQUIRE(txt.getWidth() == 80);
}

TEST_CASE("Text full constructor"){
  Text txt{"hi world!",10,'.'};
  REQUIRE(txt.getHeight() == 1);
  REQUIRE(txt.getWidth() == 10);
  REQUIRE(txt.getLine(0) == "hi world!.");
}

TEST_CASE("Text overflow full constructor"){
  Text txt{"0123456789ABCDEF",10,'.'};
  REQUIRE(txt.getHeight() == 2);
  REQUIRE(txt.getWidth() == 10);
  REQUIRE(txt.getLine(0) == "0123456789");
  REQUIRE(txt.getLine(1) == "ABCDEF....");
}

TEST_CASE("Text overflow2 full constructor"){
  Text txt{"01234567890123456789A",10,'.'};
  REQUIRE(txt.getHeight() == 3);
  REQUIRE(txt.getWidth() == 10);
  REQUIRE(txt.getLine(0) == "0123456789");
  REQUIRE(txt.getLine(1) == "0123456789");
  REQUIRE(txt.getLine(2) == "A.........");
}

TEST_CASE("Text test"){
  Text txt{};
  REQUIRE(txt.getHeight() == 0);
  REQUIRE(txt.getWidth() == 80);
  txt.clear(10,'.');
  REQUIRE(txt.getHeight() == 0);
  REQUIRE(txt.getWidth() == 10);
  txt.addText("hi");
  REQUIRE(txt.getHeight() == 1);
  REQUIRE(txt.getWidth() == 10);
  REQUIRE(txt.getLine(0) == "hi........");
  txt.addText("hi");
  REQUIRE(txt.getHeight() == 1);
  REQUIRE(txt.getWidth() == 10);
  REQUIRE(txt.getLine(0) == "hihi......");
  txt.newLine();
  REQUIRE(txt.getHeight() == 2);
  REQUIRE(txt.getWidth() == 10);
  REQUIRE(txt.getLine(0) == "hihi......");
  REQUIRE(txt.getLine(1) == "..........");
  txt.addText("0123456789ABCDEF");
  REQUIRE(txt.getHeight() == 3);
  REQUIRE(txt.getWidth() == 10);
  REQUIRE(txt.getLine(0) == "hihi......");
  REQUIRE(txt.getLine(1) == "0123456789");
  REQUIRE(txt.getLine(2) == "ABCDEF....");
}

