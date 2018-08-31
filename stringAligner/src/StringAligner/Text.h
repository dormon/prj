#pragma once

#include<StringAligner/Fwd.h>
#include<StringAligner/Block.h>
#include<StringAligner/stringaligner_export.h>
#include<vector>

class STRINGALIGNER_EXPORT stringAligner::Text: public Block{
  public:
    Text(std::string const&txt = "",size_t m = 80,char f = ' ');
    void clear(size_t maxWid = 80,char fil = ' ');
    void addText(std::string const&txt);
    void newLine();
    size_t getWidth()const override;
    size_t getHeight()const override;
    std::string getLine(size_t i)const override;
    std::string getData()const override;
  protected:
    void addLine();
    void addTextRec(std::string const&txt);
    size_t maxWidth;
    std::vector<std::string>lines;
    char filler;
};
