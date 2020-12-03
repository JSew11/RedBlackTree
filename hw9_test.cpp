//----------------------------------------------------------------------
// Name: 
// File: hw9_test.cpp
// Date: Fall 2020
// Desc: Unit tests for the red-black tree collection implementation
//----------------------------------------------------------------------


#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "array_list.h"
#include "rbt_collection.h"


using namespace std;


// Helper function to print out a key-value collection
template<typename K,typename V>
void print(const Collection<K,V>& kv_list)
{
  cout << "{";
  ArrayList<K> the_keys;
  kv_list.keys(the_keys);
  for (size_t i = 0; i < the_keys.size(); ++i) {
    K key;
    V val;
    the_keys.get(i, key);
    kv_list.find(key, val);
    cout << key << ": " << val;
    if (i < the_keys.size() - 1)
      cout << ", ";
  }
  cout << "}";
}

// Helper function to check membership in a list
template<typename T>
bool member(const T& member_val, const List<T>& list)
{
  for (int i = 0; i < list.size(); ++i) {
    T list_val;
    list.get(i, list_val);
    if (list_val == member_val)
      return true;
  }
  return false;
}


// Test 1: Check add and size
TEST(RBTCollectionTest, CorrectSize) {
  RBTCollection<string,int> c;
  ASSERT_EQ(0, c.size());
  c.add("b", 10);
  ASSERT_EQ(1, c.size());
  c.add("a", 20);
  ASSERT_EQ(2, c.size());
  c.add("c", 20);
  ASSERT_EQ(3, c.size());
  ASSERT_EQ(true, c.valid_rbt());
}


// Test 2: Test add and find
TEST(RBTCollectionTest, SimpleFind) {
  RBTCollection<string,int> c;
  int v;
  ASSERT_EQ(false, c.find("b", v));
  c.add("b", 10);
  ASSERT_EQ(true, c.find("b", v));
  ASSERT_EQ(10, v);
  ASSERT_EQ(false, c.find("a", v));
  c.add("a", 20);
  ASSERT_EQ(true, c.find("a", v));
  ASSERT_EQ(20, v);
  ASSERT_EQ(false, c.find("c", v));
  c.add("c", 30);
  ASSERT_EQ(true, c.find("c", v));
  ASSERT_EQ(30, v);
}


// Test 3: Test add, size, and remove
TEST(RBTCollectionTest, SimpleRemove) {
  RBTCollection<string,int> c;
  c.add("b", 10);
  c.add("a", 20);
  c.add("c", 30);
  ASSERT_EQ(3, c.size());
  ASSERT_EQ(true, c.valid_rbt());  
  int v;
  // all three pairs in collection
  ASSERT_EQ(true, c.find("a", v));
  ASSERT_EQ(true, c.find("b", v));
  ASSERT_EQ(true, c.find("c", v));  
  // remove one
  c.remove("a");
  ASSERT_EQ(2, c.size());
  ASSERT_EQ(false, c.find("a", v));
  ASSERT_EQ(true, c.find("b", v));
  ASSERT_EQ(true, c.find("c", v));  
  ASSERT_EQ(true, c.valid_rbt());  
  // remove another
  c.remove("b");
  ASSERT_EQ(1, c.size());
  ASSERT_EQ(false, c.find("a", v));
  ASSERT_EQ(false, c.find("b", v));
  ASSERT_EQ(true, c.find("c", v));
  ASSERT_EQ(true, c.valid_rbt());  
  // remove last one
  c.remove("c");
  ASSERT_EQ(0, c.size());
  ASSERT_EQ(false, c.find("a", v));
  ASSERT_EQ(false, c.find("b", v));
  ASSERT_EQ(false, c.find("c", v));
  ASSERT_EQ(true, c.valid_rbt());
}


