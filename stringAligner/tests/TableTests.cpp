#include<catch.hpp>

#include<StringAligner/Table.h>
#include<StringAligner/Text.h>
using namespace stringAligner;

TEST_CASE("Table constructor tests"){
  Table t;
  REQUIRE(t.getNofRows() == 0);
  REQUIRE(t.getNofColumns() == 0);
  REQUIRE(t.getHeight() == 0);
  REQUIRE(t.getWidth() == 0);
  REQUIRE(t.getData() == "");
}

TEST_CASE("Table decorator tests"){
  Table t;
  REQUIRE(t.getHorizontalDecorator(0) == "");
  REQUIRE(t.getVerticalDecorator(0) == "");
  REQUIRE(t.getHorizontalDecorator(100) == "");
  REQUIRE(t.getVerticalDecorator(100) == "");
  t.setVerticalDecorator(0,".");
  REQUIRE(t.getVerticalDecorator(0) == ".");
  REQUIRE(t.getVerticalDecorator(1) == "");
  t.setVerticalDecorator(5,"-");
  REQUIRE(t.getVerticalDecorator(0) == ".");
  REQUIRE(t.getVerticalDecorator(1) == "");
  REQUIRE(t.getVerticalDecorator(2) == "");
  REQUIRE(t.getVerticalDecorator(3) == "");
  REQUIRE(t.getVerticalDecorator(4) == "");
  REQUIRE(t.getVerticalDecorator(5) == "-");
  REQUIRE(t.getVerticalDecorator(6) == "");
  t.setVerticalDecorators({"a","b","c","d","e","f","g"});
  REQUIRE(t.getVerticalDecorator(0) == "a");
  REQUIRE(t.getVerticalDecorator(1) == "b");
  REQUIRE(t.getVerticalDecorator(2) == "c");
  REQUIRE(t.getVerticalDecorator(3) == "d");
  REQUIRE(t.getVerticalDecorator(4) == "e");
  REQUIRE(t.getVerticalDecorator(5) == "f");
  REQUIRE(t.getVerticalDecorator(6) == "g");

  t.setHorizontalDecorator(0,".");
  REQUIRE(t.getHorizontalDecorator(0) == ".");
  REQUIRE(t.getHorizontalDecorator(1) == "");
  t.setHorizontalDecorator(5,"-");
  REQUIRE(t.getHorizontalDecorator(0) == ".");
  REQUIRE(t.getHorizontalDecorator(1) == "");
  REQUIRE(t.getHorizontalDecorator(2) == "");
  REQUIRE(t.getHorizontalDecorator(3) == "");
  REQUIRE(t.getHorizontalDecorator(4) == "");
  REQUIRE(t.getHorizontalDecorator(5) == "-");
  REQUIRE(t.getHorizontalDecorator(6) == "");
  t.setHorizontalDecorators({"a","b","c","d","e","f","g"});
  REQUIRE(t.getHorizontalDecorator(0) == "a");
  REQUIRE(t.getHorizontalDecorator(1) == "b");
  REQUIRE(t.getHorizontalDecorator(2) == "c");
  REQUIRE(t.getHorizontalDecorator(3) == "d");
  REQUIRE(t.getHorizontalDecorator(4) == "e");
  REQUIRE(t.getHorizontalDecorator(5) == "f");
  REQUIRE(t.getHorizontalDecorator(6) == "g");

}

TEST_CASE("Table addRow and than addColumn tests"){
  Table t;
  REQUIRE(t.getNofRows() == 0);
  REQUIRE(t.getNofColumns() == 0);
  t.addRow();
  REQUIRE(t.getNofRows() == 1);
  REQUIRE(t.getNofColumns() == 0);
  t.addRow();
  REQUIRE(t.getNofRows() == 2);
  REQUIRE(t.getNofColumns() == 0);
  t.addColumn();
  REQUIRE(t.getNofRows() == 2);
  REQUIRE(t.getNofColumns() == 1);
  t.addColumn();
  REQUIRE(t.getNofRows() == 2);
  REQUIRE(t.getNofColumns() == 2);
}

TEST_CASE("Table addColumn and than addRow tests"){
  Table t;
  REQUIRE(t.getNofRows() == 0);
  REQUIRE(t.getNofColumns() == 0);
  t.addColumn();
  REQUIRE(t.getNofRows() == 0);
  REQUIRE(t.getNofColumns() == 0);
  t.addColumn();
  REQUIRE(t.getNofRows() == 0);
  REQUIRE(t.getNofColumns() == 0);
  t.addRow();
  REQUIRE(t.getNofRows() == 1);
  REQUIRE(t.getNofColumns() == 0);
  t.addRow();
  REQUIRE(t.getNofRows() == 2);
  REQUIRE(t.getNofColumns() == 0);
}

