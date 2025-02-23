#pragma once
#include "Component.h"
#include "../struct.h"

class TransformComponent : public Component{
    private:
        Vec2 m_pos;
        Vec2 m_scale;

    public:
        void Update(float dt) override;

        Vec2 GetPos() { return m_pos; }
        void SetPos(const Vec2& _pos){m_pos = _pos;}

        Vec2 GetScale(){return m_scale;}
        void SetScale(const Vec2 _scale) { m_scale =_scale;}
};