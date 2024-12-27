#pragma once
#include "Component.h"

class Object {
private:
//	std::vector<std::unique_ptr<Component>> m_components;	// Not Holding 
	bool dead{ false };

public:
	void Dead() { dead = true; }
	bool IsDead() { return dead; }

	Object()
	{}
};