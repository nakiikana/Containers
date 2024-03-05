#ifndef S21_CONTAINERS_SRC_S21_QUEUE_H_
#define S21_CONTAINERS_SRC_S21_QUEUE_H_

#include "s21_list.h"

namespace s21 {

template <typename T, typename Container = List<T>>
class Queue : protected Container {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  using Container::back;
  using Container::Container;
  using Container::empty;
  using Container::front;
  using Container::size;

  using Container::insert_many_back;

  void push(const_reference value) { Container::push_back(value); }
  void pop() { Container::pop_front(); }
  void swap(Queue& other) { Container::swap(other); }
};

}  // namespace s21

#endif  //  S21_CONTAINERS_SRC_S21_QUEUE_
