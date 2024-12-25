#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "Core.h"

std::list<std::list<std::unique_ptr<Object>>*> Scene::m_mgr_obj{};

void Scene::Init()
{
	auto circle = Object::CreatePtr<Circle>();
	circle->SetPos(sf::Vector2f(50.f, 50.f));
	Scene::Attach(circle);
}

void Scene::Update()
{
	for (const auto& obj_list : m_mgr_obj) {
		// 각 리스트에 대해
		for (const auto& obj : *obj_list) {
			obj->Update();
		}
	}

	// 죽은애 삭제 
	DeleteDead();
}

void Scene::Render()
{
	auto window = Core::GetWindowContext();
	window->clear();

	for (const auto& obj_list : m_mgr_obj) {
		// 각 리스트에 대해
		for (const auto& obj : *obj_list) {
			obj->Render();
		}
	}

	window->display();
}

void Scene::ShowList()
{
	for (const auto& obj_list : m_mgr_obj) {
		if (!obj_list->empty())
			// 각 리스트에 대해
			// 각 리스트의 객체 개수를 출력
			std::cout << typeid(*obj_list->begin()->get()).name() << "의 개수 : " << obj_list->size() << std::endl;
	}
}

void Scene::DeleteDead()
{
	for (auto mgr_iter = m_mgr_obj.begin(); mgr_iter != m_mgr_obj.end(); ++mgr_iter) {
		auto& obj_list = *mgr_iter;
		obj_list->erase(std::remove_if(obj_list->begin(), obj_list->end(), [](auto& x) {return x->IsDead(); }), obj_list->end());
		if (!obj_list->size()) {
			// 요소가 0이라면 리스트에서 삭제
			if (std::next(mgr_iter) == m_mgr_obj.end()) {
				// 이 요소가 마지막 요소라면
				mgr_iter = m_mgr_obj.erase(mgr_iter);
				mgr_iter = --m_mgr_obj.end();	// 강제 종료 조건
			}
			else
				mgr_iter = m_mgr_obj.erase(mgr_iter);

		}
	}
}