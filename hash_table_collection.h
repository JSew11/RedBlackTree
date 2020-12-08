//----------------------------------------------------------------------
// FILE: hash_table_collection.h
// NAME: Joshua Seward
// DATE: October 28, 2020
// DESC: Implements a version of the collection class that implements a
// hash table
//----------------------------------------------------------------------

#ifndef HASH_TABLE_COLLECTION_H
#define HASH_TABLE_COLLECTION_H

#include "collection.h"
#include "array_list.h"
#include <functional>

template<typename K, typename V>
class HashTableCollection : public Collection<K,V>
{
  public:
    HashTableCollection();
    HashTableCollection(const HashTableCollection<K,V>& rhs);
    ~HashTableCollection();
    HashTableCollection& operator=(const HashTableCollection<K,V>& rhs);

    void add(const K& key, const V& val);
    void remove(const K& key);
    bool find(const K& search_key, V& return_val) const;
    void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
    void keys(ArrayList<K>& all_keys) const;
    void sort(ArrayList<K>& all_keys_sorted) const;
    size_t size() const;

    size_t min_chain_length();
    size_t max_chain_length();
    double avg_chain_length();

  private:
    struct Node {
      K key;
      V value;
      Node* next;
    };

    std::hash<K> hash_fcn;  // declare hash function for the hash table

    Node** hash_table;
    size_t length; // number of pairs in the collection
    size_t capacity; // number of buckets in the collection
    double load_factor_threshold = 0.75;

    void resize_and_rehash();
    void make_empty();
};

template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection()
  : capacity(16), length(0)
{
  hash_table = new Node*[capacity];
  for(int i = 0; i < capacity; ++i){
    hash_table[i] = nullptr;
  }
}

template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(const HashTableCollection<K,V>& rhs)
  : capacity(16), length(0), hash_table(nullptr)
{
  // defer to assignment operator
  *this = rhs;
}

template<typename K, typename V>
HashTableCollection<K,V>::~HashTableCollection()
{
  make_empty();
}

template<typename K, typename V>
HashTableCollection<K,V>& HashTableCollection<K,V>::
operator=(const HashTableCollection<K,V>& rhs)
{
  if(this != &rhs){
    make_empty();
    capacity = rhs.capacity;  // copy the rhs capacity to the lhs
    length = rhs.length;  // copy the rhs length to the lhs
    hash_table = new Node*[capacity]; // create a new hash table for the lhs
    for(int i = 0; i < rhs.capacity; ++i){
      hash_table[i] = nullptr;
      Node* tmpR = rhs.hash_table[i]; // pointer for the chain at index i of rhs collection
      while(tmpR){
        Node* tmpL = new Node;  // create new temp node to add into the lhs table
        tmpL->key = tmpR->key;
        tmpL->value = tmpR->value;
        // insert tmp into front of hash_table[i]
        tmpL->next = hash_table[i];
        hash_table[i] = tmpL;
        // advance tmpR
        tmpR = tmpR->next;
      }
    }
  }
  return *this;
}

//  Function: add()
//  Description: Adds a new key-value pair to the collection in the correct hashed location
//  Inputs: Key and value to be added to the collection
//  Outputs: None
template<typename K, typename V>
void HashTableCollection<K,V>::add(const K& key, const V& val)
{
  // creating Node to be added at index
  Node* tmp = new Node;
  tmp->key = key;
  tmp->value = val;
  tmp->next = nullptr;
  // find the index to add the new Node in the hash table
  size_t code = hash_fcn(key);
  size_t index = code % capacity;
  // add the Node to the end of the chain at the found index
  tmp->next = hash_table[index];
  hash_table[index] = tmp;
  length++; // increment the length variable
  // check load factor and resize and rehash (if necessary)
  double load_factor = (length*1.0) / capacity;
  if(load_factor >= load_factor_threshold) resize_and_rehash();
}

//  Function: remove()
//  Description: Removes the requested key-value pair from the collection
//  Inputs: The key of the pair to be removed
//  Outputs: None
template<typename K, typename V>
void HashTableCollection<K,V>::remove(const K& key)
{
  if(size() > 0){
    size_t code = hash_fcn(key);
    size_t index = code % capacity;
    // iterate through the chain to find the Node with the key
    Node* cur = hash_table[index];
    // special case for if the key to be removed is at the front of the chain
    if(cur && key == cur->key){
      hash_table[index] = cur->next;
      delete cur;
      --length;
    }
    // removing a Node from the chain if it is not at the front
    else if(cur){
      Node* prev = cur;
      cur = cur->next;
      while(cur){
        if(key == cur->key){
          prev->next = cur->next;
          delete cur;
          --length;
       }
        prev = prev->next;
        cur = cur->next;
      }
    }
  }
}

//  Function: find()
//  Description: Finds the value associated with the given key, if it exists in 
//  the collection
//  Inputs: Key to be found
//  Outputs: Value associated with the key
template<typename K, typename V>
bool HashTableCollection<K,V>::find(const K& search_key, V& return_val) const
{
  if(length <= 0) return false; // cannot find a value in an empty table
  // find the index of the search key according to the hash function
  size_t code = hash_fcn(search_key);
  size_t index = code % capacity;
  // iterate through the chain to find the value associated with the search key
  Node* cur = hash_table[index];
  while(cur){
    if(cur->key == search_key){
      return_val = cur->value;
      return true;
    }
    cur = cur->next;
  }
  return false; // if the key does not exist in the collection return false
}

