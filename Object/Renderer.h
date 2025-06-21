#pragma once
#include "../struct.h"
#include "../SFML/Graphics.hpp"

class Renderer
{
    private:
        uint32_t m_id_owner;
        bool m_is_visible; // 렌더링 여부
        bool m_is_shading;  // Fragmentshader 적용여부

        sf::Color m_color;  // rendering color 

    public:
        Renderer() 
            : m_is_visible{true}
            , m_color{sf::Color::Black}
            , m_is_shading{true}
        {}
        void SetOwner(uint32_t _id) { m_id_owner = _id; }

        void SetVisible(bool _visible) { m_is_visible = _visible; }
        void SetColor(sf::Color _color) { m_color = _color; }
        void SetFragment(bool _v){m_is_shading = _v;}

        void Render();
};

void VertexShader(std::vector<Vertex>& _v, const Mat3& _matrix);
void FragmentShader(sf::Vertex& _point, Vec2 pos_object, Mat3& _view_matrix_inv);