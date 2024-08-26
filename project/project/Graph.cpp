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

void Graph::initFromFile(const char* filename) {
    clearGraph();
    std::ifstream file(filename);
    int n;
    file >> n;

    for (int i = 0; i < n; ++i) {
        addNode();
    }
    
    int weight;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> weight;
            addEdge(i, j, weight);
        }
    }

    file.close();
}

void Graph::resetMark() {
    for(auto node : nodes) {
        for(auto edge : node->neighbors) {
            edge.highlight = false;
        }
        node->color = false;
    }
}
int Graph::connectedComponent() {
    resetMark();
    std::vector<bool> visited(nodes.size(), false);
    int currentColor = 0;
    for (int i = 0; i < nodes.size(); ++i) {
        if (!visited[i]) {
            std::queue<int> q;
            q.push(i);
            visited[i] = true;
            nodes[i]->color = currentColor;

            while (!q.empty()) {
                int node = q.front();
                q.pop();

                for (auto& edge : nodes[node]->neighbors) {
                    int neighborIndex = getNodeIndex(edge.neighborNode);
                    if (!visited[neighborIndex]) {
                        q.push(neighborIndex);
                        visited[neighborIndex] = true;
                        nodes[neighborIndex]->color = currentColor;
                    }
                }
            }
            currentColor++;
        }
    }
    return currentColor;
}
void Graph::mstKruskal() {
    resetMark();
    std::vector<std::tuple<int, int, int>> edges;
    
    for (int i = 0; i < nodes.size(); ++i) {
        for (auto& edge : nodes[i]->neighbors) {
            edges.push_back({ edge.weight, i, getNodeIndex(edge.neighborNode) });
        }
    }

    std::sort(edges.begin(), edges.end());

    std::vector<int> parent(nodes.size());
    std::vector<int> rank(nodes.size(), 0);

    for (int i = 0; i < nodes.size(); ++i) {
        parent[i] = i;
    }

    // Kruskal's algorithm
    for (auto& [weight, u, v] : edges) {
        if (findParent(parent, u) != findParent(parent, v)) {
            unionNodes(parent, rank, u, v);
            
            // Highlight the nodes and the edges in the MST
            nodes[u]->color = true;
            nodes[v]->color = true;

            for (auto& edge : nodes[u]->neighbors) {
                if (getNodeIndex(edge.neighborNode) == v) {
                    edge.highlight = true;
                    break;
                }
            }
            for (auto& edge : nodes[v]->neighbors) {
                if (getNodeIndex(edge.neighborNode) == u) {
                    edge.highlight = true;
                    break;
                }
            }
            nodes[u]->color = true;
        }
    }
}
