#pragma once

#if defined(ENABLE_PRINT_CALL_STACK)

#define HIDDEN_VARIABLE_NAME superduperhidden
#define OUTPUT cout

#define REMOVE_RETURN(name) std::string(name).substr(std::string(name).rfind(" ")+1)
#define REMOVE_ARGS(name) std::string(name).substr(0,std::string(name).find("("))

#if _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define GE_CORE_FCENAME REMOVE_RETURN(REMOVE_ARGS(__PRETTY_FUNCTION__))

#include<type_traits>
#include<iostream>
#include<vector>
#include<memory>
#include<sstream>
#include<typeinfo>

namespace ge{
  namespace core{

    template<typename T,typename std::enable_if<std::is_fundamental<T>::value,unsigned>::type = 0>
      std::string value2str(T const&v);

    template<typename T,typename std::enable_if<std::is_pointer<T>::value,unsigned>::type = 0>
      std::string value2str(T const&v);

    template<typename T>
      std::string value2str(std::vector<T>const&v);

    template<typename T>
      std::string value2str(std::shared_ptr<T>const&v);

    std::string value2str(std::string const&v);

    template<typename T,typename std::enable_if<std::is_enum<T>::value,unsigned>::type = 0>
      inline std::string value2str(T const&v);

    template<typename T,typename std::enable_if<std::is_class<T>::value,unsigned>::type = 0>
      inline std::string value2str(T const&v);

    template<typename T,typename std::enable_if<std::is_member_function_pointer<T>::value,unsigned>::type = 0>
      inline std::string value2str(T const&v);


    template<typename T,typename std::enable_if<std::is_fundamental<T>::value,unsigned>::type>
      inline std::string value2str(T const&v){
        std::stringstream ss;
        ss<<v;
        return ss.str();
      }

    template<typename T,typename std::enable_if<std::is_pointer<T>::value,unsigned>::type>
      inline std::string value2str(T const&v){
        std::stringstream ss;
        ss<<v;
        return ss.str();
      }

    template<typename T>
      inline std::string value2str(std::vector<T>const&v){
        std::stringstream ss;
        ss << "[";
        bool first = true;
        for(auto const&x:v){
          if(first)first=false;
          else ss<<",";
          ss << value2str(x);
        }
        ss << "]";
        return ss.str();
      }

    inline std::string value2str(std::string const&v){
      return "\""+v+"\"";
    }

    template<typename T>
      inline std::string value2str(std::shared_ptr<T>const&v){
        std::stringstream ss;
        ss<<v;
        return ss.str();
      }

    template<typename T,typename std::enable_if<std::is_enum<T>::value,unsigned>::type>
      inline std::string value2str(T const&v){
        std::stringstream ss;
        ss<<(size_t)v;
        return ss.str();
      }

    template<typename T,typename std::enable_if<std::is_class<T>::value,unsigned>::type>
      inline std::string value2str(T const&){
        std::stringstream ss;
        ss<<typeid(T).name();
        return ss.str();
      }

    template<typename T,typename std::enable_if<std::is_member_function_pointer<T>::value,unsigned>::type>
      inline std::string value2str(T const&v){
        std::stringstream ss;
        ss<<v;
        return ss.str();
      }

  }
}

namespace ge{
  namespace core{
    template<typename...ARGS>
      std::string argsToStr(ARGS...);

    template<typename F,typename...ARGS>
      inline std::string argsToStr_help(F const&a,ARGS...args){
        std::stringstream ss;
        ss<<value2str(a);
        if(sizeof...(args)>0)
          ss<<","<<argsToStr(args...);
        return ss.str();
      }

    template<typename...ARGS>
      inline std::string argsToStr(ARGS...args){
        return argsToStr_help(args...);
      }

    template<>inline std::string argsToStr(){
      return "";
    }

    class CallStackPrinter{
      public:
        int&indentCounter(){
          static int counter = 0;
          return counter;
        }
        std::string&indent(){
          static std::string s = "";
          return s;
        }
        std::string argsData="";
        template<typename...ARGS>
          CallStackPrinter(ARGS...args){
            this->argsData = argsToStr(args...);
          }
        void print(std::string const&fceName){
          std::OUTPUT<<indent()<<fceName<<"("<<this->argsData<<"){"<<std::endl;
          indentCounter()+=2;
          indent()="";
          for(int i=0;i<indentCounter();++i)
            indent()+=" ";
        }
        ~CallStackPrinter(){
          indentCounter()-=2;
          if(indentCounter()<0)
            indentCounter()=0;
          indent()="";
          for(int i=0;i<indentCounter();++i)
            indent()+=" ";
          std::OUTPUT<<indent()<<"}"<<std::endl;
        }
    };


  }
}

#define PRINT_CALL_STACK(...)\
  ge::core::CallStackPrinter HIDDEN_VARIABLE_NAME{__VA_ARGS__};\
  HIDDEN_VARIABLE_NAME.print(GE_CORE_FCENAME)

#else
#define PRINT_CALL_STACK(...)
#endif
