#pragma once
#include "Object.h"
#include <vector>
#include <cmath>
#include "../struct.h"

class Polygon  : public Object{
private:
    int sides; // Number of sides (n)

public:
    // Constructor
    Polygon(uint32_t n,Vec2 _pos, Vec2 _scale) : sides(n), Object(_pos,_scale) {
        AddScene<Polygon>();
        CreateVertex(_pos, _scale);
    }

    void CreateVertex(Vec2 _pos,Vec2 _scale) override;
};