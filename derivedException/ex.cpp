#include "ex.h"
#include<stdexcept>
#include<sstream>

namespace argumentViewer{
  namespace ex{
    class Exception;
    class MatchError;
  }
}

class argumentViewer::ex::Exception : public std::runtime_error {
 public:
  Exception(std::string const& msg) : std::runtime_error(msg) {}
  virtual ~Exception() throw() {}
  virtual char const* what() const throw() override {
    return std::string(std::string("ArgumentViewer - ") +
                       std::runtime_error::what())
        .c_str();
  }
};

class argumentViewer::ex::MatchError : public Exception {
 public:
  MatchError(std::string const& msg) : Exception(msg) {}
  virtual char const* what() const throw() override {
    return std::string(std::string("ArgumentViewer::match - ") +
                       std::runtime_error::what())
        .c_str();
  }
};

class MyException: public std::runtime_error{
  public:
    MyException(const char*msg):std::runtime_error(msg){}
    virtual ~MyException()throw(){}
    virtual char const* what() const throw() override{
      return std::runtime_error::what();
    }
};


class AException : public std::runtime_error {
 public:
  AException(char const* msg) : std::runtime_error(msg) {
    aa =  std::string("(((( - ") + std::string(msg);
  }
  virtual ~AException() throw() {}
  virtual char const* what() const throw() override {
    return aa.c_str();
  }
  std::string aa = "asdasdasd";
};

class AMatchError : public AException {
 public:
  AMatchError(char const* msg) : AException(msg) {}
  virtual char const* what() const throw() override {
    return "################xx";
    return std::string(std::string("<<<< - ") +
                       std::runtime_error::what())
        .c_str();
  }
};

void thrw(int a){
  if(a==1)
    throw std::runtime_error("runtime_error - user message");
  if(a==2)
    throw argumentViewer::ex::MatchError("MatchError - user message");
  if(a==3)
    throw argumentViewer::ex::Exception("Exception - user message");
  if(a==4)
    throw AException("AException - user message");
  if(a==5)
    throw AMatchError("AMatchError - user message");


  throw MyException("MyException - user message");
}
