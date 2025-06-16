#pragma once
#include "Component.h"
#include "../struct.h"
#include <vector>
#include <initializer_list>

class TransformComponent : public Component{
    private:
        Vec2 m_pos;
        Vec2 m_scale;
        float m_rotate;

    public:
        TransformComponent() : m_pos{0, 0}, m_scale{1, 1}, m_rotate{0} {}

        void Update(float dt) override;

        Vec2 GetPos() { return m_pos; }
        void SetPos(const Vec2& _pos){
            m_pos = _pos;
        }
        void AddPos(const Vec2& offset){m_pos += offset;}

        void AddRotate(float offset);
        float GetRotate() const { return m_rotate; }

        Vec2 GetScale(){return m_scale;}
        void SetScale(const Vec2 _scale) { m_scale =_scale;}

        const Mat3 GetModelMatrix() const;
};