#include <StringAligner/Cell.h>
#include <StringAligner/Block.h>
#include <sstream>

using namespace stringAligner;
using namespace std;

Cell::Cell(Alignment a,char f):alignment(a),filler(f){
}

void Cell::setData(shared_ptr<Block>const&d){
  data = d;
}

shared_ptr<Block>Cell::getData()const{
  return data;
}

void Cell::setFiller(char f){
  filler = f;
}

char Cell::getFiller(){
  return filler;
}

Cell::Alignment Cell::getAlignment()const{
  return alignment;
}

void Cell::setAlignment(Alignment a){
  alignment = a;
}

struct Padding{
  size_t left = 0;
  size_t right = 0;
  size_t bottom = 0;
  size_t top = 0;
};

bool isLeft(Cell::Alignment a){
  return a == Cell::BOTTOM_LEFT || a == Cell::LEFT || a == Cell::TOP_LEFT;
}

bool isRight(Cell::Alignment a){
  return a == Cell::BOTTOM_RIGHT || a == Cell::RIGHT || a == Cell::TOP_RIGHT;
}

bool isTop(Cell::Alignment a){
  return a == Cell::TOP_LEFT || a == Cell::TOP || a == Cell::TOP_RIGHT;
}

bool isBottom(Cell::Alignment a){
  return a == Cell::BOTTOM_LEFT || a == Cell::BOTTOM || a == Cell::BOTTOM_RIGHT;
}

bool isHorizontalCenter(Cell::Alignment a){
  return !isLeft(a) && !isRight(a);
}

bool isVerticalCenter(Cell::Alignment a){
  return !isBottom(a) && !isTop(a);
}

void get1DPadding(size_t&a,size_t&b,size_t maxSize,size_t size,int alignment){
  a = 0;
  b = 0;
  size_t const padding = maxSize - size;
  if(alignment < 0)
    b = padding;
  else if(alignment > 0)
    a = padding;
  else{
    a = padding / 2;
    b = padding - a;
  }
}

int get1DHorizontalAlignment(Cell::Alignment a){
  return -1+static_cast<int>(isHorizontalCenter(a))+2*static_cast<int>(isRight (a));
}

int get1DVerticalAlignment(Cell::Alignment a){
  return -1+static_cast<int>(isVerticalCenter  (a))+2*static_cast<int>(isBottom(a));
}

Padding getPadding(Cell::Alignment a,size_t maxWidth,size_t maxHeight,size_t blockWidth,size_t blockHeight){
  auto const verticalPadding   = maxWidth  - blockWidth ;
  auto const horizontalPadding = maxHeight - blockHeight;
  Padding result;
  get1DPadding(result.left,result.right ,maxWidth ,blockWidth ,get1DHorizontalAlignment(a));
  get1DPadding(result.top ,result.bottom,maxHeight,blockHeight,get1DVerticalAlignment  (a));
  return result;
}

string Cell::getLine(size_t maxWidth,size_t maxHeight,size_t i)const{
  if(!data)
    return string(maxWidth,filler);
  auto const blockHeight = data->getHeight();
  auto const blockWidth = data->getWidth();
  auto const padding = getPadding(alignment,maxWidth,maxHeight,blockWidth,blockHeight);
  if(i < padding.top)return string(maxWidth,filler);
  if(i >= padding.top + blockHeight)return string(maxWidth,filler);
  stringstream ss;
  ss << string(padding.left,filler);
  ss << data->getLine(i-padding.top);
  ss << string(padding.right,filler);
  return ss.str();
}
    
size_t Cell::getWidth()const{
  if(!data)return 0;
  return data->getWidth();
}

size_t Cell::getHeight()const{
  if(!data)return 0;
  return data->getHeight();
}
