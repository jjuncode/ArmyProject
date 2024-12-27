#pragma once

class Entity {
private:
//	std::vector<std::unique_ptr<Component>> m_components;	// Not Holding 
	bool dead{ false };

public:
	void Dead() { dead = true; }
	bool IsDead() { return dead; }

	Entity()
	{}
};