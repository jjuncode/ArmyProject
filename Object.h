#pragma once

class Object {
private:
	std::vector<std::unique_ptr<Component>> m_components;	// holding components
	bool dead{ false };

public:
	void Dead() { dead = true; }
	bool IsDead() { return dead; }
 
	virtual void Update(float dt){};
	virtual void Render(){};

	Object()
	{}

	virtual ~Object() {};

public:
	// =====================
	// Object Template
	// =====================
	template<typename T> 
	static std::unique_ptr<T> CreatePtr() {	return std::make_unique<T>();}
};