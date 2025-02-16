#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "Component.h"
#include "Entity.h"

class Scene
{
private:
	std::vector<std::shared_ptr<Component>> m_vec_component;

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

	void AddComponent(std::shared_ptr<Component>&& _comp) noexcept;
	void DeleteComponent(std::shared_ptr<Component>&& _comp) noexcept;

// private:
// 	template<typename T>
// 	std::shared_ptr<T> GetComponentOrigin(const uint32_t& _owner_id){
// 		static std::unordered_map<std::weak_ptr<T>> map_component{};
// 		auto comp {map_component.find(_owner_id)};
// 	}

public:
	template<typename T>
	const std::unique_ptr<T>& GetComponent(const uint32_t& _owner_id){
		// Component getter
		// Owner ID를 이용해서 Component를 가져와야함
		// 해당 type으로 접근이 가능해야한다.
		// Using T

	}
};