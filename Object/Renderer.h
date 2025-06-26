#pragma once
#include "../struct.h"
#include "../SFML/Graphics.hpp"

enum class DrawMode
{
    kWireFrame,
    kDefault_Shading,
    kDefault_Shading_None
};

class Renderer
{
private:
    static DrawMode m_draw_mode;
    uint32_t m_id_owner;
    bool m_is_visible; // 렌더링 여부
    bool m_is_shading; // Fragmentshader 적용여부

    sf::Color m_color; // rendering color

public:
    Renderer()
        : m_is_visible{true}, m_color{sf::Color::Black}, m_is_shading{true}
    {
    }
    void SetOwner(uint32_t _id) { m_id_owner = _id; }

    void SetVisible(bool _visible) { m_is_visible = _visible; }
    void SetColor(sf::Color _color) { m_color = _color; }
    void SetFragment(bool _v) { m_is_shading = _v; }
    static void SetDrawMode(DrawMode _v){m_draw_mode = _v;}

    void Render();
};

void VertexShader(std::vector<Vertex> &_v, const Mat4 &_matrix);
void FragmentShader(sf::Vertex &_point, Vec3 pos_object, Mat4 &_view_matrix_inv);