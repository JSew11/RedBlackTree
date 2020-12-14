//----------------------------------------------------------------------
// FILE: bst_collection.h
// NAME: Joshua Seward
// DATE: November 9, 2020
// DESC: Implements a version of the collection class that implements a
// binary search tree
//----------------------------------------------------------------------

#ifndef BST_COLLECTION_H
#define BST_COLLECTION_H

#include "collection.h"
#include "array_list.h"

template<typename K, typename V>
class BSTCollection : public Collection<K,V>
{
  public:
    BSTCollection();
    BSTCollection(const BSTCollection<K,V>& rhs);
    ~BSTCollection();
    BSTCollection& operator=(const BSTCollection<K,V>& rhs);

    void add(const K& key, const V& val);
    void remove(const K& key);
    bool find(const K& search_key, V& return_val) const;
    void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
    void keys(ArrayList<K>& all_keys) const;
    void sort(ArrayList<K>& all_keys_sorted) const;
    size_t size() const;
    size_t height() const;

  private:
    struct Node{
      K key;
      V value;
      Node* left;
      Node* right;
    };

    Node* root; // root of the tree
    size_t node_count;  // number of Nodes in the tree

    // helper functions
    // for destructor
    void make_empty(Node* subtree_root);
    // for copy constructor
    void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root);
    // for remove
    Node* remove(Node* subtree_root, const K& key);
    // for find-range
    void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
    // for keys
    void keys(const Node* subtree_root, ArrayList<K>& keys) const;
    // for height
    size_t height(const Node* subtree_root) const;
};

template<typename K, typename V>
BSTCollection<K,V>::BSTCollection()
  : node_count(0)
{
  root = nullptr;
}

template<typename K, typename V>
BSTCollection<K,V>::BSTCollection(const BSTCollection<K,V>& rhs)
  : node_count(0), root(nullptr)
{
  // defer to the assignment operator
  *this = rhs;
}

template<typename K, typename V>
BSTCollection<K,V>::~BSTCollection()
{
  make_empty(root);
  root = nullptr;
}

template<typename K, typename V>
BSTCollection<K,V>& BSTCollection<K,V>::operator=(const BSTCollection<K,V>& rhs)
{
  if(this != &rhs){
    make_empty(root); // delete the lhs tree
    root = nullptr;
    root = new Node;
    if(rhs.root){
      // copy rhs root into lhs root
      root->key = rhs.root->key;
      root->value = rhs.root->value;
      root->left = nullptr;
      root->right = nullptr;
      node_count++; // increment node_count variable
      copy(root, rhs.root); // copy the rhs tree into the lhs tree
    }
  }
  return *this;
}

//  Function: add()
//  Description: Adds a new key-value pair to the tree in the correct hashed location
//  Inputs: Key and value to be added to the tree
//  Outputs: None
template<typename K, typename V>
void BSTCollection<K,V>::add(const K& key, const V& val)
{
  // create a new Node to hold key and val
  Node* tmp = new Node;
  tmp->left = nullptr;
  tmp->right = nullptr;
  tmp->key = key;
  tmp->value = val;

  // special case if there is nothing in the tree
  if(node_count == 0){
    root = tmp;
  }
  else{
    // find the location to insert key and val
    Node* cur = root; // Node pointer to keep track of current location in the tree
    Node* prev; // Node pointer to keep track of Node before cur
    while(cur){
      if(key <= cur->key){ // key at cur is less than the key to be added
        prev = cur; // set the previous Node pointer to cur
        cur = cur->left;  // iterate through cur's left subtree
      }
      else{ // key at cur is greater than key to be added
        prev = cur; // set the previous Node pointer to cur
        cur = cur->right; // iterate through cur's right subtree
      }
    }

    //insert key and val
    if(tmp->key <= prev->key)  // key is less than or equal to the key at cur
      prev->left = tmp;  // add the new Node to the left of cur
    else  // key is greater than the key at cur
      prev->right = tmp; // add the new Node to the right of cur
  }
  ++node_count; // increment node_count
}

