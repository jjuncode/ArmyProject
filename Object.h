#pragma once

class Object {
private:
	bool dead{ false };

public:
	void Dead() { dead = true; }
	bool IsDead() { return dead; }
 
	virtual void Update(){};
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
