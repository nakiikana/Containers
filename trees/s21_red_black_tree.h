#ifndef S21_CONTAINERS_SRC_TREES_S21_RED_BLACK_TREE_H_
#define S21_CONTAINERS_SRC_TREES_S21_RED_BLACK_TREE_H_

#include <iostream>
template <typename K, typename V>
class RedBlackTree {
 public:
  class Node {
   public:
    Node(K key_, V value_)
        : key_(key_),
          value_(value_),
          isBlack_(false),
          isLeftChild_(false),
          leftChild(nullptr),
          rightChild(nullptr),
          parent(nullptr){};

    Node(Node &other)
        : key_(other.key_),
          value_(other.value_),
          isBlack_(other.isBlack_),
          isLeftChild_(other.isLeftChild_),
          leftChild(other.leftChild),
          rightChild(other.rightChild),
          parent(other.parent){};

    Node(Node &&other) { *this = std::move(other); };

    Node &operator=(const Node &other) {
      Node temp(other);
      *this = std::move(temp);
      return *this;
    };

    Node &operator=(Node &&other) noexcept {
      std::swap(key_, other.key_);
      std::swap(value_, other.value_);
      std::swap(isBlack_, other.isBlack_);
      std::swap(isLeftChild_, other.isLeftChild_);
      std::swap(leftChild, other.leftChild);
      std::swap(rightChild, other.rightChild);
      std::swap(parent, other.parent);
      return *this;
    };

    ~Node() {
      delete rightChild;
      delete leftChild;
    };

    void CopyTree(Node *other) {
      if (other != NULL) {
        // if (node -> parent == NULL)
        if (other->rightChild != NULL) {
          Node *n_node = new Node(*(other->rightChild));
          n_node->parent = this;
          rightChild = n_node;
          rightChild->CopyTree(other->rightChild);
        }
        if (other->leftChild != NULL) {
          Node *n_node = new Node(*(other->leftChild));
          n_node->parent = this;
          leftChild = n_node;
          leftChild->CopyTree(other->leftChild);
        }
      }
    }

    Node *Next() {
      Node *temp = this;
      if (!temp) return temp;
      if (rightChild) {
        temp = rightChild;
        temp = temp->MinFromHere();
        return temp;
      }
      if (isLeftChild_) return parent;
      return rightChild;
    }

    Node *Previous() {
      Node *temp = NULL;
      if (leftChild) {
        temp = leftChild;
        temp = temp->MaxFromHere();
        return temp;
      }
      return NULL;
    }

    Node *MaxFromHere() {
      Node *temp = this;
      while (temp && temp->rightChild) {
        temp = temp->rightChild;
      }
      return temp;
    };

    Node *MinFromHere() {
      Node *temp = this;
      while (temp && temp->leftChild) {
        temp = temp->leftChild;
      }
      return temp;
    }

   private:
    friend class RedBlackTree;
    const K key_;
    V value_;
    bool isBlack_;
    /* if a node's parent is a left child then we are going to look
        for a node's aunt on the right side from our grandfather*/
    bool isLeftChild_;
    Node *leftChild, *rightChild, *parent;
    // if parent is NULL then we are dealing with a root node
  };

  RedBlackTree() : size_(0), root_(nullptr){};
  RedBlackTree(K key, V value) : size_(1), root_(new Node(key, value)){};
  RedBlackTree(const RedBlackTree &other)
      : size_(other.size_),
        root_(new Node(other.root_->key_, other.root_->value_)) {
    root_->isBlack_ = true;
    root_->CopyTree(other.root_);
  }
  RedBlackTree(RedBlackTree &&other) : size_(0), root_(nullptr) {
    *this = std::move(other);
  };

  RedBlackTree &operator=(const RedBlackTree &other) {
    RedBlackTree temp(other);
    *this = std::move(temp);
    return *this;
  };

  RedBlackTree &operator=(RedBlackTree &&other) noexcept {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    return *this;
  }

  ~RedBlackTree() {
    if (root_ != NULL) delete root_;
  };

