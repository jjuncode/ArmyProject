#include "ColliderComponent.h"
#include "../Mgr/SceneMgr.h"
#include "../Core.h"
#include <cmath>

#include "../Script/CameraScript.h"

std::unordered_map<uint64_t, CollisionStatus> ColliderComponent::m_map_collision_status{};

void ColliderComponent::Init()
{
    SceneMgr::AddCollisionObject(m_collision_type, GetOwnerID());
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

void ColliderComponent::SetCollisionStatus(uint32_t coll_entity_id, CollisionStatus status)
{
    CollisionInfoID info;
    info.left = GetOwnerID();
    info.right = coll_entity_id;

    auto iter = m_map_collision_status.find(info.id);

    if ( iter == m_map_collision_status.end()){
        m_map_collision_status[info.id] = status;
    }
    else{
        iter->second = status;
    }
}

void ColliderComponent::CollisionEnter(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto script_id = SceneMgr::GetScriptID(GetOwnerID());
    if ( Script::IsValid(script_id) ) {
        auto script = SceneMgr::GetScript(script_id);
        script->ExecuteCollEnter(other_entity_id, _mtv,dt);
    }
}

void ColliderComponent::CollisionStay(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto script_id = SceneMgr::GetScriptID(GetOwnerID());
    if ( Script::IsValid(script_id) ) {
        auto script = SceneMgr::GetScript(script_id);
        script->ExecuteCollStay(other_entity_id,  _mtv,dt);
    }
}

void ColliderComponent::CollisionExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto script_id = SceneMgr::GetScriptID(GetOwnerID());
    if ( Script::IsValid(script_id) ) {
        auto script = SceneMgr::GetScript(script_id);
        script->ExecuteCollExit(other_entity_id, _mtv, dt);
    }
}

ColliderComponent::~ColliderComponent()
{
   SceneMgr::DeleteCollisionObject(m_collision_type, GetOwnerID());
}

void ColliderComponent::Render()
{
    // OBB Render
    auto& transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();

	auto camera_id = SceneMgr::GetMainCamera();
    auto& camera_transform = SceneMgr::GetObject(camera_id).GetTransform();
	auto camera_script = SceneMgr::GetScript<CameraScript>(camera_id);

    auto pos = transform.GetPos();

    // Camera Zoom in/out
    float scale_value = 1.0f;
    if (camera_script) 
        scale_value = camera_script->GetZoomValue();

    // pos scaling
    pos *= scale_value;
    auto resolution = Core::GetWindowSize();

    Vec2 camera_pos = Vec2(0, 0);
    camera_pos = (camera_transform.GetPos() * scale_value) - (resolution / 2);

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
        sf::Vertex(sf::Vector2f(left_bottom.x ,left_bottom.y), m_obb.m_color),
        sf::Vertex(sf::Vector2f(left_top.x ,left_top.y),        m_obb.m_color),

        sf::Vertex(sf::Vector2f(left_top.x ,left_top.y),        m_obb.m_color),
        sf::Vertex(sf::Vector2f(right_top.x ,right_top.y),      m_obb.m_color),

        sf::Vertex(sf::Vector2f(right_top.x ,right_top.y),      m_obb.m_color),
        sf::Vertex(sf::Vector2f(right_bottom.x ,right_bottom.y),m_obb.m_color),

        sf::Vertex(sf::Vector2f(right_bottom.x ,right_bottom.y),m_obb.m_color),
        sf::Vertex(sf::Vector2f(left_bottom.x ,left_bottom.y),  m_obb.m_color)
    };
  
    auto window = Core::GetWindowContext();
    window->draw(line, 8, sf::Lines);
}

void ColliderComponent::Collision(uint32_t coll_entity_id, MTV _mtv, float dt)
{
    CollisionInfoID info;
    info.left = GetOwnerID();   // self id
    info.right = coll_entity_id;

    auto iter = m_map_collision_status.find(info.id);

    if ( iter == m_map_collision_status.end() || iter->second == CollisionStatus::kNone  || iter->second == CollisionStatus::kExit){ 
        // Collision Enter 
        m_map_collision_status[info.id] = CollisionStatus::kEnter;
        CollisionEnter(coll_entity_id, _mtv,dt);
    }
    else{
        auto collision_status = iter->second;
        if ( collision_status == CollisionStatus::kEnter){
            iter->second = CollisionStatus::kStay;
            CollisionStay(coll_entity_id,_mtv, dt);
        }
        else if ( collision_status == CollisionStatus::kStay){
            CollisionStay(coll_entity_id,_mtv, dt);
        }
    }
}

void OBB::Rotate(float angle)
{
    // Rotate
    Vec2 width_new;
    width_new.x = width_half.x * cos(angle) - width_half.y * sin(angle);
    width_new.y = width_half.x * sin(angle) + width_half.y * cos(angle);

    Vec2 height_new;
    height_new.x = height_half.x * cos(angle) - height_half.y * sin(angle);
    height_new.y = height_half.x * sin(angle) + height_half.y * cos(angle);

    width_half = width_new;
    height_half = height_new;
}
