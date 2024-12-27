#pragma once

class Entity {
private:
//	std::vector<std::unique_ptr<Component>> m_components;	// Not Holding 
	bool dead{ false };
	int m_id;
	static int id_cnt;
	static std::stack<int> remain_id;

public:
	void Dead() 
	{ 
		dead = true;
		remain_id.push(m_id);
	}
	bool IsDead() { return dead; }

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
};