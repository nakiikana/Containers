#ifndef S21_CONTAINERS_SRC_S21_STACK_H_
#define S21_CONTAINERS_SRC_S21_STACK_H_

#include <exception>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <new>
#include <string_view>
#include <utility>

#include "s21_vector.h"

namespace s21 {

template <class T, class Container = s21::Vector<T>>
class Stack {
 public:
  using value_type = typename Container::value_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;
  using size_type = typename Container::size_type;

  Stack() : Adapter() {}
  Stack(std::initializer_list<value_type> const& items) : Adapter(items) {}
  Stack(const Stack& other) : Adapter(other.Adapter) {}
  Stack(Stack&& other) : Adapter(std::move(other.Adapter)) {}
  ~Stack() {}

  Stack& operator=(const Stack& other) {
    Adapter = other.Adapter;
    return *this;
  }
  Stack& operator=(Stack&& other) noexcept {
    Adapter = std::move(other.Adapter);
    return *this;
  }

  void push(const_reference value) { Adapter.push_back(value); }

  void pop() {
    if (!Adapter.empty()) {
      Adapter.pop_back();
    }
  }

  void swap(Stack& other) { Adapter.swap(other.Adapter); }

  template <class... Args>
  void insert_many_front(Args&&... args) {
    for (auto value : {std::forward<Args>(args)...}) {
      Adapter.insert_many_back(value);
    }
  }

  const_reference top() { return Adapter.back(); }

  bool empty() { return Adapter.empty(); }

  size_type size() { return Adapter.size(); }

 private:
  Container Adapter;
};

}  // namespace s21

#endif  //  S21_CONTAINERS_SRC_S21_STACK_H_