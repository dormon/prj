#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* You have two types of classes - manager and product
 * Manager can create product
 * product has shared pointer to manager
 *
 * When manager creates new product it has to make itself shared.
 * Solution: inherit from std::enable_shared_from_this
 */

class Manager;

class Product{
  public:
    std::shared_ptr<Manager>man;///<shared pointer to manager
    int*data;///<some data
    /**
     * @brief Constructor
     *
     * @param man shared pointer to manager
     * @param n   nof data
     */
    Product(std::shared_ptr<Manager>const&man,int n){
      this->man  = man;
      this->data = new int[n];
    }
    ~Product(){
      delete[]this->data;
    }
};

class Manager: public std::enable_shared_from_this<Manager>{//<- this is important
  public:
    float*d;//data that can be access by every product via shared pointer
    Manager(){this->d=new float[100];}
    ~Manager(){delete[]this->d;}
    /**
     * @brief This function creates new product
     *
     * @param n nof data in product
     *
     * @return new product
     */
    Product newProduct(int n){
      return Product(
          this->shared_from_this(),//<- this is important
          n);
    }
};

int main(){

  std::shared_ptr<Manager>m=std::make_shared<Manager>();
  //Manager*m=new Manager();//<- this is also important, now you cant create basic pointer
  m->newProduct(10);
  return 0;
}
