#ifndef S21_CONTAINERS_SRC_TREES_S21_BINARY_TREE_H_
#define S21_CONTAINERS_SRC_TREES_S21_BINARY_TREE_H_

#include <cstddef>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>

namespace s21 {

template <typename Key, typename T>
class BinaryTree {
  template <class Iter>
  class BinaryTreeIterator;
  struct Node;

 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using size_type = size_t;
  using iterator = BinaryTreeIterator<Node>;
  using const_iterator = BinaryTreeIterator<const Node>;
  using return_type = std::pair<iterator, bool>;

  explicit BinaryTree() noexcept : root(nullptr), treeSize(0) {}
  ~BinaryTree() { clear(this->root); }
  size_type size() { return this->treeSize; }
  size_type max_size();
  bool empty() { return this->size() == 0; }
  T& operator[](const Key& key) noexcept { return this->at(key); }
  bool contains(const Key& key) { return contains(this->root, key); }
  void erase(iterator pos);
  return_type insert(value_type value);
  return_type insert(const Key& key, const T& obj);
  return_type insert_or_assign(const Key& key, const T& obj);
  typename Node::iterator begin() { return root->begin(); }
  typename Node::const_iterator cbegin() const { return root->cbegin(); }
  typename Node::iterator end() { return root->end(); }
  typename Node::const_iterator cend() const { return root->cend(); }
  typename Node::iterator find(const Key& key);
  return_type insert(struct Node** node, struct Node** par, value_type value);
  T& at(const Key& key) { return at(this->root, key); }
  T& at(Node* node, const Key& key);
  bool contains(struct Node* node, const Key& key);
  void clear() { clear(this->root); }

 protected:
  Node* root;
  size_type treeSize;
  void clear(struct Node* node);
  void copyUnique(Node* node_other);

 private:
  void transplant(Node* u, Node* v);
  Node* findMinNode(Node* node);
  return_type replace(const Key& key, const T& obj);
  return_type replace(Node* node, const Key& key, const T& obj);
};

template <typename Key, typename T>
struct BinaryTree<Key, T>::Node {
  value_type data;
  struct Node* parent;
  struct Node* left;
  struct Node* right;

  mutable bool isEnd;

  using iterator = BinaryTreeIterator<Node>;
  using const_iterator = BinaryTreeIterator<const Node>;

  iterator begin() {
    Node* node = this;
    while (node->left != nullptr) node = node->left;
    return iterator(node);
  }

  iterator end() {
    Node* node = this;
    while (node->right != nullptr) {
      node = node->right;
    }
    node->isEnd = true;
    return iterator(node);
  }

  const_iterator cbegin() const {
    const Node* node = this;
    while (node->left != nullptr) node = node->left;
    return const_iterator(node);
  }

  const_iterator cend() const {
    const Node* node = this;
    while (node->right != nullptr) {
      node = node->right;
    }
    node->isEnd = true;
    return const_iterator(node);
  }
};

template <typename Key, typename T>
template <class Iter>
class BinaryTree<Key, T>::BinaryTreeIterator {
  friend class BinaryTree;

 public:
  using iterator_type = Iter;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = iterator_type;

  iterator_type* value;

  iterator_type* find_root() {
    if (value == nullptr) return nullptr;
    value->isEnd = false;
    while (value->parent != nullptr) {
      value = value->parent;
    }
    return value;
  }

  BinaryTreeIterator(const BinaryTreeIterator& it) : value(it.value) {}

  bool operator!=(BinaryTreeIterator const& other) const noexcept {
    return value != other.value;
  }

  bool operator==(BinaryTreeIterator const& other) const noexcept {
    return value == other.value;
  }

  BinaryTreeIterator& operator++() {
    if (value == nullptr || value->isEnd) {
      value = find_root();
      if (value == nullptr) throw std::runtime_error("Empty Tree");
      while (value->left != nullptr) {
        value = value->left;
      }
    } else if (value->right != nullptr) {
      value = value->right;
      while (value->left != nullptr) value = value->left;
    } else {
      iterator_type* p;

      p = value->parent;
      while (p != nullptr && value == p->right) {
        value = p;
        p = p->parent;
      }

      value = p;
    }
    return *this;
  }

  BinaryTreeIterator operator++(int) {
    BinaryTreeIterator temp = *this;
    ++(*this);
    return temp;
  }

  BinaryTreeIterator& operator--() {
    if (value == nullptr || value->isEnd) {
      value = find_root();
      if (value == nullptr) throw std::runtime_error("Empty Tree");
      while (value->right != nullptr) {
        value = value->right;
      }
    } else if (value->left != nullptr) {
      value = value->left;
      while (value->right != nullptr) value = value->right;
    } else {
      iterator_type* p;

      p = value->parent;
      while (p != nullptr && value == p->left) {
        value = p;
        p = p->parent;
      }

      value = p;
    }
    return *this;
  }

  BinaryTreeIterator operator--(int) {
    BinaryTreeIterator temp = *this;
    --(*this);
    return temp;
  }

  const std::pair<const Key, T>* operator->() const { return &(value->data); }

  const std::pair<const Key, T> operator*() const { return value->data; }

  BinaryTreeIterator& operator=(const BinaryTreeIterator& other) {
    if (this != &other) {
      this->value = other.value;
    }
    return *this;
  }

