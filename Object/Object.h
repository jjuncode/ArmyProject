#pragma once
#include <vector>
#include <stack>
#include <memory>
#include "../struct.h"

#include "Transform.h"
#include "Renderer.h"
#include "../Script/Script.h"

class Component;

enum class CollisionObjectType {
    kPlayer = 0,
    kBox,
    
    kEND
};

enum class ObjectStatus{
	// Object 상태
	kActive,	
	kDeActive,
	kDead,
	END
};

class Object{
private:
    static std::stack<uint32_t> remain_id;
    uint32_t m_id;
    ObjectStatus m_status;

    std::unique_ptr<Transform> m_transform;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Script> m_script;

    std::size_t m_mesh_key;
    std::size_t m_texture_key;

    std::vector<uint32_t> m_vec_component_id; // component id vector

public:
    virtual ~Object(){remain_id.push(m_id);}
    void Render() const;
    void Execute(float dt) const;

    void SetComponentID(std::unique_ptr<Component>& _comp);
    void SetVisible(bool _visible) { m_renderer->SetVisible(_visible); }
    void SetTexutre(std::string &&_name);
    void SetMesh(std::string &&_name);
    void SetScript(std::unique_ptr<Script>&& _script);
    void SetComponentID(const uint32_t& _id) {m_vec_component_id.emplace_back(_id);}
    void SetColor(sf::Color _color){ m_renderer->SetColor(_color); }
    void SetShading(bool _v){ m_renderer->SetFragment(_v);}

    const uint32_t GetObjectID() const { return m_id; }
    const auto &GetStatus() const { return m_status; }
    auto &GetTransform() { return *(m_transform.get()); }
    auto &GetScript() { return *(m_script.get());}
    const auto &GetTextureKey() { return m_texture_key; };
    const auto &GetMeshKey() { return m_mesh_key; };
    const auto& GetComponentsID(){ return m_vec_component_id; }

    static std::unique_ptr<Object> CreateObject(Vec3 _pos = Vec3(), Vec3 _scale = Vec3()) {
        return std::unique_ptr<Object>(new Object(_pos, _scale));
    }

private:
    virtual void Init(Vec3 _pos, Vec3 _scale);
    Object(Vec3 _pos = Vec3(), Vec3 _scale = Vec3());
};