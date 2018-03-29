#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class S
{
    public:
        static S& getInstance()
        {

            static S    instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        ~S(){std::cerr <<"S::~S()"<<std::endl;}
    private:
        S() {};                   // Constructor? (the {} brackets) are needed here.

        // C++ 11
        // =======
        // We can use the better technique of deleting the methods
        // we don't want.
    public:
        S(S const&)               = delete;
        void operator=(S const&)  = delete;

        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
};


int main(){


  std::cerr << "ahoj" << std::endl;
  S::getInstance();
  std::cerr << "ahoj" << std::endl;
  return 0;
}





