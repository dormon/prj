#pragma once

namespace ge{
  namespace gl{
    namespace opengl{
      class FunctionProvider{
        public:
          FunctionProvider(std::shared_ptr<TableInterface>const&t):m_table(t){}
          ~FunctionProvider(){}
          void a()const{this->getTable()->a();}
          void b()const{this->getTable()->b();}
          std::shared_ptr<TableInterface>const&getTable()const{return this->m_table;}
          void setTable(std::shared_ptr<TableInterface>const&t){this->m_table = t;}
        protected:
          std::shared_ptr<TableInterface>m_table;
      };


    }
  }
}

