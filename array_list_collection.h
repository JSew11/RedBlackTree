//----------------------------------------------------------------------
// FILE: array_list_collection.h
// NAME: Joshua Seward
// DATE: October 8, 2020
// DESC: Implements an array list version of the collection class
//----------------------------------------------------------------------

#ifndef ARRAY_LIST_COLLECTION_H
#define ARRAY_LIST_COLLECTION_H

#include "collection.h"

template<typename K, typename V>
class ArrayListCollection : public Collection<K,V>
{
    public:
    void add(const K& key, const V& val);
    void remove(const K& key);
    bool find(const K& search_key, V& return_val) const;
    void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
    void keys(ArrayList<K>& all_keys) const;
    void sort(ArrayList<K>& all_keys_sorted) const;
    size_t size() const;

    private:
    ArrayList<std::pair<K,V>> kv_list;
};

//  Function: add()
//  Description: Adds a new key-value pair to the collection
//  Inputs: Key and value to be added to the collection
//  Outputs: None
template<typename K, typename V>
void ArrayListCollection<K,V>::add(const K& key, const V& val)
{
  kv_list.add(std::pair<K,V>(key, val));  // call array list add to end function
}

//  Function: remove()
//  Description: Removes the requested key-value pair from the collection
//  Inputs: The key of the pair to be removed
//  Outputs: None
template<typename K, typename V>
void ArrayListCollection<K,V>::remove(const K& key)
{
  if(size() == 0) return;
  size_t i = 0; // loop index variable
  std::pair<K,V> item;  // variable to hold the pair at the current loop index
  kv_list.get(i,item);
  while(item.first != key){
    i++;  // find the index of the item to be removed
    kv_list.get(i,item);
  }
  kv_list.remove(i);  // remove the item at the found index
}

//  Function: find()
//  Description: Finds the value associated with the given key, if it exists in 
//  the collection
//  Inputs: Key to be found
//  Outputs: Value associated with the key
template<typename K, typename V>
bool ArrayListCollection<K,V>::find(const K& search_key, V& return_val) const
{
  std::pair<K,V> item;  // variable to hold the pair at the current loop index
  for(int i = 0; i < size(); ++i){
    kv_list.get(i,item);
    if(item.first == search_key){
      return_val = item.second; // set the output variable to the value at key
      return true;
    }
  }
  return false; // return false if the given key is not found
}

//  Function: find()
//  Description: Finds and returns all keys between the given k1 and k2 keys
//  Inputs: Given key "limits"
//  Outputs: All keys between the given "limits"
template<typename K, typename V>
void ArrayListCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  std::pair<K,V> item;  // variable to hold the pair at the current loop index
  for(int i = 0; i < size(); ++i){
    kv_list.get(i,item);
    if(item.first >= k1 && item.first <= k2){ // if the current key is within the given range
      keys.add(item.first); // add the current key to the output list of keys
    }
  }
}

//  Function: keys()
//  Description: Returns a list of all the keys in the collection
//  Inputs: None
//  Outputs: List of all keys in the collection
template<typename K, typename V>
void ArrayListCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  std::pair<K,V> item;  // variable to hold the pair at the current loop index
  for(int i = 0; i < size(); ++i){
    kv_list.get(i,item);
    all_keys.add(item.first);
  }
}

//  Function: sort()
//  Description: Sorts and returns a list of all the keys in the collection
//  in sorted order
//  Inputs: None
//  Outputs: A list of the keys in the system in sorted order
template<typename K, typename V>
void ArrayListCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted); // return a list of all the keys in sorted order in the output var
  all_keys_sorted.sort(); // sort the list
}

//  Function: size()
//  Description: Returns the number of key-value pairs of the collection
//  Inputs: None
//  Outputs: The number of key-value pairs in the collection
template<typename K, typename V>
size_t ArrayListCollection<K,V>::size() const
{
  return kv_list.size();
}

#endif