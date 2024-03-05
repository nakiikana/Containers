#include "../s21_queue.h"

#include <gtest/gtest.h>

#include <deque>
#include <iostream>
#include <queue>

namespace s21 {

TEST(Queue, Construtor) {
  Queue<int> a;
  EXPECT_EQ(a.size(), 0);

  Queue<int> b(5);
  EXPECT_EQ(b.size(), 5);
  EXPECT_EQ(b.back(), 0);
  EXPECT_EQ(b.front(), 0);

  Queue<int> c(b);
  EXPECT_EQ(b.size(), 5);
  EXPECT_EQ(b.back(), 0);
  EXPECT_EQ(b.front(), 0);
  EXPECT_EQ(c.size(), 5);
  EXPECT_EQ(c.back(), 0);
  EXPECT_EQ(c.front(), 0);

  Queue<int> d(std::move(c));
  EXPECT_EQ(d.size(), 5);
  EXPECT_EQ(d.back(), 0);
  EXPECT_EQ(d.front(), 0);
  EXPECT_EQ(c.size(), 0);

  Queue<int> e = {1, 2, 3, 4, 5};
  EXPECT_EQ(e.size(), 5);
  EXPECT_EQ(e.back(), 5);
  EXPECT_EQ(e.front(), 1);
}

TEST(Queue, Operators) {
  Queue<int> e = {1, 2, 3, 4, 5};
  Queue<int> w(5);
  w = e;
  EXPECT_EQ(e.size(), 5);
  EXPECT_EQ(e.back(), 5);
  EXPECT_EQ(e.front(), 1);
  EXPECT_EQ(w.size(), 5);
  EXPECT_EQ(w.back(), 5);
  EXPECT_EQ(w.front(), 1);

  Queue<int> m(5);
  m = std::move(e);
  EXPECT_EQ(m.size(), 5);
  EXPECT_EQ(m.back(), 5);
  EXPECT_EQ(m.front(), 1);
  EXPECT_EQ(e.size(), 0);
}

TEST(Queue, ElementAccess) {
  Queue<int> a = {1, 2, 3, 4, 5};
  EXPECT_EQ(a.back(), 5);
  EXPECT_EQ(a.front(), 1);

  a.pop();
  a.push(6);
  EXPECT_EQ(a.back(), 6);
  EXPECT_EQ(a.front(), 2);
}

TEST(Queue, Size) {
  Queue<int> a = {1, 2, 3, 4, 5};
  EXPECT_EQ(a.size(), 5);
  EXPECT_EQ(a.empty(), 0);

  Queue<int> b(std::move(a));
  EXPECT_EQ(b.size(), 5);
  EXPECT_EQ(b.empty(), 0);
  EXPECT_EQ(a.size(), 0);
  EXPECT_EQ(a.empty(), 1);
}

TEST(Queue, PushAndPop) {
  Queue<int> a = {1, 2, 3, 4, 5};
  while (!a.empty()) {
    a.pop();
  }
  EXPECT_EQ(a.size(), 0);
  EXPECT_EQ(a.empty(), 1);

  for (int i = 1; i < 6; ++i) {
    a.push(i);
    EXPECT_EQ(a.back(), i);
  }
  EXPECT_EQ(a.size(), 5);
  EXPECT_EQ(a.empty(), 0);
}

TEST(Queue, Swap) {
  Queue<int> a = {1, 2, 3, 4, 5};
  Queue<int> b;
  a.swap(b);
  EXPECT_EQ(b.size(), 5);
  EXPECT_EQ(b.empty(), 0);
  EXPECT_EQ(a.size(), 0);
  EXPECT_EQ(a.empty(), 1);
  EXPECT_EQ(b.back(), 5);

  Queue<int> c = b;
  c.swap(b);
  EXPECT_EQ(b.size(), 5);
  EXPECT_EQ(b.empty(), 0);
  EXPECT_EQ(b.back(), 5);
  EXPECT_EQ(b.front(), 1);
  EXPECT_EQ(c.size(), 5);
  EXPECT_EQ(c.empty(), 0);
  EXPECT_EQ(c.back(), 5);
  EXPECT_EQ(c.front(), 1);

  c.pop();
  c.push(6);
  c.swap(b);
  EXPECT_EQ(b.size(), 5);
  EXPECT_EQ(b.empty(), 0);
  EXPECT_EQ(b.back(), 6);
  EXPECT_EQ(b.front(), 2);
  EXPECT_EQ(c.size(), 5);
  EXPECT_EQ(c.empty(), 0);
  EXPECT_EQ(c.back(), 5);
  EXPECT_EQ(c.front(), 1);
}

TEST(Queue, InsertManyBack) {
  Queue<int> a = {1, 2, 3, 4, 5};
  a.insert_many_back(6, 7, 8);
  for (int i = 1; i < 9; ++i) {
    EXPECT_EQ(a.front(), i);
    a.pop();
  }

  Queue<int> b = {};
  a.insert_many_back(6, 7, 8);
  for (int i = 6; i < 9; ++i) {
    EXPECT_EQ(a.front(), i);
    a.pop();
  }

  Queue<int> c = {};
  a.insert_many_back(1);
  for (int i = 1; i < 2; ++i) {
    EXPECT_EQ(a.front(), i);
    a.pop();
  }
}

}  // namespace s21
