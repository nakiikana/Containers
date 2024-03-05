#ifndef S21_CONTAINERS_SRC_S21_LIST_H_
#define S21_CONTAINERS_SRC_S21_LIST_H_

#include <initializer_list>
#include <iostream>
#include <limits>
#include <utility>

namespace s21 {

template <typename T>
class List {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  struct Node {
    Node* next;
    Node* prev;
    value_type value;
    Node(const_reference other_value)
        : next(nullptr), prev(nullptr), value(other_value) {}
  };

  class ListConstIterator {
   public:
    ListConstIterator() : ptr(nullptr) {}
    ListConstIterator(const ListConstIterator& other) : ptr(other.ptr) {}
    explicit ListConstIterator(const Node* other) : ptr(other) {}
    const_reference operator*() const noexcept;
    ListConstIterator& operator++() noexcept;
    ListConstIterator& operator--() noexcept;
    bool operator==(const ListConstIterator& other) const noexcept;
    bool operator!=(const ListConstIterator& other) const noexcept;

   private:
    Node* ptr;
    friend class List;
  };

  class ListIterator : public ListConstIterator {
   public:
    ListIterator();
    ListIterator(const ListIterator& other) : ListConstIterator(other) {}
    explicit ListIterator(Node* other);
    reference operator*() noexcept;
    ListIterator& operator=(const ListIterator& other);
  };

  using iterator = ListIterator;
  using const_iterator = ListConstIterator;

  List();
  List(size_type n);
  List(std::initializer_list<value_type> const& items);
  List(const List& l);
  List(List&& l) noexcept;
  ~List();

  List& operator=(List&& l) noexcept;
  List& operator=(const List& l);

  const_reference front() const noexcept;
  const_reference back() const noexcept;

  iterator begin() const noexcept;
  iterator end() const noexcept;

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;

  void clear() noexcept;
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos) noexcept;
  void push_back(const_reference value);
  void pop_back() noexcept;
  void push_front(const_reference value);
  void pop_front() noexcept;
  void swap(List& other) noexcept;
  void merge(List& other);
  void splice(const_iterator pos, List& other) noexcept;
  void reverse() noexcept;
  void unique();
  void sort();

  template <typename... Args>
  void insert_many_back(Args&&... args);
  template <typename... Args>
  void insert_many_front(Args&&... args);
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args);

  // вспомогательные методы
  bool operator==(const List& other) const noexcept;
  bool operator!=(const List& other) const noexcept;

 private:
  Node* head_;
  Node* tail_;
  Node* end_;
  size_type size_;

  void set_end();
};

template <typename value_type>
template <typename... Args>
void List<value_type>::insert_many_back(Args&&... args) {
  insert_many(end(), args...);
}

template <typename value_type>
template <typename... Args>
void List<value_type>::insert_many_front(Args&&... args) {
  insert_many(begin(), args...);
}

template <typename value_type>
template <typename... Args>
typename List<value_type>::iterator List<value_type>::insert_many(
    const_iterator pos, Args&&... args) {
  iterator it(pos.ptr);
  for (auto temp : {std::forward<Args>(args)...}) {
    it = insert(it, temp);
    ++it;
  }
  --it;
  return it;
}

template <typename value_type>
List<value_type>::iterator::ListIterator() {
  this->ptr = nullptr;
}

template <typename value_type>
List<value_type>::iterator::ListIterator(Node* other) {
  this->ptr = other;
}

template <typename value_type>
List<value_type>& List<value_type>::operator=(const List& l) {
  if (*this != l) {
    clear();
    for (iterator it = l.begin(); it != l.end(); ++it) {
      push_back(*it);
    }
  }
  return *this;
}

template <typename value_type>
List<value_type>& List<value_type>::operator=(List&& l) noexcept {
  if (*this != l) {
    clear();
    swap(l);
  }
  return *this;
}

template <typename value_type>
typename List<value_type>::reference
List<value_type>::ListIterator::operator*() noexcept {
  return this->ptr->value;
}

template <typename value_type>
typename List<value_type>::ListIterator&
List<value_type>::ListIterator::operator=(const ListIterator& other) {
  this->ptr = other.ptr;
  return *this;
}

template <typename value_type>
typename List<value_type>::const_reference
List<value_type>::ListConstIterator::operator*() const noexcept {
  return this->ptr->value;
}

