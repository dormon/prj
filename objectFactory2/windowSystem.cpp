#include"windowSystem.h"

using namespace ge::util::ui;

Window::Window(glm::ivec4 coords,std::shared_ptr<Window>parent,std::string name){
  this->setCoords(coords);
  this->_parent = parent;
  this->setName(name);
}

Window::Window(glm::ivec2 leftBottom,glm::ivec2 rightTop,std::shared_ptr<Window>parent,std::string name){
  this->setLeftBottom(leftBottom);
  this->setRightTop(rightTop);
  this->_parent = parent;
  this->setName(name);
}

Window::Window(int x0,int y0,int x1,int y1,std::shared_ptr<Window>parent,std::string name){
  this->setLeftBottom(x0,y0);
  this->setRightTop(x1,y1);
  this->_parent = parent;
  this->setName(name);
}

void Window::draw(glm::ivec4 coords)const{

}
