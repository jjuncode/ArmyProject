#include "ColliderComponent.h"
#include "../Mgr/SceneMgr.h"
#include "../Core.h"

#include "TransformComponent.h"

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
    // auto color = SceneMgr::GetComponentOrigin<ColorComponent>(GetOwnerID());
    // color->SetColor(sf::Color::Cyan);
}

void ColliderComponent::CollisionExit(uint32_t coll_entity_id)
{
    // CollisionInfoID info;
    // info.left = GetOwnerID();
    // info.right = coll_entity_id;

    // m_map_collision_status[info.id] = CollisionStatus::kNone;

    // auto color = SceneMgr::GetComponentOrigin<ColorComponent>(GetOwnerID());
    // color->SetColor(sf::Color::Yellow);
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
    // auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
    // auto pos = transform ->GetPos();
    // for (const auto& edge : m_vec_entity_edge) {

    //     sf::Vertex line[] = {  
    //     sf::Vertex(sf::Vector2f(edge.start.x + pos.x, edge.start.y + pos.y)),
    //     sf::Vertex(sf::Vector2f(edge.end.x + pos.x , edge.end.y + pos.y))
    //     };

    //     auto window = Core::GetWindowContext();
    //     window->draw(line, 2, sf::Lines);
    // }
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
