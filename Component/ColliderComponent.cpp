#include "ColliderComponent.h"
#include "../Mgr/SceneMgr.h"
#include "../Core.h"

#include "TransformComponent.h"
#include "CameraScript.h"

std::unordered_map<uint64_t, CollisionStatus> ColliderComponent::m_map_collision_status{};


void ColliderComponent::Init(CollisionEntityType _type)
{
    auto& cur_scene = SceneMgr::GetCurScene();

    m_collision_type = _type;
    auto id = GetOwnerID();

    cur_scene->AddCollisionEntity(_type, GetOwnerID());
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

void ColliderComponent::CollisionEnter(CollisionInfoID _info)
{
    auto script = SceneMgr::GetComponentOrigin<ScriptComponent>(_info.left);
    
}

void ColliderComponent::CollisionStay(CollisionInfoID _info)
{
}

void ColliderComponent::CollisionExit(CollisionInfoID _info)
{
}

ColliderComponent::~ColliderComponent()
{
   SceneMgr::GetCurScene()->DeleteCollisionEntity(m_collision_type, GetOwnerID());
}

void ColliderComponent::Render()
{
    // OBB Render
    auto id_owner = GetOwnerID();	// self id
	auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(id_owner);

	auto camera_id = SceneMgr::GetMainCamera();
	auto camera_transform = SceneMgr::GetComponentOrigin<TransformComponent>(camera_id);
	auto camera_script = SceneMgr::GetComponentOrigin<CameraScript>(camera_id);

    auto pos = transform->GetPos();

    // Camera Zoom in/out
    auto scale_value = camera_script->GetZoomValue();

    // pos scaling
    pos *= scale_value;
    auto resolution = Core::GetWindowSize();
    auto camera_pos = (camera_transform->GetPos() * scale_value) - (resolution / 2);

    auto render_pos = pos - camera_pos;

    Vec2 left_top = (Vec2(0,0)-m_obb.width_half  + m_obb.height_half ) * scale_value;
    Vec2 right_bottom = ( m_obb.width_half - m_obb.height_half) * scale_value;
    Vec2 right_top = ( m_obb.width_half + m_obb.height_half) * scale_value;
    Vec2 left_bottom = (Vec2(0,0)-m_obb.width_half - m_obb.height_half) * scale_value;

    left_top += render_pos;
    right_bottom += render_pos;
    right_top += render_pos;
    left_bottom += render_pos;

    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(left_bottom.x ,left_bottom.y), sf::Color::Green),
        sf::Vertex(sf::Vector2f(left_top.x ,left_top.y), sf::Color::Green),

        sf::Vertex(sf::Vector2f(left_top.x ,left_top.y), sf::Color::Green),
        sf::Vertex(sf::Vector2f(right_top.x ,right_top.y), sf::Color::Green),

        sf::Vertex(sf::Vector2f(right_top.x ,right_top.y), sf::Color::Green),
        sf::Vertex(sf::Vector2f(right_bottom.x ,right_bottom.y), sf::Color::Green),

        sf::Vertex(sf::Vector2f(right_bottom.x ,right_bottom.y), sf::Color::Green),
        sf::Vertex(sf::Vector2f(left_bottom.x ,left_bottom.y), sf::Color::Green)
    };
  
    auto window = Core::GetWindowContext();
    window->draw(line, 8, sf::Lines);
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
        CollisionEnter(info);
    }
    else{
        auto collision_status = iter->second;
        if ( collision_status == CollisionStatus::kEnter){
            iter->second = CollisionStatus::kStay;
            CollisionStay(info);
        }
        else if ( collision_status == CollisionStatus::kStay){
            CollisionStay(info);
        }
    }


}
