#include "../trees/s21_red_black_tree.h"

#include <gtest/gtest.h>

TEST(RedBlackTest, test1) {
  RedBlackTree<int, int> rbt;
  rbt.AddNode(3, 3);
  rbt.AddNode(1, 1);
  rbt.AddNode(5, 5);
  rbt.AddNode(7, 7);
  rbt.AddNode(6, 8);
  rbt.AddNode(8, 0);
  rbt.AddNode(9, 10);
  rbt.AddNode(10, 10);
  rbt.AddNode(15, 60);
  rbt.DeleteNode(3);
  rbt.DeleteNode(5);
  rbt.DeleteNode(7);
  rbt.DeleteNode(-6);
  rbt.DeleteNode(11);
  rbt.DeleteNode(-3);
  EXPECT_EQ(rbt.GetSize(), 6);
}

TEST(RedBlackTest, test2) {
  RedBlackTree<int, int> rbt;
  rbt.AddNode(10, 1);
  rbt.AddNode(7, 1);
  rbt.AddNode(15, 5);
  rbt.AddNode(5, 7);
  rbt.AddNode(8, 8);
  rbt.AddNode(2, 0);
  rbt.AddNode(6, 10);
  RedBlackTree<int, int> rbt2(rbt);
  RedBlackTree<int, int> rbt3(std::move(rbt));
  EXPECT_EQ(rbt2.GetSize(), rbt3.GetSize());
  EXPECT_EQ(rbt2.CountBlack(), rbt3.CountBlack());
  EXPECT_EQ(rbt2.Height(), rbt3.Height());
}

TEST(RedBlackTest, test3) {
  RedBlackTree<int, int> rbt;
  rbt.AddNode(5, 3);
  rbt.AddNode(8, 1);
  rbt.AddNode(18, 5);
  rbt.AddNode(15, 0);
  rbt.AddNode(17, 8);
  rbt.AddNode(16, 16);
  rbt.AddNode(25, 0);
  rbt.AddNode(40, 11);
  rbt.AddNode(80, 11);
  EXPECT_EQ(rbt.Height(), 3);
}

TEST(RedBlackTest, test5) {
  RedBlackTree<int, int> rbt(1, 1);
  RedBlackTree<int, int> rbt1(2, 2);
  EXPECT_EQ(rbt.CountBlack(), rbt1.CountBlack());
}

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);

//   return RUN_ALL_TESTS();
// }
