#ifndef S21_CONTAINERS_SRC_S21_ARRAY_H_
#define S21_CONTAINERS_SRC_S21_ARRAY_H_
#include <exception>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <new>
#include <string_view>
#include <utility>

namespace s21 {

template <typename T, std::size_t N>

class Array {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = std::size_t;

  constexpr Array() noexcept : arr_{} {}

  constexpr Array(std::initializer_list<value_type> const &items) {
    if (items.size() > N) std::out_of_range("Out of range");
    auto count = items.begin();
    for (size_type i = 0; i < N; ++i) {
      arr_[i] = *count;
      ++count;
    }
  }

  constexpr Array(const Array &other) {
    if (other.size() > N) std::out_of_range("Out of range");
    for (size_type i = 0; i < N; ++i) {
      arr_[i] = other.arr_[i];
    }
  }

  Array(Array &&other) {
    for (size_type i = 0; i < other.size(); ++i)
      arr_[i] = std::move(other.arr_[i]);
  }

  ~Array() = default;

  Array &operator=(Array &&other) {
    for (size_type i = 0; i < N; ++i) {
      arr_[i] = std::move(other.arr_[i]);
    }
    return *this;
  }

  constexpr Array &operator=(Array &other) noexcept {
    if (other.size() > N) std::out_of_range("Out of range");
    for (size_type i = 0; i < N; ++i) {
      arr_[i] = other.arr_[i];
    }
    return *this;
  }

  reference at(size_type pos) {
    if (pos >= N) {
      throw std::out_of_range("Index out of range");
    }
    return arr_[pos];
  }

  constexpr reference operator[](size_type pos) { return arr_[pos]; }

  const_reference front() const { return arr_[0]; }

  const_reference back() const { return arr_[N - 1]; }

  iterator data() noexcept { return arr_; }

  iterator begin() noexcept { return arr_; }

  iterator end() noexcept { return arr_ + N; }

  bool empty() const noexcept {
    if (N == 0) {
      return true;
    } else {
      return false;
    }
  }
  size_type size() const noexcept { return N; }

  size_type max_size() const noexcept { return N; }

  void fill(const_reference value) { std::fill_n(arr_, N, value); }

  void swap(Array &other) noexcept(std::is_nothrow_swappable_v<T>) {
    std::swap_ranges(begin(), end(), other.begin());
  }

 private:
  value_type arr_[N];
};
}  // namespace s21

#endif  //  S21_CONTAINERS_SRC_S21_ARRAY_H_