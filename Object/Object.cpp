#include "Object.h"

#include "../Mgr/SceneMgr.h"
#include "../Component/AnimationComponent.h"

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

void Object::SetAnimation(std::unique_ptr<Component> &&_comp)
{
    _comp->SetOwner(m_id);
    m_animation = std::move(_comp);
}

AnimationComponent* Object::GetAnimation()
{
    return static_cast<AnimationComponent*>( m_animation.get());
}

void Object::Init(Vec3 _pos, Vec3 _scale)
{
    m_transform = std::make_unique<Transform>();
    m_transform->SetOwner(m_id);
    m_transform->SetPos(_pos);
    m_transform->SetScale(_scale);

    m_renderer = std::make_unique<Renderer>();
    m_renderer->SetOwner(m_id);
}

Object::Object(Vec3 _pos, Vec3 _scale)
        : m_status{ObjectStatus::kActive}
        , m_transform{nullptr}
        , m_renderer{nullptr}
        , m_script{nullptr}
        , m_mesh_key{NO_KEY}
        , m_texture_key{NO_KEY}
        , m_animation{nullptr}
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
    if ( m_script)
        m_script->Execute(dt);
}

void Object::SetComponentID(std::unique_ptr<Component> &_comp)
{
    _comp->SetOwner(m_id);
    m_vec_component_id.emplace_back(_comp->GetID());
}