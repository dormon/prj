#pragma once

#include<StringAligner/Block.h>
#include<StringAligner/Cell.h>
#include<StringAligner/stringaligner_export.h>
#include<vector>

class STRINGALIGNER_EXPORT stringAligner::Table: public Block{
  public:
    void        addRow();
    void        addColumn();
    size_t      getNofColumns()const;
    size_t      getNofRows()const;

    void        setVerticalDecorators(std::vector<std::string>const&d);
    void        setHorizontalDecorators(std::vector<std::string>const&d);
    void        setVerticalDecorator(size_t column,std::string const&decorator);
    void        setHorizontalDecorator(size_t row,std::string const&decorator);
    std::string getVerticalDecorator(size_t i)const;
    std::string getHorizontalDecorator(size_t i)const;

    void        setCell(size_t row,size_t column,std::shared_ptr<Block>const&block);
    std::shared_ptr<Block>getCell(size_t row,size_t column)const;

    size_t      getColumnWidth(size_t i)const;
    size_t      getRowHeight(size_t i)const;
    std::string getLine(size_t i)const override;
    std::string getData()const override;
    size_t      getWidth()const override;
    size_t      getHeight()const override;
  protected:
    mutable bool needToRecomputeWidth = true;
    mutable bool needToRecomputeHeight = true;
    mutable size_t width = 0;
    mutable size_t height = 0;
    std::vector<std::string>verticalDecorators;
    std::vector<std::string>horizonalDecorators;
    std::vector<size_t>columnWidths;
    std::vector<size_t>rowHeights;
    std::vector<std::vector<Cell>>cells;
    bool isDecoratorThenConvertIndexToRowIndexAndDecoratorIndex(size_t&row,size_t&decorator,size_t&i)const;
    std::string getRowLine(size_t row,size_t line)const;
    void setOnChangeCallback(std::shared_ptr<Block>const&block,size_t row,size_t column);
};
