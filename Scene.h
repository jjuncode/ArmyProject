#pragma once
#include "pch.h"
#include "Component/Component.h"
#include "Script/Script.h"
#include "Object/Entity.h"

enum class CollisionEntityType {
    kPlayer = 0,
    kBox,
    
    kEND
};


class Scene
{
protected:
	std::vector<std::shared_ptr<Component>> m_vec_component;
	std::vector<std::shared_ptr<Script>> m_vec_script;	// script component vector
	
	std::vector<EntityStatus> m_vec_entity_status;	// entity 상태 vector
	uint32_t m_main_camear_id{};					// main camera id
	
	private:
	// Entity Components Map
	std::unordered_map<uint32_t, std::vector<uint32_t>> m_map_entity_components_id;	
	
	// Entity Scripts Vec
	std::vector<int> m_vec_script_id;	// script id vector ( idx == entity id )

	// Collision Entity Map
	std::unordered_map<CollisionEntityType, std::list<uint32_t>> m_map_collision_entity;  

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
	virtual void Update(float dt);		
	virtual void Execute(float dt);	// script execute
	virtual void Render();				

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
	}

	void DeleteCollisionEntity(CollisionEntityType _type, const uint32_t& entity_id) noexcept;

	// ================================
	// Entity Method
	// ================================
public:
	void AddEntity(uint32_t _entity_id) noexcept;
	
	const auto& GetComponentsID(const uint32_t _owner_id){
		return m_map_entity_components_id[_owner_id];
	}

	EntityStatus GetEntityStatus(uint32_t entity_id ){
		return m_vec_entity_status[entity_id];
	}

	void DeleteEntity(uint32_t entity_id) noexcept;

	// ================================
	// Component Method 
	// ================================
private:
	template <typename T>
	auto &AccessComponentMap()
	{
		static std::unordered_map<uint32_t, std::weak_ptr<T>> map_component{};	// 각 자료형별 Component Map ( 순차적으로 되도록 Entity_id 로 저장 )
		return map_component;
	}

public:
	template<typename T>
	void AddComponent(const std::shared_ptr<T>& _comp)
	{
		auto idx{_comp->GetID()}; 
		auto owner_id{_comp->GetOwnerID()};

		// size setting 
		if (m_vec_component.capacity() <= idx)
			m_vec_component.reserve(idx + 1 * 2);

		if (idx + 1 <= m_vec_component.size()){
			// 만약 넣으려는 요소가 앞쪽이라면
			// 기존것은 삭제되고 새로 대체된다.
			m_vec_component[idx] = _comp;
		}
		else{
			// 아니면 그냥 insert
			m_vec_component.emplace_back(_comp);
		}

		auto& map = AccessComponentMap<T>();
		map[owner_id] = _comp;
		m_map_entity_components_id[owner_id].emplace_back(idx);
	}

	template<typename T>
	std::shared_ptr<T> GetComponent(const uint32_t& _owner_id){
		auto& map_component = AccessComponentMap<T>();

		auto iter {map_component.find(_owner_id)};
		if ( iter != map_component.end()){
			// Find
			
			if ( !iter->second.expired()){
				auto comp = iter->second.lock(); // 죽었는가 
				auto id = comp->GetID();
				if (m_vec_entity_status[_owner_id] == EntityStatus::kActive)
					return comp;
				else if (m_vec_entity_status[_owner_id] == EntityStatus::kDead)
					return nullptr;
			}
		}

		return nullptr;
	} 

	void DeleteComponent(uint32_t _comp_id) noexcept;

	// ==============================
	// Script Method
	// ==============================
	private:
	template <typename T>
	auto &AccessScriptMap()
	{
		static std::unordered_map<uint32_t, std::weak_ptr<T>> map_script{};
		return map_script;
	}

	public:

	template<typename T>
	void AddScript(const std::shared_ptr<T>& _script){
		auto idx{_script->GetID()}; 
		auto owner_id{_script->GetOwnerID()};

		// size setting 
		if (m_vec_script.capacity() <= idx)
			m_vec_script.reserve(idx + 1 * 2);

		if (idx + 1 <= m_vec_script.size()){
			// 만약 넣으려는 요소가 앞쪽이라면
			// 기존것은 삭제되고 새로 대체된다.
			m_vec_script[idx] = _script;
		}
		else{
			// 아니면 그냥 insert
			m_vec_script.emplace_back(_script);
		}

		auto& map = AccessScriptMap<T>();
		map[owner_id] = _script;


		// size setting 
		if (m_vec_script_id.capacity() <= owner_id)
			m_vec_script_id.reserve(owner_id + 1 * 2);

		// Fill Empty Space
		while (m_vec_script_id.size() <= owner_id)
			m_vec_script_id.emplace_back(-1);

		m_vec_script_id[owner_id] = (idx);
	}

	std::shared_ptr<Script>& GetScript(const uint32_t _script_id){
		return m_vec_script[_script_id];
	}
	
	template<typename T>
	std::shared_ptr<T> GetScript(const uint32_t& _owner_id){
		auto& map_script = AccessScriptMap<T>();

		auto iter {map_script.find(_owner_id)};
		if ( iter != map_script.end()){
			// Find
			
			if ( !iter->second.expired()){
				auto script = iter->second.lock(); // 죽었는가 
				auto id = script->GetID();
				if (m_vec_entity_status[_owner_id] == EntityStatus::kActive)
					return script;
				else if (m_vec_entity_status[_owner_id] == EntityStatus::kDead)
					return nullptr;
			}
		}

		return nullptr;
	} 

	int GetScriptID(const uint32_t& _owner_id);
	
	void DeleteScript(uint32_t _script_id) noexcept;

	// ================================
	// Camera Method 
	// ================================
	void SetMainCamera(uint32_t _id) noexcept{
		m_main_camear_id = _id;
	}
	uint32_t GetMainCamera() noexcept{
		return m_main_camear_id;
	}

};