TEST_CASE("Table setCell"){
  Table t;
  REQUIRE(t.getNofRows() == 0);
  REQUIRE(t.getNofColumns() == 0);
  t.addRow();
  t.addColumn();
  REQUIRE(t.getNofRows() == 1);
  REQUIRE(t.getNofColumns() == 1);
  t.setCell(0,0,std::make_shared<Text>("Hi world!",10,'.'));

  REQUIRE(t.getRowHeight(0) == 1);
  REQUIRE(t.getColumnWidth(0) == 10);
  REQUIRE(t.getHeight() == 1);
  REQUIRE(t.getWidth() == 10);
  REQUIRE(t.getLine(0) == "Hi world!.");

  t.addRow();
  t.addColumn();
  REQUIRE(t.getNofRows() == 2);
  REQUIRE(t.getNofColumns() == 2);
  t.setCell(1,1,std::make_shared<Text>("wawa",5,'.'));

  REQUIRE(t.getRowHeight(0) == 1);
  REQUIRE(t.getRowHeight(1) == 1);
  REQUIRE(t.getColumnWidth(0) == 10);
  REQUIRE(t.getColumnWidth(1) == 5);
  REQUIRE(t.getHeight() == 2);
  REQUIRE(t.getWidth() == 15);
  REQUIRE(t.getLine(0) == "Hi world!.     ");
  REQUIRE(t.getLine(1) == "          wawa.");

  std::dynamic_pointer_cast<Text>(t.getCell(1,1))->addText("asd");
  auto cell = std::dynamic_pointer_cast<Text>(t.getCell(1,1));
  REQUIRE(cell->getWidth() == 5);
  REQUIRE(cell->getHeight() == 2);
  REQUIRE(t.getRowHeight(0) == 1);
  REQUIRE(t.getRowHeight(1) == 2);
  REQUIRE(t.getColumnWidth(0) == 10);
  REQUIRE(t.getColumnWidth(1) == 5);
  REQUIRE(t.getHeight() == 3);
  REQUIRE(t.getWidth() == 15);
  REQUIRE(t.getLine(0) == "Hi world!.     ");
  REQUIRE(t.getLine(1) == "          wawaa");
  REQUIRE(t.getLine(2) == "          sd...");
  REQUIRE(t.getData()  == "Hi world!.     \n"
                          "          wawaa\n"
                          "          sd...\n");
  t.setHorizontalDecorators({"|","||","|"});
  REQUIRE(t.getLine(0) == "|Hi world!.||     |");
  REQUIRE(t.getLine(1) == "|          ||wawaa|");
  REQUIRE(t.getLine(2) == "|          ||sd...|");
  t.setVerticalDecorators({"_","-","X"});
  REQUIRE(t.getLine(0) == "___________________");
  REQUIRE(t.getLine(1) == "|Hi world!.||     |");
  REQUIRE(t.getLine(2) == "-------------------");
  REQUIRE(t.getLine(3) == "|          ||wawaa|");
  REQUIRE(t.getLine(4) == "|          ||sd...|");
  REQUIRE(t.getLine(5) == "XXXXXXXXXXXXXXXXXXX");
}

template<typename R,typename...A,typename...B>
bool didThrow(Table&t,R(Table::*fce)(A...),B...b){
  try{
    (t.*fce)(b...);
  }catch(std::runtime_error&e){
    return true;
  }
  return false;
}

template<typename R,typename...A,typename...B>
bool didThrow(Table const&t,R(Table::*fce)(A...)const,B...b){
  try{
    (t.*fce)(b...);
  }catch(std::runtime_error&e){
    return true;
  }
  return false;
}

TEST_CASE("Table exceptions tests"){
  Table t;
  REQUIRE(didThrow(t,&Table::getRowHeight,0) == true);
  REQUIRE(didThrow(t,&Table::getColumnWidth,0) == true);
  REQUIRE(didThrow(t,&Table::getLine,0) == true);
  REQUIRE(didThrow(t,&Table::getCell,0,0) == true);
  REQUIRE(didThrow(t,&Table::setCell,0,0,nullptr) == true);
}

