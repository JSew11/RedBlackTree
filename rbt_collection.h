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

template<typename K, typename V>
RBTCollection<K,V>::RBTCollection()
  : node_count(0)
{
  root = nullptr;
}

template<typename K, typename V>
RBTCollection<K,V>::RBTCollection(const RBTCollection<K,V>& rhs)
  : node_count(0), root(nullptr)
{
  // defer to the assignment operator
  *this = rhs;
}

template<typename K, typename V>
RBTCollection<K,V>::~RBTCollection()
{
  make_empty(root);
  root = nullptr;
}

template<typename K, typename V>
RBTCollection<K,V>& RBTCollection<K,V>::operator=(const RBTCollection<K,V>& rhs)
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
//  Description: Adds a new key-value pair to the tree
//  Inputs: Key and value to be added to the tree
//  Outputs: None
template<typename K, typename V>
void RBTCollection<K,V>::add(const K& a_key, const V& a_val){
  // create the node to be added to the tree
  Node* n = new Node;
  n->key = a_key;
  n->value = a_val;
  n->color = RED;
  n->left = nullptr;
  n->right = nullptr;
  n->parent = nullptr;

  Node* x = root; // pointer to the current node
  Node* p = nullptr;  // pointer to the parent of the current node
  // iterate through the tree and rebalance
  while(x){
    add_rebalance(x); // rebalance the RBT
    p = x;  // move the parent node forward
    // move the current node ahead through the tree
    if(a_key <= x->key)
      x = x->left;
    else
      x = x->right;
  }
  // add the new node at the correct location in the tree
  if(!p)  // special case - add to an empty list
    root = n;
  else if(a_key <= p->key){
    p->left = n;
    n->parent = p;
    add_rebalance(n);
  }
  else{
    p->right = n;
    n->parent = p;
    add_rebalance(n);
  }
  root->color = BLACK; // color the root BLACK
  node_count++; // increase the node count
}

//  Function: remove()
//  Description: Removes the requested key-value pair from the tree
//  Inputs: The key of the pair to be removed
//  Outputs: None
template<typename K, typename V>
void RBTCollection<K,V>::remove(const K& a_key){
  // create sentinel Node
  Node* sentinel = new Node;
  sentinel->right = root;
  sentinel->left = nullptr;
  sentinel->parent = nullptr;
  sentinel->color = RED;
  root->parent = sentinel;

  Node* x = root;
  Node* p = sentinel;
  bool found = false;

  // iteratively find the node to delete
  while(x && !found){
    // rebalance and go left
    if(a_key < x->key){
      remove_rebalance(x, false);
      x = x->left;
    }
    // rebalance and go right
    else if(a_key > x->key){
      remove_rebalance(x, true);
      x = x->right;
    }
    // found node to remove
    else if(a_key == x->key){
      remove_rebalance(x, x->left);
      found = true;
    }
  }
  if(!found) return; // the key does not exist in the tree
  p = x->parent;

  // delete cases
  // case 1 - 0 or 1 children
  if(!x->left || !x->right){
    if(x->left){
      Node* tmp = x->left;
      tmp->parent = p;
      if(x == p->left) p->left = tmp;
      if(x == p->right) p->right = tmp;
      delete x;   
      x = tmp; 
    }
    else if(x->right){
      Node* tmp = x->right;
      tmp->parent = p;
      if(x == p->left) p->left = tmp;
      if(x == p->right) p->right = tmp;
      delete x;
      x = tmp;
    }
    else{
      if(x == p->left) p->left = nullptr;
      if(x == p->right) p->right = nullptr;
      delete x;
    }
  }
  // case 2 - 2 children
  else{
    Node* s = x->right;
    remove_rebalance(s,false);
    // special case - successor is the node to the right of x
    if(!s->left){
      // copy the values of s into x
      x->key = s->key;
      x->value = s->value;
      Node* tmp = x->right;
      tmp->left = nullptr;
      delete s; // delete successor
    }
    else{
      while(s->left){
        s = s->left;
        remove_rebalance(s,false);
      }
      // copy s pair into x
      x->key = s->key;
      x->value = s->value;
      // remove s
      Node* s_p = s->parent;
      if(s->right){
        Node* tmp = s->right;
        s_p->left = tmp;
        tmp->parent = s_p;
        delete s;
      }
      else{
        s_p->left = nullptr;
        delete s;
      }
    }
  }
  
  // clean up
  root = sentinel->right;
  if(root) {
    root->parent = nullptr;
    root->color = BLACK;
  }
  delete sentinel;
  node_count--;
}