template <typename value_type>
typename List<value_type>::ListConstIterator&
List<value_type>::const_iterator::operator++() noexcept {
  this->ptr = this->ptr->next;
  return *this;
}

template <typename value_type>
typename List<value_type>::ListConstIterator&
List<value_type>::const_iterator::operator--() noexcept {
  this->ptr = this->ptr->prev;
  return *this;
}

template <typename value_type>
bool List<value_type>::const_iterator::operator==(
    const ListConstIterator& other) const noexcept {
  return this->ptr == other.ptr;
}

template <typename value_type>
bool List<value_type>::const_iterator::operator!=(
    const ListConstIterator& other) const noexcept {
  return this->ptr != other.ptr;
}

template <typename value_type>
List<value_type>::List()
    : head_(nullptr), tail_(nullptr), end_(nullptr), size_(0) {
  end_ = new Node(0);
  set_end();
}

template <typename value_type>
void List<value_type>::set_end() {
  end_->next = head_;
  end_->prev = tail_;
  end_->value = size_;
  if (head_) head_->prev = end_;
  if (tail_) tail_->next = end_;
}

template <typename value_type>
List<value_type>::List(size_type n)
    : head_(nullptr), tail_(nullptr), end_(nullptr), size_(0) {
  if (n > max_size()) throw std::invalid_argument("incorrect size");
  end_ = new Node(size_);
  for (size_type i = 0; i < n; ++i) {
    push_back(value_type());
  }
  set_end();
}

template <typename value_type>
List<value_type>::List(std::initializer_list<value_type> const& items)
    : head_(nullptr), tail_(nullptr), end_(nullptr), size_(0) {
  end_ = new Node(size_);
  for (auto it = items.begin(); it != items.end(); ++it) {
    push_back(*it);
  }
  set_end();
}

template <typename value_type>
List<value_type>::List(const List& l)
    : head_(nullptr), tail_(nullptr), end_(nullptr), size_(0) {
  end_ = new Node(size_);
  for (iterator it = l.begin(); it != l.end(); ++it) {
    push_back(*it);
  }
  set_end();
}

template <typename value_type>
List<value_type>::List(List&& l) noexcept
    : head_(nullptr), tail_(nullptr), end_(nullptr), size_(0) {
  swap(l);
}

template <typename value_type>
List<value_type>::~List() {
  clear();
  if (end_) delete end_;
}

template <typename value_type>
typename List<value_type>::const_reference List<value_type>::front()
    const noexcept {
  return head_ ? head_->value : end_->value;
}

template <typename value_type>
typename List<value_type>::const_reference List<value_type>::back()
    const noexcept {
  return tail_ ? tail_->value : end_->value;
}

template <typename value_type>
typename List<value_type>::iterator List<value_type>::begin() const noexcept {
  return head_ ? iterator(head_) : iterator(end_);
}

template <typename value_type>
typename List<value_type>::iterator List<value_type>::end() const noexcept {
  return iterator(end_);
}

template <typename value_type>
bool List<value_type>::empty() const noexcept {
  return size_ == 0;
}

template <typename value_type>
typename List<value_type>::size_type List<value_type>::size() const noexcept {
  return size_;
}

template <typename value_type>
typename List<value_type>::size_type List<value_type>::max_size()
    const noexcept {
  return (std::numeric_limits<size_type>::max() / sizeof(Node) / 2);
}

template <typename value_type>
void List<value_type>::clear() noexcept {
  while (!empty()) {
    pop_back();
  }
}

template <typename value_type>
typename List<value_type>::iterator List<value_type>::insert(
    iterator pos, const_reference value) {
  iterator it = end();
  if (pos == begin()) {
    push_front(value);
    it = begin();
  } else if (pos == end()) {
    push_back(value);
    --it;
  } else {
    Node* temp = new Node(value);
    temp->next = pos.ptr;
    temp->prev = pos.ptr->prev;
    pos.ptr->prev = temp;
    temp->prev->next = temp;
    ++size_;
    set_end();
    it = iterator(temp);
  }
  return it;
}

template <typename value_type>
void List<value_type>::erase(iterator pos) noexcept {
  if (!empty() && pos != end()) {
    if (pos == begin()) {
      pop_front();
    } else if (pos.ptr == end_->prev) {
      pop_back();
    } else {
      pos.ptr->prev->next = pos.ptr->next;
      pos.ptr->next->prev = pos.ptr->prev;
      delete pos.ptr;
      --size_;
      set_end();
    }
  }
}

