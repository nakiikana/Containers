#ifndef S21_CONTAINERS_SRC_S21_VECTOR_H_
#define S21_CONTAINERS_SRC_S21_VECTOR_H_
#include <exception>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <new>
#include <string_view>
#include <utility>

namespace s21 {

template <class T>

class Vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;

  //* Vector Member functions

  Vector() : size_(0U), capacity_(0U), arr_(nullptr){};

  explicit Vector(size_type n)
      : size_(n),
        capacity_(n),
        arr_(n ? reinterpret_cast<value_type *>(operator new(
                     capacity_ * sizeof(value_type)))
               : nullptr) {
    for (size_type i = 0; i < n; ++i) {
      new (arr_ + i) value_type();
    }
  };

  Vector(std::initializer_list<value_type> const &items) {
    if (items.size() > max_size()) throw std::bad_alloc();
    size_ = items.size();
    capacity_ = items.size();
    arr_ = reinterpret_cast<value_type *>(operator new(capacity_ *
                                                       sizeof(value_type)));
    std::uninitialized_copy(items.begin(), items.end(), arr_);
  }

  Vector(const Vector &v) {
    size_ = v.size_;
    capacity_ = v.capacity_;
    arr_ = reinterpret_cast<value_type *>(operator new(capacity_ *
                                                       sizeof(value_type)));
    try {
      std::uninitialized_copy(v.arr_, v.arr_ + v.size_, arr_);
    } catch (...) {
      // delete[] reinterpret_cast<int8_t *>(arr_);
      operator delete(arr_);
      throw std::bad_alloc();
    }
  }

  Vector(Vector &&v) : size_(v.size_), capacity_(v.capacity_), arr_(v.arr_) {
    v.arr_ = nullptr;
    v.size_ = 0;
    v.capacity_ = 0;
  }

  ~Vector() {
    for (size_type i = 0; i < size_; ++i) {
      (arr_ + i)->~T();
    }
    // delete[] reinterpret_cast<int8_t *>(arr_);
    operator delete(arr_);
  }

  Vector &operator=(Vector &&v) noexcept {
    if (this != &v) {
      std::swap(arr_, v.arr_);
      std::swap(size_, v.size_);
      std::swap(capacity_, v.capacity_);
    }
    return *this;
  }

  Vector &operator=(const Vector &v) {
    Vector cpy(v);
    *this = std::move(cpy);
    return *this;
  }

  const_reference operator[](size_type pos) const { return arr_[pos]; }

  // *Vector Element access

  reference at(size_type pos) {
    if (pos >= size_) throw std::out_of_range("Out of range");
    return arr_[pos];
  };

  reference operator[](size_type pos) { return arr_[pos]; }

  const_reference front() { return arr_[0]; }

  const_reference back() { return arr_[size_ - 1]; }

  iterator data() { return arr_; }

  //* Vector Iterators

  iterator begin() { return iterator(arr_); }

  iterator end() { return iterator(arr_ + size_); }  ////////

  //* Vector Capacity

  bool empty() {
    if (size_ > 0) {
      return false;
    } else {
      return true;
    }
  }

  size_type size() { return size_; }

  size_type max_size() { return SIZE_MAX / sizeof(value_type); }

  void reserve(size_type size) {
    if (size > capacity_) {
      value_type *newarr = reinterpret_cast<value_type *>(operator new(
          size * sizeof(value_type)));
      try {
        std::uninitialized_copy(arr_, arr_ + size_, newarr);
      } catch (...) {
        operator delete(newarr);
        throw;
      }
      for (size_type i = 0; i < size_; ++i) {
        (arr_ + i)->~T();
      }
      operator delete(arr_);
      // delete[] reinterpret_cast<int8_t *>(arr_);
      arr_ = newarr;
      capacity_ = size;
    }
  }

  size_type capacity() { return capacity_; }

  void shrink_to_fit() {
    if (size_ < capacity_) {
      value_type *newarr = reinterpret_cast<value_type *>(operator new(
          size_ * sizeof(value_type)));
      try {
        std::uninitialized_copy(arr_, arr_ + size_, newarr);
      } catch (...) {
        operator delete(newarr);
        throw;
      }
      for (size_type i = 0; i < size_; ++i) {
        (arr_ + i)->~T();
      }
      // delete[] reinterpret_cast<int8_t *>(arr_);
      operator delete(arr_);
      arr_ = newarr;
      capacity_ = size_;
    }
  }

  //* Vector Modifiers

  void clear() noexcept {
    for (size_type i = 0; i < size_; ++i) {
      (arr_ + i)->~T();
    }
    size_ = 0;
  }

  iterator insert(iterator pos, const_reference value) {
    if (pos > end()) throw std::out_of_range("Out of range");
    size_type index = pos - begin();
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    for (size_type i = size_; arr_ + i > begin() + index; --i) {
      arr_[i] = arr_[i - 1];
    }
    arr_[index] = value;
    ++size_;
    return begin() + index;
  }

  void erase(iterator pos) {
    if (pos > end()) throw std::out_of_range("Out of range");
    size_type index = pos - this->begin();
    (arr_ + index)->~T();
    for (size_type i = index; i < size_ - 1; ++i) {
      arr_[i] = arr_[i + 1];
    }
    (arr_ + size_ - 1)->~T();
    --size_;
  }

  void push_back(value_type v) {
    if (capacity_ == size_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (arr_ + size_) value_type(v);
    ++size_;
  }

  void pop_back() {
    --size_;
    (arr_ + size_)->~T();
  }

  void swap(Vector &other) noexcept {
    std::swap(arr_, other.arr_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    iterator iter = const_cast<iterator>(pos);
    for (auto value : {std::forward<Args>(args)...}) {
      iter = insert(iter, value);
      iter++;
    }
    return --iter;
  }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    for (auto value : {std::forward<Args>(args)...}) {
      push_back(value);
    }
  }

 private:
  size_t size_;
  size_t capacity_;
  T *arr_;
};

}  // namespace s21

#endif  //  S21_CONTAINERS_SRC_S21_VECTOR_H_