#pragma once
#include "../struct.h"
#include "Object.h"

class PlayerObject : public Object{
    public:
        PlayerObject(Vec2 _pos,Vec2 _scale) : Object(_pos, _scale ){
            AddScene<PlayerObject>();
            SetColor(sf::Color::Red);
            SetShape();
            SetScript();
            CreateVertex(Vec2(0,0), _scale);
        }

        void SetShape() override;
        void SetScript() override;
        void CreateVertex(Vec2 _pos,Vec2 _scale) override;
};