  void AddNode(Node *other) { AddNode(other->key_, other->value_); }

  void AddNode(K key, V value) {
    if (FindNode(key) == NULL) {
      Node *node = new Node(key, value);
      if (root_ == NULL) {
        root_ = node;  //
        root_->isBlack_ = true;
        size_ += 1;
        return;
      }
      size_ += 1;
      AddNode(root_, node);
    }
  };

  void DeleteNode(K key) {
    Node *removeThis = FindNode(key);
    Node *change = NULL;
    if (!removeThis) return;
    size_ -= 1;
    if (!removeThis->leftChild) {
      change = removeThis->rightChild;
      Transplantation(removeThis, change);
    } else if (!removeThis->rightChild) {
      change = removeThis->leftChild;
      Transplantation(removeThis, change);
    } else {
      change = removeThis->Previous();
      Transplantation(change, change->leftChild);
      change->leftChild = removeThis->leftChild;
      change->rightChild = removeThis->rightChild;
      if (change->leftChild) change->leftChild->parent = change;
      if (change->rightChild) change->rightChild->parent = change;
      Transplantation(removeThis, change);
    }
    if (change) {
      change->isLeftChild_ = removeThis->isLeftChild_;
    }
    if (change) FixAfterDelete(change);
    removeThis->rightChild = removeThis->leftChild = removeThis->parent = NULL;
    delete removeThis;
  }

  void Transplantation(Node *dad, Node *child) {
    if (dad == root_) {
      root_ = child;
    } else if (dad->isLeftChild_) {
      dad->parent->leftChild = child;
    } else {
      dad->parent->rightChild = child;
    }
    if (child) child->parent = dad->parent;
  }

  void FixAfterDelete(Node *node) {
    while (node != root_ && (node == NULL || node->isBlack_)) {
      if (node == node->parent->leftChild) {
        Node *sibling = node->parent->rightChild;
        if (sibling == NULL) return;

        if (!sibling->isBlack_) {
          sibling->isBlack_ = true;
          node->parent->isBlack_ = false;
          LeftRotate(node->parent);
          sibling = node->parent->rightChild;
        }

        if ((sibling->leftChild == NULL || sibling->leftChild->isBlack_) &&
            (sibling->rightChild == NULL || sibling->rightChild->isBlack_)) {
          sibling->isBlack_ = false;
          node = node->parent;
        } else {
          if (sibling->rightChild == NULL || sibling->rightChild->isBlack_) {
            if (sibling->leftChild != NULL) sibling->leftChild->isBlack_ = true;
            sibling->isBlack_ = false;
            RightRotate(sibling);
            sibling = node->parent->rightChild;
          }

          sibling->isBlack_ = node->parent->isBlack_;
          node->parent->isBlack_ = true;
          if (sibling->rightChild != NULL) sibling->rightChild->isBlack_ = true;
          LeftRotate(node->parent);
          node = root_;
        }
      } else {
        Node *sibling = node->parent->leftChild;
        if (sibling == NULL) return;

        if (!sibling->isBlack_) {
          sibling->isBlack_ = true;
          node->parent->isBlack_ = false;
          RightRotate(node->parent);
          sibling = node->parent->leftChild;
        }

        if ((sibling->leftChild == NULL || sibling->leftChild->isBlack_) &&
            (sibling->rightChild == NULL || sibling->rightChild->isBlack_)) {
          sibling->isBlack_ = false;
          node = node->parent;
        } else {
          if (sibling->leftChild == NULL || sibling->leftChild->isBlack_) {
            if (sibling->rightChild != NULL)
              sibling->rightChild->isBlack_ = true;
            sibling->isBlack_ = false;
            LeftRotate(sibling);
            sibling = node->parent->leftChild;
          }

          sibling->isBlack_ = node->parent->isBlack_;
          node->parent->isBlack_ = true;
          if (sibling->leftChild != NULL) sibling->leftChild->isBlack_ = true;
          RightRotate(node->parent);
          node = root_;
        }
      }
    }

    if (node != NULL) {
      node->isBlack_ = true;
    }
  }

