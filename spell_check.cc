/******************* 
Ingmar Fjolla
Homework 2 
SPELL CHECK CC
PART 3 of our project implements a spell checker
i used the quadratic probing implementation of the hash table (easiest one imo)
and divided it into 3 seperate functions

****************/




#include "quadratic_probing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

// Possible correction when adding one character in each possible position of the misspelled word
void AddingOneChar(string& word, HashTable<string>& dictionary){
  // Loop through every char in the string
  for(size_t i = 0; i < word.size()+1; i++)
  {
    // Loop through every possible letter in the alphabet
    for(char j = 'a'; j <= 'z'; j++)
    {
      // Insert a character from the alphabet in every position of the string
      string string_add_char = word.substr(0,i) + j + word.substr(i);
      
      // If the newly created string (with the inserted character) is found in the dictionary
      if(dictionary.Contains(string_add_char))
      {
        // Print out the possible correction
        cout <<"** "<<word <<" -> " <<string_add_char << " ** " <<"case A" <<endl;
      }
    }
 
  }
}

// Possible correction when removing one character from the misspelled word
void RemovingOneChar(string& word, HashTable<string>& dictionary){
  // Loop through every char in the string
  for(size_t i = 0; i < word.size(); i++){
    // Extract the substrings inbetween a character
    // Thus, this "removes"/disregards a character in a string without having to alter the string
    string string_remove_char = word.substr(0,i) + word.substr(i+1);
    if(dictionary.Contains(string_remove_char)){
      //Print out the possible correction
      cout <<"** "<<word <<" -> " <<string_remove_char << " ** " <<"case B" <<endl;
      
    }
  }
}

// Possible correction when swapping adjacent character in the misspelled word
void SwappingAdjacentChars(string& word, HashTable<string>& dictionary){  
  // Loop through every character in the string
  string copier;
  
  for (size_t i = 0; i < word.size()-1; i++)  
  {
      // Swap the current character and the adjacent character
      //swap(word[i], word[i+1]);
      copier = word;
      swap(copier[i],copier[i+1]);
      // If the string(with newly swapped characters) is found in the dictionary
      if(dictionary.Contains(copier)){
        // Print out the possible correction
        cout <<"** "<<word <<" -> " <<copier << " ** " <<"case C" <<endl;
      }

      //WRONG LOGIC USED HERE ! FIXED WITH copier = word 
      // If the string(with newly swapped characters) is not found in the dictionary
      // Then reswap the characters back to their original position and try the next two adjacent characters
      // else{
      //   // Reswapping the characters back to their originial position
      //   swap(word[i], word[i+1]);
      //   //swap(copier[i], copier[i+1]);
      // } 
  }     
}

// Function mainly reads input as words and checks the spelling of each word
void checkSpelling(const string& document_filename, const string& dictionary_filename)
{
  // Create a hash table (implementing quadratic probing) to store the dictionary
  HashTable<string>quadratic_dictionary;
  HashTable<string>misspelled_words;        //Hash table containing misspelled words (prevents redisplaying misspelled words)

  // Read the dictionary and insert all of the words in the hash table
  ifstream dictionary(dictionary_filename);
  string word_in_dictionary;     // Store each word in the dictionary

  // While each word in the dictonary file is being extracted
  while(getline(dictionary, word_in_dictionary)){
    // Store the word inside the hash table (acting as a dictonary)
    quadratic_dictionary.Insert(word_in_dictionary);
  }

  // Read the document file that will interface with the dicitonary
  ifstream document_text(document_filename);
  string document_line;     // Store each line from the document

  while(getline(document_text, document_line)){
    stringstream string_parser;           // Create stringstream object to parse a line
    string_parser.str(document_line);     // Make sure object contains the line (from the document) to parse at the space
    string document_word;                 // When a parsed part is obtained, store it in this string

    // Each word from string_parser (which contains the extracted line) is piped into document_word
    while(string_parser >> document_word)
    {
  
      for(size_t i = 0; i < document_word.size(); i++)
        document_word[i] = tolower(document_word[i]);

      // Ensure no punctuation marks (in the beginning of the string like quotation marks)
      if(ispunct(document_word[0])){
        document_word = document_word.substr(1);
      }
      // Ensure no punctation marks (at the end of the string like periods or quotation marks)
      if(ispunct(document_word[document_word.size()-1])){
        document_word.pop_back();
        // Ensure there are not two punctation marks in a row at the end
        if(ispunct(document_word[document_word.size()-1])){
          document_word.pop_back();
        }
      }
      //cout<<document_word<<endl;
      //DEBUG LINE

      // Check if the newly formatted word is possibly in the dictionary
      // Also check if the string was never separated (never contained hyphens)
      if(!quadratic_dictionary.Contains(document_word)){ //&& before_string == "" && after_string == ""){
        
        //To prevent a misspelled word being display several times, ensure that the word and its corrections are displayed once
        if(!misspelled_words.Contains(document_word))
        {
          cout << document_word << " is INCORRECT"<< endl;
        
          
   
          AddingOneChar(document_word, quadratic_dictionary);
          RemovingOneChar(document_word, quadratic_dictionary);
          SwappingAdjacentChars(document_word, quadratic_dictionary);
          
          misspelled_words.Insert(document_word);
          //cout << endl; 
        }
      }
      else if(quadratic_dictionary.Contains(document_word)){
        cout<<document_word<<" is CORRECT"<<endl;
      }
    }
  }
}

int testSpellingWrapper(int argument_count, char** argument_list) {

  const string document_filename(argument_list[1]);
  const string dictionary_filename(argument_list[2]);
  
  //cout << "Input document filename is " << document_filename << endl;
  //cout << "Input dictionary filename is " << dictionary_filename << endl;
  checkSpelling(document_filename, dictionary_filename);

  

}

// Sample main for program spell_check.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE MAIN.
// WE WILL DIRECTLY CALL testSpellingWrapper. ALL FUNCTIONALITY SHOULD BE THERE.
// This main is only here for your own testing purposes.
int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>" << endl;
    return 0;
  }
  
  testSpellingWrapper(argc, argv);

  return 0;
}
