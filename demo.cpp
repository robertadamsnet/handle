/* demo.hpp
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
#include "handle.hpp"
#include <iostream>
#include <vector>
using namespace std;

class Animal {
public:
  string name() const { return v_name(); }
private:
  virtual string v_name() const = 0;
};

class Cat : public Animal {
public:
  Cat() { }
  string meow() const {
    return  "meow";
  }
private:
  virtual string v_name() const override {
    return "cat";
  }
};

class Dog : public Animal {
public:
  Dog() { }
  string woof() const {
    return "woof";
  }
private:
  virtual string v_name() const override {
    return "dog";
  }
};

class Fox : public Animal {
public:
  Fox() { }
  string say() const {
    return "???";
  }
private:
  virtual string v_name() const override {
    return "fox";
  }
};

void dog_service(Dog& dog) {
  cout << "The dog says: " << dog.woof() << endl;
}

void cat_service(Cat& cat) {
  cout << "The cat says: " << cat.meow() << endl;
  cat.meow();
}

auto fox_service = [](Fox& fox) {
    cout << "The fox says: " << fox.say() << endl;
    fox.say();
};

class Vet {
public:
  void operator()(Fox& fox) {
    say(fox, fox.say());
  }

  void operator()(Dog& dog) {
    say(dog, dog.woof());
  }

  void operator()(Cat& cat) {
    say(cat, cat.meow());
  }

  void say(const Animal& animal, string phrase) {
    string name = animal.name();
    cout << "Fixing a " << name << "..." << endl
         << "\"" << phrase << "!\"" << endl
         << endl;
  }
} vet_service;

int main() {
  typedef Switchboard<0>::Service Service;
  typedef Switchboard<0>::Handle Handle;
  Service service;

  service.add<Dog>(dog_service);
  service.add<Cat>(cat_service);
  service.add<Fox>(fox_service);

  Dog dog;
  Cat cat;
  Fox fox;

  vector<Handle> handles = {
    Handle(dog),
    Handle(cat),
    Handle(fox)
  };

  for(auto handle : handles) {
    handle.visit(service);
  }

  cout << endl;

  service.add<Dog>(vet_service);
  service.add<Cat>(vet_service);
  service.add<Fox>(vet_service);

  for(auto animal : handles) {
    animal.visit(service);
  }

  return 0;
}

