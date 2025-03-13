#include "Object.h"
#include "Entity.h"

#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/ColorComponent.h"
#include "../Component/ColliderComponent.h"

void Object::Init(Vec2 _pos, Vec2 _scale)
{
    auto& cur_scene = SceneMgr::GetCurScene();

    auto render = obj.AddComponent<RenderComponent>();
    cur_scene->AddComponent<RenderComponent>(std::move(render));
    
    auto transform = obj.AddComponent<TransformComponent>();
    transform->SetPos(_pos);
    transform->SetScale(_scale);
    cur_scene->AddComponent<TransformComponent>(std::move(transform));
}

void Object::CreateVertex()
{
}

void Object::SetCollider(CollisionEntityType _type)
{ 
    auto& cur_scene = SceneMgr::GetCurScene();
    auto collider = obj.AddComponent<ColliderComponent>();
    collider->Init(_type);
    cur_scene->AddComponent<ColliderComponent>(std::move(collider));
}

void Object::SetColor(sf::Color _color)
{
    auto& cur_scene = SceneMgr::GetCurScene();

    auto color = obj.AddComponent<ColorComponent>();
    color->SetColor(_color);
    cur_scene->AddComponent<ColorComponent>(std::move(color));
}

void Object::SetShape()
{
}
