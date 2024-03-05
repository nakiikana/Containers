#include "../s21_map.h"

#include <gtest/gtest.h>

#include <string>

TEST(MapTest, DefaultConstructor) {
  s21::map<int, std::string> myMap;
  EXPECT_EQ(myMap.size(), 0);
}

TEST(MapTest, CopyConstructor) {
  s21::map<int, std::string> original_map = {
      {1, "one"}, {2, "two"}, {3, "three"}};

  s21::map<int, std::string> copied_map(original_map);

  ASSERT_EQ(original_map.size(), copied_map.size());

  for (const auto& item : original_map) {
    auto it = copied_map.find(item.first);
    ASSERT_TRUE(it != copied_map.end());
    ASSERT_EQ(it->second, item.second);
  }

  copied_map[1] = "modified";
  ASSERT_NE(original_map[1], copied_map[1]);
}

TEST(MapTest, MoveConstructor) {
  s21::map<int, std::string> original_map = {
      {1, "one"}, {2, "two"}, {3, "three"}};

  s21::map<int, std::string> moved_map(std::move(original_map));

  ASSERT_TRUE(original_map.empty());

  ASSERT_EQ(moved_map.size(), 3);
  ASSERT_EQ(moved_map[1], "one");
  ASSERT_EQ(moved_map[2], "two");
  ASSERT_EQ(moved_map[3], "three");
}

TEST(MapTest, MoveAssignmentOperator) {
  s21::map<int, std::string> original_map = {
      {1, "one"}, {2, "two"}, {3, "three"}};

  s21::map<int, std::string> another_map;

  another_map = std::move(original_map);

  ASSERT_EQ(another_map.size(), 3);
  ASSERT_EQ(another_map[1], "one");
  ASSERT_EQ(another_map[2], "two");
  ASSERT_EQ(another_map[3], "three");

  ASSERT_TRUE(original_map.empty());
}

TEST(MapTest, SwapFunction) {
  s21::map<int, std::string> map1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> map2 = {{3, "three"}, {4, "four"}};

  map1.swap(map2);

  ASSERT_EQ(map1.size(), 2);
  ASSERT_EQ(map1[3], "three");
  ASSERT_EQ(map1[4], "four");

  ASSERT_EQ(map2.size(), 2);
  ASSERT_EQ(map2[1], "one");
  ASSERT_EQ(map2[2], "two");
}

TEST(MapTest, MergeFunction) {
  s21::map<int, std::string> map1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> map2 = {{3, "three"}, {4, "four"}};

  map1.merge(map2);

  ASSERT_EQ(map1.size(), 4);
  ASSERT_EQ(map1[1], "one");
  ASSERT_EQ(map1[2], "two");
  ASSERT_EQ(map1[3], "three");
  ASSERT_EQ(map1[4], "four");
}

TEST(MapTest, ClearFunction) {
  s21::map<int, std::string> map1 = {{1, "one"}, {2, "two"}, {3, "three"}};

  ASSERT_EQ(map1.size(), 3);

  map1.clear();

  ASSERT_TRUE(map1.empty());
}

TEST(MapTest, Iterators) {
  s21::map<int, std::string> tree = {
      {3, "TV"}, {2, "123"}, {4, "bebra"}, {4, "Biba"}};

  auto it2 = tree.end();

  it2--;
  it2--;
  ASSERT_TRUE(it2->second == "TV");
  it2++;
  ASSERT_TRUE(it2->second == "bebra");
  --it2;
  ASSERT_TRUE(it2->second == "TV");
  ++it2;
  ASSERT_TRUE(it2->second == "bebra");

  it2 = tree.begin();

  ASSERT_TRUE(it2->second == "123");

  it2 = tree.end();
  it2++;
  it2 = tree.begin();
  it2--;
}

TEST(MapTest, EraseTest) {
  s21::map<int, std::string> map1 = {{1, "one"}, {2, "two"}, {3, "three"}};

  ASSERT_EQ(map1.size(), 3);

  auto it = map1.find(2);
  ASSERT_NE(it, map1.end());
  map1.erase(it);

  ASSERT_EQ(map1.size(), 2);

  it = map1.find(42);
  ASSERT_EQ(it, map1.end());
  ASSERT_NO_THROW(map1.erase(it));
}

