#include "Object.h"

#include "../Mgr/SceneMgr.h"
#include "../Component/ColliderComponent.h"
#include "../Component/Texture.h"

std::stack<uint32_t> Object::remain_id{};

void Object::SetTexutre(std::string &&_name)
{
    m_texture_key = std::hash<std::string>()(std::move(_name));
}

void Object::SetMesh(std::string &&_name)
{
    m_mesh_key = std::hash<std::string>()(std::move(_name));
}

void Object::SetScript(std::unique_ptr<Script> &&_script)
{
    _script->SetOwner(m_id);
    m_script = std::move(_script);
}

void Object::Init(Vec2 _pos, Vec2 _scale)
{
    m_transform = std::make_unique<Transform>();
    m_transform->SetPos(_pos);
    m_transform->SetScale(_scale);

    m_renderer = std::make_unique<Renderer>();
}

Object::Object(Vec2 _pos, Vec2 _scale)
{
	static uint id_cnt{};	

    if ( !remain_id.size()){
        // Is Empty
        m_id = id_cnt++;
    }
    else{
        m_id = remain_id.top();
        remain_id.pop();
    }

    Init(_pos, _scale);
}

void Object::Render() const
{
    m_renderer->Render();
}

void Object::Execute(float dt) const
{
    m_script->Execute(dt);
}

void Object::SetCollider(CollisionObjectType _type, Vec2 _size)
{ 
    auto&& collider = Component::CreateComponent<ColliderComponent>(_type, _size);
    collider->Init();
    m_vec_component_id.emplace_back(collider->GetID());
    SceneMgr::AddComponent<ColliderComponent>(std::move(collider));
}
