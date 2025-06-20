#pragma once
#include "../struct.h"

class Renderer
{
    private:
        uint32_t m_id_owner;
        bool m_is_visible; // 렌더링 여부

    public:
        Renderer() : m_is_visible{true}{}
        void SetOwner(uint32_t _id) { m_id_owner = _id; }
        
        void SetVisible(bool _visible) { m_is_visible = _visible; }

        void Render();
};

void VertexShader(std::vector<Vertex>& _v, const Mat3& _matrix);