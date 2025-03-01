#include "pch.h"
#include "Scene.h"
#include "Core.h"

void Scene::Init()
{
}

void Scene::Update(float dt)
{
	for (const auto& obj : m_vec_component) {
		if (m_vec_status[obj->GetID()] == EntityStatus::kActive)
			obj->Update(dt);
	}
	// for (const auto& obj : m_list_obj) {
	// 	// 각 obj에 대해
	// 	obj->Update(dt);
	// }

	// 죽은애 삭제 
	// DeleteDead();
}

void Scene::Render()
{
	auto window = Core::GetWindowContext();
	window->clear();

	for (const auto& obj : m_vec_component) {
		obj->Render();
	}

	window->display();
}

void Scene::DeleteComponent(std::shared_ptr<Component>&& _comp) noexcept
{
	// Component dead in Loop
	m_vec_status[_comp->GetID()] = EntityStatus::kDead;

	_comp->Delete();

	// Really Delete
	_comp.reset();
}

