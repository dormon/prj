#include <iostream>

int FunctionName(int one, int two)
{
  static int calls=0;
  return (++calls+one)*two;
}

#define REPORT

#ifdef REPORT
  // class to capture the caller and print it.  
  class Reporter
  {
    public:
      Reporter(std::string Caller, std::string File, int Line)
        : caller_(Caller)
        , file_(File)
        , line_(Line)
      {}

      int operator()(int one, int two)
      {
        std::cout
          << "Reporter: FunctionName() is being called by "
          << caller_ << "() in " << file_ << ":" << line_ << std::endl;
        // can use the original name here, as it is still defined
        return FunctionName(one,two);
      }
    private:
      std::string   caller_;
      std::string   file_;
      int           line_;

  };

// remove the symbol for the function, then define a new version that instead
// creates a stack temporary instance of Reporter initialized with the caller
#  undef FunctionName
#  define FunctionName Reporter(__FUNCTION__,__FILE__,__LINE__)
#endif


void Caller1()
{
  int val = FunctionName(7,9);  // <-- works for captured return value
  std::cout << "Mystery Function got " << val << std::endl;
}

void Caller2()
{
  // Works for inline as well.
  std::cout << "Mystery Function got " << FunctionName(11,13) << std::endl;
}

int main(int argc, char** argv)
{
  Caller1();
  Caller2();
  return 0;
}
