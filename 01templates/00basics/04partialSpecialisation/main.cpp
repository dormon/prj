#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>

template<typename TYPE,size_t SIZE>
class Vector{
  public:
    TYPE dot(Vector const&other)const{
      TYPE result = 0;
      for(size_t i = 0; i < SIZE; ++i)
        result += this->_data[i] * other._data[i];
      return result;
    }
    void set(size_t const&i,TYPE const& value){
      this->_data[i] = value;
    }
  protected:
    TYPE _data[SIZE];
};

template<size_t SIZE>
class Vector<std::string,SIZE>{
  public:
    std::string dot(Vector const&other)const{
      std::stringstream ss;
      for(size_t i = 0; i < SIZE; ++i)
        ss << "(" << this->_data[i] << "," << other._data[i] << ")";
      return ss.str();
    }
    void set(size_t const&i,std::string const& value){
      this->_data[i] = value;
    }
  protected:
    std::string _data[SIZE];
};

int main(){
  {
    Vector<float,2>a;
    Vector<float,2>b;

    a.set(0,12.2f);
    a.set(1,33.2f);
    b.set(0,10.1f);
    b.set(1,31.9f);

    auto const c = a.dot(b);
    
    std::cout << c << std::endl;
  }

  {
    Vector<std::string,2>a;
    Vector<std::string,2>b;

    a.set(0,"asd"   );
    a.set(1,"sdssss");
    b.set(0,"a"     );
    b.set(1,"b"     );

    auto const c = a.dot(b);

    std::cout << c << std::endl;
  }

  {
    Vector<std::string,3>a;
    Vector<std::string,3>b;

    a.set(0,"asd"   );
    a.set(1,"sdssss");
    a.set(2,"x"     );
    b.set(0,"a"     );
    b.set(1,"b"     );
    b.set(2,"c"     );

    auto const c = a.dot(b);

    std::cout << c << std::endl;
  }

  return 0;
}
