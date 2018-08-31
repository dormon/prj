#include<cstdint>
#include<type_traits>
#include<cstring>
#include<iostream>
#include<vector>
#include<sstream>
#include<memory>

#define ___ std::cerr << __FILE__ << " " << __LINE__ << std::endl

class Block{
  public:
    virtual size_t      getNofLines         (        )const = 0;
    virtual size_t      getLargestLineLength(        )const = 0;
    virtual std::string getLine             (size_t i)const = 0;
    virtual std::string getData             (        )const = 0;
};

enum class HorizonalAlignment{
  LEFT  ,
  RIGHT ,
  MIDDLE,
};

enum class VerticalAlignment{
  TOP   ,
  BOTTOM,
  MIDDLE,
};

class TextBlock: public Block{
  public:
    TextBlock(std::string const&txt = "",size_t m = 80,char f = ' '):maxLength(m),filler(f){
      lines.push_back("");
      addText(txt);
    }
    void clear(size_t maxLen = 80,char fil = ' '){
      lines.clear();
      lines.push_back("");
      maxLength = maxLen;
      filler = fil;
    }
    void addText(std::string const&txt){
      auto &line = lines.back();
      if(line.length()+txt.length() < maxLength){
        line += txt;
        return;
      }
      size_t const headLength = maxLength - line.length();
      auto const head = txt.substr(0,headLength);
      auto const tail = txt.substr(headLength,txt.length()-headLength);
      line += head;
      newLine();
      addText(tail);
    }
    void newLine(){
      lines.push_back("");
    }
    size_t getNofLines()const override{
      return lines.size();
    }
    size_t getLargestLineLength()const override{
      return maxLength;
    }
    std::string getLine(size_t i)const override{
      auto const&line = lines.at(i);
      std::stringstream ss;
      ss << line;
      size_t const fillerLength = maxLength - line.length();
      ss << std::string(fillerLength,filler);
      ss << std::endl;
      return ss.str();
    }
    std::string getData()const override{
      std::stringstream ss;
      for(size_t i=0;i<getNofLines();++i)
        ss << getLine(i);
      return ss.str();
    }
  protected:
    size_t maxLength;
    std::vector<std::string>lines;
    char filler;
};

class Row{
  public:
    Row(VerticalAlignment a = VerticalAlignment::TOP,char f = ' '):alignment(a),filler(f){}
    void addBlock(std::shared_ptr<Block>const&b){
      blocks.push_back(b);
      maxRows = std::max(maxRows,b->getNofLines());
    }
    size_t getNofRows()const{
      return maxRows;
    }
    std::string getLine(size_t b,size_t i)const{
      auto const&block = blocks.at(b);
      auto const blockRows = block->getNofLines();
      auto const fillerRows = maxRows - blockRows;
      auto const fillerLine = std::string(block->getLargestLineLength(),filler);

      blocks.at(b)->getLine(i);
      if(alignment == VerticalAlignment::TOP){
        if(i >= blockRows)return fillerLine;
        return block->getLine(i);
      }
      if(alignment == VerticalAlignment::BOTTOM){
        if(i < fillerRows)
          return fillerLine;
        return block->getLine(i-fillerRows);
      }
      auto const upFillerRows = fillerRows / 2;
      auto const downFillerRows = fillerRows - upFillerRows;
      if(i < upFillerRows)return fillerLine;
      if(i >= upFillerRows + blockRows)return fillerLine;
      return block->getLine(i-upFillerRows);
    }
    std::vector<std::shared_ptr<Block>>blocks;
    VerticalAlignment alignment;
    size_t maxRows = 0;
    char filler = ' ';
};

