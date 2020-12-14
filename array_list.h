//----------------------------------------------------------------------
// FILE: array_list.h
// NAME: Joshua Seward
// DATE: October 1, 2020
// DESC: Implements a resizable array version of the list
//       class. Elements are added by default to the last available
//       index in the array.(This iteration includes selction sort, 
//       insertion sort, merge sort, and quick sort algorithms)
//----------------------------------------------------------------------

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "list.h"

template<typename T>
class ArrayList : public List<T>
{
public:
  ArrayList();
  ArrayList(const ArrayList<T>& rhs);
  ~ArrayList();
  ArrayList& operator=(const ArrayList<T>& rhs);

  void add(const T& item);
  bool add(size_t index, const T& item);
  bool get(size_t index, T& return_item) const;
  bool set(size_t index, const T& new_item);
  bool remove(size_t index);
  size_t size() const;
  void selection_sort();
  void insertion_sort();
  void merge_sort();
  void quick_sort();
  void sort();

private:
  T* items;
  size_t capacity;
  size_t length;

  // helper to resize items array
  void resize();
  // helper for merge_sort
  void merge_sort(const size_t& start, const size_t& end);
  // helper for quick_sort
  void quick_sort(const size_t& start, const size_t& end);
};


template<typename T>
ArrayList<T>::ArrayList()
  : capacity(10), length(0)
{
  items = new T[capacity];
}


template<typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& rhs)
  : capacity(10), length(0), items(nullptr)
{
  // defer to assignment operator
  *this = rhs;
}

template<typename T>
ArrayList<T>::~ArrayList()
{
  delete[] items;
  length = 0; 
  capacity = 0;
}

template<typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& rhs)
{
  if(this != &rhs){
    delete[] items; // deletes the lhs array
    capacity = rhs.capacity;
    length = rhs.length;
    items = new T[capacity];
    for(int i = 0; i < length; ++i){
      items[i] = rhs.items[i];  // deep copies values of rhs array into lhs array
    }
  }
  return *this;
}

//  Function: add()
//  Description: Appends a given input to the end of an ArrayList object
//  Inputs: Item to be added
//  Outputs: None
template<typename T>
void ArrayList<T>::add(const T& item)
{
  if(length == capacity){
    resize(); // if the Array List is at capacity, then resize it
  }
  items[length] = item; // add the desired item to the end of the Array List
  length++; // update the length variable
}

//  Function: add()
//  Description: Appends a given input at a given index of an ArrayList object
//  Inputs: Index where you wouldlike to add the item, and the item to be added
//  Outputs: None
template<typename T>
bool ArrayList<T>::add(size_t index, const T& item)
{
  if(index > length || index < 0) return false;
  if(index == length){
    add(item);  // if the index after the last item in the array, add the item to the end
    return true;
  }
  if(length+1 > capacity) resize(); // if the new length will exceed the capacity, resize the array
  for(int i = length; i > index; --i){
    items[i] = items[i-1];  // shifts the items after index right by one
  }
  items[index] = item;  // sets the item at index to item
  length++; // increases the length by 1
  return true;
}

//  Function: get()
//  Description: Provides the user with the item at a specific index of an ArrayList object
//  Inputs: Index you would like the item of
//  Outputs: The item at the given index
template<typename T>
bool ArrayList<T>::get(size_t index, T& return_item) const
{
  if(index >= length || index < 0) return false;
  return_item = items[index]; // set the output variable to the value of the Array List at index
  return true;
}

//  Function: set()
//  Description: Sets a specific index of an ArrayList object to a specific item
//  Inputs: Index wherre you would like the given item, and the item to be added
//  Outputs: None
template<typename T>
bool ArrayList<T>::set(size_t index, const T& new_item)
{
  if(index >= length || index < 0) return false;
  items[index] = new_item;  // set the value of the Array List at index to the given value
  return true;
}

//  Function: remove()
//  Description: Removes the item at the given index of an ArrayList object, and adjusts the remaining list accordingly
//  Inputs: Index of where you would like the item to be removed from
//  Outputs: None
template<typename T>
bool ArrayList<T>::remove(size_t index)
{
  if(index >= length || index < 0) return false;
  for(int i = index; i < length-1; i++){
    items[i] = items[i+1];  // all items in the Array List are shifted one to the left and the item at index is overwritten
  }
  length--; // the length of the array is shortened, which cuts off the last value of the old Array List
  return true;
}

//  Function: size()
//  Description: Gives the size (number of items) of an ArrayList object
//  Inputs: None
//  Outputs: Size of the ArrayList object (number of items it contains)
template<typename T>
size_t ArrayList<T>::size() const
{
  return length;
}

