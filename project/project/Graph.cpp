#include "Graph.hpp"

Graph::~Graph() {
    clearGraph();
}

void Graph::clearGraph() {
    for (int i = 0; i < int(nodes.size()); i++) {
        delete nodes[i];
    }
    this->nodes.clear();
    this->frameCounting = 0;
}

int Graph::findParent(std::vector<int>& parent, int node) {
    if (parent[node] != node)
        parent[node] = findParent(parent, parent[node]);
    return parent[node];
}

void Graph::unionNodes(std::vector<int>& parent, std::vector<int>& rank, int u, int v) {
    int root_u = findParent(parent, u);
    int root_v = findParent(parent, v);
    if (rank[root_u] > rank[root_v]) {
        parent[root_v] = root_u;
    } else if (rank[root_u] < rank[root_v]) {
        parent[root_u] = root_v;
    } else {
        parent[root_v] = root_u;
        rank[root_u]++;
    }
}

int Graph::getNodeIndex(GraphNode* node) {
    auto it = std::find(nodes.begin(), nodes.end(), node);
    if (it != nodes.end()) {
        return (int)std::distance(nodes.begin(), it);
    }
    return -1; // Return -1 if node is not found (should not happen)
}

void Graph::addNode(GraphNode* node) {
    nodes.push_back(node);
}

void Graph::addNode() {
    auto* node = new GraphNode();
    node->pos = {500.f + rand() % 950, 100.f + rand() % 650};
    addNode(node);
}

int Graph::addEdge(int from, int to, int weight) {
    if (from < nodes.size() && to < nodes.size() && from != to && weight != 0) {
        bool edgeExists = false;
        // Check if the edge already exists
        for (auto& edge : nodes[from]->neighbors) {
            if (edge.neighborNode == nodes[to]) {
                edgeExists = true;
                edge.weight = weight;
                break;
            }
        }
        for (auto& edge : nodes[to]->neighbors) {
            if (edge.neighborNode == nodes[from]) {
                edgeExists = true;
                edge.weight = weight;
                break;
            }
        }

        if (!edgeExists) {
            nodes[from]->neighbors.push_back({nodes[to], weight, 0});
            nodes[to]->neighbors.push_back({nodes[from], weight, 0});
            return 1;
        }
    }
    return 0;
}

void Graph::randomize(int nodeCount, int edgeCount, int maxWeight) {
    clearGraph();
    for (int i = 0; i < nodeCount; ++i) {
        addNode();
    }

    srand((int)time(nullptr));
    for (int i = 0; i < edgeCount;) {
        int from = rand() % nodeCount;
        int to = rand() % nodeCount;
        if (from != to) {
            int weight = rand() % maxWeight + 1;
            i += addEdge(from, to, weight);
        }
    }
}
