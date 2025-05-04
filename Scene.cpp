#include "pch.h"
#include "Scene.h"
#include "Core.h"
#include "Mgr/SceneMgr.h"
#include "Component/ColliderComponent.h"

void Scene::Init()
{
}

void Scene::Update(float dt)
{
}

void Scene::Execute(float dt)
{
}

void Scene::Render()
{
	auto window = Core::GetWindowContext();
	window->clear();

	for (const auto& comp : m_vec_component) {
        if ( comp ) 
            if (m_vec_entity_status[comp->GetOwnerID()] == EntityStatus::kActive)
                comp->Render();
	}

	window->display();
}

void Scene::Exit()
{
    m_vec_component.clear();
    m_vec_script.clear();
    m_vec_entity_status.clear();
    m_map_entity_components_id.clear();
    m_map_collision_entity.clear();
    m_vec_script_id.clear();
    m_main_camear_id = -1;
    for (auto& layer : m_collision_layer) {
        layer.reset();
    }
}

void Scene::SetCollisionLayer(CollisionEntityType l_type, CollisionEntityType r_type, bool check)
{
    int row{};
    int col{};

    if ( l_type < r_type ) {
        row = static_cast<int>(l_type);
        col = static_cast<int>(r_type);
    }
    else{
        row = static_cast<int>(r_type);
        col = static_cast<int>(l_type);
    }

    m_collision_layer[row][col] = check;
}

void Scene::DeleteCollisionEntity(CollisionEntityType _type, const uint32_t &entity_id) noexcept
{
    auto& list = m_map_collision_entity[_type];

    auto it = std::find(list.begin(), list.end(), entity_id);
    if (it != list.end()){
        list.erase(it);
    }

    std::cout << "DELETE Collision ENTITY ID : " << entity_id << std::endl;
}

void Scene::DeleteComponent(uint32_t _comp_id) noexcept
{
    // ID reset
    m_vec_component[_comp_id]->Delete();
    // Really Delete
    m_vec_component[_comp_id].reset();
}
void Scene::DeleteCollider(uint32_t entity_id) noexcept
{
	auto coll = SceneMgr::GetComponent<ColliderComponent>(entity_id);
    if (!coll) 
        return;
    auto type = coll->GetCollisionType();
    DeleteCollisionEntity(type, entity_id);
    DeleteComponent(coll->GetID());
}

void Scene::DeleteScript(uint32_t _script_id) noexcept
{
    // ID reset
    m_vec_script[_script_id]->Delete();
	// Really Delete
    m_vec_script[_script_id].reset();
}

void Scene::AddEntity(uint32_t _entity_id) noexcept
{
    // Entity Activate
    if (m_vec_entity_status.capacity() <= _entity_id)
        m_vec_entity_status.reserve( _entity_id + 1 * 2);

    if ( _entity_id < m_vec_entity_status.size()){
        // 만약 넣으려는 요소가 앞쪽이라면
        // 기존것은 삭제되고 새로 대체된다.
        m_vec_entity_status[_entity_id] = EntityStatus::kActive;
    }
    else{
        // 아니면 그냥 insert
        m_vec_entity_status.emplace_back(EntityStatus::kActive);
    }
}

void Scene::DeleteEntity(uint32_t entity_id) noexcept
{
    auto vec_components_id = GetComponentsID(entity_id);
    auto script_id = GetScriptID(entity_id);
    DeleteCollider(entity_id);

    // Delete Component
    for (auto& id : vec_components_id){
        DeleteComponent(id);
    }

    // Delete Script
    if ( Script::IsValid(script_id)){
        DeleteScript(script_id);
        m_vec_script_id[entity_id] = -1;
    }

    m_map_entity_components_id.erase(entity_id);
    m_vec_entity_status[entity_id] = EntityStatus::kDead;
    Entity::Dead(entity_id);	// Entity ID Reset
}

int Scene::GetScriptID(const uint32_t& _owner_id)
{
    if ( m_vec_script_id.size() <= _owner_id)
        return -1;
    return m_vec_script_id[_owner_id];
}