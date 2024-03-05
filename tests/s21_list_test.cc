#include "../s21_list.h"

#include <gtest/gtest.h>

#include <iostream>
#include <list>

namespace s21 {

TEST(List, Max_size) {
  std::list<int> a;
  List<int> b;
  EXPECT_EQ(a.max_size(), b.max_size());

  std::list<double> a1;
  List<double> b1;
  EXPECT_EQ(a1.max_size(), b1.max_size());

  std::list<char> a2;
  List<char> b2;
  EXPECT_EQ(a2.max_size(), b2.max_size());

  std::list<size_t> a3;
  List<size_t> b3;
  EXPECT_EQ(a3.max_size(), b3.max_size());

  std::list<List<int>::Node> a4;
  List<List<int>::Node> b4;
  EXPECT_EQ(a4.max_size(), b4.max_size());
}

TEST(List, Empty) {
  std::list<int> a;
  List<int> b;
  EXPECT_EQ(a.empty(), b.empty());
  EXPECT_EQ(a.size(), b.size());

  a.push_back(1);
  b.push_back(1);
  EXPECT_EQ(a.empty(), b.empty());
  EXPECT_EQ(a.size(), b.size());

  b.pop_back();
  EXPECT_EQ(0, b.back());
}

TEST(List, ElementAccess) {
  std::initializer_list<int> a = {1, 2, 3, 4};
  std::list<int> b(a);
  List<int> c(a);
  EXPECT_EQ(b.front(), c.front());
  EXPECT_EQ(b.back(), c.back());
  EXPECT_EQ(c.size(), b.size());

  c.clear();
  b.clear();
  EXPECT_EQ(c.empty(), b.empty());
  EXPECT_EQ(b.front(), c.front());

  std::list<int> b1;
  List<int> c2;

  EXPECT_EQ(b1.back(), c2.front());
}

TEST(List, Iterator) {
  std::initializer_list<int> a = {1, 2, 3, 4};
  std::list<int> b(a);
  List<int> c(a);
  auto it_b = b.begin();
  for (auto it = c.begin(); it != c.end(); ++it) {
    EXPECT_EQ(*it, *it_b);
    ++it_b;
  }

  EXPECT_EQ(*(c.begin()), *b.begin());
  EXPECT_EQ(*(c.end()), *c.end());

  while (!c.empty()) {
    c.pop_front();
  }
  b.clear();
  EXPECT_EQ(*(c.begin()), *b.begin());
  EXPECT_EQ(*(c.end()), *c.end());
}

TEST(List, Clear) {
  std::initializer_list<int> a = {1, 2, 3, 4};
  List<int> c(a);
  c.clear();
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(c.size(), 0);
  c.clear();
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(c.size(), 0);
}

TEST(List, InsertAndErase) {
  std::initializer_list<int> a = {1, 2, 3, 4};
  List<int> b(a);
  List<int> c;

  List<int>::iterator it = c.end();
  it = c.insert(it, 3);
  it = c.insert(c.begin(), 1);
  ++it;
  it = c.insert(it, 2);
  it = c.insert(c.end(), 4);
  EXPECT_EQ((c == b), 1);
  EXPECT_EQ((c != b), 0);

  it = c.begin();
  it = c.insert(it, 5);
  EXPECT_EQ(*it, 5);
  EXPECT_EQ(c.size(), 5);
  EXPECT_EQ((c == b), 0);
  EXPECT_EQ((c != b), 1);
  while (!c.empty()) {
    c.erase(c.begin());
  }
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(c.front(), 0);
  EXPECT_EQ(c.back(), 0);
  EXPECT_EQ(*c.end(), 0);
  EXPECT_EQ(*c.begin(), 0);

  for (int i = 4; i > 0; --i) {
    it = c.insert(c.end(), i);
  }
  it = c.begin();
  ++it;
  ++it;
  c.erase(it);
  it = c.end();
  --it;
  c.erase(it);
  while (!c.empty()) {
    c.erase(c.begin());
  }
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(c.front(), 0);
  EXPECT_EQ(c.back(), 0);
  EXPECT_EQ(*c.end(), 0);
  EXPECT_EQ(*c.begin(), 0);
}

TEST(List, PushAndPop) {
  std::initializer_list<int> a = {1, 2, 3, 4};
  List<int> b(a);
  List<int> c;

  for (auto it = b.begin(); it != b.end(); ++it) {
    c.push_back(*it);
  }
  EXPECT_EQ((c == b), 1);

  for (auto it = b.begin(); it != b.end(); ++it) {
    c.pop_front();
  }
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(c.front(), 0);
  EXPECT_EQ(c.back(), 0);
  EXPECT_EQ(*c.end(), 0);
  EXPECT_EQ(*c.begin(), 0);

  b.push_back(5);
  b.push_front(0);
  for (int i = 5; i > -1; --i) {
    c.push_front(i);
  }
  EXPECT_EQ((c == b), 1);
  EXPECT_EQ(c.size(), 6);
  EXPECT_EQ(*c.end(), 6);

  for (auto it = b.begin(); it != b.end(); ++it) {
    c.pop_back();
  }
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(c.front(), 0);
  EXPECT_EQ(c.back(), 0);
  EXPECT_EQ(*c.end(), 0);
  EXPECT_EQ(*c.begin(), 0);

  b.pop_back();
  b.pop_front();
  for (int i = 4; i > 0; --i) {
    c.push_front(i);
  }
  EXPECT_EQ((c == b), 1);
  EXPECT_EQ(c.size(), 4);
  EXPECT_EQ(*c.end(), 4);
}

TEST(List, Swap) {
  std::initializer_list<int> a = {1, 2, 3, 4};
  List<int> b(a);
  List<int> c;

  c.swap(b);
  EXPECT_EQ(b.front(), 0);
  EXPECT_EQ(b.back(), 0);
  EXPECT_EQ(*b.end(), 0);
  EXPECT_EQ(*b.begin(), 0);
  EXPECT_EQ(c.size(), 4);
  auto it = c.begin();
  for (int i = 1; i < 5; ++i) {
    EXPECT_EQ(*it, i);
    ++it;
  }

  b = c;
  it = b.begin();
  for (int i = 1; i < 5; ++i) {
    EXPECT_EQ(*it, i);
    ++it;
  }
  b.swap(c);
  EXPECT_EQ((c == b), 1);

  b.push_back(5);
  b.swap(c);
  it = c.begin();
  for (int i = 1; i < 6; ++i) {
    EXPECT_EQ(*it, i);
    ++it;
  }
  it = b.begin();
  for (int i = 1; i < 5; ++i) {
    EXPECT_EQ(*it, i);
    ++it;
  }
}

TEST(List, merge) {
  std::initializer_list<int> a = {1, 2, 3, 4};
  List<int> b(a);
  List<int> c(b);

  c.clear();
  for (int i = -5; i < 0; i++) {
    c.push_back(i);
  }
  b.merge(c);
  EXPECT_EQ(c.empty(), 1);
  List<int> temp = {-5, -4, -3, -2, -1, 1, 2, 3, 4};
  EXPECT_EQ(b == temp, 1);

  b.clear();
  for (int i = 1; i < 5; i++) {
    c.push_back(i);
    b.push_front(i);
  }
  List<int> temp2 = {1, 2, 3, 4, 3, 2, 1, 4};
  b.merge(c);
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(b == temp2, 1);

  b.clear();
  for (int i = 1; i < 5; i++) {
    c.push_front(i);
    b.push_back(i);
  }
  List<int> temp3 = {1, 2, 3, 4, 4, 3, 2, 1};
  b.merge(c);
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(b == temp3, 1);
  b.clear();
  for (int i = 1; i < 5; i++) {
    c.push_back(i);
    b.push_back(i);
  }
  List<int> temp4 = {1, 1, 2, 2, 3, 3, 4, 4};
  b.merge(c);
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(b == temp4, 1);
}

TEST(List, Reverse) {
  List<int> b = {1, 2, 3, 4};
  List<int> c(b);
  c.reverse();
  auto it_b = b.end();
  --it_b;
  for (auto it = c.begin(); it != c.end(); ++it) {
    EXPECT_EQ(*it, *it_b);
    --it_b;
  }

  List<int> g = {1, 2, 3, 4, 5};
  c = g;
  EXPECT_EQ(c == g, 1);
  c.reverse();
  auto it_g = g.end();
  --it_g;
  for (auto it = c.begin(); it != c.end(); ++it) {
    EXPECT_EQ(*it, *it_g);
    --it_g;
  }
  c.reverse();
  EXPECT_EQ(c == g, 1);

  c.clear();
  c.reverse();
  EXPECT_EQ(c.size(), 0);
  EXPECT_EQ(c.empty(), 1);
}

TEST(List, splice) {
  List<int> b = {1, 2, 3, 4, 5};
  List<int> c = b;
  auto it = b.begin();
  ++it;
  ++it;
  b.splice(it, c);
  List<int> res = {1, 2, 1, 2, 3, 4, 5, 3, 4, 5};
  EXPECT_EQ(c.empty(), 1);
  EXPECT_EQ(c.front(), 0);
  EXPECT_EQ(c.back(), 0);
  EXPECT_EQ(*c.end(), 0);
  EXPECT_EQ(*c.begin(), 0);
  EXPECT_EQ(b == res, 1);

  b.splice(it, c);
  EXPECT_EQ(b == res, 1);

  c.splice(c.end(), b);
  EXPECT_EQ(c == res, 1);
  EXPECT_EQ(b.empty(), 1);
  EXPECT_EQ(b.front(), 0);
  EXPECT_EQ(b.back(), 0);
  EXPECT_EQ(*b.end(), 0);
  EXPECT_EQ(*b.begin(), 0);
}

TEST(List, Unique) {
  List<int> a = {1, 3, 5, 5, 1, 4, 4, 4, 1, 2, 2, 3, 3, 2};
  List<int> res = {1, 3, 5, 1, 4, 1, 2, 3, 2};
  a.unique();
  EXPECT_EQ(a == res, 1);
  a.unique();
  EXPECT_EQ(a == res, 1);

  List<int> b = {1, 1};
  List<int> res2 = {1};
  b.unique();
  EXPECT_EQ(b == res2, 1);

  List<char> s = {'w', 'a', 'b', ' ', ' ', 'w'};
  List<char> res3 = {'w', 'a', 'b', ' ', 'w'};
  s.unique();
  EXPECT_EQ(s == res3, 1);

  List<char> w = {};
  w.unique();
  EXPECT_EQ(w.empty(), 1);
  EXPECT_EQ(w.front(), 0);
  EXPECT_EQ(w.back(), 0);
  EXPECT_EQ(*w.end(), 0);
  EXPECT_EQ(*w.begin(), 0);
}

TEST(List, Sort) {
  List<int> a = {1, 5, 3, 6, 4, 2, 10, 9, 8, 7};
  List<int> res = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  a.sort();
  EXPECT_EQ(a == res, 1);

  a.sort();
  EXPECT_EQ(a == res, 1);

  List<int> b = {5, 5, 4, 3, 2, 2, 1};
  List<int> res2 = {1, 2, 2, 3, 4, 5, 5};
  b.sort();
  EXPECT_EQ(b == res2, 1);
}

TEST(List, Constructor) {
  List<int> a(5);
  for (auto it = a.begin(); it != a.end(); ++it) {
    EXPECT_EQ(*it, 0);
  }
  EXPECT_EQ(a.size(), 5);

  List<int> b(std::move(a));
  for (auto it = b.begin(); it != b.end(); ++it) {
    EXPECT_EQ(*it, 0);
  }
  EXPECT_EQ(b.size(), 5);
  EXPECT_EQ(a.empty(), 1);

  List<int>::iterator g;
}

TEST(List, Operator) {
  List<int> a(5);
  List<int> b(4);
  b = std::move(a);
  for (auto it = b.begin(); it != b.end(); ++it) {
    EXPECT_EQ(*it, 0);
  }
  EXPECT_EQ(b.size(), 5);
  EXPECT_EQ(a.empty(), 1);
  EXPECT_EQ(a.front(), 0);
  EXPECT_EQ(a.back(), 0);
  EXPECT_EQ(*a.end(), 0);
  EXPECT_EQ(*a.begin(), 0);

  a = b;
  b = a;
  EXPECT_EQ(b == a, 1);
}

TEST(List, Const_reference) {
  List<int> a = {1, 2, 3, 4, 5};
  int i = 1;
  for (List<int>::const_iterator it = a.begin(); it != a.end(); ++it) {
    EXPECT_EQ(*it == i, 1);
    ++i;
  }
}

TEST(List, InsertManyBack) {
  List<int> a = {1, 2, 3, 4, 5};
  a.insert_many_back(6, 7, 8);
  List<int> b = {1, 2, 3, 4, 5, 6, 7, 8};
  EXPECT_EQ(b == a, 1);

  List<int> c;
  c.insert_many_back(1, 2, 3);
  List<int> d = {1, 2, 3};
  EXPECT_EQ(c == d, 1);

  List<int> e;
  e.insert_many_back(1);
  List<int> f = {1};
  EXPECT_EQ(f == e, 1);
}

TEST(List, InsertManyFront) {
  List<int> a = {1, 2, 3, 4, 5};
  a.insert_many_front(6, 7, 8);
  List<int> b = {6, 7, 8, 1, 2, 3, 4, 5};
  EXPECT_EQ(b == a, 1);

  List<int> c;
  c.insert_many_front(1, 2, 3);
  List<int> d = {1, 2, 3};
  EXPECT_EQ(c == d, 1);

  List<int> e;
  e.insert_many_back(1);
  List<int> f = {1};
  EXPECT_EQ(f == e, 1);
}

TEST(List, InsertMany) {
  List<int> a = {1, 2, 3, 4, 5};
  auto it = a.insert_many(a.begin(), 6, 7, 8);
  List<int> b = {6, 7, 8, 1, 2, 3, 4, 5};
  EXPECT_EQ(b == a, 1);
  EXPECT_EQ(*it, 8);

  List<int> c = {1, 2, 3, 4, 5};
  it = c.insert_many(c.end(), 6, 7, 8);
  List<int> d = {1, 2, 3, 4, 5, 6, 7, 8};
  EXPECT_EQ(c == d, 1);
  EXPECT_EQ(*it, 8);

  List<int> e;
  it = e.insert_many(e.end(), 1);
  List<int> f = {1};
  EXPECT_EQ(f == e, 1);
  EXPECT_EQ(*it, 1);

  List<int> g = {1, 2, 3, 4, 5};
  it = g.begin();
  ++it;
  ++it;
  it = g.insert_many(it, 6, 7, 8);
  List<int> h = {1, 2, 6, 7, 8, 3, 4, 5};
  EXPECT_EQ(h == g, 1);
  EXPECT_EQ(*it, 8);
}

}  // namespace s21