// Test 4: Test add, size, and find range
TEST(RBTCollectionTest, SimpleRange) {
  RBTCollection<string,int> c;
  c.add("e", 50);
  c.add("c", 30);
  c.add("d", 40);
  c.add("f", 60);
  c.add("b", 20);
  // search for proper sublist of keys
  ArrayList<string> s1;
  c.find("c", "e", s1);
  ASSERT_EQ(3, s1.size());
  ASSERT_EQ(true, member(string("c"), s1));
  ASSERT_EQ(true, member(string("d"), s1));
  ASSERT_EQ(true, member(string("e"), s1));
  // search for overlapping prefix
  ArrayList<string> s2;
  c.find("a", "c", s2);
  ASSERT_EQ(2, s2.size());
  ASSERT_EQ(true, member(string("b"), s2));
  ASSERT_EQ(true, member(string("c"), s2));
  // search for overlapping suffix
  ArrayList<string> s3;
  c.find("e", "g", s3);
  ASSERT_EQ(2, s3.size());
  ASSERT_EQ(true, member(string("e"), s3));
  ASSERT_EQ(true, member(string("f"), s3));
  // search for empty sublist of keys
  ArrayList<string> s4;
  c.find("f", "b", s4);
  ASSERT_EQ(0, s4.size());
}


// Test 5: Test add and sort
TEST(RBTCollectionTest, SimpleSort) {
  RBTCollection<string,int> c;
  c.add("a", 10);
  c.add("e", 50);
  c.add("c", 30);
  c.add("b", 20);
  c.add("d", 40);
  ArrayList<string> sorted_keys;
  c.sort(sorted_keys);
  ASSERT_EQ(5, sorted_keys.size());
  // check if in sorted order
  for (size_t i = 1; i < sorted_keys.size(); ++i) {
    string k1;
    string k2;
    ASSERT_EQ(true, sorted_keys.get(i-1, k1));
    ASSERT_EQ(true, sorted_keys.get(i, k2));
    ASSERT_LT(k1, k2);
  }
}


// Test 6: copy and assignment operator
TEST(RBTCollectionTest, CopyAndAssignment) {
  RBTCollection<string,int> c1;
  c1.add("b", 20);
  c1.add("a", 10);
  c1.add("c", 30);
  RBTCollection<string,int> c2(c1);
  ASSERT_EQ(c1.size(), c2.size());
  c2.add("e", 50);
  ASSERT_NE(c1.size(), c2.size());
  c1 = c2;
  ASSERT_EQ(c1.size(), c2.size());
  c1 = c1;
  ASSERT_EQ(c1.size(), c2.size());
  RBTCollection<string,int> c3;
  c3.add("d", 40);
  c3 = c2 = c1;
  ASSERT_EQ(c3.size(), c2.size());
  ASSERT_EQ(c2.size(), c1.size());
  string keys[4] = {"a", "b", "c", "e"};
  for (int i = 0; i < c1.size(); ++i) {
    int v1, v2, v3;
    ASSERT_EQ(true, c1.find(keys[i], v1));
    ASSERT_EQ(true, c2.find(keys[i], v2));
    ASSERT_EQ(true, c3.find(keys[i], v3));
    ASSERT_EQ(v1, v2);
    ASSERT_EQ(v2, v3);
  }
}


// Test 7: remove node with two children
TEST(RBTCollectionTest, RemoveNodeWithTwoChildrenRightCases) {
  RBTCollection<string,int> c;
  c.add("b", 20);
  c.add("a", 10);
  c.add("d", 40);
  c.add("c", 30);
  c.add("e", 50);
  c.add("f", 60);
  ASSERT_EQ(6, c.size());
  // remove d (where right child has no left child)
  c.remove("d");
  ASSERT_EQ(5, c.size());
  ASSERT_EQ(true, c.valid_rbt());
  ASSERT_EQ(3, c.height());
  // remove b (with right child having a left child)
  c.remove("b");
  ASSERT_EQ(4, c.size());
  ASSERT_EQ(3, c.height());
  ASSERT_EQ(true, c.valid_rbt());
}

