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

void Mesh::CreateMesh(std::string &&_name, const Mesh &_mesh)
{  
    std::size_t key = std::hash<std::string>()(_name);
    
    if (map_meshes.find(key) != map_meshes.end()) {
        std::cerr << "Mesh already exists: " << _name << std::endl;
        return; // Mesh already exists
    } else {
        const auto& _vec = _mesh.GetVertexs();
        const auto& _idx = _mesh.GetIndexs();

        auto mesh = std::unique_ptr<Mesh>(new Mesh(std::move(_name), _vec, _idx));
        mesh->CreateBound();

        map_meshes[key] = std::move(mesh);
        std::cout << "Creating mesh: " << _name << std::endl;
    }
}

void Mesh::SetTextureCoordinate(const std::vector<Vec2>&_vec)
{
    if (m_vertexs.size() != _vec.size()){
        assert(false && "Vertex count does not match texture coordinates");
    }

    for (size_t i = 0; i < _vec.size(); ++i){
        m_vertexs[i].uv = _vec[i];
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

const Mesh &Mesh::GetMesh(std::string _name)
{
    std::size_t _key = std::hash<std::string>()(_name);

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

    // Create Box
    Vec3 min{}, max{};
    for ( const auto& v : m_vertexs){
        min.x = std::min(min.x, v.v.x);
        min.y = std::min(min.y, v.v.y);
        min.z = std::min(min.z, v.v.z);
        
        max.x = std::max(max.x, v.v.x);
        max.y = std::max(max.y, v.v.y);
        max.z = std::max(max.z, v.v.z);
    }

    m_bound_box.min = min;
    m_bound_box.max = max;
}