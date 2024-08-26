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

void Graph::drawGraph(Font font, std::vector<Color> color) {
    if(!color.empty()) {
        for (auto node : nodes) {
            for (auto edge : node->neighbors) {
                DrawLineEx(node->pos, edge.neighborNode->pos, 2, color[node->color]);
                Vector2 mid = { (node->pos.x + edge.neighborNode->pos.x) / 2, (node->pos.y + edge.neighborNode->pos.y) / 2 };
                DrawTextEx(font, TextFormat("%d", edge.weight), {mid.x, mid.y}, 15, 2, WHITE);
            }
        }
        for(int i = 0; i < nodes.size(); i++) {
            Vector2 size = MeasureTextEx(font, TextFormat("%d", i), 15, 2);
            DrawCircleV(nodes[i]->pos, 15, color[nodes[i]->color]);
            printf("Color: (%d, %d, %d, %d)\n", color[nodes[i]->color].r, color[nodes[i]->color].g, color[nodes[i]->color].b, color[nodes[i]->color].a);
            DrawTextEx(font, TextFormat("%d", i), {nodes[i]->pos.x - size.x/2, nodes[i]->pos.y - size.y/2}, 15, 2, BLACK);
        }
    } else {
        for (auto node : nodes) {
            for (auto edge : node->neighbors) {
                DrawLineEx(node->pos, edge.neighborNode->pos, 2, edge.highlight ? RED : NODE_COLOR);
                Vector2 mid = { (node->pos.x + edge.neighborNode->pos.x) / 2, (node->pos.y + edge.neighborNode->pos.y) / 2 };
                DrawTextEx(font, TextFormat("%d", edge.weight), {mid.x, mid.y}, 15, 2, WHITE);
            }
        }
        for(int i = 0; i < nodes.size(); i++) {
            Vector2 size = MeasureTextEx(font, TextFormat("%d", i), 15, 2);
            DrawCircleV(nodes[i]->pos, 15, nodes[i]->color ? RED : NODE_COLOR);
            DrawTextEx(font, TextFormat("%d", i), {nodes[i]->pos.x - size.x/2, nodes[i]->pos.y - size.y/2}, 15, 2, BLACK);
        }
    }
}

void Graph::updatePositions() {
    float repulsion_const = 500000.0f;
    float attraction_const = 0.1f;

    for (auto node : nodes) {
        Vector2 force = {0, 0};

        for (auto other : nodes) {
            if (node != other) {
                float dx = node->pos.x - other->pos.x;
                float dy = node->pos.y - other->pos.y;
                float dis = sqrt(dx * dx + dy * dy);

                if (dis > 0) {
                    float repulsion = repulsion_const / (dis * dis);
                    force.x += repulsion * dx / dis;
                    force.y += repulsion * dy / dis;
                }
            }
        }

        //Spring Force
        for (auto edge : node->neighbors) {
            float dx = node->pos.x - edge.neighborNode->pos.x;
            float dy = node->pos.y - edge.neighborNode->pos.y;
            float dis = sqrt(dx * dx + dy * dy);

            if (dis > 0) {
                float attraction = attraction_const * (dis - edge.weight);
                force.x -= attraction * dx / dis;
                force.y -= attraction * dy / dis;
            }
        }

        if(this->frameCounting > 2000) continue;
        
        node->pos.x += force.x;
        node->pos.y += force.y;

        node->pos.x = std::max(std::min(1450.f, node->pos.x), 500.f);
        node->pos.y = std::max(std::min(750.f, node->pos.y), 100.f);
        frameCounting++;
    }
}

GraphVisualize::GraphVisualize(Font font) {
    this->font = font;
    this->progressBar = ProgressBar(font);
    this->isCreateChosen = false;
    std::string infor = "";
    this->numComponent = 0;

    this->createButton = Button({25, 490, 110, 30}, "Create", -1, BLACK, 20, font);
    this->connectedComponentButton = Button({25, 535, 110, 30}, "C.Component", -1, BLACK, 20, font);
    this->mstKruskalButton = Button({25, 580, 110, 30}, "MST", -1, BLACK, 20, font);
    this->randomButton = Button({230, 585, 125, 30}, "Random", -1, BLACK, 20, font);
    this->loadFileButton = Button({230, 655, 125, 30}, "LoadFile", -1, BLACK, 20, font);
    this->inputEdges = InputStr(225, 535, 145, 25, "Edged", 20, this->font);
    this->inputNodes = InputStr(225, 500, 145, 25, "Nodes", 20, this->font);
}

std::vector<Color> GraphVisualize::generateRandomColors(int n) {
    std::vector<Color> colors;
    colors.reserve(n);

    for (int i = 0; i < n; ++i) {
        unsigned char r = GetRandomValue(0, 255);
        unsigned char g = GetRandomValue(0, 255);
        unsigned char b = GetRandomValue(0, 255);
        colors.push_back(Color{r, g, b, 255});
    }

    return colors;
}

void GraphVisualize::drawButton() {
    this->createButton.draw(50);
    this->connectedComponentButton.draw(50);
    this->mstKruskalButton.draw(50);

    if(this->isCreateChosen) {
        inputNodes.draw();
        inputNodes.update();
        inputEdges.draw();
        inputEdges.update();
        this->randomButton.draw();
        this->loadFileButton.draw();
    }
}

void GraphVisualize::drawGraph() {
    this->graph.drawGraph(this->font, this->colorComponent);
}

void GraphVisualize::draw() {
    drawSidebar("", {}, this->infor, this->progressBar, this->font);
    drawButton();
    drawGraph();
}

int GraphVisualize::handle() {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (this->createButton.getIsHovered()) {
            this->isCreateChosen = true;
            this->numComponent = 0;
            this->colorComponent.clear();
        }
        if (this->connectedComponentButton.getIsHovered()) {
            this->isCreateChosen = false;
            connectedComponent();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        if (this->mstKruskalButton.getIsHovered()) {
            this->isCreateChosen = false;
            this->numComponent = 0;
            this->colorComponent.clear();
            mstKruskal();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    if (this->randomButton.handle()) {
        randomize();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }
    if (this->loadFileButton.handle()) {
        loadFile();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }

    this->graph.updatePositions();

    return 0;
}

void GraphVisualize::randomize() {
    std::string v = this->inputNodes.getText();
    std::string e = this->inputEdges.getText();
    this->inputEdges.changePlaceHolder(e);
    this->inputNodes.changePlaceHolder(v);
    this->infor = TextFormat("Randomize graph with v = %s, e = %s", v.c_str(), e.c_str());
    this->graph.randomize(std::stoi(v), std::stoi(e), 100);
    this->progressBar.updateMaxStep(1);
    this->ProgressBar.updateStep(1);
}

void GraphVisualize::loadFile() {
    const char *path = tinyfd_openFileDialog("Open File", ".", 0, nullptr, nullptr, 0);
    this->infor = TextFormat("Load graph from file %s", path);
    this->graph.initFromFile(path);
    this->progressBar.updateMaxStep(1);
    this->progressBar.updateStep(1);
}

void GraphVisualize::connectedComponent() {
    this->infor = TextFormat("Find connected Component");
    this->numComponent = this->graph.connectedComponent();
    this->colorComponent = generateRandomColors(this->numComponent);
    this->progressBar.updateMaxStep(1);
    this->progressBar.updateStep(1);
}

void GraphVisualize::mstKruskal() {
    this->infor = TextFormat("Find minimun spanning tree with Kruskal algorithm");
    this->graph.mstKruskal();
    this->progressBar.updateMaxStep(1);
    this->progressBar.updateStep(1);
}
