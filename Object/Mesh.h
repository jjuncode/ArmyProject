#pragma once
#include "../struct.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

class Mesh{
private:
    std::string m_name; // mesh name
    std::vector<Vertex> m_vertexs;  // vertexs
    std::vector<uint32_t> m_indexs; // index
    
    Sphere m_bound_sphere; // bound sphere
    Box m_bound_box; // bound box

    static std::unordered_map<std::size_t, std::unique_ptr<Mesh>> map_meshes;

public:
    Mesh(const std::string& _name, const std::vector<Vertex>& _vec = {}, const std::vector<uint32_t>& _idx = {})
        : m_name{_name}, m_vertexs{_vec} , m_indexs{_idx}{}
    Mesh(const Mesh& _mesh) :m_name{_mesh.m_name}, m_vertexs{_mesh.m_vertexs}, m_indexs{_mesh.m_indexs} {}

    static void CreateMesh(std::string&& _name, std::vector<Vertex>&& _vec
            ,std::vector<uint32_t>&& _idx); 

    const auto& GetVertexs() const { return m_vertexs; }
    const auto& GetIndexs() const { return m_indexs; }
    const auto& GetSphere() const { return m_bound_sphere; }
    const auto& GetBox() const { return m_bound_box; }

    static const Mesh& GetMesh(std::size_t _key);

    const bool IsValid() const;

    void CreateBound();
};