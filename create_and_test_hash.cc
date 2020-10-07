/******************* 
Ingmar Fjolla
Homework 2 
Create and test hash cc
This is the "main file" which will be responsible for outputting evreything
we are responsible for :
 A) Thetotalnumberofelementsinthetable(N),thesizeofthetable(T),theloadfactor (N/T), 
the total number of collisions (C), and the average number of collisions (C/N).
B) You will check whether each word in another given file, query_words.txt, is in the hash table, 
and print corresponding output depending on whether the word is found and not found, and how many probes it took to find the word,
if it exists. Although you are provided with a file, we will use an unseen test file with another file name, which may contain any subset of
words from words.txt.
 


****************/


#include "quadratic_probing.h"
#include "linear_probing.h"
#include "double_hashing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename, const string r_val) {
  // cout << "TestFunctionForHashTables..." << endl;
  // cout << "Words filename: " << words_filename << endl;
  // cout << "Query filename: " << query_filename << endl;
  hash_table.MakeEmpty();  
  //..Insert your own code

  //code starts for implementing the insertion of the data
  ifstream wordfile(words_filename); //stream for file name
  string linesofwords; //each line of word
  while(getline(wordfile,linesofwords))
  {
    //while we get each line as inpput, insert into the hash table
    hash_table.Insert(linesofwords);
  }
  size_t number_of_elements = hash_table.getNumberOfElements();
  size_t hash_table_size = hash_table.getSizeOfHashTable();
  size_t number_of_collisions = hash_table.getNumberOfCollisions();
  //print the rvalue IF and only if its not empty, don't know how else to pass it here
  if(r_val!="")
  {
    cout<<"r_value: "<<stoi(r_val)<<endl;
  }
  
  cout << "number_of_elements: " << number_of_elements<< endl;
  cout << "size_of_table: " << hash_table_size << endl;
  cout << "load_factor: " << float(number_of_elements)/float(hash_table_size)<< endl;
  cout << "collisions: " << number_of_collisions <<endl;
  cout << "avg_collisions: " << float(number_of_collisions)/float(number_of_elements)<<endl;

  cout << endl;
  //cout << "Beginning searching for query words:" << endl;
  ifstream query_file(query_filename);  // Create ifstream object to read the query_file
  string query_line;                    // Store every word extracted from the file
  // While each line (only 1 word essentially) from the file is being placed in query_line
  while(getline(query_file, query_line)){
    // If the query word is found in the hash table
    if(hash_table.Contains(query_line)){
      // Determine how many probes it took for the word to be found in the hash table
      size_t number_of_probes = hash_table.getNumberOfProbes(query_line);
      // Print out the number of probes
      cout << query_line << " Found " << number_of_probes <<endl;
    }
    //If the query word is not found in the hash table
    else {
      // Determine how many probes it took for the word to be not found in the hash table
      size_t number_of_probes = hash_table.getNumberOfProbes(query_line);
      // Print out the number of probes
      cout << query_line << " Not_Found " << number_of_probes <<endl;
    }
  }

}

int testFunctionWrapper(int argument_count, char **argument_list) {
  const string words_filename(argument_list[1]);
  const string query_filename(argument_list[2]);
  const string param_flag(argument_list[3]);

  if (param_flag == "linear") {
    HashTableLinear<string> linear_probing_table;
    TestFunctionForHashTable(linear_probing_table, words_filename, query_filename,"");    
  } else if (param_flag == "quadratic") {
    HashTable<string> quadratic_probing_table;
    TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename,"");    
  } else if (param_flag == "double") {
    HashTableDouble<string> double_probing_table;
    string r_val(argument_list[4]); //take that r value for the double
    int temp = stoi(r_val);
    double_probing_table.setrval(temp); 
    TestFunctionForHashTable(double_probing_table, words_filename, query_filename,r_val);    
  } else {
    cout << "Unknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
  }
  return 0;
}

// Sample main for program create_and_test_hash.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE MAIN.
// WE WILL DIRECTLY CALL testFunctionWrapper. ALL FUNCTIONALITY SHOULD BE THERE.
// This main is only here for your own testing purposes.
int main(int argc, char **argv) {

  if (argc != 5) {
    cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
  }
  
  testFunctionWrapper(argc, argv);
  return 0;
}