 private:
  BinaryTreeIterator(Iter* p) : value(p) {}
};

template <typename Key, typename T>
void BinaryTree<Key, T>::erase(iterator pos) {
  Node* node = pos.value;

  if (node == nullptr) {
    throw std::runtime_error("Invalid iterator");
  }

  if (node->left == nullptr) {
    transplant(node, node->right);
  } else if (node->right == nullptr) {
    transplant(node, node->left);
  } else {
    Node* minRight = findMinNode(node->right);

    if (minRight->parent != node) {
      transplant(minRight, minRight->right);
      minRight->right = node->right;
      minRight->right->parent = minRight;
    }

    transplant(node, minRight);
    minRight->left = node->left;
    minRight->left->parent = minRight;
  }

  delete node;
  treeSize--;
}

template <typename Key, typename T>
void BinaryTree<Key, T>::transplant(Node* u, Node* v) {
  if (u->parent == nullptr) {
    root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }

  if (v != nullptr) {
    v->parent = u->parent;
  }
}

template <typename Key, typename T>
typename BinaryTree<Key, T>::Node* BinaryTree<Key, T>::findMinNode(Node* node) {
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}

template <typename Key, typename T>
std::pair<typename BinaryTree<Key, T>::iterator, bool>
BinaryTree<Key, T>::insert(value_type value) {
  if (!this->contains(value.first)) {
    return insert(&(this->root), &(this->root), value);
  } else {
    return std::make_pair(iterator(nullptr), false);
  }
}

template <typename Key, typename T>
std::pair<typename BinaryTree<Key, T>::iterator, bool>
BinaryTree<Key, T>::insert(const Key& key, const T& obj) {
  return this->insert(&(this->root), &(this->root), std::make_pair(key, obj));
}

template <typename Key, typename T>
std::pair<typename BinaryTree<Key, T>::iterator, bool>
BinaryTree<Key, T>::replace(const Key& key, const T& obj) {
  return replace(this->root, key, obj);
}

template <typename Key, typename T>
void BinaryTree<Key, T>::copyUnique(Node* node_other) {
  if (node_other == nullptr) {
    return;
  }

  insert(node_other->data);

  copyUnique(node_other->left);
  copyUnique(node_other->right);
}

template <typename Key, typename T>
std::pair<typename BinaryTree<Key, T>::iterator, bool>
BinaryTree<Key, T>::replace(Node* node, const Key& key, const T& obj) {
  if (node == NULL) {
    throw std::runtime_error("Invalid node");
  } else {
    if (key == node->data.first) {
      node->data.second = obj;
      return std::make_pair(iterator(node), true);
    }
    if (key > node->data.first) {
      return replace(node->right, key, obj);
    } else {
      return replace(node->left, key, obj);
    }
  }
}

template <typename Key, typename T>
std::pair<typename BinaryTree<Key, T>::iterator, bool>
BinaryTree<Key, T>::insert_or_assign(const Key& key, const T& obj) {
  if (this->contains(key)) {
    return this->replace(key, obj);
  } else {
    return this->insert(key, obj);
  }
}

template <typename Key, typename T>
typename BinaryTree<Key, T>::Node::iterator BinaryTree<Key, T>::find(
    const Key& key) {
  Node* node = this->root;
  while (node != nullptr) {
    if (key == node->data.first) {
      return typename Node::iterator(node);
    } else if (key > node->data.first) {
      node = node->right;
    } else {
      node = node->left;
    }
  }
  return this->end();
}

template <typename Key, typename T>
std::pair<typename BinaryTree<Key, T>::iterator, bool>
BinaryTree<Key, T>::insert(Node** node, Node** par, value_type value) {
  if (!*node) {
    *node = new Node{value, *par, nullptr, nullptr, false};
    this->treeSize++;
    return {iterator(*node), true};
  }

  return (value.first > (*node)->data.first)
             ? insert(&(*node)->right, node, value)
         : (value.first < (*node)->data.first)
             ? insert(&(*node)->left, node, value)
             : std::pair<iterator, bool>(iterator(*node), false);
}

template <typename Key, typename T>
T& BinaryTree<Key, T>::at(Node* node, const Key& key) {
  if (node == nullptr) {
    throw std::runtime_error("Invalid node");
  } else {
    if (key == node->data.first) {
      return node->data.second;
    } else if (key > node->data.first) {
      return at(node->right, key);
    } else {
      return at(node->left, key);
    }
  }
}

template <typename Key, typename T>
bool BinaryTree<Key, T>::contains(struct Node* node, const Key& key) {
  if (node == nullptr) {
    return false;
  } else {
    if (key == node->data.first) {
      return true;
    } else if (key > node->data.first) {
      return contains(node->right, key);
    } else {
      return contains(node->left, key);
    }
  }
}

template <typename Key, typename T>
void BinaryTree<Key, T>::clear(struct Node* node) {
  if (node != nullptr && treeSize > 0) {
    clear(node->left);
    clear(node->right);
    delete node;
    treeSize--;
  }
}

template <typename Key, typename T>
typename BinaryTree<Key, T>::size_type BinaryTree<Key, T>::max_size() {
  return std::numeric_limits<size_type>::max() / sizeof(BinaryTree<Key, T>) / 2;
}

}  // namespace s21

#endif  // S21_CONTAINERS_SRC_TREES_S21_BINARY_TREE_H_
