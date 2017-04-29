#pragma once

namespace ge{
  namespace gl{
    namespace opengl{
      class TableInterface{
        public:
          TableInterface(){}
          virtual ~TableInterface(){}
          bool construct(){return this->m_init();}
          void a()const{(this->*(this->m_ptr_a))();}
          void b()const{(this->*(this->m_ptr_b))();}
        protected:
          using FCE = void(*)();
          using MFCE = void(TableInterface::*)()const;
          virtual bool m_init(){return true;}
          MFCE m_ptr_a = nullptr;
          MFCE m_ptr_b = nullptr;
          FCE m_a = nullptr;
          FCE m_b = nullptr;
      };
    }
  }
}