template <typename value_type>
void List<value_type>::push_back(const_reference value) {
  Node* temp = new Node(value);
  if (empty()) {
    head_ = temp;
    tail_ = temp;
  } else {
    tail_->next = temp;
    temp->prev = tail_;
    tail_ = temp;
  }
  ++size_;
  set_end();
}

template <typename value_type>
void List<value_type>::push_front(const_reference value) {
  Node* temp = new Node(value);
  if (empty()) {
    head_ = temp;
    tail_ = temp;
  } else {
    temp->next = head_;
    head_->prev = temp;
    head_ = temp;
  }
  ++size_;
  set_end();
}

template <typename value_type>
void List<value_type>::pop_back() noexcept {
  if (!empty()) {
    Node* temp = tail_;
    if (size() > 1) {
      tail_ = tail_->prev;
    } else {
      head_ = nullptr;
      tail_ = nullptr;
    }
    delete temp;
    --size_;
    set_end();
  }
}

template <typename value_type>
void List<value_type>::pop_front() noexcept {
  if (!empty()) {
    Node* temp = head_;
    if (size() > 1) {
      head_ = head_->next;
    } else {
      head_ = nullptr;
      tail_ = nullptr;
    }
    delete temp;
    --size_;
    set_end();
  }
}

template <typename value_type>
void List<value_type>::swap(List& other) noexcept {
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
  std::swap(end_, other.end_);
}

template <typename value_type>
void List<value_type>::merge(List& other) {
  if (this != &other) {
    iterator it = begin();
    iterator it_other = other.begin();
    while (!other.empty()) {
      if (*it_other < *it || it == end()) {
        insert(it, *it_other);
        iterator temp = it_other;
        ++it_other;
        other.erase(temp);
      } else {
        ++it;
      }
    }
  }
}

template <typename value_type>
bool List<value_type>::operator==(const List& other) const noexcept {
  bool for_return = true;
  if (size_ != other.size_)
    for_return = false;
  else {
    for (iterator it(begin()), it_other(other.begin()); it != end();
         ++it, ++it_other) {
      if (*it != *it_other) {
        for_return = false;
        break;
      }
    }
  }
  return for_return;
}

template <typename value_type>
bool List<value_type>::operator!=(const List& other) const noexcept {
  return !(*this == other);
}

template <typename value_type>
void List<value_type>::reverse() noexcept {
  if (size() > 1) {
    iterator it_beg = begin();
    iterator it_end = end();
    --it_end;
    for (size_type i = 0; i < size() / 2; i++) {
      std::swap(it_beg.ptr->value, it_end.ptr->value);
      ++it_beg;
      --it_end;
    }
  }
}

template <typename value_type>
void List<value_type>::splice(const_iterator pos, List& other) noexcept {
  if (!other.empty()) {
    if (head_) {
      other.head_->prev = pos.ptr->prev;
      pos.ptr->prev->next = other.head_;
      if (pos == begin()) head_ = other.head_;
      if (pos == end()) tail_ = other.tail_;
    } else {
      other.head_->prev = pos.ptr;
      pos.ptr->next = other.head_;
      head_ = other.head_;
      tail_ = other.tail_;
    }
    other.tail_->next = pos.ptr;
    pos.ptr->prev = other.tail_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    size_ += other.size_;
    other.size_ = 0;
    set_end();
    other.set_end();
  }
}

template <typename value_type>
void List<value_type>::unique() {
  if (size() > 1) {
    iterator it = begin();
    ++it;
    while (it != end()) {
      value_type temp = it.ptr->prev->value;
      iterator for_erase = it;
      ++it;
      if (temp == *for_erase) {
        erase(for_erase);
      }
    }
  }
}

template <typename value_type>
void List<value_type>::sort() {
  if (size() > 1) {
    for (iterator it = begin(); it != end(); ++it) {
      iterator it_begin = begin();
      ++it_begin;
      for (iterator it2 = it_begin; it2 != end(); ++it2) {
        if (*it2 < it2.ptr->prev->value) std::swap(*it2, it2.ptr->prev->value);
      }
    }
  }
}

}  // namespace s21

#endif  //  S21_CONTAINERS_SRC_S21_LIST_H_
