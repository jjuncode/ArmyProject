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

	for (const auto& obj : m_vec_object) {
        if (obj->GetStatus() == ObjectStatus::kActive)
            obj->Render();
	}

	window->display();
}

void Scene::Exit()
{
    m_vec_object.clear();
    m_vec_component.clear();
    m_map_collision_object.clear();
    m_main_camear_id = -1;
    for (auto& layer : m_collision_layer) {
        layer.reset();
    }
}

void Scene::SetCollisionLayer(CollisionObjectType l_type, CollisionObjectType r_type, bool check)
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

void Scene::DeleteCollisionObject(CollisionObjectType _type, const uint32_t &obj_id) noexcept
{
    auto& list = m_map_collision_object[_type];

    auto it = std::find(list.begin(), list.end(), obj_id);
    if (it != list.end()){
        list.erase(it);
    }

    std::cout << "DELETE Collision Object ID : " << obj_id << std::endl;
}

void Scene::DeleteComponent(uint32_t _comp_id) noexcept
{
    m_vec_component[_comp_id].reset();
}

void Scene::AddObject(std::unique_ptr<Object>&& _obj) noexcept
{
	auto idx{_obj->GetObjectID()}; 
    
    // size setting
    if (m_vec_object.capacity() <= idx)
        m_vec_object.reserve(idx + 1 * 2);

    if (idx + 1 <= m_vec_object.size()){
        // 만약 넣으려는 요소가 앞쪽이라면
        // 기존것은 삭제되고 새로 대체된다.
        m_vec_object[idx] = std::move(_obj);
    }
    else{
        // 아니면 그냥 insert
        m_vec_object.push_back(std::move(_obj));
    }
}

void Scene::DeleteObject(uint32_t _obj_id) noexcept
{
    auto& obj = m_vec_object[_obj_id];
    
    // Delete Component
    for ( auto& comp_id : obj->GetComponentsID()){
        m_vec_component[comp_id].reset();
    }

    // Delete Object
    m_vec_object[_obj_id].reset();
}

// int Scene::GetScriptID(const uint32_t& _owner_id)
// {
//     return -1;
// }