//  Function: remove()
//  Description: Removes the requested key-value pair from the tree
//  Inputs: The key of the pair to be removed
//  Outputs: None
template<typename K, typename V>
void BSTCollection<K,V>::remove(const K& key)
{
  if(size() > 0)
  root = remove(root, key);
}

//  Function: find()
//  Description: Finds the value associated with the given key, if it exists in the tree
//  Inputs: Key to be found
//  Outputs: Value associated with the key, whether or not the pair exists in the tree
template<typename K, typename V>
bool BSTCollection<K,V>::find(const K& search_key, V& return_val) const
{
  // special case if the tree is empty
  if(node_count == 0) return false;

  Node* cur = root; // Node pointer to keep track of current location in the tree
  while(cur){
    if(search_key == cur->key){ // search_key is found
      return_val = cur->value;  // return the value at cur
      return true;  // return true since you found search_key
    }
    else if(search_key < cur->key) // key at cur is less than search_key
      cur = cur->left;  // iterate through cur's left subtree
    else  // key at cur is greater than search_key
      cur = cur->right; // iterate through cur's right subtree
  }
  return false; // return false since you did not find search_key
}

//  Function: find()
//  Description: Finds and returns all keys between the given k1 and k2 keys
//  Inputs: Given key "limits"
//  Outputs: All keys between the given "limits"
template<typename K, typename V>
void BSTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  find(root, k1, k2, keys);
}

//  Function: keys()
//  Description: Returns a list of all the keys in the tree (using in-order traversal)
//  Inputs: None
//  Outputs: List of all keys in the collection
template<typename K, typename V>
void BSTCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  keys(root, all_keys);
}

//  Function: sort()
//  Description: Returns a list of all the keys in sorted order
//  Inputs: None
//  Outputs: A list of the keys in the system in sorted order
template<typename K, typename V>
void BSTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
}

//  Function: size()
//  Description: Returns the number of key-value pairs of the tree
//  Inputs: None
//  Outputs: The number of key-value pairs in the tree
template<typename K, typename V>
size_t BSTCollection<K,V>::size() const
{
  return node_count;
}

//  Function: height()
//  Description: Returns the height of the tree
//  Inputs: None
//  Outputs: The height of the tree (counting of nodes)
template<typename K, typename V>
size_t BSTCollection<K,V>::height() const
{
  return height(root);
}

// helper function for destructor
template<typename K, typename V>
void BSTCollection<K,V>::make_empty(Node* subtree_root)
{
  // if the subtree root is null, return (base case)
  if(!subtree_root){
    return;
  }
  // delete the left and right subtrees of the current node
  if(subtree_root->left){
    make_empty(subtree_root->left);
    subtree_root->left = nullptr;
  }
  if(subtree_root->right){
    make_empty(subtree_root->right);
    subtree_root->right = nullptr;
  }
  // delete the current node
  delete subtree_root;
  node_count--;
}

// helper function for copy constructor
template<typename K, typename V>
void BSTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
{
  // base case (if the root is nullptr)
  if(!rhs_subtree_root) return;
  // copy left subtree over if it exists
  if(rhs_subtree_root->left){
    // new Node to copy to left subtree of lhs_subtree_root
    Node* tmp = new Node;
    tmp->key = rhs_subtree_root->left->key;
    tmp->value = rhs_subtree_root->left->value;
    tmp->left = nullptr;
    tmp->right = nullptr;

    // add new Node to the left subtree of lhs_subtree_root
    lhs_subtree_root->left = tmp;
    node_count++; // increment node_count variable

    // copy the rest of the left subtree of rhs_subtree_root
    copy(lhs_subtree_root->left, rhs_subtree_root->left);
  }

  // copy right subtree over if it exists
  if(rhs_subtree_root->right){
    // new Node to copy to right subtree of lhs_subtree_root
    Node* tmp = new Node;
    tmp->key = rhs_subtree_root->right->key;
    tmp->value = rhs_subtree_root->right->value;
    tmp->left = nullptr;
    tmp->right = nullptr;

    // add new Node to the right subtree of lhs_subtree_root
    lhs_subtree_root->right = tmp;
    node_count++; // increment node_count variable

    // copy the rest of the right subtree of rhs_subtree_root
    copy(lhs_subtree_root->right, rhs_subtree_root->right);
  }
}

