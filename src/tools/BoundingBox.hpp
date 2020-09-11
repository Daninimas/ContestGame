#pragma once
#include <vector>

struct BoundingBox {
    float xLeft{ 0.f };
    float xRight{ 0.f };
    float yUp{ 0.f };
    float yDown{ 0.f };
};


struct BoundingBoxNode {
    explicit BoundingBoxNode() = default; // para seguir teniendo el constructor por defecto
    explicit BoundingBoxNode(float xLeft, float xRight, float yUp, float yDown) 
        : bounding{ xLeft, xRight, yUp, yDown } {}

    BoundingBox bounding;
    std::vector<BoundingBoxNode> childs;
};