TEST(MapTest, IntIntTest) {
  s21::map<int, int> map1;

  map1.insert({1, 10});
  map1.insert({2, 20});
  map1.insert({3, 30});

  ASSERT_EQ(map1.size(), 3);
  ASSERT_EQ(map1[1], 10);
  ASSERT_EQ(map1[2], 20);
  ASSERT_EQ(map1[3], 30);

  map1[2] = 25;
  ASSERT_EQ(map1[2], 25);

  ASSERT_EQ(map1.at(1), 10);
  ASSERT_THROW(map1.at(42), std::runtime_error);

  auto it = map1.find(2);
  ASSERT_NE(it, map1.end());
  map1.erase(it);
  ASSERT_EQ(map1.size(), 2);
  ASSERT_EQ(map1.find(2), map1.end());

  s21::map<int, int> map2 = {{4, 40}, {5, 50}};

  map1.merge(map2);

  ASSERT_EQ(map1.size(), 4);
  ASSERT_EQ(map1[4], 40);
  ASSERT_EQ(map1[5], 50);
}

TEST(MapTest, ContainsTest) {
  s21::map<int, std::string> map1;

  map1.insert({1, "one"});
  map1.insert({2, "two"});
  map1.insert({3, "three"});

  ASSERT_TRUE(map1.contains(1));
  ASSERT_TRUE(map1.contains(2));
  ASSERT_TRUE(map1.contains(3));

  ASSERT_FALSE(map1.contains(0));
  ASSERT_FALSE(map1.contains(4));
  ASSERT_FALSE(map1.contains(42));

  s21::map<std::string, int> map2;

  map2.insert({"one", 1});
  map2.insert({"two", 2});
  map2.insert({"three", 3});

  ASSERT_TRUE(map2.contains("one"));
  ASSERT_TRUE(map2.contains("two"));
  ASSERT_TRUE(map2.contains("three"));

  ASSERT_FALSE(map2.contains("zero"));
  ASSERT_FALSE(map2.contains("four"));
}

TEST(MapTest, InsertOrAssign) {
  s21::map<int, std::string> map;

  auto [it1, inserted1] = map.insert_or_assign(1, "one");
  auto [it2, inserted2] = map.insert_or_assign(2, "two");
  auto [it3, inserted3] = map.insert_or_assign(3, "three");

  EXPECT_TRUE(inserted1);
  EXPECT_TRUE(inserted2);
  EXPECT_TRUE(inserted3);

  EXPECT_EQ(it1->first, 1);
  EXPECT_EQ(it2->first, 2);
  EXPECT_EQ(it3->first, 3);
  EXPECT_EQ(it1->second, "one");
  EXPECT_EQ(it2->second, "two");
  EXPECT_EQ(it3->second, "three");

  auto [it4, inserted4] = map.insert_or_assign(3, "four");

  EXPECT_TRUE(inserted4);

  EXPECT_EQ(it4->first, 3);
  EXPECT_EQ(it4->second, "four");
}

TEST(MapTest, EraseWithTwoChildren) {
  s21::map<int, std::string> map;

  map.insert_or_assign(1, "one");
  map.insert_or_assign(2, "two");
  map.insert_or_assign(3, "three");
  map.insert_or_assign(4, "four");
  map.insert_or_assign(5, "five");

  auto it = map.find(2);
  ASSERT_NE(it, map.end());
  map.erase(it);

  EXPECT_EQ(map.size(), 4);
  EXPECT_EQ(map.find(2), map.end());

  EXPECT_EQ(map.find(1)->second, "one");
  EXPECT_EQ(map.find(3)->second, "three");
  EXPECT_EQ(map.find(4)->second, "four");
  EXPECT_EQ(map.find(5)->second, "five");
}

TEST(MapTest, Find) {
  s21::map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({2, "two"});
  map.insert({3, "three"});

  auto it1 = map.find(1);
  EXPECT_NE(it1, map.end());
  EXPECT_EQ((*it1).first, 1);
  EXPECT_EQ((*it1).second, "one");

  auto it2 = map.find(3);
  EXPECT_EQ((*it2).first, 3);
  EXPECT_EQ((*it2).second, "three");
}

