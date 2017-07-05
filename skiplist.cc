#include <iostream>
#include <fstream>
#include <sstream>
#include <string>       // provides string class
#include <cctype>       // provides isalpha() and tolower()
#include <algorithm>
using namespace std;
#include "skiplist.h"
// The maximum height of nodes in this project
const int MAX_HEIGHT = 16;
// Global array to record nodes visited for find and insert
vector<Node*> update(MAX_HEIGHT);

// The following function should be moved into your skiplist.cc file when you
// write that.  It implements the random bit generation.
#include <ctime>                // for time()
#include <cstdlib>              // for rand(), srand(), and RAND_MAX


int randBit(void) {             // return a "random" bit
  static int bitsUpperBd=0;
  static int bits;              // store bits returned by rand()
  if( bitsUpperBd == 0 ) {      // refresh store when empty
    bitsUpperBd = RAND_MAX;
    bits = rand();
  }
  int b = bits & 1;
  bits >>= 1;
  bitsUpperBd >>= 1;
  return b;
}


// Remove all characters except letters (A-Z,a-z) from line,
// except keep '-' or '\'' if they are between letters.
void lowercaseWords(string & line) {
  for( string::iterator it = line.begin(); it != line.end(); ++it ) {
    if( !isalpha(*it) ) {
      if( (*it != '-' && *it != '\'') ||
          it == line.begin() || it+1 == line.end() ||
          !isalpha(*(it-1)) || !isalpha(*(it+1)) ) {
        *it = ' ';
      }
    } else {
      *it = tolower(*it);
    }
  }
}

// Look through the skip list to see if there's a node that has an entry
// containing the input word. If yes, return the node; otherwise return NULL.
// At the same time record every node last visited for each height value in the
// global array update[h] so it's easier to insert a new node if we can't
// find a node that contains the input word.
Node* find(string word, Node* head) {
  Node* node = head;

  for(int h = MAX_HEIGHT-1; h>=0; --h) {

    while((node->next[h] != NULL) &&
      (node->next[h]->entry.word < word)) {
      // Loop invariant: node->next[h]->entry.word <= word

          // skip the words (alphabetically) before the word
          // we're looking for
          node = node->next[h];
    }

    if((node->next[h] != NULL) &&
      (node->next[h]->entry.word.compare(word) == 0)) {
    //if the node that node->next[h] points has an entry containing the same word as
    //the input word
      return node->next[h]; //found!
    }

    update[h] = node; //record the last pointer visited for current h in index h
  }

  //not found!
  return NULL;
}

// Insert a new node into the skip list and update the pointers properly.
// The new node has a randomly generated height.
// The entry this new node contains has the given word,
// with # of occurrence = 1 and the given page number
// added to the pages array.
void insert(int page_number, string word) {
  int i, height;
  // To generate the random height.
  // Code taken directly from p1.pdf
  for(i = 1; i<MAX_HEIGHT; ++i){
    if (randBit()==1) break;
  }
  height = i;

  // This block is creating the node we want to insert.
  Node* node = new Node;
  vector<Node*> next_array(height, NULL);  //next array of this node
  node->next = next_array;
  Entry entry(word, 1, page_number);  //Entry of this node
  node->entry = entry;


  // Only pointers in the update array from index 0 to height need to be updated,
  // because the nodes visited at h>height will not be affected by this new
  // node to insert
  // Code taken from p1.pdf with minor changes
  for(int h = 0; h < height; ++h) {
    node->next[h] = update[h]->next[h]; //initialize (the to-be-inserted) node's next pointers
    update[h]->next[h] = node;          //update next pointers that change
  }
}

//Update the entry of node_found by adding the page_number and
//increasing the occurences by 1.
void updateNode(int page_number, Node* node_found){
  node_found->entry.occurrences = node_found->entry.occurrences + 1;
  node_found->entry.pages.push_back(page_number);
}


//Write the words, # of occurrences, and page numbers of every entry of every node
//to the output txt file
void printInOrder(ostream & output, Node* head) {
  Node* node = head;
  // Print everything except the head
  while(node->next[0]!= NULL) {
    node = node->next[0];
    Entry entry = node->entry;
    vector<int> pages = entry.pages;
    string page_numbers = processPage(pages);
    output << entry.word << " (" << entry.occurrences << ") " << page_numbers << endl;
  }
}

template <typename T>
  string NumberToString ( T Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }

// This function processes the pages of each entry and produces a single string like
// "1-5, 8" with consecutive page numbers condensed so that it's easier to print out the result
string processPage(vector<int> pages) {
  int previous_page = pages[0];
  string result = NumberToString(previous_page);
  int current_page;
  bool consecutive = false;
  for(std::size_t i = 0; i < pages.size(); ++i) {
    current_page = pages[i];
    if (current_page - previous_page > 1) { //inconsecutive pages
      result = result.append(",");
      result = result.append(NumberToString(current_page));
      consecutive = false;
    }
    else if (current_page - previous_page == 1) { //this current page is the page after the previous one
      if(!consecutive){ //if before this the pages are inconsecutive
        //change the previous result string to include the range
        //of previous consecutive pages
      result = result.append("-");
      }
      else{ //before this the pages are consecutive
        result = result.substr(0, result.length() - 1); //trim the last consecutive page number out
      }
      result = result.append(NumberToString(current_page));
      consecutive = true;
    }
    previous_page = current_page;
  }
  return result;
}

int main(int argc, char *argv[]) {
  if( argc != 3 ) {
    cerr << "Usage: " << argv[0] << " input.txt output.txt" << endl;
    exit(1);
  }

  ifstream fin;
  fin.open(argv[1]);
  if( !fin.is_open() ) {
    cerr << "Unable to open " << argv[1] << endl;
    exit(2);
  }

  ofstream fout;
  fout.open(argv[2]);
  if( !fout.is_open() ) {
    cerr << "Unable to open " << argv[2] << endl;
    exit(3);
  }

  // Initialize the random number generator
  srand(time(0));

  // Create the skip list by creating a head.
  // The head contains an entry with entry->word = "",
  // number of occurrences = 0, and an empty pages array.
  // The head has height MAX_HEIGHT.
  Node* head = new Node;
  vector<Node*> next_array(MAX_HEIGHT, NULL);
  head->next = next_array;


  string line, word;
  int page_number = 1;
  int line_count = 0;
  while( !fin.eof() ) {
    getline(fin,line);
    lowercaseWords(line);
    istringstream iss(line, istringstream::in);

    //since there is 40 lines/page, this keeps track of
    //which page and which line on that
    //page we're processing
    if (line_count < 40) {
      line_count = line_count + 1;
    }
    else {
      line_count = 1;
      page_number = page_number + 1;
    }

    while( iss >> word ) {
      Node* node_found = find(word, head);
      if(node_found == NULL) insert(page_number, word); //not found, then insert
      else updateNode(page_number, node_found);  //found, then update the existing node
    }
  }
  fin.close();
  printInOrder(fout, head);
  fout.close();
}
