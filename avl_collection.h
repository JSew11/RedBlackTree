//----------------------------------------------------------------------
// FILE: avl_collection.h
// NAME: Joshua Seward
// DATE: November 27, 2020
// DESC: Implements a version of the collection class that implements an
// AVL tree
//----------------------------------------------------------------------

#ifndef AVL_COLLECTION_H
#define AVL_COLLECTION_H

#include "collection.h"
#include "array_list.h"

template<typename K, typename V>
class AVLCollection : public Collection<K,V>
{
  public:
    AVLCollection();
    AVLCollection(const AVLCollection<K,V>& rhs);
    ~AVLCollection();
    AVLCollection& operator=(const AVLCollection<K,V>& rhs);

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
      int height;
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
    // for add
    Node* add(Node* subtree_root, const K& key, const V& val);
    // for remove
    Node* remove(Node* subtree_root, const K& key);
    // for find-range
    void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
    // for keys
    void keys(const Node* subtree_root, ArrayList<K>& keys) const;
    // to help with right rotations
    Node* rotate_right(Node* k2);
    // to help with left rotations
    Node* rotate_left(Node* k2);
    // to rebalance the AVL tree
    Node* rebalance(Node* subtree_root);
};

template<typename K, typename V>
AVLCollection<K,V>::AVLCollection()
  : node_count(0)
{
  root = nullptr;
}

template<typename K, typename V>
AVLCollection<K,V>::AVLCollection(const AVLCollection<K,V>& rhs)
  : node_count(0), root(nullptr)
{
  // defer to the assignment operator
  *this = rhs;
}

template<typename K, typename V>
AVLCollection<K,V>::~AVLCollection()
{
  make_empty(root);
  root = nullptr;
}

template<typename K, typename V>
AVLCollection<K,V>& AVLCollection<K,V>::operator=(const AVLCollection<K,V>& rhs)
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
void AVLCollection<K,V>::add(const K& key, const V& val)
{
  root = add(root, key, val);
  ++node_count; // increment node_count
}

//  Function: remove()
//  Description: Removes the requested key-value pair from the tree
//  Inputs: The key of the pair to be removed
//  Outputs: None
template<typename K, typename V>
void AVLCollection<K,V>::remove(const K& key)
{
  root = remove(root, key);
  --node_count; // decrease node_count
}

//  Function: find()
//  Description: Finds the value associated with the given key, if it exists in the tree
//  Inputs: Key to be found
//  Outputs: Value associated with the key, whether or not the pair exists in the tree
template<typename K, typename V>
bool AVLCollection<K,V>::find(const K& search_key, V& return_val) const
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
void AVLCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  find(root, k1, k2, keys);
}

//  Function: keys()
//  Description: Returns a list of all the keys in the tree (using in-order traversal)
//  Inputs: None
//  Outputs: List of all keys in the collection
template<typename K, typename V>
void AVLCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  keys(root, all_keys);
}

//  Function: sort()
//  Description: Returns a list of all the keys in sorted order
//  Inputs: None
//  Outputs: A list of the keys in the system in sorted order
template<typename K, typename V>
void AVLCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
}

//  Function: size()
//  Description: Returns the number of key-value pairs of the tree
//  Inputs: None
//  Outputs: The number of key-value pairs in the tree
template<typename K, typename V>
size_t AVLCollection<K,V>::size() const
{
  return node_count;
}

//  Function: height()
//  Description: Returns the height of the tree
//  Inputs: None
//  Outputs: The height of the tree (counting of nodes)
template<typename K, typename V>
size_t AVLCollection<K,V>::height() const
{
  if(size() > 0)  return root->height;
  else return 0;
}

// helper function for destructor
template<typename K, typename V>
void AVLCollection<K,V>::make_empty(Node* subtree_root)
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
void AVLCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
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

// helper function for add
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::add(Node* subtree_root, const K& key, const V& val)
{
  if(!subtree_root){
    // if the subtree root is null, add a new node containing key and val
    Node* new_node = new Node;
    new_node->key = key;
    new_node->value = val;
    new_node->height = 1;
    new_node->left = nullptr;
    new_node->right = nullptr;
    subtree_root = new_node;
  }
  else{
    if(key <= subtree_root->key){ // key should be in the left subtree
      subtree_root->left = add(subtree_root->left, key, val);
    }
    else{ // key should be in the right subtree
      subtree_root->right = add(subtree_root->right, key, val);
    }
    // adjust (calculate) heights
    Node* lptr = subtree_root->left;
    Node* rptr = subtree_root->right;
    if(lptr && !rptr){
      subtree_root->height = lptr->height + 1;
    }
    else if(rptr && !lptr){
      subtree_root->height = rptr->height + 1;
    }
    else if(lptr && rptr){
      if(lptr->height > rptr->height){
        subtree_root->height = lptr->height + 1;
      }
      else{
        subtree_root->height = rptr->height + 1;
      }
    }
  }
  return rebalance(subtree_root); // calculate balance factors and rotate (if necessary)
}

