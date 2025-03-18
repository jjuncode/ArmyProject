#pragma once
#include "Component.h"
#include "../struct.h"
#include <vector>
#include <initializer_list>

class TransformComponent : public Component{
    private:
        std::vector<Vec2> m_vec_vertex;
        Vec2 m_pos;
        Vec2 m_scale;

    public:
        void Update(float dt) override;

        Vec2 GetPos() { return m_pos; }
        void SetPos(const Vec2& _pos){
            m_pos = _pos;
        }

        Vec2 GetScale(){return m_scale;}
        void SetScale(const Vec2 _scale) { m_scale =_scale;}

        // =====================
        // Vertex Func 
        // =====================
        void SetVertexs(std::initializer_list<Vec2> _dots) {
            for (const auto& dot : _dots)
                m_vec_vertex.emplace_back(dot);
        }
        const auto& GetVertexs(){return m_vec_vertex;}

        void VertexMove(Vec2 v){
            for (auto& dot : m_vec_vertex){
                dot += v;
            }
        }
};