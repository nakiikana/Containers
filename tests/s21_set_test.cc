#include "../s21_set.h"

#include <gtest/gtest.h>

TEST(setCtor, test1) {
  set<int> set;
  set.insert(3);
  set.insert(2);
  set.insert(7);
  EXPECT_EQ(set.size(), 3);
}

TEST(eraseTest, test2) {
  set<int> set1{2, 3, 423, 51, 62, 71};
  set1.erase(set1.find(2));
  set1.erase(set1.find(3));
  set1.erase(set1.find(-9));
  set1.erase(set1.find(423));
  set1.erase(set1.find(51));
  set1.erase(set1.find(62));
  set1.erase(set1.find(71));
  set1.erase(set1.find(2));
  EXPECT_EQ(set1.size(), 0);
}

TEST(containsTest, test3) {
  set<int> set1{2, 3, 423, 51, 62, 71};
  set<int> set2{1, 5, 7, 2, 111, 3, 62, -1};
  set1.insert(7);
  EXPECT_EQ(set1.contains(2), set2.contains(2));
  EXPECT_EQ(set1.contains(7), set2.contains(7));
  EXPECT_EQ(set1.contains(99), set2.contains(99));
}

TEST(mergeTest, test4) {
  set<int> set1;
  set<int> set2;
  set1.insert(3);
  set1.insert(2);
  set1.insert(7);
  set1.insert(3);
  set1.insert(9);
  set1.insert(8);
  set2.insert(33);
  set2.insert(22);
  set2.insert(77);
  set2.insert(33);
  set2.insert(99);
  set2.insert(88);
  set1.merge(set2);
  EXPECT_EQ(set1.size(), 10);
}

TEST(clearTest, test5) {
  set<int> set1{2, 3, 423, 51, 62, 71};
  set<int> set2{1, 5, 7, 2, 111, 3, 62, -1};
  set1.clear();
  set2.clear();
  EXPECT_EQ(set1.size(), set2.size());
}

TEST(emptyTest, test6) {
  set<int> set1;
  set<int> set2;
  set1.insert(1);
  set1.insert(2);
  set1.insert(3);
  set1.insert(11);
  set1.insert(22);
  set1.insert(33);
  set2.insert(4);
  set2.insert(5);
  set2.insert(6);
  set1.clear();
  set2.clear();
  EXPECT_EQ(set1.empty(), set2.empty());
}

TEST(emptyTest2, test7) {
  set<int> set1;
  set<int> set2;
  set1.insert(1);
  set1.insert(2);
  set1.insert(3);
  set2.insert(1);
  set2.insert(2);
  set2.insert(3);
  EXPECT_EQ(set1.empty(), set2.empty());
}

TEST(maxTest, test8) {
  set<int> set1{2, 3, 423, 51, 62, 71};
  set<int> set2{1, 5, 7, 2, 111, 3, 62, -1};
  set1.clear();
  set2.clear();
  EXPECT_EQ(set1.max_size(), set2.max_size());
}

TEST(sameTest, test9) {
  set<int> set1;
  set1.insert(-1);
  set1.insert(-2);
  set1.insert(-3);
  set1.insert(456);
  set1.insert(-599);
  set<int> set2(set1);
  set<int> set3(std::move(set1));
  EXPECT_EQ(set2.max_size(), set3.max_size());
  EXPECT_EQ(set2.size(), set3.size());
  EXPECT_EQ(set2.contains(456), set3.contains(456));
  set2.merge(set3);
  EXPECT_EQ(set2.size(), 5);
}

TEST(eqTest, test10) {
  set<int> set1;
  set1.insert(-1);
  set1.insert(-2);
  set1.insert(-3);
  set1.insert(456);
  set1.insert(-599);
  set<int> set2;
  set2.insert(11);
  set2.insert(13);
  set2.insert(14);
  set2.insert(15);
  set2.insert(16);
  set2 = set1;
  EXPECT_EQ(set1.contains(-1), set2.contains(-1));
  EXPECT_EQ(set1.contains(-2), set2.contains(-2));
  EXPECT_EQ(set1.contains(-3), set2.contains(-3));
  EXPECT_EQ(set1.contains(456), set2.contains(456));
  EXPECT_EQ(set1.contains(-599), set2.contains(-599));
  EXPECT_EQ(set1.size(), set2.size());
}

TEST(eqTest2, test11) {
  set<int> set1;
  set1.insert(-1);
  set1.insert(-2);
  set<int> set2;
  set2.insert(11);
  set2.insert(13);
  set2.insert(14);
  set2.insert(15);
  set2.insert(16);
  set2 = set1;
  EXPECT_EQ(set1.contains(-1), set2.contains(-1));
  EXPECT_EQ(set1.contains(-2), set2.contains(-2));
  EXPECT_EQ(set1.size(), 2);
}

TEST(swapTest, test12) {
  set<int> set1;
  set1.insert(-1);
  set1.insert(-2);
  set<int> set2;
  set2.insert(11);
  set2.insert(13);
  set2.insert(14);
  set2.insert(15);
  set2.insert(16);
  set2 = set1;
  EXPECT_EQ(set1.contains(-1), set2.contains(-1));
  EXPECT_EQ(set1.contains(-2), set2.contains(-2));
  EXPECT_EQ(set1.size(), 2);
}

TEST(swapTest, test13) {
  set<int> set1;
  set<int> set2;
  set<int> set3;
  set<int> set4;
  set1.insert(1);
  set1.insert(2);
  set1.insert(3);
  set1.insert(11);
  set1.insert(22);
  set1.insert(33);
  set2.insert(4);
  set2.insert(5);
  set2.insert(6);
  set3.insert(1);
  set3.insert(2);
  set3.insert(3);
  set3.insert(11);
  set3.insert(22);
  set3.insert(33);
  set4.insert(4);
  set4.insert(5);
  set4.insert(6);
  set1.swap(set2);
  set3.swap(set4);
  EXPECT_EQ(set1.contains(4), set3.contains(4));
  EXPECT_EQ(set1.contains(5), set3.contains(5));
  EXPECT_EQ(set1.contains(6), set3.contains(6));
  EXPECT_EQ(set1.size(), set3.size());
  EXPECT_EQ(set2.size(), set4.size());
  EXPECT_EQ(set2.contains(1), set4.contains(1));
  EXPECT_EQ(set2.contains(2), set4.contains(2));
  EXPECT_EQ(set2.contains(3), set4.contains(3));
  EXPECT_EQ(set2.contains(11), set4.contains(11));
  EXPECT_EQ(set2.contains(22), set4.contains(22));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