//  Function: find()
//  Description: Finds the value associated with the given key, if it exists in the tree
//  Inputs: Key to be found
//  Outputs: Value associated with the key, whether or not the pair exists in the tree
template<typename K, typename V>
bool RBTCollection<K,V>::find(const K& search_key, V& return_val) const
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
void RBTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  find(root, k1, k2, keys);
}

//  Function: keys()
//  Description: Returns a list of all the keys in the tree (using in-order traversal)
//  Inputs: None
//  Outputs: List of all keys in the collection
template<typename K, typename V>
void RBTCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  keys(root, all_keys);
}

//  Function: sort()
//  Description: Returns a list of all the keys in sorted order
//  Inputs: None
//  Outputs: A list of the keys in the system in sorted order
template<typename K, typename V>
void RBTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
}

//  Function: size()
//  Description: Returns the number of key-value pairs of the tree
//  Inputs: None
//  Outputs: The number of key-value pairs in the tree
template<typename K, typename V>
size_t RBTCollection<K,V>::size() const
{
  return node_count;
}

//  Function: height()
//  Description: Returns the height of the tree
//  Inputs: None
//  Outputs: The height of the tree (counting of nodes)
template<typename K, typename V>
size_t RBTCollection<K,V>::height() const{
  if(size() == 0) return 0;
  return height(root);
}

//----------------------------------------------------------------------
// Helper Functions
//----------------------------------------------------------------------

// helper function for destructor
template<typename K, typename V>
void RBTCollection<K,V>::make_empty(Node* subtree_root)
{
  /*
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
  */
}

// helper function for copy constructor
template<typename K, typename V>
void RBTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
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

