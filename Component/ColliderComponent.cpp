#include "ColliderComponent.h"
#include "../Mgr/SceneMgr.h"
#include "../Core.h"

#include "TransformComponent.h"
#include "ColorComponent.h"

std::unordered_map<uint64_t, CollisionStatus> ColliderComponent::m_map_collision_status{};


void ColliderComponent::Init(CollisionEntityType _type)
{
    auto& cur_scene = SceneMgr::GetCurScene();

    m_collision_type = _type;
    auto id = GetOwnerID();

    cur_scene->AddCollisionEntity(_type, GetOwnerID());

    CreateEdge();
}

CollisionStatus ColliderComponent::GetCollisionStatus(uint32_t coll_entity_id)
{
    CollisionInfoID info;
    info.left = GetOwnerID();
    info.right = coll_entity_id;

    auto iter = m_map_collision_status.find(info.id);

    if ( iter == m_map_collision_status.end()){
        return CollisionStatus::kNone;
    }

    return iter->second;
}

void ColliderComponent::CollisionEnter()
{
}

void ColliderComponent::CollisionStay()
{
    auto color = SceneMgr::GetComponentOrigin<ColorComponent>(GetOwnerID());
    color->SetColor(sf::Color::Cyan);
}

void ColliderComponent::CollisionExit(uint32_t coll_entity_id)
{
    CollisionInfoID info;
    info.left = GetOwnerID();
    info.right = coll_entity_id;

    m_map_collision_status[info.id] = CollisionStatus::kNone;

    auto color = SceneMgr::GetComponentOrigin<ColorComponent>(GetOwnerID());
    color->SetColor(sf::Color::Yellow);
}

void ColliderComponent::CreateEdge()
{
    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());

    Vec2 start;
    Edge edge;
    
    int cnt{};
    for ( const auto& dot : transform->GetVertexs()){
        ++cnt;
        if(cnt == 1 ) {
            edge.start = dot;
            start = dot;
        }
        else{
            edge.end = dot;
            m_vec_entity_edge.emplace_back(edge);

            edge.start = dot;
        }
    }

    // Finish Edge
    Edge edge_end{edge.end, start};
    m_vec_entity_edge.emplace_back(edge_end);
}

ColliderComponent::~ColliderComponent()
{
   SceneMgr::GetCurScene()->DeleteCollisionEntity(m_collision_type, GetOwnerID());
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
    CollisionInfoID info;
    info.left = GetOwnerID();
    info.right = coll_entity_id;

    auto iter = m_map_collision_status.find(info.id);

    if ( iter == m_map_collision_status.end() || iter->second == CollisionStatus::kNone ) {
        // Collision Enter 
        m_map_collision_status[info.id] = CollisionStatus::kEnter;
        CollisionEnter();
    }
    else{
        auto collision_status = iter->second;
        if ( collision_status == CollisionStatus::kEnter){
            iter->second = CollisionStatus::kStay;
            CollisionStay();
        }
        else if ( collision_status == CollisionStatus::kStay){
            CollisionStay();
        }
    }


}
