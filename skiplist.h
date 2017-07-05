#include <string>      
#include <cctype>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>

// My structure for every word with the string of this word,
// number of occurrences, and page numbers.
struct Entry {
  string word;
  int occurrences;
  vector<int> pages;

  // Default constructor
  Entry(){
    word = "";
    occurrences = 0;
    vector<int> pages;
  }

  // Constructor with an input word, number of occurrences,
  // and page number to add.
  Entry(string w, int o, int page_number) {
    word = w;
    occurrences = o;
    pages.push_back(page_number);
  }


  
};

// Each node in the skip list contains an Entry
// and an array of next pointers
struct Node {  
	Entry entry;
	vector<Node*> next;
};


// function declarations
int randBit(void);
void lowercaseWords(string & line);
Node* find(string word, Node* head);
void insert(int page_number, string word);
void updateNode(int page_number, Node* node_found);
void printInOrder(ostream & output, Node* head);
string processPage(vector<int> pages);