// helper function for remove
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::remove(Node* subtree_root, const K& key)
{
  if(!subtree_root) return nullptr;
  // traverse the left subtree if the key is less than the key of subtree_root
  if(subtree_root && key < subtree_root->key){
    subtree_root->left = remove(subtree_root->left, key);
  }
 
  // traverse the right subtree if the key is greater than the key of subtree_root
  else if(subtree_root && key > subtree_root->key){
    subtree_root->right = remove(subtree_root->right, key);
  }

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
      else if(subtree_root->right){
        Node* tmp = subtree_root->right;
        delete subtree_root;
        // replace subtree_root with subtree_root's right subtree
        subtree_root = tmp;
      }
      else{
        delete subtree_root;
        return nullptr;
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
        subtree_root->right = remove(subtree_root->right, successor->key);
      }
      // special case (if subtree_root's right subtree contains only one Node)
      else{
        // in order successor is the only Node in the right subtree
        subtree_root->right = successor->right;

        // copying successor's key and value into subtree_root
        subtree_root->key = successor->key;
        subtree_root->value = successor->value;

        // redirect and delete subtree_root
        subtree_root->right = remove(subtree_root->right, successor->key);
      }
    }
  }
  // adjust (calculate) heights
  Node* lptr = subtree_root->left;
  Node* rptr = subtree_root->right;
  if(lptr && !rptr){
    subtree_root->height = lptr->height + 1;
  }
  else if(rptr && !lptr){
   subtree_root->height = rptr->height + 1;
  }
  else if(lptr && rptr){
    if(lptr->height > rptr->height){
      subtree_root->height = lptr->height + 1;
    }
    else{
      subtree_root->height = rptr->height + 1;
    }
  }
  else{
    subtree_root->height = 1;
  }
  return rebalance(subtree_root);
}

// helper function for find-range
template<typename K, typename V>
void AVLCollection<K,V>::find(const Node* subtree_root, 
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
void AVLCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
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

// helper function for right rotations
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rotate_right(Node* k2)
{
  Node* k1 = k2->left;  // k1 is k2's left child
  k2->left = k1->right; // point k2's left to k1's right subtree
  k1->right = k2; // move k2 to be k1's right child
  return k1;
}

// helper function for left rotations
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rotate_left(Node* k2)
{
  Node* k1 = k2->right;  // k1 is k2's right child
  k2->right = k1->left; // point k2's right to k1's left subtree
  k1->left = k2; // move k2 to be k1's right child
  return k1;
}

// helper function for rebalancing the tree
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rebalance(Node* subtree_root)
{
  if(!subtree_root) return subtree_root;  // special case if there is nothing in the tree

  Node* lptr = subtree_root->left;  // pointer to the left subtree of subtree_root
  Node* rptr = subtree_root->right; // pointer to the right subtree of subtree_root

  // left child but no right child
  if(lptr && !rptr && lptr->height > 1){  
    // check if child has a right or left child
    if(lptr->left && !lptr->right){ // left child but no right child
      // "left-left" case - single right rotation
      subtree_root = rotate_right(subtree_root); // rotate right at subtree_root
      // update heights
      subtree_root->right->height -= 2;
    }
    else if(lptr->right && !lptr->left){  // right child but no left child
      // "left-right" case - double rotation
      subtree_root->left = rotate_left(lptr); // rotate left at lptr
      subtree_root = rotate_right(subtree_root);  // rotate right at subtree_root
      // update heights
      subtree_root->height += 1;
      subtree_root->left->height -= 1;
      subtree_root->right->height -= 2; 
    }
  }
   //right child but no left child
  else if(rptr && !lptr && rptr->height > 1){ 
    // check if child has right or left child
    if(rptr->left && !rptr->right){ // left child but no right child
      // "right-left" case - double rotation
      subtree_root->right = rotate_right(rptr); // rotate right at rptr
      subtree_root = rotate_left(subtree_root); // rotate left at subtree_root
      // update heights
      subtree_root->height += 1;
      subtree_root->right->height -= 1;
      subtree_root->left->height -= 2; 

    }
    else if(rptr->right && !rptr->left){  // right child but no left child
      // "right-right" case - single rotation
      subtree_root = rotate_left(subtree_root); // rotate left at subtree_root
      // update heights
      subtree_root->left->height -= 2;
    }
  }
  // cases for both left and right
  else if(lptr && rptr){
    // tree is left leaning
    if(lptr->height - rptr->height > 1){
      if(lptr->left->height - lptr->right->height > 0){
        // "left-left" case - single rotation
        subtree_root = rotate_right(subtree_root);  // rotate right at subtree_root
        // update heights
        subtree_root->right->height -= 2;
      }
      else if(lptr->left->height - lptr->right->height < 0){
        // "left-right" case - double rotation
        subtree_root->left = rotate_left(subtree_root->left); // rotate left at lptr
        subtree_root = rotate_right(subtree_root);  // rotate right at subtree_root
        // update heights
        subtree_root->height += 1;
        subtree_root->left->height -= 1;
        subtree_root->right->height -= 2; 
      }
    }
    // tree is right leaning
    else if(lptr->height - rptr->height < -1){
      if(rptr->left->height - rptr->right->height > 0){
        // "right-left" case - double rotation
        subtree_root->right = rotate_right(subtree_root->right); // rotate right at rptr
        subtree_root = rotate_left(subtree_root); // rotate left at subtree_root
        // update heights
        subtree_root->height += 1;
        subtree_root->right->height -=1;
        subtree_root->left->height -= 2;
      }
      else if(rptr->left->height - rptr->right->height < 0){
        // "right-right" case - single left rotation
        subtree_root = rotate_left(subtree_root); // rotate left at subtree_root
        // update heights
        subtree_root->left->height -= 2;
      }
    }
  }
  
  return subtree_root;
}

#endif