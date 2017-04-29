#pragma once

#include<geGL/FunctionLoaderInterface.h>

namespace ge{
  namespace gl{
    namespace opengl{
      template<typename T>
        class LoaderTableDecorator: public T{
          public:
            template<typename...ARGS>
              LoaderTableDecorator(std::shared_ptr<FunctionLoaderInterface>const&functionLoader,ARGS&&...args):T(args...){
                assert(this!=nullptr);
                this->m_functionLoader = functionLoader;
              }
            virtual ~LoaderTableDecorator(){}
          protected:
            std::shared_ptr<FunctionLoaderInterface>m_functionLoader = nullptr;
            virtual bool m_init(){
              assert(this!=nullptr);
              if(!T::m_init())return false;
              assert(this->m_functionLoader!=nullptr);
              this->m_a = this->m_functionLoader->load("a");
              this->m_b = this->m_functionLoader->load("b");
              if(this->m_a)this->m_ptr_a = (TableInterface::MFCE)&LoaderTableDecorator::m_a_impl;
              if(this->m_b)this->m_ptr_b = (TableInterface::MFCE)&LoaderTableDecorator::m_b_impl;
              return true;
            }
            void m_a_impl()const{this->m_a();}
            void m_b_impl()const{this->m_b();}
        };


    }
  }
}