//  Function: resize()
//  Description: Doubles the capacity of an ArrayList object
//  Inputs: None
//  Outputs: None
template<typename T>
void ArrayList<T>::resize()
{
  capacity *= 2;  // double capacity of the current array
  T* tmp = new T[capacity]; 
  for(int i = 0; i < length; i++){
    tmp[i] = items[i];  // store items in new temporary arrray of size capacity
  }
  delete []items; // delete old Array List
  items = tmp;
}

//  Function: selection_sort()
//  Description: Sorts the items in an ArrayList object
//  Inputs: None
//  Outputs: None
template<typename T>
void ArrayList<T>::selection_sort()
{
  int sorted = length;  // variable for the cut-off of the sorted list
  for(int i = length-1; i > 1; --i){
    T largest = items[0]; // variable for the largest item in the list
    int index = 0;  // variable for the index of the largest item in the list
    for(int j = 0; j < sorted; ++j){
      if(items[j] > largest){
        index = j;  // sets the indexof the largest item to j
        largest = items[j]; // sets the largest item to the item at j
      }
    }
    items[index] = items[i]; 
    items[i] = largest; // swaps the largest item into the sorted list
    sorted--; // moves the cut-off for the sorted list
  }
}

//  Function: insertion_sort()
//  Description: Sorts the items in an ArrayList object
//  Inputs: None
//  Outputs: None
template<typename T>
void ArrayList<T>::insertion_sort()
{
  int sorted = 1; // variable for the size of the sorted list
  for(int i = 1; i < length; ++i){
    T val = items[i]; // finding the next value to be add into the sorted list
    for(int j = 0; j < sorted; ++j){
      if(val < items[j]){
        items[i] = items[j];  // putting the new value in the correct position within the sirted list
        items[j] = val;
        val = items[i]; // shifting the remaining values in the sorted list
      }
    }
    sorted++;
  }
}

//  Function: merge_sort()
//  Description: Sorts the items in an ArrayList object
//  Inputs: None
//  Outputs: None
template<typename T>
void ArrayList<T>::merge_sort()
{
  merge_sort(0,length-1);
}

//  Function: merge_sort()
//  Description: Helper function for merge_sort()
//  Inputs: Start and end points of the ArrayList to be sorted
//  Outputs: None
template<typename T>
void ArrayList<T>::merge_sort(const size_t& start, const size_t& end)
{
  if(start < end){
    // split the list
    size_t mid = (end+start)/2;  // find the mid point of list
    merge_sort(start,mid);  // sort the first half of the list
    merge_sort(mid+1, end); // sorts the second half of the list

    // merge the lists
    T* tmp = new T[end-start+1];  // temp array to sort the list
    size_t first1 = start;  // start index for first half to sort
    size_t first2 = mid+1;  // start index for second half to sort
    int i = 0;  // index for temp array
    while(first1 <= mid && first2 <= end){
      if(items[first1] < items[first2]) tmp[i++] = items[first1++]; 
      else tmp[i++] = items[first2++];  // place the smallest item in either list into temp
    }
    while(first1 <= mid) tmp[i++] = items[first1++];  // finish filling temp
    while(first2 <= end) tmp[i++] = items[first2++];
    for(int j = 0; j <= end-start; ++j) items[j+start] = tmp[j];  // copy temp into correct place in items
  }
}

//  Function: quick_sort()
//  Description: Sorts the items in an ArrayList object
//  Inputs: None
//  Outputs: None
template<typename T>
void ArrayList<T>::quick_sort()
{
  if(length > 1) quick_sort(0,length-1);
}

//  Function: quick_sort()
//  Description: Helper function for public quick_sort function
//  Inputs: Start and end of ArrayList to be sorted
//  Outputs: None
template<typename T>
void ArrayList<T>::quick_sort(const size_t& start, const size_t& end)
{
  if(start < end){
    // selecting a pivot
    T pivot = items[start];
    size_t last_p1 = start; // last element in the first partition

    // sorting based on the pivot
    for(int i = start+1; i <= end; ++i){
      if(items[i] < pivot){
        last_p1++;
        T tmp = items[last_p1];
        items[last_p1]= items[i];
        items[i] = tmp;
      }
    }

    // swap the pivot with the last element of the first partition
    items[start] = items[last_p1];
    items[last_p1] = pivot;

    // sort the two partitions
    if(last_p1 > start) quick_sort(start, last_p1-1); // sort the left half of the list
    if(last_p1 < end) quick_sort(last_p1+1 ,end); // sort the right half of the list
    // the lists should already be sorted since the changes are being made within the
    // same array list
  }
}

// Function: sort()
// Description: Sorts the Array List using quick_sort()
// Inputs: None
// Outputs: None
template<typename T>
void ArrayList<T>::sort()
{
  quick_sort();
}

#endif