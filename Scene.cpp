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
	DeleteDead();
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

void Scene::DeleteDead()
{
	// for (auto iter = m_vec_component.begin(); iter != m_vec_component.end(); ++iter ){
	// 	std::remove_if(m_vec_component.begin(), m_vec_component.end(), [](auto& x) {return x->IsDead(); });
	// }
}