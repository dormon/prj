#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>

namespace vars{

using Destructor = std::function<void(void*)>;
using OnDestroy  = std::function<void(void*)>;
using OnChange   = std::function<void(void*, void*)>;

class VarsImpl;
class Vars;

}

class vars::Vars {
 public:
  Vars();
  ~Vars();
  void  add(std::string const&    n,
            void*                 d,
            Destructor const&     dst,
            std::type_info const& t);
  void* get(std::string const& n)const;
  void  erase(std::string const& n);
  bool  has(std::string const& n)const;
  std::type_info const& getType(std::string const& n) const;
  template <typename CLASS, typename... ARGS>
  void add(std::string const& n, ARGS const&... args);
  template <typename CLASS>
  CLASS* get(std::string const& n)const;

 private:
  void checkTypes(std::string const&n,std::type_info const&t)const;
  friend class VarsImpl;
  std::unique_ptr<VarsImpl> impl;
};


template <typename CLASS, typename... ARGS>
void vars::Vars::add(std::string const& n, ARGS const&... args) {
  void* data = new CLASS{args...};
  add(n, data, [](void* ptr) { delete reinterpret_cast<CLASS*>(ptr); },
      typeid(CLASS));
}

template <typename CLASS>
CLASS* vars::Vars::get(std::string const& n)const {
  checkTypes(n,typeid(CLASS));
  return reinterpret_cast<CLASS*>(get(n));
}

