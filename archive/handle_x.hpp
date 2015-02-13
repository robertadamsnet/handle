#ifndef handle_hpp_20150209_1149
#define handle_hpp_20150209_1149

#include <functional>
#include <typeindex>
#include <map>

class Service {
public:
  template<class resource_type>
  void add(std::function<void(resource_type&)> fn) {
    using namespace std;
    specialty_map.insert(make_pair(type_index(typeid(resource_type)), &fn));
  }

  template<class resource_type>
  void service(resource_type& r) {
    using namespace std;
    typedef std::function<void(resource_type&)> fn_type;
    auto found_fn = specialty_map.find(type_index(typeid(resource_type)));
    if(found_fn != specialty_map.end()) {
      auto fn = *static_cast<fn_type*>(found_fn->second);
      fn(r);
    }
  }
private:
  std::map<std::type_index, void*> specialty_map;
};

class Handle {
public:
  template<class resource_type>
  explicit Handle(resource_type& r) : unknown_(&r) {
    visit_ = [&](Service& s) {
      resource_type& resource = *static_cast<resource_type*>(unknown_);
      s.service(resource);
    };
  }

  template<class service_type>
  void visit(service_type& s) {
    visit_(s);
  }

  Handle(const Handle& r) : unknown_(r.unknown_), visit_(r.visit_) {

  }

  Handle(Handle&& r) noexcept : unknown_(r.unknown_), visit_(std::move(r.visit_)) {
  }
private:
  void* unknown_;
  std::function<void(Service& s)> visit_;
};
#endif//handle_hpp_20150209_1149
