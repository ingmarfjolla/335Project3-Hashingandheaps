/******************* 
Ingmar Fjolla
Homework 2 
double hashing
This is the "double hashing" part which will be responsible for the double hashing mechanism of the hash table
we are responsible for :
 A) Thetotalnumberofelementsinthetable(N),thesizeofthetable(T),theloadfactor (N/T), 
the total number of collisions (C), and the average number of collisions (C/N).
B) You will check whether each word in another given file, query_words.txt, is in the hash table, 
and print corresponding output depending on whether the word is found and not found, and how many probes it took to find the word,
if it exists. Although you are provided with a file, we will use an unseen test file with another file name, which may contain any subset of
words from words.txt.
 


****************/


#ifndef DOUBLE_HASHING_H
#define DOUBLE_HASHING_H

#include <vector>
#include <algorithm>
#include <functional>

// double hashing implementation.
template <typename HashedObj>
class HashTableDouble {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableDouble(size_t size = 101) : array_(NextPrime(size))
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    number_of_collisions_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos)){
      return false;
    }
    
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
    if (IsActive(current_pos)){
      return false;
    }
      
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

  // Return the number of elements in a hash table
  size_t getNumberOfElements(){
    return current_size_;
  }

  // Return the size of the hash table (aka the array)
  size_t getSizeOfHashTable(){
    return array_.size();
  }

  // Return the number of collisions
  size_t getNumberOfCollisions(){
    return number_of_collisions_;
  }

  size_t getNumberOfProbes(const HashedObj & x)
  {
    // size_t current_pos = InternalHash(x);
    // size_t hash_value = ExternalHash(x);
    // size_t jump = hash2(x);
    // size_t number_of_probes = 1;
      
    // while (array_[current_pos].info_ != EMPTY && array_[current_pos].element_ != x) {
    //   current_pos = (hash_value + (jump)) % array_.size();  // Compute ith probe.
    //   jump += jump;
    //   number_of_probes++;
    //   //offset += 2;
      
    //   if (current_pos >= array_.size()){
    //     current_pos -= array_.size();
    //   }
    // }
    return number_of_probes;
  }
  void setrval(int r)
  {
    rvalue = r;
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
  size_t current_size_;
  mutable size_t number_of_collisions_;
  int rvalue;
  mutable int number_of_probes = 1;

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  size_t FindPos(const HashedObj & x) const {
    number_of_probes = 1;
    size_t current_pos = InternalHash(x);     // Get the associated index of the object using a hash function
    size_t hash_value = ExternalHash(x);      
    size_t jump = hash2(x);          // Calculate how far to jump using a second hash function (r - (x % r))
      
    while (array_[current_pos].info_ != EMPTY && array_[current_pos].element_ != x) {
      current_pos += hash2(x);  // Compute ith probe (in a double hashing manner)
      jump += jump;                 // Multiply the jump (adding a number to itself is similar to multiplication)
      number_of_probes +=1;
      number_of_collisions_++;      // Increment counter
      
      if (current_pos >= array_.size()){
        current_pos -= array_.size();
      }
    }
    return current_pos;
  }
  
  // Second hash functon
  size_t hash2(const HashedObj &x) const{
    static std::hash<HashedObj> hf;
    return (rvalue - (hf(x)  % rvalue));      // Return the hash_value computed after performing the second hash functiion
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

  size_t ExternalHash(const HashedObj &x) const{
    static std::hash<HashedObj> hf;
    return hf(x);
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }


};

#endif 