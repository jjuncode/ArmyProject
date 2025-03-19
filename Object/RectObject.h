#pragma once
#include "../struct.h"
#include "Object.h"

class RectObject : public Object{
    public:
        RectObject(Vec2 _pos,Vec2 _scale) : Object(_pos,_scale){
            AddScene<RectObject>();
            CreateVertex(Vec2(0,0), _scale);
        }

        void SetScript() override;
        void CreateVertex(Vec2 _pos,Vec2 _scale) override;
};