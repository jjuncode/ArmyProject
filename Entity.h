#pragma once
#include <stack>

class Entity {
	// Entity는 ID update만 돌려주는 클래스
	// + Component생성

private:
	bool dead{ false };
	uint m_id;
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

	void Dead() 
	{ 
		dead = true;
		remain_id.push(m_id);
	}
	bool IsDead() { return dead; }

	// Component Template 
	template<typename T, typename... V>
	std::unique_ptr<T> AddComponent(V&&... params)
	{
		std::unique_ptr<T> component{std::make_unique<T>(std::forward<V>(params)...)};
		component->SetOwner(m_id);

		return component;
	}

	// template<typename T>
	// std::unique_ptr<T> AddComponent()
	// {
	// 	std::unique_ptr<T> component{std::make_unique<T>()};
	// 	component->SetOwner(m_id);

	// 	return component;
	// }
};