  Node *Sibling(Node *node) {
    if (node->isLeftChild_) return node->parent->rightChild;
    return node->parent->leftChild;
  }

  void TreeCorrection(Node *node) {
    if (node->parent->isLeftChild_) {  // aunt is right-wing
      if (node->parent->parent->rightChild == NULL ||
          node->parent->parent->rightChild->isBlack_)
        return DoRotation(node);
      if (node->parent->parent->rightChild != NULL) {
        node->parent->parent->rightChild->isBlack_ = true;
        node->parent->parent->isBlack_ = false;
        node->parent->isBlack_ = true;
        return;
      }
    } else {  // our aunt is a leftie
      if (node->parent->parent->leftChild == NULL ||
          node->parent->parent->leftChild->isBlack_)
        return DoRotation(node);
      if (node->parent->parent->leftChild != NULL) {
        node->parent->parent->leftChild->isBlack_ = true;
        node->parent->parent->isBlack_ = false;
        node->parent->isBlack_ = true;
        return;
      }
    }
  }

  void CheckColor(Node *newNode) {
    if (newNode == root_) return;
    if (!newNode->isBlack_ && !newNode->parent->isBlack_) {
      TreeCorrection(newNode);
      root_->isBlack_ = true;
      root_->parent = NULL;
    }
    CheckColor(newNode->parent);
  };

  void DoRotation(Node *node) {
    if (node == NULL) return;
    if (node->isLeftChild_) {
      if (node->parent->isLeftChild_) {
        RightRotate(node->parent->parent);
        node->isBlack_ = false;
        if (node->parent->rightChild != NULL)
          node->parent->rightChild->isBlack_ = false;
        node->parent->isBlack_ = true;
        return;
      } else {
        RightLeftRotate(node->parent->parent);
        node->isBlack_ = true;
        if (node->rightChild != NULL) node->rightChild->isBlack_ = false;
        if (node->leftChild != NULL) node->leftChild->isBlack_ = false;
        return;
      }
    } else {
      if (!node->parent->isLeftChild_) {
        LeftRotate(node->parent->parent);
        node->isBlack_ = false;
        node->parent->isBlack_ = true;
        if (node->parent->leftChild != NULL)
          node->parent->leftChild->isBlack_ = false;
        return;
      } else {
        LeftRightRotate(node->parent->parent);
        node->isBlack_ = false;
        if (node->rightChild != NULL) node->rightChild->isBlack_ = false;
        if (node->leftChild != NULL) node->leftChild->isBlack_ = false;
        return;
      }
    }
  }
  void LeftRotate(Node *node) {
    Node *temp = node->rightChild;
    node->rightChild = temp->leftChild;
    if (node->rightChild != NULL) {
      node->rightChild->parent = node;
      node->rightChild->isLeftChild_ = false;
    }
    FixChild(node, temp);
    temp->leftChild = node;
    node->isLeftChild_ = true;
    node->parent = temp;
  }

  void RightRotate(Node *node) {
    Node *temp = node->leftChild;
    node->leftChild = temp->rightChild;
    if (node->leftChild != NULL) {
      node->leftChild->isLeftChild_ = true;
      node->leftChild->parent = node;
    }
    FixChild(node, temp);
    temp->rightChild = node;
    node->isLeftChild_ = false;
    node->parent = temp;
  }

  void FixChild(Node *node, Node *temp) {
    if (node->parent == NULL) {  // we are root_
      root_ = temp;
      temp->parent = NULL;
    } else {
      temp->parent = node->parent;
      if (node->isLeftChild_) {
        temp->isLeftChild_ = true;
        temp->parent->leftChild = temp;
      } else {
        temp->isLeftChild_ = false;
        temp->parent->rightChild = temp;
      }
    }
  }

  void LeftRightRotate(Node *node) {
    LeftRotate(node->leftChild);
    RightRotate(node);
  }

  void RightLeftRotate(Node *node) {
    RightRotate(node->rightChild);
    LeftRotate(node);
  }

  size_t GetSize() { return size_; };

  Node *MaxInTree() { return root_->MinFromHere(); }

