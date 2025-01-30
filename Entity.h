#pragma once

class Entity {
private:
//	std::vector<std::unique_ptr<Component>> m_components;	// Not Holding 
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
		std::unique_ptr<T> component{std::make_unique<T>(std::forward<T>(params)...)};
		component->SetOwner(m_id);

		return component;
	}
};