//----------------------------------------------------------------------
// Name: 
// File: rbt_collection.h
// Date: Fall, 2020
// Desc: A Key-Value Collection implementation using a Red-Black tree.
//----------------------------------------------------------------------


#ifndef RBT_COLLECTION_H
#define RBT_COLLECTION_H


#include "string.h"
#include "collection.h"
#include "array_list.h"


template<typename K, typename V>
class RBTCollection : public Collection<K,V>
{
public:

  // create an empty collection
  RBTCollection();
  
  // copy constructor
  RBTCollection(const RBTCollection<K,V>& rhs);

  // assignment operator
  RBTCollection<K,V>& operator=(const RBTCollection<K,V>& rhs);

  // delete collection
  ~RBTCollection();
  
  // add a new key-value pair into the collection 
  void add(const K& a_key, const V& a_val);

  // remove a key-value pair from the collectiona
  void remove(const K& a_key);

  // find and return the value associated with the key
  bool find(const K& search_key, V& the_val) const;

  // find and return each key >= k1 and <= k2 
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  
  // return all of the keys in the collection 
  void keys(ArrayList<K>& all_keys) const;

  // return all of the keys in ascending (sorted) order
  void sort(ArrayList<K>& all_keys_sorted) const;

  // return the number of key-value pairs in the collection
  size_t size() const;

  // return the height of the tree
  size_t height() const;

  // for testing:

  // check if tree satisfies the red-black tree constraints
  bool valid_rbt() const;
  
  // pretty-print the red-black tree (with heights)
  void print() const;

  
private:
  
  // RBT node structure
  enum color_t {RED, BLACK};
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
    Node* parent;
    color_t color;
  };

  // root node
  Node* root;

  // number of k-v pairs stored in the collection
  size_t node_count;

  // helper to empty entire hash table
  void make_empty(Node* subtree_root);

  // copy helper
  void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root); 
    
  // helper to recursively find range of keys
  void find(const Node* subtree_root, const K& k1, const K& k2,
            ArrayList<K>& keys) const;

  // helper to build sorted list of keys (used by keys and sort)
  void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;

  // rotate right helper
  void rotate_right(Node* k2);

  // rotate left helper
  void rotate_left(Node* k2);

  // restore red-black constraints in add
  void add_rebalance(Node* x);

  // restore red-black constraints in remove
  void remove_rebalance(Node* x, bool going_right);
  
  // height helper
  size_t height(Node* subtree_root) const;
  
  // ------------
  // for testing:
  // ------------
  
  // validate helper
  bool valid_rbt(Node* subtree_root) const;

  // validate helper
  size_t black_node_height(Node* subtree_root) const;
  
  // recursive pretty-print helper
  void print_tree(std::string indent, Node* subtree_root) const;
};


// TODO: Finish the above functions below




//----------------------------------------------------------------------
// Provided Helper Functions:
//----------------------------------------------------------------------

template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt() const
{
  return !root or (root->color == BLACK and valid_rbt(root));
}


template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt(Node* subtree_root) const
{
  if (!subtree_root)
    return true;
  color_t rc = subtree_root->color;
  color_t lcc = subtree_root->left ? subtree_root->left->color : BLACK;
  color_t rcc = subtree_root->right ? subtree_root->right->color : BLACK;  
  size_t lbh = black_node_height(subtree_root->left);
  size_t rbh = black_node_height(subtree_root->right);
  bool lv = valid_rbt(subtree_root->left);
  bool rv = valid_rbt(subtree_root->right);
  // check equal black node heights, no two consecutive red nodes, and
  // left and right are valid RBTs
  return (lbh == rbh) and (rc != RED or (lcc != RED and rcc != RED)) and lv and rv;
}


template<typename K, typename V>
size_t RBTCollection<K,V>::black_node_height(Node* subtree_root) const
{
  if (!subtree_root)
    return 1;
  size_t hl = black_node_height(subtree_root->left);
  size_t hr = black_node_height(subtree_root->right);
  size_t h = hl > hr ? hl : hr;
  if (subtree_root->color == BLACK)
    return 1 + h;
  else
    return h;
}


template<typename K, typename V>
void RBTCollection<K,V>::print() const
{
  print_tree("", root);
}


template<typename K, typename V>
void RBTCollection<K,V>::print_tree(std::string indent, Node* subtree_root) const
{
  if (!subtree_root)
    return;
  std::string color = "[BLACK]";
  if (subtree_root->color == RED)
    color = "[RED]";
  std::cout << indent << subtree_root->key << " "
	    << color << " (h="
	    << height(subtree_root) << ")" << std::endl;
  print_tree(indent + "  ", subtree_root->left);
  print_tree(indent + "  ", subtree_root->right);
}


#endif
