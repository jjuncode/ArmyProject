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
        CreateVertex(Vec2(0,0), _scale);
    }

    void SetScript() override;
    void CreateVertex(Vec2 _pos,Vec2 _scale) override;
};