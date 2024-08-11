#ifndef Graph_hpp
#define Graph_hpp

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

struct Edge {
    int src, rest;
    int weight;
};

class Graph {
private:
    int vertices;
    vector <vector <pair<int,int>>> adjList; // first dest, second weight
    void DFS(int v, vector<bool>& visited, vector <int>& components );
public:
    Graph(int vertices);
    
    void addEdge(int u, int v, int weight);
    void createRandom(int numVertices);
    void createFromFile(const string& filename);
    vector <vector <int>> findConnectedComponent();
    vector <int> dijkstra(int start);
    vector <Edge> prim(int start);
};

#endif /* Graph_hpp */
