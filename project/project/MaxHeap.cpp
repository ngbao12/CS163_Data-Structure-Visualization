#include "MaxHeap.hpp"

void Max_Heap::drawEdge(Vector2 pos1, Vector2 pos2) {
    DrawLineEx(pos1, pos2, 2, NODE_COLOR);
}

void Max_Heap::drawNode(std::vector<Node> scene, int index, Font font) {
    int size =(int) scene.size();
    int left = 2*index + 1, right = 2*index + 2;
    if (left < size) drawEdge(scene[index].pos, scene[left].pos);
    if (right < size) drawEdge(scene[index].pos, scene[right].pos);
    DrawCircle(scene[index].pos.x, scene[index].pos.y, NODE_RADIUS, scene[index].color);
    if (scene[index].fontSize == 0) return;
    Vector2 textSize = MeasureTextEx(font, TextFormat("%d", scene[index].value), scene[index].fontSize, 0);
    DrawTextPro(font, TextFormat("%d", scene[index].value), {scene[index].pos.x - textSize.x/2, scene[index].pos.y - textSize.y/2}, ORIGIN, 0, scene[index].fontSize, 0, WHITE);
}

void Max_Heap::drawHeap(Step step, Font font) {
    int size =(int) step.nodes.size();
    for (int i=0; i<size; i++) {
        drawNode(step.nodes, i);
    }
}

void MaxHeap::saveStep(int index1, int index2, std::vector<int> lines, const std::string infor, std::string code, int type) {
    if (this->data.empty()) return;
    
    int size =(int) this->data.size();
    Vector2 pos = {1013,100};
    float dx = 50*log2(size), dy = 100;
    std::vector<Max_Heap::Node> nodes;
    nodes.push_back({this->data[0], pos, dx, CODE_SIZE, NODE_COLOR});
    
    for (int i=1; i<size; i++) {
        dx = nodes[(i-1)/2].dx;
        pos = nodes[(i-1)/2].pos;
        nodes.push_back({this->data[i], {pos.x + ((i & 1) ? (-dx) : dx), pos.y + dy}, dx/2, CODE_SIZE, NODE_COLOR});
    }
    
    if (type == 0 || type == 1 || type == 2) {
        nodes[index2].color = HIGHTLIGHT_NODE_COLOR_2;
        nodes[index1].color = HIGHTLIGHT_NODE_COLOR_1;
    }
    
    this->steps.push_back({nodes, lines, {index1, index2}, infor, code, type});
}