class Table: public Block{
  public:
    void setAlignment(size_t column,HorizonalAlignment alignment){
      while(column >= alignments.size())alignments.push_back(HorizonalAlignment::LEFT);
      alignments.at(column) = alignment;
      if(alignments.size() > columnSizes.size()){
        while(columnSizes.size() < alignments.size())
          columnSizes.push_back(0);
      }
    }
    void setDecorator(size_t column,std::string const&decorator){
      while(column >= decorators.size())decorators.push_back(" ");
      decorators.at(column) = decorator;
    }
    Table& newCell(std::string const&val){
      auto&row = rows.back();
      auto&blocks = row.blocks;
      blocks.push_back(std::make_shared<TextBlock>(val));
      if(row.blocks.size() > columnSizes.size())
        columnSizes.push_back(val.length());
      columnSizes.at(blocks.size()-1) = std::max(columnSizes.at(blocks.size()-1),val.length());
      return *this;
    }
    Table& newRow(VerticalAlignment a = VerticalAlignment::TOP,char f = ' '){
      rows.push_back(Row(a,f));
      return *this;
    }
    size_t getNofColumns()const{
      return columnSizes.size();
    }
    size_t getColumnSize(size_t i)const{
      return columnSizes.at(i);
    }
    std::string getLine(size_t i)const override{
      size_t counter = 0;
      size_t rowId = 0;
      while(i >= counter+rows.at(rowId).getNofRows()){
        counter += rows.at(rowId).getNofRows();
        rowId++;
      }
      std::stringstream ss;
      auto const&row = rows.at(rowId);
      ss << getDecorator(0);
      for(size_t c=0;c<getNofColumns();++c){
        ss << align(row.getLine(c,i-counter),getColumnSize(c),'.',getAlignment(c));
        ss << getDecorator(c+1);
      }
      ss << std::endl;
      return ss.str();
    }
    std::string getData()const override{
      std::stringstream ss;
      for(size_t r=0;r<getNofRows();++r)
        ss << getLine(r);
      return ss.str();
    }
    std::string getDecorator(size_t i)const{
      if(i >= decorators.size())return " ";
      return decorators.at(i);
    }
    HorizonalAlignment getAlignment(size_t i)const{
      if(i >= alignments.size())return HorizonalAlignment::LEFT;
      return alignments.at(i);
    }
    std::string align(std::string const&txt,size_t maxLength,char filler,HorizonalAlignment alignment)const{
      std::stringstream ss;
      size_t const length = txt.length();
      size_t const fillerLength = maxLength - length;
      if(alignment == HorizonalAlignment::LEFT){
        ss << txt;
        for(size_t i=0;i<fillerLength;++i)
          ss << filler;
      }
      if(alignment == HorizonalAlignment::RIGHT){
        for(size_t i=0;i<fillerLength;++i)
          ss << filler;
        ss << txt;
      }
      if(alignment == HorizonalAlignment::MIDDLE){
        size_t const leftFillerLength = fillerLength / 2;
        size_t const rightFillerLength = fillerLength - leftFillerLength;
        for(size_t i=0;i<leftFillerLength;++i)
          ss << filler;
        ss << txt;
        for(size_t i=0;i<rightFillerLength;++i)
          ss << filler;
      }
      return ss.str();
    }
    size_t getNofRows()const override{
      size_t counter = 0;
      for(auto const&r:rows)
        counter += r.getNofRows();
      return counter;
    }
    size_t getLargestRowLength()const override{
      size_t result = 0;
      result += getDecorator(0).length();
      for(size_t c=0;c<getNofColumns();++c){
        result += getColumnSize(c);
        result += getDecorator(c+1).length();
      }
      return result;
    }
    void setDecorators(std::vector<std::string>const&d){
      decorators = d;
    }
  protected:
    std::vector<Row>rows;
    std::vector<HorizonalAlignment>alignments;
    std::vector<std::string>decorators;
    std::vector<size_t>columnSizes;
};


int main() {
  Table t;
  t.setDecorators({""," "," "," ","",""," - "});
  t.newRow().newCell("--camera-far" ).newCell("=").newCell("inf"                                         ).newCell("[").newCell("f32"   ).newCell("]").newCell("camera far plane position"          );
  t.newRow().newCell("--camera-fovy").newCell("=").newCell("1.5708"                                      ).newCell("[").newCell("f32"   ).newCell("]").newCell("camera field of view in y direction");
  t.newRow().newCell("--model"      ).newCell("=").newCell("/media/windata/ft/prace/models/2tri/2tri.3ds").newCell("[").newCell("string").newCell("]").newCell("model file name"                    );
  t.newRow().newCell("--no-shadows" ).newCell("" ).newCell(""                                            ).newCell("" ).newCell(""      ).newCell("" ).newCell("turns off shadows"                  );
  std::cout << t.getData();

  TextBlock b("",20,'.');
  b.addText("Dormon jede na kole, veze sebou kobzole. Narazil na papoucha, velkeho to padoucha.");
  b.addText("Papouch ukradl kobzole, dal dormonovi do kebule.");
  b.addText("Dormon narazil do vola, a spadl z kola");
  std::cout << b.getData();
  return 0;
}
