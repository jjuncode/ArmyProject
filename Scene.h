#pragma once
#include "pch.h"
#include "Component/Component.h"
#include "Object/Entity.h"

enum class CollisionEntityType {
    kPlayer = 0,
    kWall,
    
    kEND
};


class Scene
{
protected:
	std::vector<std::shared_ptr<Component>> m_vec_component;
	std::vector<EntityStatus> m_vec_status;	// entity 상태 vector

private:
	// Entity Components Map
	std::unordered_map<uint32_t, std::list<uint32_t>> m_map_entity_components_id;	

	// Collision Entity Map
	std::unordered_map<CollisionEntityType, std::vector<uint32_t>> m_map_collision_entity; // ㅇㅅㄲ가 문제임 

	// Collision Layer
	std::bitset<static_cast<std::size_t>(CollisionEntityType::kEND)>
	m_collision_layer[static_cast<uint32_t>(CollisionEntityType::kEND)];


public:	
	Scene() {}
	virtual ~Scene() = default;

	// =====================
	// Loop function
	// =====================
	virtual void Init();
	virtual void Update(float dt);		// 임시로 오브젝트를 업데이트 돌리기 
	virtual void Render();				// 임시로 오브젝트를 업데이트 돌리기

	virtual void Exit();

	// ======================
	// Collision Func
	// ======================
	void SetCollisionLayer(CollisionEntityType l_type, CollisionEntityType r_type, bool check);

	const auto& GetCollisionLayer(){
		return m_collision_layer;
	}

	const auto& GetCollisionEntity(uint32_t _type ){
		auto type = static_cast<CollisionEntityType>(_type);
		return m_map_collision_entity[type];
	}

	void AddCollisionEntity(CollisionEntityType _type, uint32_t entity_id){
		auto& list = m_map_collision_entity[_type];
		list.emplace_back(entity_id);
		std::sort(list.begin(), list.end());
	}

	void DeleteCollisionEntity(CollisionEntityType _type, const uint32_t& entity_id) noexcept;

	// ================================
	// Entity Method
	// ================================
public:
	template <typename T>
	auto& GetEntityVector(){
		static std::vector<uint32_t> vec_entity_id{}; 	// entity id vector 
		return vec_entity_id;
	}

	template<typename T> 
	void AddEntity(const Entity& _entity){
		auto& vec = GetEntityVector<T>();
		vec.emplace_back(_entity.GetEntityID());	// entity의 자신 id를 return하도록
	}

	const auto& GetComponentsID(const uint32_t _owner_id){
		return m_map_entity_components_id[_owner_id];
	}

	EntityStatus GetEntityStatus(uint32_t entity_id ){
		return m_vec_status[entity_id];
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

		if (m_vec_status.capacity() <= idx)
			m_vec_status.reserve(idx + 1 * 2);

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

		auto& map = AccessComponentMap<T>();
		map[_comp->GetOwnerID()] = _comp;
		m_map_entity_components_id[_comp->GetOwnerID()].emplace_back(idx);
	}	

	std::shared_ptr<Component>& GetComponent(const uint32_t _id){
		return m_vec_component[_id];
	}
	
	template<typename T>
	std::shared_ptr<T> GetComponentOrigin(const uint32_t& _owner_id){
		auto& map_component = AccessComponentMap<T>();

		auto iter {map_component.find(_owner_id)};
		if ( iter != map_component.end()){
			// Find
			
			if ( !iter->second.expired()){
				auto comp = iter->second.lock(); // 죽었는가 
				auto id = comp->GetID();
				if (m_vec_status[id] == EntityStatus::kActive)
					return comp;
				else if (m_vec_status[id] == EntityStatus::kDead)
					return nullptr;
			}
		}

		return nullptr;
	} 

	void DeleteComponent(std::shared_ptr<Component>&& _comp) noexcept;
};