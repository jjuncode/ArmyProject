#pragma once

class Entity {
private:
//	std::vector<std::unique_ptr<Component>> m_components;	// Not Holding 
	bool dead{ false };
	int id;
	static int id_cnt;
	static std::stack<int> remain_id;

public:
	void Dead() { dead = true; }
	bool IsDead() { return dead; }

	Entity()
	{
		if ( !remain_id.size()){
			// Is Empty
			id = id_cnt++;
		}
		else{
			id = remain_id.top();
			remain_id.pop();
		}		
	}
};