// helper function for remove
template<typename K, typename V>
typename BSTCollection<K,V>::Node*
BSTCollection<K,V>::remove(Node* subtree_root, const K& key)
{
  // traverse the left subtree if the key is less than the key of subtree_root
  if(subtree_root && key < subtree_root->key)
    subtree_root->left = remove(subtree_root->left, key);
 
  // traverse the right subtree if the key is greater than the key of subtree_root
  else if(subtree_root && key > subtree_root->key)
    subtree_root->right = remove(subtree_root->right, key);
  
  // remove subtree_root (since you have found the Node containing key)
  else if(subtree_root){
    // case 1 - 0 or 1 child
    if(!subtree_root->left || !subtree_root->right){
      if(subtree_root->left){
        Node* tmp = subtree_root->left;
        delete subtree_root;
        // replace subtree_root with subtree_root's left subtree
        subtree_root = tmp;
      }
      else{
        Node* tmp = subtree_root->right;
        delete subtree_root;
        // replace subtree_root with subtree_root's right subtree
        subtree_root = tmp;
      }
    }

    // case 2 - 2 children
    else{
      // finding in order successor
      Node* successor = subtree_root;
      Node* prev = successor;
      successor = subtree_root->right;
      // normal case
      if(successor->left){
        while(successor->left){
          prev = successor;
          successor = successor->left;
        }

        // copying successor's key and value into subtree_root
        subtree_root->key = successor->key;
        subtree_root->value = successor->value;

        // delete successor
        prev->left = successor->right; // point prev to successor's right subtree
        delete successor;
      }
      // special case (if subtree_root's right subtree contains only one Node)
      else{
        // in order successor is the only Node in the right subtree
        subtree_root->right = successor->right;

        // copying successor's key and value into subtree_root
        subtree_root->key = successor->key;
        subtree_root->value = successor->value;

        // redirect and delete subtree_root
        delete successor;
      }
    }
    --node_count; // decrease node count variable
  }
  return subtree_root;
}

// helper function for find-range
template<typename K, typename V>
void BSTCollection<K,V>::find(const Node* subtree_root, 
const K& k1, const K& k2, ArrayList<K>& keys) const
{
  // check if you have reached the end of a path (base case)
  if(!subtree_root) return;

  // traverse through the tree
  if(subtree_root->key < k1){
    find(subtree_root->right, k1, k2, keys);  // traverse right
  }
  else if(subtree_root->key >= k1 && subtree_root->key <= k2){
    keys.add(subtree_root->key);  // if the key is within range, add it to the array list
    find(subtree_root->right, k1, k2, keys);  // traverse right
    find(subtree_root->left, k1, k2, keys); // traverse left
  }
  else{
    find(subtree_root->left, k1, k2, keys); // traverse left
  }
}

// helper function for keys
template<typename K, typename V>
void BSTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
{
  // check if you have reached the end of a path (base case)
  if(!subtree_root) return;
  // traverse left subtree
  keys(subtree_root->left, all_keys);
  // visit subtree_root
  all_keys.add(subtree_root->key);
  // traverse right subtree
  keys(subtree_root->right, all_keys);
}

// helper function for height
template<typename K, typename V>
size_t BSTCollection<K,V>::height(const Node* subtree_root) const
{
  if(!subtree_root) return 0; // return if the subtree_root is nullptr (base case)
  size_t left_height = 0;  // variable to keep track of the height of the left subtree
  size_t right_height = 0; // variable to keep track of the height of the right subtree
  // get the height of the left subtree if it exists
  if(subtree_root->left){
    left_height = height(subtree_root->left);
  }
  if(subtree_root->right){
    right_height = height(subtree_root->right);
  }
  // check which height is larger and return that height +1
  if(left_height > right_height){
    return left_height+1;
  }
  else return right_height +1;
  
}

#endif