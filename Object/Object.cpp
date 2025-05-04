#include "Object.h"
#include "Entity.h"

#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/ColliderComponent.h"

void Object::Init(Vec2 _pos, Vec2 _scale)
{
    auto& cur_scene = SceneMgr::GetCurScene();
    
    auto transform = obj.AddComponent<TransformComponent>();
    transform->SetPos(_pos);
    transform->SetScale(_scale);
    cur_scene->AddComponent<TransformComponent>(std::move(transform));

    auto render = obj.AddComponent<RenderComponent>();
    cur_scene->AddComponent<RenderComponent>(std::move(render));

    cur_scene->AddEntity(GetEntityID());
}

void Object::SetCollider(CollisionEntityType _type, Vec2 _size)
{ 
    auto& cur_scene = SceneMgr::GetCurScene();
    auto collider = obj.AddComponent<ColliderComponent>(_type, _size);
    collider->Init();
    cur_scene->AddComponent<ColliderComponent>(std::move(collider));
}
