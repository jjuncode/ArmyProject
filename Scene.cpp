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

void Scene::DeleteComponent(std::shared_ptr<Component>&& _comp) noexcept
{
    if ( !_comp) return;

    // ID reset
    _comp->Delete();

    // Really Delete
    _comp.reset();
}
void Scene::DeleteCollider(uint32_t entity_id) noexcept
{
	auto coll = SceneMgr::GetComponent<ColliderComponent>(entity_id);
    if (!coll) return;
    auto type = coll->GetCollisionType();
    DeleteCollisionEntity(type, entity_id);
    DeleteComponent(coll);
}

void Scene::DeleteScript(std::shared_ptr<Script> &&_script) noexcept
{
    if ( !_script) return;
    
    // ID reset
	_script->Delete();
    
	// Really Delete
	_script.reset();
}
