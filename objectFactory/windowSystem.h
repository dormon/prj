#pragma once

#include<vector>
#include<map>
#include<set>
#include<memory>
#include<glm/glm.hpp>

namespace ge{
  namespace util{
    namespace ui{
      class Window{
        protected:
          std::shared_ptr<Window>_parent = nullptr;
          std::set<std::weak_ptr<Window>>_subWindows;
          std::string _name = "";
          glm::ivec2 _leftBottom;
          glm::ivec2 _rightTop;
          inline void _correctCoords();
        public:
          Window(glm::ivec4 coords,std::shared_ptr<Window>parent,std::string name);
          Window(glm::ivec2 leftBottom,glm::ivec2 rightTop,std::shared_ptr<Window>parent,std::string name);
          Window(int x0,int y0,int x1,int y1,std::shared_ptr<Window>parent,std::string name);
          inline void setName(std::string name);
          inline void setCoords(glm::ivec4 coords);
          inline void setLeftBottom(glm::ivec2 leftBottom);
          inline void setLeftBottom(int x0,int y0);
          inline void setRightTop(glm::ivec2 rightTop);
          inline void setRightTop(int x1,int y1);
          inline void setLeft(int x0);
          inline void setRight(int x1);
          inline void setBottom(int y0);
          inline void setTop(int y1);
          inline int  getLeft()const;
          inline int  getRight()const;
          inline int  getBottom()const;
          inline int  getTop()const;
          inline glm::ivec2 getLeftBottom()const;
          inline glm::ivec2 getRightTop()const;
          inline glm::ivec4 getCoords()const;
          inline int getWidth()const;
          inline int getHeight()const;
          inline glm::ivec2 getSize()const;
          void draw(glm::ivec4 coords)const;
      };

      inline void Window::setName(std::string name){
        this->_name = name;
      }

      inline void Window::setCoords(glm::ivec4 coords){
        this->_leftBottom.x = coords.x;
        this->_leftBottom.y = coords.y;
        this->_rightTop.x   = coords.z;
        this->_rightTop.y   = coords.w;
        this->_correctCoords();
      }

      inline void Window::setLeftBottom(glm::ivec2 leftBottom){
        this->_leftBottom = leftBottom;
        this->_correctCoords();
      }

      inline void Window::setLeftBottom(int x0,int y0){
        this->_leftBottom.x = x0;
        this->_leftBottom.y = y0;
        this->_correctCoords();
      }

      inline void Window::setRightTop(glm::ivec2 rightTop){
        this->_rightTop = rightTop;
        this->_correctCoords();
      }
      
      inline void Window::setRightTop(int x1,int y1){
        this->_rightTop.x = x1;
        this->_rightTop.y = y1;
        this->_correctCoords();
      }

      inline void Window::setLeft(int x0){
        this->_leftBottom.x = x0;
        this->_correctCoords();
      }

      inline void Window::setRight(int x1){
        this->_rightTop.x = x1;
        this->_correctCoords();
      }

      inline void Window::setBottom(int y0){
        this->_leftBottom.y = y0;
        this->_correctCoords();
      }

      inline void Window::setTop(int y1){
        this->_rightTop.y = y1;
        this->_correctCoords();
      }

      inline int  Window::getLeft()const{
        return this->_leftBottom.x;
      }

      inline int  Window::getRight()const{
        return this->_rightTop.x;
      }
     
      inline int  Window::getBottom()const{
        return this->_leftBottom.y;
      }

      inline int  Window::getTop()const{
        return this->_rightTop.y;
      }

      inline glm::ivec2 Window::getLeftBottom()const{
        return this->_leftBottom;
      }

      inline glm::ivec2 Window::getRightTop()const{
        return this->_rightTop;
      }

      inline glm::ivec4 Window::getCoords()const{
        return glm::ivec4(this->_leftBottom,this->_rightTop);
      }

      inline int Window::getWidth()const{
        return this->_rightTop.x-this->_leftBottom.x+1;
      }
      inline int Window::getHeight()const{
        return this->_rightTop.y-this->_leftBottom.y+1;
      }

      inline glm::ivec2 Window::getSize()const{
        return this->_rightTop-this->_leftBottom+glm::ivec2(1);
      }

      inline void Window::_correctCoords(){
        if(this->_rightTop.x<this->_leftBottom.x)this->_rightTop.x=this->_leftBottom.x;
        if(this->_rightTop.y<this->_leftBottom.y)this->_rightTop.y=this->_leftBottom.y;
      }

    }
  }
}