TEST_CASE("Table setCell and eraseData"){
  Table t;
  t.addRow();
  t.addRow();
  t.addColumn();
  t.addColumn();
  REQUIRE(t.getNofRows() == 2);
  REQUIRE(t.getNofColumns() == 2);
  REQUIRE(t.getRowHeight(0)==0);
  REQUIRE(t.getRowHeight(1)==0);
  REQUIRE(t.getColumnWidth(0)==0);
  REQUIRE(t.getColumnWidth(1)==0);
  t.setCell(0,0,std::make_shared<Text>("test",5,'.'));
  t.setCell(1,1,std::make_shared<Text>("test",5,'.'));

  REQUIRE(t.getNofRows() == 2);
  REQUIRE(t.getNofColumns() == 2);
  REQUIRE(t.getRowHeight(0)==1);
  REQUIRE(t.getRowHeight(1)==1);
  REQUIRE(t.getColumnWidth(0)==5);
  REQUIRE(t.getColumnWidth(1)==5);
  REQUIRE(t.getLine(0) == "test.     ");
  REQUIRE(t.getLine(1) == "     test.");
  t.setCell(0,0,nullptr);
  REQUIRE(t.getNofRows() == 2);
  REQUIRE(t.getNofColumns() == 2);
  REQUIRE(t.getRowHeight(0)==0);
  REQUIRE(t.getRowHeight(1)==1);
  REQUIRE(t.getColumnWidth(0)==0);
  REQUIRE(t.getColumnWidth(1)==5);
  REQUIRE(t.getLine(0) == "test.");
}

TEST_CASE("Table getData tests simple simple0"){
  Table t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addColumn();
  REQUIRE(t.getNofRows() == 1);
  REQUIRE(t.getNofColumns() == 1);
  REQUIRE(t.getHeight() == 0);
  REQUIRE(t.getWidth() == 0);
  t.setHorizontalDecorators({"|","|"});
  REQUIRE(t.getData() == "");
}

TEST_CASE("Table getData tests simple0"){
  Table t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addColumn();
  t.setHorizontalDecorators({"|","|"});
  t.setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  REQUIRE(t.getData() == "|hi...|\n");
}

TEST_CASE("Table_getCellInternal"){
  class Tab2: public Table{
    public:
      Cell const& getCellInternal(size_t row,size_t line)const{
        return Table::getCellInternal(row,line);
      }
  };
  Tab2 t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addColumn();
  t.addColumn();
  t.setHorizontalDecorators({"|","|","|"});
  auto Block0 = std::make_shared<Text>("hi",5,'.');
  auto Block1 = std::make_shared<Text>("hi",5,'.');
  REQUIRE(Block0->getWidth() == 5);
  REQUIRE(Block1->getWidth() == 5);
  REQUIRE(Block0->getHeight() == 1);
  REQUIRE(Block1->getHeight() == 1);
  REQUIRE(Block0->getFiller() == '.');
  REQUIRE(Block1->getFiller() == '.');
  t.setCell(0,0,Block0);
  t.setCell(0,1,Block1);
  auto const&cell0 = t.getCellInternal(0,0);
  REQUIRE(cell0.getFiller() == ' ');
  REQUIRE(cell0.getData() == Block0);
  REQUIRE(t.getColumnWidth(0) == 5);
  REQUIRE(t.getRowHeight(0) == 1);
  REQUIRE(cell0.getLine(t.getColumnWidth(0),t.getRowHeight(0),0) == "hi...");
  auto const&cell1 = t.getCellInternal(0,1);
  REQUIRE(cell1.getFiller() == ' ');
  REQUIRE(cell1.getData() == Block1);
  REQUIRE(t.getColumnWidth(1) == 5);
  REQUIRE(cell1.getLine(t.getColumnWidth(1),t.getRowHeight(0),0) == "hi...");
}

TEST_CASE("Table_getRowLine"){
  class Tab2: public Table{
    public:
      std::string getRowLine(size_t row,size_t line)const{
        return Table::getRowLine(row,line);
      }
  };
  Tab2 t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addColumn();
  t.addColumn();
  t.setHorizontalDecorators({"|","|","|"});
  t.setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t.setCell(0,1,std::make_shared<Text>("hi",5,'.'));
  REQUIRE(t.getRowLine(0,0) == "|hi...|hi...|");
}

TEST_CASE("Table getData tests simple1"){
  Table t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addColumn();
  t.addColumn();
  t.setHorizontalDecorators({"|","|","|"});
  t.setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t.setCell(0,1,std::make_shared<Text>("hi",5,'.'));
  REQUIRE(t.getLine(0) == "|hi...|hi...|");
  REQUIRE(t.getData() == "|hi...|hi...|\n");
}

TEST_CASE("Table getData tests simple2"){
  Table t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addColumn();
  t.addColumn();
  t.addColumn();
  t.setHorizontalDecorators({"|","|","|","|"});
  t.setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t.setCell(0,1,std::make_shared<Text>("hi",5,'.'));
  t.setCell(0,2,std::make_shared<Text>("hi",5,'.'));
  REQUIRE(t.getData() == "|hi...|hi...|hi...|\n");
}

