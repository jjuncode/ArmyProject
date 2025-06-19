#include "Object.h"

#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"
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

void Object::Init(Vec2 _pos, Vec2 _scale)
{
    auto& cur_scene = SceneMgr::GetCurScene();
    
    auto transform = CreateComponent<TransformComponent>();
    transform->SetPos(_pos);
    transform->SetScale(_scale);
    cur_scene.AddComponent<TransformComponent>(std::move(transform));

    auto render = CreateComponent<RenderComponent>();
    cur_scene.AddComponent<RenderComponent>(std::move(render));
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

void Object::DeadID(int _id)
{
    remain_id.push(_id);
}

void Object::SetCollider(CollisionObjectType _type, Vec2 _size)
{ 
    auto& cur_scene = SceneMgr::GetCurScene();
    auto collider = CreateComponent<ColliderComponent>(_type, _size);
    collider->Init();
    cur_scene.AddComponent<ColliderComponent>(std::move(collider));
}
