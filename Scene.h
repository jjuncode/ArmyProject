#pragma once

class Scene
{
private:
	std::list<std::unique_ptr<Entity>> m_list_entity;

public:
	Scene() {}
	~Scene() {}

	// =====================
	// Object function
	// =====================
	void Init();
	void Update(float dt);		// 임시로 오브젝트를 업데이트 돌리기 
	void Render();		// 임시로 오브젝트를 업데이트 돌리기 

	// =====================
	// Object Template
	// =====================

private:
	void DeleteDead();
};