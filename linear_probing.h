/******************* 
Ingmar Fjolla
Homework 2 
linear probing
This is the "lingear probing" part which will be responsible for the linear probing mechanism of the hash table
we are responsible for :
 A) Thetotalnumberofelementsinthetable(N),thesizeofthetable(T),theloadfactor (N/T), 
the total number of collisions (C), and the average number of collisions (C/N).
B) You will check whether each word in another given file, query_words.txt, is in the hash table, 
and print corresponding output depending on whether the word is found and not found, and how many probes it took to find the word,
if it exists. Although you are provided with a file, we will use an unseen test file with another file name, which may contain any subset of
words from words.txt.
 


****************/

#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>



template <typename HashedObj>
class HashTableLinear 
{
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableLinear(size_t size = 101) : array_(NextPrime(size))
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }
  size_t getNumberOfElements(){
  
  return current_size_;
  }

  size_t getSizeOfHashTable(){
    return array_.size();
  }

  size_t getNumberOfCollisions(){
    return collisions;
  }

  size_t getNumberOfProbes(const HashedObj & x){
    size_t current_pos = InternalHash(x);
    size_t number_of_probes = 1;    // Counter that tracks the amount of probes
      
    while (array_[current_pos].info_ != EMPTY && array_[current_pos].element_ != x) {
      current_pos += 1;       // Compute ith probe (move to the next index, linear movement)
      ++number_of_probes;

      if (current_pos >= array_.size())
        current_pos -= array_.size();
    }

    return number_of_probes;
  }


 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_ = 0;
  mutable size_t collisions = 0;

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  size_t FindPos(const HashedObj & x) const {
    //size_t offset = 1;
    size_t current_pos = InternalHash(x);
      
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      current_pos += 1;  // Compute ith probe.
      //offset += 2;
      collisions++; //here we count because every time we increase
      //the current positions, we are lookin for the next spot
      if (current_pos >= array_.size())
	current_pos -= array_.size();
    }
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }
};

#endif