TEST_CASE("Table getData tests simple3"){
  Table t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addColumn();
  t.addColumn();
  t.addColumn();
  t.addColumn();
  t.setHorizontalDecorators({"|","|","|","|","|"});
  t.setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t.setCell(0,1,std::make_shared<Text>("hi",5,'.'));
  t.setCell(0,2,std::make_shared<Text>("hi",5,'.'));
  t.setCell(0,2,std::make_shared<Text>("hi",5,'.'));
  REQUIRE(t.getData() == "|hi...|hi...|hi...||\n");
}


TEST_CASE("Table getData tests"){
  Table t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addColumn();
  t.addColumn();
  t.addColumn();
  t.addColumn();
  t.setHorizontalDecorators({"|","|","|","|","|"});
  t.setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t.setCell(0,1,std::make_shared<Text>("world",5,'.'));
  t.setCell(0,2,std::make_shared<Text>("this",5,'.'));
  t.setCell(0,3,std::make_shared<Text>("is",5,'.'));
  REQUIRE(t.getCell(0,0)->getHeight() == 1);
  REQUIRE(t.getCell(0,1)->getHeight() == 1);
  REQUIRE(t.getCell(0,2)->getHeight() == 1);
  REQUIRE(t.getCell(0,3)->getHeight() == 1);
  REQUIRE(t.getHeight() == 1);
  REQUIRE(t.getData() == "|hi...|world|this.|is...|\n");
}

TEST_CASE("Table_in_table"){
  Table t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addRow();
  t.addColumn();
  t.addColumn();
  t.setHorizontalDecorators({"|","|","|"});
  auto t00 = std::make_shared<Table>();
  auto t01 = std::make_shared<Table>();
  auto t10 = std::make_shared<Table>();
  auto t11 = std::make_shared<Table>();
  t00->addRow();
  t00->addColumn();
  t00->setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t01->addRow();
  t01->addColumn();
  t01->setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t10->addRow();
  t10->addColumn();
  t10->setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t11->addRow();
  t11->addColumn();
  t11->setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t.setCell(0,0,t00);
  t.setCell(0,1,t01);
  t.setCell(1,0,t10);
  t.setCell(1,1,t11);
  REQUIRE(t.getData() == "|hi...|hi...|\n|hi...|hi...|\n");
}

TEST_CASE("Table_in_table1"){
  Table t;
  REQUIRE(t.getData() == "");
  t.addRow();
  t.addRow();
  t.addColumn();
  t.addColumn();
  t.setHorizontalDecorators({"|","|","|"});
  auto t00 = std::make_shared<Table>();
  auto t01 = std::make_shared<Table>();
  auto t10 = std::make_shared<Table>();
  auto t11 = std::make_shared<Table>();
  t00->addRow();
  t00->addRow();
  t00->addColumn();
  t00->addColumn();
  t00->setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t00->setCell(0,1,std::make_shared<Text>("hi",5,'.'));
  t00->setCell(1,0,std::make_shared<Text>("hi",5,'.'));
  t00->setCell(1,1,std::make_shared<Text>("hi",5,'.'));

  t01->addRow();
  t01->addRow();
  t01->addColumn();
  t01->addColumn();
  t01->setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t01->setCell(0,1,std::make_shared<Text>("hi",5,'.'));
  t01->setCell(1,0,std::make_shared<Text>("hi",5,'.'));
  t01->setCell(1,1,std::make_shared<Text>("hi",5,'.'));

  t10->addRow();
  t10->addRow();
  t10->addColumn();
  t10->addColumn();
  t10->setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t10->setCell(0,1,std::make_shared<Text>("hi",5,'.'));
  t10->setCell(1,0,std::make_shared<Text>("hi",5,'.'));
  t10->setCell(1,1,std::make_shared<Text>("hi",5,'.'));

  t11->addRow();
  t11->addRow();
  t11->addColumn();
  t11->addColumn();
  t11->setCell(0,0,std::make_shared<Text>("hi",5,'.'));
  t11->setCell(0,1,std::make_shared<Text>("hi",5,'.'));
  t11->setCell(1,0,std::make_shared<Text>("hi",5,'.'));
  t11->setCell(1,1,std::make_shared<Text>("hi",5,'.'));

  t.setCell(0,0,t00);
  t.setCell(0,1,t01);
  t.setCell(1,0,t10);
  t.setCell(1,1,t11);
  REQUIRE(t.getData() == "|hi...hi...|hi...hi...|\n|hi...hi...|hi...hi...|\n|hi...hi...|hi...hi...|\n|hi...hi...|hi...hi...|\n");
}
