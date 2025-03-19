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
}

void Object::SetCollider(CollisionEntityType _type)
{ 
    auto& cur_scene = SceneMgr::GetCurScene();
    auto collider = obj.AddComponent<ColliderComponent>();
    collider->Init(_type);
    cur_scene->AddComponent<ColliderComponent>(std::move(collider));
}
