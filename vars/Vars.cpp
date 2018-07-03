#include <Vars.h>

using namespace vars;

class Element {
 public:
  Element(void* d, Destructor const& dst, std::type_info const& t)
      : data(d), destructor(dst), type(t) {}
  ~Element() { destructor(data); }
  void*                 data;
  Destructor            destructor;
  std::type_info const& type;
};

class vars::VarsImpl {
 public:
  std::map<std::string, Element> data;
  void                           add(std::string const&    n,
                                     void*                 d,
                                     Destructor const&     dst,
                                     std::type_info const& t) {
    if (has(n))
      throw std::runtime_error(std::string("variable: ") + n +
                               " already exists!");
    data.emplace(std::piecewise_construct, std::forward_as_tuple(n),
                 std::forward_as_tuple(d, dst, t));
  }
  void* get(std::string const& n) const {
    if (!has(n))
      throw std::runtime_error(std::string("variable: ") + n +
                               " does not exist!");
    return data.at(n).data;
  }
  void erase(std::string const& n) { data.erase(n); }
  bool has(std::string const& n) const { return data.count(n) > 0; }
  std::type_info const& getType(std::string const& n) const {
    return data.at(n).type;
  }
  void checkTypes(std::string const& n, std::type_info const& t) {
    if (getType(n) == t) return;
    throw std::runtime_error(std::string("variable: ") + n +
                             " has different type");
  }
  ~VarsImpl() {}
};

void Vars::add(std::string const&                n,
               void*                             d,
               std::function<void(void*)> const& dst,
               std::type_info const&             t) {
  impl->add(n, d, dst, t);
}

void* Vars::get(std::string const& n) const { return impl->get(n); }

void Vars::erase(std::string const& n) { impl->erase(n); }

bool Vars::has(std::string const& n) const { return impl->has(n); }

std::type_info const& Vars::getType(std::string const& n) const {
  return impl->getType(n);
}

void Vars::checkTypes(std::string const& n, std::type_info const& t) const {
  impl->checkTypes(n, t);
}

Vars::Vars() { impl = std::make_unique<VarsImpl>(); }

Vars::~Vars() {}
