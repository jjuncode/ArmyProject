#pragma once
#include <stack>
#include <memory>
#include "../Component/Component.h"

enum class EntityStatus{
	// Entity 상태
	kActive,	
	kDeActive,
	kDead,
	END
};

class Entity {
	// Entity는 ID update만 돌려주는 클래스
	// + Component생성

private:
	uint m_id{};
	static uint id_cnt;	
	static std::stack<uint> remain_id;

public:
	Entity()
	{
		if ( !remain_id.size()){
			// Is Empty
			m_id = id_cnt++;
		}
		else{
			m_id = remain_id.top();
			remain_id.pop();
		}		
	}
	const uint32_t GetEntityID() const {return m_id;}

	static void Dead(int _id) 
	{ 
		remain_id.push(_id);
	}

	static uint32_t GetID(std::shared_ptr<Component>&& _comp)noexcept{
		return _comp->GetID();
	}

	// Component Template 
	template<typename T, typename... V>
	std::unique_ptr<T> AddComponent(V&&... params)
	{
		std::unique_ptr<T> component{std::make_unique<T>(std::forward<V>(params)...)};
		component->SetOwner(m_id);
		
		return component;
	}

	template<typename T,typename Other>
	static std::unique_ptr<T> AttachComponent(const std::shared_ptr<Other>& _comp)	// Entity에 다른 component를 부착
	{
		std::unique_ptr<T> component{std::make_unique<T>()};
		component->SetOwner(_comp->GetOwnerID());

		return component;
	}

	// Script Template 
	template<typename T, typename... V>
	std::unique_ptr<T> SetScript(V&&... params)
	{
		std::unique_ptr<T> script{std::make_unique<T>(std::forward<V>(params)...)};
		script->SetOwner(m_id);
		
		return script;
	}
};