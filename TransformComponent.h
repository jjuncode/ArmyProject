#pragma once
#include "Component.h"
#include "struct.h"

class TransformComponent : public Component{
    private:
        Vec2 m_pos;
        uint32_t m_scale;

    public:
        void Update(float dt) override;

        Vec2 GetPos() { return m_pos; }
        void SetPos(const Vec2& _pos){m_pos = _pos;}

        uint32_t GetScale(){return m_scale;}
        void SetScale(const uint32_t _scale) { m_scale =_scale;}
};