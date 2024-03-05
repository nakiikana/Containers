#ifndef S21_CONTAINERS_SRC_S21_SET_H_
#define S21_CONTAINERS_SRC_S21_SET_H_
#include <limits>

#include "trees/s21_red_black_tree.h"

template <typename T>
class set {
 public:
  typedef RedBlackTree<T, T> tree;
  typedef T key_type;
  typedef T value_type;
  typedef T Key;
  typedef T &reference;
  typedef const T &const_reference;
  typedef typename tree::iterator iterator;
  typedef typename tree::const_iterator const_iterator;
  typedef size_t size_type;

  set() = default;

  set(std::initializer_list<value_type> const &items) {
    for (auto i = items.begin(); i < items.end(); i++) {
      tree_.AddNode(*i, *i);
    }
  };

  set(const set &s) : tree_(s.tree_){};

  set(set &&s) : tree_(std::move(s.tree_)){};

  set &operator=(set &s) {
    set temp(s);
    *this = std::move(temp);
    return *this;
  };

  set &operator=(set &&s) {
    tree_ = std::move(s.tree_);
    return *this;
  };

  ~set() = default;

  iterator begin() { return tree_.begin(); }

  iterator end() { return tree_.end(); }

  bool empty() { return !tree_.GetSize(); };

  size_type size() { return tree_.GetSize(); };

  size_type max_size() { return std::numeric_limits<size_type>::max(); };

  void clear() {
    for (auto it = begin(), en = end(); it != en;) {
      tree_.DeleteNode(*it);
      it = begin();
    }
  }

  std::pair<iterator, bool> insert(const value_type &value) {
    bool inserted = false;
    if (tree_.FindNode(value) == NULL) {
      tree_.AddNode(value, value);
      inserted = true;
    }
    iterator pos(tree_.FindNode(value));
    return std::make_pair(pos, inserted);
  }

  void erase(iterator pos) {
    if (pos != NULL && tree_.GetSize() > 0 && tree_.FindNode(*pos) != NULL) {
      tree_.DeleteNode(*pos);
    }
  }

  void swap(set &other) {
    set temp(*this);
    *this = other;
    other = temp;
  }

  void merge(set &other) {
    for (auto it = other.begin(), et = other.end(); it != et; ++it) {
      tree_.AddNode(*it, *it);
    }
    other.clear();
  }

  iterator find(const Key &key) {
    if (tree_.FindNode(key)) {
      iterator pos(tree_.FindNode(key));
      return pos;
    } else {
      return ++iterator(tree_.MaxInTree());
    }
  }

  bool contains(const Key &key) {
    bool found = false;
    if (tree_.FindNode(key)) found = true;
    return found;
  }

 private:
  tree tree_;
};

#endif  // S21_CONTAINERS_SRC_S21_SET_H_
