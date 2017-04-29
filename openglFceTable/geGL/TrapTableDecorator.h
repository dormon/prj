#pragma once

namespace ge{
  namespace gl{
    namespace opengl{
      template<typename T>
        class TrapTableDecorator: public T{
          public:
            template<typename...ARGS>
              TrapTableDecorator(ARGS&&...args):T(args...){}
            virtual~TrapTableDecorator(){}
          protected:
            virtual bool m_init(){
              if(!T::m_init())return false;
              if(!this->m_ptr_a)this->m_ptr_a = (TableInterface::MFCE)&TrapTableDecorator::m_trap_a;
              if(!this->m_ptr_b)this->m_ptr_b = (TableInterface::MFCE)&TrapTableDecorator::m_trap_b;
              return true;
            }
            void m_error(std::string fceName)const{
              std::cerr<<"ERROR: function: "<<fceName<<" == nullptr"<<std::endl;
            }
            void m_trap_a()const{
              this->m_error("a");
              std::exit(1);
            }
            void m_trap_b()const{
              this->m_error("b");
              std::exit(1);
            }
        };


    }
  }
}