TEST(MapTest, EraseWithLeftNull) {
  s21::map<int, std::string> map;

  map.insert({1, "one"});
  map.insert({2, "two"});

  auto it = map.find(1);
  ASSERT_NE(it, map.end());

  map.erase(it);

  EXPECT_EQ(map.size(), 1);
  EXPECT_EQ(map.find(1), map.end());
  EXPECT_EQ(map.find(2)->second, "two");
}

TEST(MapTest, EraseWithRightNull) {
  s21::map<int, std::string> map;

  map.insert({1, "one"});
  map.insert({2, "two"});

  auto it = map.find(2);

  map.erase(it);

  EXPECT_EQ(map.size(), 1);
  EXPECT_EQ(map.find(1)->second, "one");
}

TEST(MapTest, EraseWithBothChildren) {
  s21::map<int, std::string> map;

  map.insert({2, "two"});
  map.insert({1, "one"});
  map.insert({3, "three"});

  auto it = map.find(2);
  ASSERT_NE(it, map.end());

  map.erase(it);

  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map.find(2), map.end());
  EXPECT_EQ(map.find(1)->second, "one");
  EXPECT_EQ(map.find(3)->second, "three");
}

TEST(BinaryTreeIteratorTest, OperatorIncrementWithRightNotNull) {
  s21::map<int, std::string> map;
  map.insert({2, "two"});
  map.insert({1, "one"});
  map.insert({3, "three"});

  auto it = map.find(1);
  ASSERT_NE(it, map.end());

  ++it;

  EXPECT_EQ(it->first, 2);
  EXPECT_EQ(it->second, "two");
}

TEST(BinaryTreeIteratorTest, OperatorIncrementTraverseUpTree) {
  s21::map<int, std::string> map;
  map.insert({2, "two"});
  map.insert({1, "one"});
  map.insert({3, "three"});

  auto it = map.find(2);
  ASSERT_NE(it, map.end());

  ++it;

  EXPECT_EQ(it->first, 3);
  EXPECT_EQ(it->second, "three");
}

TEST(MapTest, MaxSize) {
  s21::map<int, std::string> map;
  map.insert({2, "two"});
  map.insert({1, "one"});
  map.insert({3, "three"});

  ASSERT_NE(map.max_size(), 0);
}

TEST(MapTest, ConstIter) {
  s21::map<int, std::string> map;
  map.insert({2, "two"});
  map.insert({1, "one"});
  map.insert({3, "three"});

  s21::map<int, std::string>::const_iterator it = map.cbegin();

  it++;

  ASSERT_EQ(it->second, "two");

  it = map.cend();

  ASSERT_EQ(it->second, "three");
}

TEST(BinaryTreeEraseTest, EraseNodeWithTwoChildren) {
  s21::map<int, int> my_map;
  my_map.insert_or_assign(3, 30);
  my_map.insert_or_assign(2, 20);
  my_map.insert_or_assign(5, 50);
  my_map.insert_or_assign(4, 40);
  my_map.insert_or_assign(6, 60);

  auto it = my_map.find(3);
  ASSERT_NE(it, my_map.end());

  my_map.erase(it);

  EXPECT_EQ(my_map.find(3), my_map.end());
}

TEST(BinaryTreeIteratorTest, DecrementationWithLeftChild) {
  s21::map<int, int> my_map;
  my_map.insert_or_assign(3, 30);
  my_map.insert_or_assign(2, 20);
  my_map.insert_or_assign(5, 50);

  auto it = my_map.find(5);

  --it;

  EXPECT_EQ(it->first, 3);
  EXPECT_EQ(it->second, 30);
}

TEST(BinaryTreeIteratorTest, DecrementWithLeftChild) {
  s21::map<int, int> my_map;
  my_map.insert_or_assign(5, 50);
  my_map.insert_or_assign(3, 30);
  my_map.insert_or_assign(7, 70);
  my_map.insert_or_assign(2, 20);
  my_map.insert_or_assign(4, 40);
  my_map.insert_or_assign(6, 60);

  auto it = my_map.find(7);

  --it;

  EXPECT_EQ(it->first, 6);
  EXPECT_EQ(it->second, 60);
}