  Node *MinInTree() { return root_->MinFromHere(); }

  int Height() {
    if (root_ == NULL) return 0;
    return Height(root_) - 1;
  }

  int Height(Node *node) {
    if (node == NULL) return 0;
    int leftHeight = 0, rightHeight = 0;
    leftHeight = Height(node->leftChild) + 1;
    rightHeight = Height(node->rightChild) + 1;
    if (leftHeight > rightHeight) return leftHeight;
    return rightHeight;
  }

  int CountBlack() {
    if (root_ == NULL) return 1;
    return CountBlack(root_) - 1;
  }

  int CountBlack(Node *node) {
    if (node == NULL) return 1;
    int rightSideBlacks = CountBlack(node->rightChild);
    int leftSideBlacks = CountBlack(node->leftChild);
    if (rightSideBlacks != leftSideBlacks) {
      return -1;
    };
    if (node->isBlack_) leftSideBlacks++;
    return leftSideBlacks;
  }

  Node *FindNode(K key_) {
    Node *result = NULL;
    if (root_ != NULL) {
      if (root_->key_ == key_) return root_;
      result = FindNode(root_, key_);
    }
    return result;
  }

  Node *FindNode(Node *node, K key_) {
    Node *result = NULL;
    if (node != NULL) {
      if (node->key_ == key_) return node;
      if (key_ < node->key_) {
        result = FindNode(node->leftChild, key_);
      } else {
        result = FindNode(node->rightChild, key_);
      }
    }
    return result;
  }

  // additional print
  void PrintTree(const std::string &prefix, const Node *node) {
    if (node != NULL) {
      std::cout << prefix;
      std::cout << (node->isLeftChild_ ? "├──" : "└──");
      std::cout << node->key_ << (node->isBlack_ ? " (B)" : " (R)")
                << std::endl;
      PrintTree(prefix + (node->isLeftChild_ ? "│   " : "    "),
                node->leftChild);
      PrintTree(prefix + (node->isLeftChild_ ? "│   " : "    "),
                node->rightChild);
    }
  }

  void PrintTree() { PrintTree("", root_); }

  template <typename T>
  class RbIterator {
   public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;

    RbIterator() : ptr_(nullptr){};

    RbIterator(T *ptr) : ptr_(ptr){};

    RbIterator &operator++() {
      ptr_ = ptr_->Next();
      return *this;
    }

    RbIterator &operator--() {
      ptr_ = ptr_->Previous();
      return *this;
    }

    bool operator==(const RbIterator &other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const RbIterator &other) const {
      return ptr_ != other.ptr_;
    }

    T &operator*() const { return *ptr_; }

    typename std::conditional<std::is_const<T>::value, V, V &>::type
    operator*() {
      return ptr_->value_;
    }
    T *operator->() const { return ptr_; }

   private:
    T *ptr_;
  };

  typedef RbIterator<Node> iterator;
  typedef RbIterator<const Node> const_iterator;

  iterator begin() { return iterator(root_->MinFromHere()); }

  iterator end() { return ++iterator(root_->MaxFromHere()); }

  const_iterator begin() const { return iterator(root_->MinFromHere()); }

  const_iterator end() const { return ++iterator(root_->MaxFromHere()); }

  // additional print

 private:
  void AddNode(Node *parent, Node *newNode) {
    if (parent->key_ < newNode->key_) {
      if (parent->rightChild == NULL) {
        parent->rightChild = newNode;
        newNode->parent = parent;
        newNode->isLeftChild_ = false;
        return;
      } else {
        AddNode(parent->rightChild, newNode);
        CheckColor(newNode);
      }
    } else {
      if (parent->leftChild == NULL) {
        parent->leftChild = newNode;
        newNode->parent = parent;
        newNode->isLeftChild_ = true;
        return;
      } else {
        AddNode(parent->leftChild, newNode);
        CheckColor(newNode);
      }
    }
  };
  size_t size_;
  Node *root_;
};

#endif  // S21_CONTAINERS_SRC_TREES_S21_RED_BLACK_TREE_H_
