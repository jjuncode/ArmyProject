#pragma once
#include "../struct.h"
#include "Object.h"

class RectObject : public Object{
    public:
        RectObject(Vec2 _pos,Vec2 _scale) : Object(_pos,_scale){
            AddScene<RectObject>();
            CreateVertex(_pos, _scale);
        }

        void CreateVertex(Vec2 _pos,Vec2 _scale) override;
};