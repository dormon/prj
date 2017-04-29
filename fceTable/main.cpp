#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

#define DEF_FCE(name)\
void name(){std::cout<<#name<<std::endl;}

using namespace std;

DEF_FCE(ahoj);
DEF_FCE(svete);

class Table{
  public:
    using FCE = void(*)();
    FCE a;
    FCE b;
    Table(){
      a = ahoj;
      b = nullptr;
      //b = svete;
    }
    ~Table(){
      this->a = nullptr;
      this->b = nullptr;
    }
};

class FceProvider{
  protected:
    shared_ptr<Table>m_table;
    shared_ptr<Table>const&m_getTable()const{return this->m_table;}
  public:
    FceProvider(shared_ptr<Table>const&t):m_table(t){
      this->m_ptra = &FceProvider::a_impl;
      this->m_ptrb = &FceProvider::b_impl;
    }
    void a()const{(this->*(this->m_ptra))();}
    void b()const{(this->*(this->m_ptrb))();}
  protected:
    using MFCE = void(FceProvider::*)()const;
    MFCE m_ptra;
    MFCE m_ptrb;
    void a_impl()const{this->m_getTable()->a();}
    void b_impl()const{this->m_getTable()->b();}
};

class DSAFceProvider: public FceProvider{
  public:
    DSAFceProvider(shared_ptr<Table>const&t):FceProvider(t){
      if(!t->b)this->m_ptrb = (MFCE)(&DSAFceProvider::b_dsa_impl);
    }
  private:
    void b_dsa_impl()const{
      std::cout<<"("<<std::endl;
      this->m_getTable()->a();
      std::cout<<")"<<std::endl;
    }
};

int main(){
  auto table = make_shared<Table>();
  auto provider = make_shared<DSAFceProvider>(table);
  provider->a();
  provider->b();
  //((*provider).*(provider->bb))();
  return 0;
}