// Test 8: remove of node with two children, with left
TEST(RBTCollectionTest, RemoveNodeWithTwoChildrenLeftCases) {
  RBTCollection<string,int> c;
  c.add("e", 50);
  c.add("c", 30);
  c.add("f", 60);
  c.add("b", 20);
  c.add("d", 40);
  c.add("a", 10);
  ASSERT_EQ(6, c.size());
  // remove e (where right child has no left child)
  c.remove("e");
  ASSERT_EQ(5, c.size());
  ASSERT_EQ(true, c.valid_rbt());
  ASSERT_EQ(3, c.height());
  // remove c (with right child having a left child)
  c.remove("c");
  ASSERT_EQ(4, c.size());
  ASSERT_EQ(3, c.height());
  ASSERT_EQ(true, c.valid_rbt());
}


// Test 9
TEST(RBTCollectionTest, AddRebalanceCheckLeftCases) {
  RBTCollection<string,int> c1;
  c1.add("h", 10);
  c1.add("g", 15);
  c1.add("f", 20);
  ASSERT_EQ(2, c1.height());
  c1.add("e", 25);
  c1.add("d", 30);
  ASSERT_EQ(3, c1.height());  
  c1.add("c", 40);
  ASSERT_EQ(4, c1.height());    
  c1.add("a", 35);
  ASSERT_EQ(4, c1.height());    
  c1.add("b", 45);
  ASSERT_EQ(4, c1.height());
  ASSERT_EQ(true, c1.valid_rbt());
}


// Test 10
TEST(BasicListTest, RemoveRebalanceChecksLeftCases) {
  // case 3: "outside"
  RBTCollection<string,int> c1;
  c1.add("b", 20);
  c1.add("a", 10);
  c1.add("c", 30);
  c1.add("d", 40);
  ASSERT_EQ(4, c1.size());
  ASSERT_EQ(3, c1.height());
  c1.remove("a");
  ASSERT_EQ(3, c1.size());
  ASSERT_EQ(2, c1.height());
  ASSERT_EQ(true, c1.valid_rbt());
  // case 4: "inside"
  RBTCollection<string,int> c2;
  c2.add("b", 20);
  c2.add("a", 10);
  c2.add("d", 40);
  c2.add("c", 30);
  ASSERT_EQ(4, c2.size());
  ASSERT_EQ(3, c2.height());
  c2.remove("a");
  ASSERT_EQ(3, c2.size());
  ASSERT_EQ(2, c2.height());
  ASSERT_EQ(true, c2.valid_rbt());
  // case 1: red child not along path
  RBTCollection<string,int> c3;
  c3.add("c", 30);
  c3.add("b", 20);
  c3.add("e", 50);
  c3.add("a", 10);
  c3.add("d", 40);
  c3.add("f", 60);
  c3.add("g", 70);
  ASSERT_EQ(7, c3.size());
  ASSERT_EQ(4, c3.height());
  c3.remove("b");
  ASSERT_EQ(6, c3.size());
  ASSERT_EQ(3, c3.height());
  ASSERT_EQ(true, c3.valid_rbt());
  // case 2: color flip
  RBTCollection<string,int> c4;
  for (int i = 0; i <= 17; ++i) {
    string s = "";
    s += (char)(i + 97);
    c4.add(s, i+10);
  }
  ASSERT_EQ(18, c4.size());
  ASSERT_EQ(6, c4.height());
  c4.remove("a");
  ASSERT_EQ(17, c4.size());
  ASSERT_EQ(6, c4.height());  
  ASSERT_EQ(true, c4.valid_rbt());  
}


//----------------------------------------------------------------------
// TODO:
//
//   (1) Add the mirror image case for AddRebalanceCheckLeftCases in
//       a new unit test called AddRebalanceCheckRightCases
//
//   (2) Add the mirror image case for RemoveRebalanceChecksLeftCases in
//       a new unit test called RemoveRebalanceChecksRightCases
//
//   (3) Add at least 3 non-trival tests below for your RBT Collection
//       class. Be sure to provide comments describing the detailed
//       purpose of each the test.
//
//----------------------------------------------------------------------


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

