#pragma once
#include "../struct.h"
#include "Object.h"

class PlayerObject : public Object{
    public:
        PlayerObject(Vec2 _pos,Vec2 _scale) : Object(_pos, _scale ){
            AddScene<PlayerObject>();
            CreateVertex(_pos, _scale);
        }

        void CreateVertex(Vec2 _pos,Vec2 _scale) override;
};