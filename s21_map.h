#ifndef S21_CONTAINERS_SRC_S21_MAP_H_
#define S21_CONTAINERS_SRC_S21_MAP_H_

#include "trees/s21_binary_tree.h"

namespace s21 {

template <typename Key, typename T>
class map : public BinaryTree<Key, T> {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using size_type = size_t;

  explicit map() noexcept : BinaryTree<Key, T>() {}

  map(std::initializer_list<value_type> const &items) noexcept {
    for (const auto &item : items) {
      this->insert(item);
    }
  }

  explicit map(const map &other) noexcept : BinaryTree<Key, T>() {
    this->copyUnique(other.root);
  }

  explicit map(map &&other) noexcept : BinaryTree<Key, T>() {
    std::swap(this->root, other.root);
    std::swap(this->treeSize, other.treeSize);
  }

  ~map() = default;

  map &operator=(map &&other) noexcept {
    std::swap(this->root, other.root);
    std::swap(this->treeSize, other.treeSize);
    return *this;
  }

  void swap(map &other) noexcept {
    std::swap(this->root, other.root);
    std::swap(this->treeSize, other.treeSize);
  }

  void merge(map &other) noexcept { this->copyUnique(other.root); }

 private:
};

}  // namespace s21

#endif  // S21_CONTAINERS_SRC_S21_MAP_H_