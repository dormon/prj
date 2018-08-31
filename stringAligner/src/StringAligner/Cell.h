#pragma once

#include<StringAligner/stringaligner_export.h>
#include<StringAligner/Fwd.h>
#include<memory>

class STRINGALIGNER_EXPORT stringAligner::Cell{
  public:
    enum Alignment{
      BOTTOM_LEFT  = 0 ,
      BOTTOM       = 1 ,
      BOTTOM_RIGHT = 2 ,
      LEFT         = 4 ,
      CENTER       = 5 ,
      RIGHT        = 6 ,
      TOP_LEFT     = 8 ,
      TOP          = 9 ,
      TOP_RIGHT    = 10,
    };
    Cell(Alignment alignment = TOP_LEFT,char filler = ' ');
    std::string getLine(size_t maxLineLength,size_t maxNofLines,size_t i)const;
    void setData(std::shared_ptr<Block>const&data);
    std::shared_ptr<Block>getData()const;
    void setFiller(char filler);
    char getFiller();
    Alignment getAlignment()const;
    void setAlignment(Alignment alignment);
    size_t getWidth()const;
    size_t getHeight()const;
  protected:
    std::shared_ptr<Block>data;
    bool changed = true;
    char filler = ' ';
    Alignment alignment;
};
