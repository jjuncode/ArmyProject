#include "ColliderComponent.h"
#include "../Mgr/SceneMgr.h"
#include "../Core.h"
#include "TransformComponent.h"

void ColliderComponent::Init(CollisionEntityType _type)
{
    auto& cur_scene = SceneMgr::GetCurScene();

    m_type = _type;
    auto id = GetOwnerID();

    cur_scene->AddCollisionEntity(_type, GetOwnerID());
}

ColliderComponent::~ColliderComponent()
{
   SceneMgr::GetCurScene()->DeleteCollisionEntity(m_type, GetOwnerID());
}

void ColliderComponent::Render()
{
    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
    if (transform){
        auto pos = transform->GetPos();
        auto scale = transform->GetScale();

        sf::RectangleShape rect(sf::Vector2f(1, 1));
        rect.setScale(scale.x, scale.y);
        rect.setPosition(pos.x, pos.y);            // 위치 설정
        rect.setFillColor(sf::Color::Transparent); // 내부 색상을 투명으로
        rect.setOutlineThickness(0.1f);             // 테두리 두께 설정
        rect.setOutlineColor(sf::Color::Green);    // 테두리 색상 설정

        auto window = Core::GetWindowContext();
        window->draw(rect);
    }
}

void ColliderComponent::Collision(uint32_t coll_entity_id)
{
    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
    auto pos = transform->GetPos();
    auto scale = transform->GetScale();
    std::cout << "COLLISON OCCUR ID : " << GetID() << std::endl;
    std::cout << "COLLISON OCCUR OPPONENT ENTITY ID : " << coll_entity_id << std::endl;
}
