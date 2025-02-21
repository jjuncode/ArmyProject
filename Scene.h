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

private:
	template <typename T>
	auto&& AccessComponentMap()
	{
		static std::unordered_map<uint32_t, std::weak_ptr<T>> map_component{};
		return map_component;
	}

	template <typename T>
	void SetComponentMap(std::shared_ptr<T> &&rhs)
	{
		auto&& map_component = AccessComponentMap<T>();
		map_component[rhs->GetOwnerID()] = rhs;
	};

public:
	// void AddComponent(std::shared_ptr<Component>&& _comp) noexcept;
	template<typename T>
	void AddComponent(const std::shared_ptr<T>& _comp){
		auto idx{_comp->GetID()}; 
		if (idx + 1 <= m_vec_component.size())
		{ // id - 2 , 요소가 3개면 0 1 2
			// 만약 넣으려는 요소가 앞쪽이라면
			// 기존것은 삭제되고 새로 대체된다.
			m_vec_component[idx] = _comp;
		}	
		else
		// 아니면 그냥 insert
			m_vec_component.emplace_back(_comp);

		// 기본으로 활성화	
		if (m_vec_status.size() <= idx)
			m_vec_status.reserve(idx + 1 * 2);

		m_vec_status[idx] = EntityStatus::kActive;	

		auto&& map = AccessComponentMap<T>();
		map[_comp->GetOwnerID()] = _comp;
	}	
	
	template<typename T>
	std::shared_ptr<T> GetComponentOrigin(const uint32_t& _owner_id){
		auto&& map_component = AccessComponentMap<T>();
		auto iter {map_component.find(_owner_id)};
		
		if ( iter != map_component.end()){
			// Find
			auto comp {iter->second};
			return std::move(comp.lock());
		}
		else
			return nullptr;
	} 

	void DeleteComponent(std::shared_ptr<Component>&& _comp) noexcept;
};