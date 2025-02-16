#include "pch.h"
#include "Scene.h"
#include "Core.h"

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

void Scene::AddComponent(std::unique_ptr<Component> &&_comp)
{
	auto idx{_comp->GetID()};
	if (idx + 1 <= m_vec_component.size())
	{ // id - 2 , 요소가 3개면 0 1 2
		// 만약 넣으려는 요소가 앞쪽이라면
		// 기존것은 삭제되고 새로 대체된다.
		m_vec_component[idx] = std::move(_comp);
	}
	else
		// 아니면 그냥 insert
		m_vec_component.emplace_back(std::move(_comp));

	// 기본으로 활성화
	if (m_vec_status.size() <= idx)
		m_vec_status.reserve(idx + 1 * 2);

	m_vec_status[idx] = EntityStatus::kActive;
}

void Scene::DeleteComponent(const std::unique_ptr<Component> &_comp)
{
	_comp->Delete();

	// Component deactivate in Loop
	m_vec_status[_comp->GetID()] = EntityStatus::kDeActive;
}

