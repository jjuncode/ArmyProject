#pragma once
#include <vector>
#include <memory>
#include "Component.h"

class Scene
{
private:
	std::vector<std::unique_ptr<Component>> m_vec_component;

public:
	Scene() {}
	~Scene() {}

	// =====================
	// Object function
	// =====================
	void Init();
	void Update(float dt);		// 임시로 오브젝트를 업데이트 돌리기 
	void Render();				// 임시로 오브젝트를 업데이트 돌리기 

	// =====================
	// Object Template
	// =====================

private:
	void DeleteDead();
};