//  Function: find()
//  Description: Finds and returns all keys between the given k1 and k2 keys
//  Inputs: Given key "limits"
//  Outputs: All keys between the given "limits"
template<typename K, typename V>
void HashTableCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  for(int i = 0; i < capacity; ++i){
    Node* cur = hash_table[i];
    while(cur){
      if(cur->key >= k1 && cur->key <=k2){
        keys.add(cur->key); // add key to the output list if it is within provided range
      }
      cur = cur->next;  // iterate through the chain
    }
  }
}

//  Function: keys()
//  Description: Returns a list of all the keys in the collection
//  Inputs: None
//  Outputs: List of all keys in the collection
template<typename K, typename V>
void HashTableCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  for(int i = 0; i < capacity; ++i){  // search each bucket in the hash table
    Node* cur = hash_table[i];
    // add the key of each node in the chain at index 'i' to the output list
    while(cur){
      all_keys.add(cur->key);
      cur = cur->next;  // iterate through the chain
    }
  }
}

//  Function: sort()
//  Description: Sorts the collection of keys and returns a list of all the keys
//  in sorted order
//  Inputs: None
//  Outputs: A list of the keys in the system in sorted order
template<typename K, typename V>
void HashTableCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);  // get a list of all keys in the hash table
  all_keys_sorted.sort(); // sort the list of all keys in the hash table
}

//  Function: size()
//  Description: Returns the number of key-value pairs of the collection
//  Inputs: None
//  Outputs: The number of key-value pairs in the collection
template<typename K, typename V>
size_t HashTableCollection<K,V>::size() const
{
  return length;
}

//  Function: min_chain_length()
//  Description: Returns the length of the smallest chain in the hash table
//  Inputs: None
//  Outputs: The length of the smallest chain in the hash table
template<typename K, typename V>
size_t HashTableCollection<K,V>::min_chain_length()
{
  if(length == 0) return 0;
  size_t min_len = length;
  for(int i = 0; i < capacity; ++i){
    size_t cur_len = 0;
    Node* cur = hash_table[i];
    while(cur){
      ++cur_len;
      cur = cur->next;
    }
    if(cur_len < min_len) min_len = cur_len;
  }
  return min_len;
}

//  Function: max_chain_length()
//  Description: Returns the length of the longest chain in the hash table
//  Inputs: None
//  Outputs: The length of the longest chain in the hash table
template<typename K, typename V>
size_t HashTableCollection<K,V>::max_chain_length()
{
  if(length == 0) return 0;
  size_t max_len = 0;
  for(int i = 0; i < capacity; ++i){
    size_t cur_len = 0;
    Node* cur = hash_table[i];
    while(cur){
      ++cur_len;
      cur = cur->next;
    }
    if(cur_len > max_len) max_len = cur_len;
  }
  return max_len;
}

//  Function: avg_chain_length()
//  Description: Returns the average length of all chains in the hash table
//  Inputs: None
//  Outputs: The average length of the all chains in the hash table
template<typename K, typename V>
double HashTableCollection<K,V>::avg_chain_length()
{
  return length/(1.0*capacity);
}

//  Function: resize_and_rehash()
//  Description: Resizes the hash table so more pairs can fit in it, and rehashes the pairs
//  already in the hash table
//  Inputs: None
//  Outputs: None
template<typename K, typename V>
void HashTableCollection<K,V>::resize_and_rehash()
{
  size_t new_capacity = 2*capacity;  // double the capacity
  size_t new_length = length;
  Node** new_table = new Node*[new_capacity]; // new list for the items to be rehashed into
  for(int i = 0; i < new_capacity; ++i) new_table[i] = nullptr;
  std::hash<K> hash_fcn;  // creating a new hash function
  for(int i = 0; i < capacity; ++i){
    Node* cur = hash_table[i];  // temp Node for itreating through each chain
    while(cur){
      size_t code = hash_fcn(hash_table[i]->key); // creating hash code
      size_t index = code % new_capacity; // finding the index for the new table

      // creating new Node to put into the new table
      Node* tmp = new Node;
      tmp->key = cur->key;
      tmp->value = cur->value;

      // inserting the new node into the front of the chain at index
      tmp->next = new_table[index];
      new_table[index] = tmp;

      // advance cur
      cur = cur->next;
    }
  }
  make_empty();
  hash_table = new_table; // point hash_table to new_table
  capacity = new_capacity;  // update capacity
  length = new_length;
}

//  Function: make_empty()
//  Description: Deletes each chain in the hash table
//  Inputs: None
//  Outputs: None
template<typename K, typename V>
void HashTableCollection<K,V>::make_empty()
{
  if(hash_table){
    for(int i = 0; i < capacity; ++i){
      Node* cur = hash_table[i];  // pointer to Node at the head of chain at hash_table[i]
      while(cur){
        hash_table[i] = cur->next;
        cur->next = nullptr;
        delete cur;
        cur = hash_table[i];
     }
    }
  }
  length = 0;
}
#endif