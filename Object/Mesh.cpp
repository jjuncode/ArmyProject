#include "Mesh.h"
#include <algorithm>

std::unordered_map<std::size_t, std::unique_ptr<Mesh>> Mesh::map_meshes;

void Mesh::CreateMesh(std::string&& _name
            , std::vector<Vertex>&& _vec
            ,std::vector<uint32_t>&& _idx)
{
    std::size_t key = std::hash<std::string>()(_name);
    
    if (map_meshes.find(key) != map_meshes.end()) {
        std::cerr << "Mesh already exists: " << _name << std::endl;
        return; // Mesh already exists
    } else {
        auto mesh = std::unique_ptr<Mesh>(new Mesh(std::move(_name), std::move(_vec), std::move(_idx)));
        mesh->CreateBound();
        
        map_meshes[key] = std::move(mesh);
        std::cout << "Creating mesh: " << _name << std::endl;
    }
}

const Mesh &Mesh::GetMesh(std::size_t _key)
{
    if ( map_meshes.find(_key) != map_meshes.end()) {
        auto p_mesh = map_meshes[_key].get();
        return *(p_mesh);
    }
    else{
          if ( _key == NO_KEY ) {
            // No Texture 
            static Mesh default_mesh{"NULL"};  // 기본 mesh
            return default_mesh;
        }
        else
            assert(false && "Mesh not found");
    }
}

const bool Mesh::IsValid() const
{
    if ( m_name == "NULL" ){
        return false;
    }
    return true;
}

void Mesh::CreateBound()
{
    // Create Sphere
    Vec4 sum{};

    for ( const auto& v : m_vertexs ) {
        sum += v.v;
    }

    auto center = (sum / static_cast<float>(m_vertexs.size())).ToVec3();

    Vertex point_max = (
        *std::max_element(m_vertexs.begin(), m_vertexs.end(), 
            [&](const Vertex& lhs, const Vertex& rhs) {
                return Vec::LengthSquare(lhs.v.ToVec3() - center) 
                    < Vec::LengthSquare(rhs.v.ToVec3() - center);
            })
        );

    m_bound_sphere.radius = Vec::Length(point_max.v.ToVec3() - center);
}