// helper function for find-range
template<typename K, typename V>
void RBTCollection<K,V>::find(const Node* subtree_root, 
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
void RBTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
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
void RBTCollection<K,V>::rotate_right(Node*k2){
  Node* k1 = k2->left;  // k1 is k2's left child
  // point k2's left to k1's right subtree
  k2->left = k1->right;
  if(k2->left) k2->left->parent = k2;
  k1->right = k2; // move k2 to be k1's right child
  // update k1 and k2's parents
  k1->parent = k2->parent;
  if(!k2->parent) root = k1;
  else{
    if(k2 == k2->parent->right) k2->parent->right = k1;
    else k2->parent->left = k1;
  }
  k2->parent = k1;
}

// helper function for left rotations
template<typename K, typename V>
void RBTCollection<K,V>::rotate_left(Node* k2){
  Node* k1 = k2->right;  // k1 is k2's right child
  // point k2's right to k1's left subtree
  k2->right = k1->left;
  if(k2->right) k2->right->parent = k2;  
  k1->left = k2; // move k2 to be k1's right child
  // update k1 and k2's parents
  k1->parent = k2->parent;
  if(!k2->parent) root = k1;
  else{
    if(k2 == k2->parent->right) k2->parent->right = k1;
    else k2->parent->left = k1;
  }
  k2->parent = k1;
}


// helper function for rebalancing during the add function
template<typename K, typename V>
void RBTCollection<K,V>::add_rebalance(Node* x){
  if(!x) return;  // case if the list is empty

  Node* p = x->parent;  // parent node pointer
  Node* lptr = x->left; // left child pointer
  Node* rptr = x->right;  // right child pointer

  //if(!lptr && !rptr) return;  // case if we have reached a leaf node

  // case 1 - color flip
  if(lptr && rptr && x->color == BLACK && lptr->color == RED && rptr->color == RED){
    x->color = RED;
    lptr->color = BLACK;
    rptr->color = BLACK;
  }
  if(p && p->color == RED && x->color == RED){
    Node* g = p->parent;
    // right cases
    if(g && p == g->right){
      // case 3 - right-left rotation (x is an inside node)
      if(x == p->left){
        rotate_right(p);  // rotate right at p
        rotate_left(g); // rotate left at g
        // adjust colors
        x->color = BLACK;
        g->color = RED;
      }
      // case 2 - left rotation (x is an outside node)
      else if(x == p->right){
        rotate_left(g);
        p->color = BLACK;
        g->color = RED;
      }
    }
    // left cases
    else if(g && p == g->left){
      // case 2 - right rotation (X is an outside node)
      if(x == p->left){
        rotate_right(g);
        // adjust colors
        p->color = BLACK;
        g->color = RED;
      }
      // case 3 - left-right rotation (x is an inside node)
      else if(x == p->right){
        rotate_left(p);
        rotate_right(g);
        // adjust colors
        x->color = BLACK;
        g->color = RED;
      }
    }
  }
}

// helper function for rebalancing during the remove function
template<typename K, typename V>
void RBTCollection<K,V>::remove_rebalance(Node* x, bool going_right){
  if(x->color == RED) return;

  Node* p = x->parent;
  Node* t = p;
  if(x == p->left) t = p->right;
  if(x == p->right) t = p->left;
  Node* lptr = x->left;
  Node* rptr = x->right;

  // case 1 - x has non-nav RED child
  if((lptr && lptr->color == RED) || (rptr && rptr->color == RED)){
    if(lptr && lptr->color == RED && going_right){
      rotate_right(x);
      x->color = RED;
      lptr->color = BLACK;
    }
    else if(rptr && rptr->color == RED && !going_right){
      rotate_left(x);
      x->color = RED;
      rptr->color = BLACK;
    }
  }
  // case 2 - color flip
  else if(t && (!t->left || t->left->color == BLACK)
          && (!t->right || t->right->color == BLACK)){
    p->color = BLACK;
    x->color = RED;
    t->color = RED;
  }
  // case 3 & 4 - rotations
  else if(t && ((t->left && t->left->color == RED) || 
                (t->right && t->right->color == RED))){
    if(t->left && t->left->color == RED){
      Node* r = t->left;
      // case 3 - t has an outside RED child
      if(t == p->left){
        rotate_right(p);
        t->color = RED;
        r->color = BLACK;
        x->color = RED;
        p->color = BLACK;
      }
      // case 4 - t has an inside RED child
      else if(t == p->right){
        rotate_right(t);
        rotate_left(p);
        p->color = BLACK;
        x->color = RED;
      }
    }
    else if(t->right && t->right->color == RED){
      Node* r = t->right;
      // case 4 - t has an inside RED child
      if(t ==p->left){
        rotate_left(t);
        rotate_right(p);
        p->color = BLACK;
        x->color = RED;
      }
      // case 3 - t has an outside RED child
      else if(t == p->right){
        rotate_left(p);
        t->color = RED;
        r->color = BLACK;
        x->color = RED;
        p->color = BLACK;
      }
    }
  }
}

// helper function for height
template<typename K, typename V>
size_t RBTCollection<K,V>::height(Node* subtree_root) const{
  if(!subtree_root) return 0;
  size_t left_height = 0;  // variable to keep track of the height of the left subtree
  size_t right_height = 0; // variable to keep track of the height of the right subtree
  // get the height of each subtree
  left_height = height(subtree_root->left);
  right_height = height(subtree_root->right);
  // check which height is larger and return that height +1
  if(left_height > right_height){
    return left_height+1;
  }
  else return right_height +1;
}

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
