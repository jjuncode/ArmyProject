#pragma once
#include <vector>
#include <memory>
#include "Component.h"
#include "Entity.h"

class Scene
{
private:
	std::vector<std::unique_ptr<Component>> m_vec_component;

	// Deactive Component
	std::vector<EntityStatus> m_vec_status;	// entity 상태 vector

public:
	Scene() {}
	virtual ~Scene() = default;

	// =====================
	// Object function
	// =====================
	virtual void Init();
	virtual void Update(float dt);		// 임시로 오브젝트를 업데이트 돌리기 
	virtual void Render();				// 임시로 오브젝트를 업데이트 돌리기 

	void AddComponent(std::unique_ptr<Component>&& _comp);
	void DeleteComponent(const std::unique_ptr<Component>& _comp);
	
	template<typename T>
	const std::unique_ptr<T>& GetComponent(const uint32_t& _owner_id){
		// Component getter
		// Owner ID를 이용해서 Component를 가져와야함
		// 해당 type으로 접근이 가능해야한다.
		// Using T

	};
};