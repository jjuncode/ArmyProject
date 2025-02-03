#pragma once
#include <vector>
#include <memory>
#include "Component.h"
#include "Entity.h"

class Scene
{
private:
	std::vector<std::unique_ptr<Component>> m_vec_component;

public:
	Scene() {}
	virtual ~Scene() = default;

	// =====================
	// Object function
	// =====================
	virtual void Init();
	virtual void Update(float dt);		// 임시로 오브젝트를 업데이트 돌리기 
	virtual void Render();				// 임시로 오브젝트를 업데이트 돌리기 

	template<typename T>
	void AddComponent(){
		Entity obj;
		auto comp = obj.AddComponent<T>();
		m_vec_component.emplace_back(std::move(comp));
	}

	// =====================
	// Object Template
	// =====================

private:
	void DeleteDead();
};