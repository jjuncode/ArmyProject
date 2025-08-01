#pragma once
#include "pch.h"
#include "Component/Component.h"
#include "Script/Script.h"
#include "Object/Object.h"

class Scene
{
protected:
	std::vector<std::shared_ptr<Component>> m_vec_component;	// Components
	std::vector<std::unique_ptr<Object>> m_vec_object;			// object vector
	
	uint32_t m_main_camear_id{};					// main camera id
	std::vector<uint32_t> m_vec_light_id;				// light vector 

private:
	// Collision Object Map
	std::unordered_map<CollisionObjectType, std::list<uint32_t>> m_map_collision_object;  

	// Collision Layer
	std::bitset<static_cast<std::size_t>(CollisionObjectType::kEND)>
	m_collision_layer[static_cast<uint32_t>(CollisionObjectType::kEND)];


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
	void SetCollisionLayer(CollisionObjectType l_type, CollisionObjectType r_type, bool check);

	const auto& GetCollisionLayer(){return m_collision_layer;}

	const auto& GetCollisionObject(uint32_t _type ){
		auto type = static_cast<CollisionObjectType>(_type);
		return m_map_collision_object[type];
	}

	void AddCollisionObject(CollisionObjectType _type, uint32_t entity_id){
		auto& list = m_map_collision_object[_type];
		list.emplace_back(entity_id);
	}

	void DeleteCollisionObject(CollisionObjectType _type, const uint32_t& entity_id) noexcept;

	// ================================
	// Object Method
	// ================================
protected:
	void AddObject(std::unique_ptr<Object>&& _obj) noexcept;
	Object& GetObject(const uint32_t _obj_id) const noexcept{return *(m_vec_object[_obj_id].get());}
	bool IsActiveObject(uint32_t _obj_id) const noexcept{return m_vec_object[_obj_id]->GetStatus() == ObjectStatus::kActive;}
	void DeleteObject(uint32_t _obj_id) noexcept;
	void AddLight(std::unique_ptr<Object>&& _obj) noexcept;
	const auto& GetLightIDs() const noexcept{return m_vec_light_id;}

	// ================================
	// Component Method 
	// ================================
	template <typename T>
	auto &AccessComponentMap()
	{
		static std::unordered_map<uint32_t, std::weak_ptr<T>> map_component{};	// 각 자료형별 Component Map ( 순차적으로 되도록 Entity_id 로 저장 )
		return map_component;
	}
	template<typename T>
	void AddComponent(std::unique_ptr<T>&& _comp)
	{
		auto idx{_comp->GetID()}; 
		auto owner_id{_comp->GetOwnerID()};

		  // size setting
		if (m_vec_component.size() <= idx)
			m_vec_component.resize(idx + 1 * 2);

		std::shared_ptr<T> comp = std::move(_comp);
		m_vec_component[idx] = comp;

		auto& map = AccessComponentMap<T>();
		map[owner_id] =	comp;
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
				if (m_vec_object[_owner_id]->GetStatus() == ObjectStatus::kActive)
					return comp;
				else if (m_vec_object[_owner_id]->GetStatus() == ObjectStatus::kDead)
					return nullptr;
			}
		}

		return nullptr;
	} 

	void DeleteComponent(uint32_t _comp_id) noexcept;

	// ================================
	// Camera Method 
	// ================================
	void SetMainCamera(uint32_t _id) noexcept{
		m_main_camear_id = _id;
	}
	uint32_t GetMainCamera() noexcept{
		return m_main_camear_id;
	}

	friend class SceneMgr;
};