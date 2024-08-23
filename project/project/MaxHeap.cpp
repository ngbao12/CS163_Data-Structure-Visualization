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
