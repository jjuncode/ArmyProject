#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Core.h"
#include "Component.h"

// std::list<std::unique_ptr<Object>> Scene::m_list_obj{};

void Scene::Init()
{
}

void Scene::Update(float dt)
{
	// for (const auto& obj : m_list_obj) {
	// 	// 각 obj에 대해
	// 	obj->Update(dt);
	// }

	// 죽은애 삭제 
	DeleteDead();
}

void Scene::Render()
{
	auto window = Core::GetWindowContext();
	window->clear();

	// for (const auto& obj : m_list_obj) {
	// 	obj->Render();
	// }

	window->display();
}

void Scene::DeleteDead()
{
	for (auto iter = m_list_entity.begin(); iter != m_list_entity.end(); ++iter ){
		std::remove_if(m_list_entity.begin(), m_list_entity.end(), [](auto& x) {return x->IsDead(); });
	}
}