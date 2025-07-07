#pragma once
#include "../struct.h"
#include "../SFML/Graphics.hpp"

enum class DrawMode
{
    kWireFrame,
    kDefault_Shading,
    kDefault_Shading_None,
    kDepthBuffer
};

class Renderer
{
private:
    static DrawMode m_draw_mode;
    static std::vector<std::vector<float>> m_vec_depth_buffer; // depth buffer

    uint32_t m_id_owner;
    bool m_is_visible; // 렌더링 여부
    bool m_is_shading; // Fragmentshader 적용여부

    sf::Color m_color; // rendering color

public:
    Renderer()
        : m_is_visible{true}, m_color{sf::Color::Black}, m_is_shading{true}
    {}
    void SetOwner(uint32_t _id) { m_id_owner = _id; }

    void SetVisible(bool _visible) { m_is_visible = _visible; }
    void SetColor(sf::Color _color) { m_color = _color; }
    void SetFragment(bool _v) { m_is_shading = _v; }
    static void SetDrawMode(DrawMode _v){m_draw_mode = _v;}
    
    void SetDepthBuffer(const Vec2& _v, float _depth);
    static void ClearDepthBuffer();
    
    void Render();
    
    static void CreateRenderingBuffer();
};

bool BackFaceCulling(std::array<Vertex, 3> _tri);
BoundValue FrustumCulling(const Frustum& _frustum, const Sphere& _sphere);
BoundValue FrustumCulling(const Frustum& _frustum, const Box& _box);

void VertexShader(std::vector<Vertex> &_v, const Mat4 &_matrix);
void VertexShader(std::array<Vertex,3> &_v, const Mat4 &_matrix);

void PrimitiveShader(std::array<Vertex,3> &_v, const Mat4& _proj);
void FragmentShader(sf::Vertex &_point, Vec3 pos_object, Mat4 &_view_matrix_inv);
