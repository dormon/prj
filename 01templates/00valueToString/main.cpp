#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<type_traits>
#include<memory>

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

template<typename KEY,typename VALUE>
inline std::string value2str(std::map<KEY,VALUE>const&v){
  std::stringstream ss;
  ss << "{";
  bool first = true;
  for(auto const&x:v){
    if(first)first = false;
    else ss << ",";
    ss << value2str(x.first) << ":" << value2str(x.second);
  }
  ss << "}";
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

int main(){
  std::vector<int>a = {1,3,2,4};
  std::vector<std::string>b = {"a","b","c"};
  std::map<std::string,std::vector<float>>c;
  c["a"] = std::vector<float>({1.f,3.f,4.f});
  c["b"] = std::vector<float>({.3f,.2f,4.1f});
  c["c"] = std::vector<float>({.11f,.33f,3.1f});
  std::cout << value2str(a) << std::endl;
  std::cout << value2str(b) << std::endl;
  std::cout << value2str(c) << std::endl;

  return 0;
}
