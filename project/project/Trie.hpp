#ifndef Trie_hpp
#define Trie_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct NodeTrie {
    NodeTrie* child[26];
    bool isEndOfWord;
    
    NodeTrie();
    ~NodeTrie();
};

class Trie {
private:
    
    NodeTrie* root;
    bool remove(NodeTrie* node, const string& word, int depth);
    int charToIndex(char ch);
    
public:
    Trie();
    ~Trie();
    
    void createRandom(int numWords, int maxLength);
    void createFromFile(const string& filename);
    void insert(const string& word);
    void remove(const string& word);
    bool search(const string& word);
    
};


#endif /* Trie_hpp */
