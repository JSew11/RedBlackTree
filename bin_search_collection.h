//----------------------------------------------------------------------
// FILE: bin_search_collection.h
// NAME: Joshua Seward
// DATE: October 19, 2020
// DESC: Implements a version of the collection class that implements a
// binary search function (It remains sorted as items are added/removed)
//----------------------------------------------------------------------

#ifndef BIN_SEARCH_COLLECTION_H
#define BIN_SEARCH_COLLECTION_H

#include "collection.h"
#include "array_list.h"

template<typename K, typename V>
class BinSearchCollection : public Collection<K,V>
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
  bool binsearch(const K& key, size_t& index) const;
};

//  Function: add()
//  Description: Adds a new key-value pair to the collection in the correct sorted location
//  Inputs: Key and value to be added to the collection
//  Outputs: None
template<typename K, typename V>
void BinSearchCollection<K,V>::add(const K& key, const V& val)
{
  size_t index;
  binsearch(key,index); // find the correct index to add the new pair using binsearch
  if(size() == 0) kv_list.add(std::pair<K,V>(key,val));
  else{
    std::pair<K,V> item;
    kv_list.get(index,item);
    if(item.first > key){
      kv_list.add(index, std::pair<K,V>(key,val));  // add the new pair at the correct index
    }
    else kv_list.add(index+1,std::pair<K,V> (key,val));
  }
}

//  Function: remove()
//  Description: Removes the requested key-value pair from the collection
//  Inputs: The key of the pair to be removed
//  Outputs: None
template<typename K, typename V>
void BinSearchCollection<K,V>::remove(const K& key)
{
  if(size() == 0) return;
  size_t index;
  if(binsearch(key, index)){  // find the index of the item to be removed
    kv_list.remove(index);  // if the key is in the collection, remove it
  }
}

//  Function: find()
//  Description: Finds the value associated with the given key, if it exists in 
//  the collection
//  Inputs: Key to be found
//  Outputs: Value associated with the key
template<typename K, typename V>
bool BinSearchCollection<K,V>::find(const K& search_key, V& return_val) const
{
  size_t index;
  if(binsearch(search_key, index)){
    std::pair<K,V> item;
    kv_list.get(index, item); // get the item at the index of the desired key
    return_val = item.second; // set the return value to the value at index
    return true;
  }
  return false;
}

//  Function: find()
//  Description: Finds and returns all keys between the given k1 and k2 keys
//  Inputs: Given key "limits"
//  Outputs: All keys between the given "limits"
template<typename K, typename V>
void BinSearchCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  size_t index;
  binsearch(k1,index);
  std::pair<K,V> item;
  kv_list.get(index,item);  // get the first item within the desired range
  while(item.first <= k2 && index < size()){
    keys.add(item.first); // add the keys within the range to the outout list
    kv_list.get(++index,item);  // get the next item in the list
  }
}

//  Function: keys()
//  Description: Returns a list of all the keys in the collection
//  Inputs: None
//  Outputs: List of all keys in the collection
template<typename K, typename V>
void BinSearchCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  for(int i = 0; i < size(); ++i){
    std::pair<K,V> item;
    kv_list.get(i,item);  // get each item in the list
    all_keys.add(item.first); // add each key to the output list
  }
}

//  Function: sort()
//  Description: Sorts the collection of keys and returns a list of all the keys
//  in sorted order
//  Inputs: None
//  Outputs: A list of the keys in the system in sorted order
template<typename K, typename V>
void BinSearchCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
}

//  Function: size()
//  Description: Returns the number of key-value pairs of the collection
//  Inputs: None
//  Outputs: The number of key-value pairs in the collection
template<typename K, typename V>
size_t BinSearchCollection<K,V>::size() const
{
  return kv_list.size();
}

//  Function: binsearch()
//  Description: Binary searches the list for a specific key, returns true if it is present/
//  false if not present
//  Inputs: The key to search for
//  Outputs: The index of the key to search for
template<typename K, typename V>
bool BinSearchCollection<K,V>::binsearch(const K& key, size_t& index) const
{
  if(size() > 0){
    size_t start = 0; // variable for the start of the section to be sorted
    size_t end = size()-1;  // variable for the end of the section to be sorted
    size_t mid; // variable for the midpoint of the section to be sorted
    while(start <= end){
      mid = (end+start)/2; // calculate the midpoint for binary search 
      std::pair<K,V> mid_pair;
      kv_list.get(mid, mid_pair); // get the pair at the midpoint
      if(key == mid_pair.first){  // if the key is equal to the key of the middle pair
        index = mid;  // you have found the index of the key
        return true;  // return true
      }
      else{
        // if the key is greater than the midpoint
        if(key < mid_pair.first){
          if(mid == 0){
            index = mid;  // return 0 if the last index to be searched is 0
            return false;
          }
          end = mid-1;  // otherwise update the end of the section to be sorted
        } 
        // if the key is less than the key of the midpoint
        // update the end of the section to be sorted
        if(key > mid_pair.first) start = mid+1;
      }
    }
    index = mid;
    return false;
  }
  index = 0;
  return false;
}

#endif