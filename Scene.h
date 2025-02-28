#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "Component/Component.h"
#include "Object/Entity.h"

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
	
	// ================================
	// Entity Method
	// ================================
public:
	template <typename T>
	auto& GetEntityVector(){
		static std::vector<uint32_t> vec_entity_id{};
		return vec_entity_id;
	}

	template<typename T> 
	void AddEntity(const Entity& _entity){
		auto& vec = GetEntityVector<T>();
		vec.emplace_back(_entity.GetEntityID());	// entity의 자신 id를 return하도록
	}

	// ================================
	// Component Method 
	// ================================
private:
	template <typename T>
	auto &AccessComponentMap()
	{
		static std::unordered_map<uint32_t, std::weak_ptr<T>> map_component{};
		return map_component;
	}

public:
	template<typename T>
	void AddComponent(const std::shared_ptr<T>& _comp){
		auto idx{_comp->GetID()}; 

		// size setting 
		if (m_vec_component.capacity() <= idx)
			m_vec_component.reserve(idx + 1 * 2);

		if (idx + 1 <= m_vec_component.size()){ 
			// 만약 넣으려는 요소가 앞쪽이라면
			// 기존것은 삭제되고 새로 대체된다.
			m_vec_component[idx] = _comp;

			// 기본으로 활성화
			m_vec_status[idx] = EntityStatus::kActive;
		}
		else{
			// 아니면 그냥 insert
			m_vec_component.emplace_back(_comp);
			m_vec_status.emplace_back(EntityStatus::kActive);
		}

		if (m_vec_status.capacity() <= idx)
			m_vec_status.reserve(idx + 1 * 2);

		// 기본으로 활성화	
		m_vec_status[idx] = EntityStatus::kActive;	

		auto& map = AccessComponentMap<T>();
		map[_comp->GetOwnerID()] = _comp;
	}	
	
	template<typename T>
	std::shared_ptr<T> GetComponentOrigin(const uint32_t& _owner_id){
		auto& map_component = AccessComponentMap<T>();

		auto iter {map_component.find(_owner_id)};
		if ( iter != map_component.end()){
			// Find
			auto comp = iter->second.lock();
			auto id = comp->GetID();
			if ( m_vec_status[id] == EntityStatus::kActive ) 
				return comp;
			else if ( m_vec_status[id] == EntityStatus::kDead ) 
				return nullptr;

		}

		return nullptr;
	} 

	void DeleteComponent(std::shared_ptr<Component>&& _comp) noexcept;
};