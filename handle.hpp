/* handle.hpp
 * 
 * Copyright (c) 2015 by Robert T. Adams 
 * All rights reserved.
 *
 * The MIT License (MIT)
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#ifndef handle_hpp_20150209_1149
#define handle_hpp_20150209_1149

#include <functional>
#include <typeindex>
#include <map>

template<int N>
class Switchboard {
public:
  class Service final {
  public:
    Service() {
    }
    template<class resource_type>
    void add(std::function<void(resource_type&)> fn) {
      handler<resource_type>() = fn;
    }

    template<class resource_type>
    void operator()(resource_type& resource) {
      auto handle = handler<resource_type>();
      handle(resource);
    }
  private:
    template<class resource_type>
    static std::function<void(resource_type&)>& handler() {
      static std::function<void(resource_type&)> handler_ = 
        default_handler<resource_type>;
      return handler_;
    }

    template<class resource_type>
    static void default_handler(resource_type&) { }
  };

  class Handle {
  public:

    template<class resource_type>
    explicit Handle(resource_type& r) : unknown_(&r) {
      visit_ = [&](Service& service) {
        resource_type& resource = *static_cast<resource_type*>(unknown_);
        service(resource);
      };
    }

    void visit(Service& service) {
      visit_(service);
    }

    Handle(const Handle& r) : unknown_(r.unknown_), visit_(r.visit_) {

    }

    Handle(Handle&& r) noexcept : unknown_(r.unknown_), visit_(std::move(r.visit_)) {
    }
  private:
    void* unknown_;
    std::function<void(Service&)> visit_;
  };
};
#endif//handle_hpp_20150209_1149
