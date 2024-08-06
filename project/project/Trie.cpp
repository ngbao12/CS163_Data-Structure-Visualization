#include "Trie.hpp"

NodeTrie::NodeTrie() : isEndOfWord(false) {
    for (int i = 0; i < 26; i++) {
        child[i] = nullptr;
    }
}

NodeTrie::~NodeTrie() {
    for (int i=0; i < 26; i++) {
        if (child[i] != nullptr) {
            delete child[i];
        }
    }
}

Trie::Trie() {
    root = new NodeTrie();
}

Trie::~Trie() {
    delete root;
}

int Trie::charToIndex(char ch) {
    return ch - 'a';
}

void Trie::insert(const string& word) {
    NodeTrie* node = root;
    for (char ch : word) {
        int index = charToIndex(ch);
        if (node->child[index] == nullptr) {
            node->child[index] = new NodeTrie();
        }
        node = node->child[index];
    }
    node->isEndOfWord = true;
}

bool Trie::search(const string& word) {
    NodeTrie* node = root;
    for (char ch : word) {
        int index = charToIndex(ch);
        if (node->child[index] == nullptr) {
            return false;
        }
        node = node->child[index];
    }
    return node->isEndOfWord;
}

void Trie::createFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string word;
        while (getline(file,word)) {
            if (!word.empty()) {
                insert(word);
            }
        }
        file.close();
    }
}

void Trie::createRandom(int numWords, int maxLength) {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    for (int i=0; i<numWords; i++) {
        int length = rand() % maxLength + 1;
        string word;
        for (int i=0; i<length; i++) {
            word += 'a' + rand() % 26;
        }
        insert(word);
    }
}
