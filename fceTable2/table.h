#pragma once

#include<iostream>
#include<memory>

class FunctionLoaderInterface{
  public:
    typedef void(*FUNCTION_POINTER)();
    FunctionLoaderInterface(){}
    virtual ~FunctionLoaderInterface(){}
    virtual FUNCTION_POINTER load(char const*fceName)const = 0;
};

class TableInterface{
  public:
    TableInterface(){this->m_init();}
    virtual ~TableInterface(){}
    void a()const{(this->*(this->m_ptr_a))();}
    void b()const{(this->*(this->m_ptr_b))();}
  protected:
    using FCE = void(*)();
    using MFCE = void(TableInterface::*)()const;
    bool m_init(){return true;}
    void(TableInterface::*m_ptr_a)()const = nullptr;
    MFCE m_ptr_b = nullptr;
    FCE m_a = nullptr;
    FCE m_b = nullptr;
};

template<typename T>
class LoaderTableDecorator: public T{
  public:
    template<typename...ARGS>
    LoaderTableDecorator(std::shared_ptr<FunctionLoaderInterface>const&functionLoader,ARGS&&...args):T(args...){
      assert(this!=nullptr);
      this->m_functionLoader = functionLoader;
      T::m_init();
      this->m_init();
    }
    virtual ~LoaderTableDecorator(){}
  protected:
    std::shared_ptr<FunctionLoaderInterface>m_functionLoader = nullptr;
    bool m_init(){
      assert(this!=nullptr);
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

template<typename T>
class DSATableDecorator: public T{
  public:
    template<typename...ARGS>
    DSATableDecorator(ARGS&&...args):T(args...){
      T::m_init();
      this->m_init();
    }
    virtual ~DSATableDecorator(){}
  protected:
    virtual bool m_init(){
      if(!this->m_ptr_a)this->m_ptr_a = (TableInterface::MFCE)&DSATableDecorator::m_a_impl_dsa;
      if(!this->m_ptr_b)this->m_ptr_b = (TableInterface::MFCE)&DSATableDecorator::m_b_impl_dsa;
      return true;
    }
    void m_a_impl_dsa()const{
      std::cout<<"a dsa variant"<<std::endl;
    }
    void m_b_impl_dsa()const{
      std::cout<<"("<<std::endl;
      this->a();
      std::cout<<")"<<std::endl;
    }
};

template<typename T>
class TrapTableDecorator: public T{
  public:
    template<typename...ARGS>
      TrapTableDecorator(ARGS&&...args):T(args...){
        T::m_init();
        this->m_init();
      }
    virtual~TrapTableDecorator(){}
  protected:
    virtual bool m_init(){
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

class FceProvider{
  public:
    FceProvider(std::shared_ptr<TableInterface>const&t):m_table(t){}
    ~FceProvider(){}
    void a()const{this->getTable()->a();}
    void b()const{this->getTable()->b();}
    std::shared_ptr<TableInterface>const&getTable()const{return this->m_table;}
    void setTable(std::shared_ptr<TableInterface>const&t){this->m_table = t;}
  protected:
    std::shared_ptr<TableInterface>m_table;
};


