#pragma once

class Object;

class Scene
{
private:
	static std::list <std::unique_ptr<Object>> m_list_obj;

public:
	Scene() {}
	~Scene() {}

	// =====================
	// Object function
	// =====================
	static void Init();
	static void Update(float dt);		// 임시로 오브젝트를 업데이트 돌리기 
	static void Render();		// 임시로 오브젝트를 업데이트 돌리기 

	// =====================
	// Object Template
	// =====================

private:
	static void DeleteDead();
};