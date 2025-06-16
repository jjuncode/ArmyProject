#pragma once
#include "../struct.h"
#include "Component.h"
#include <vector>

class Mesh : public Component{
private:
    std::vector<Vertex> m_vertexs;  // vertexs
    std::vector<uint32_t> m_indexs; // index

public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& _vec, const std::vector<uint32_t>& _idx)
        : m_vertexs{_vec} , m_indexs{_idx}{}
    Mesh(const Mesh& _mesh)
        : m_vertexs{_mesh.m_vertexs}, m_indexs{_mesh.m_indexs} {}

    const auto& GetVertexs() const { return m_vertexs; }
    const auto& GetIndexs() const { return m_indexs; }

    static Mesh Square;
};