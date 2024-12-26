#pragma once

class Object {
private:
	bool dead{ false };
	sf::Vector2f m_obj_pos;
	float m_obj_scale;

public:
	void Dead() { dead = true; }
	bool IsDead() { return dead; }
 
	virtual void Update() = 0;
	virtual void Render() {};
	virtual void ProcessColl(const Object& other) = 0;

	sf::Vector2f GetPos() { return m_obj_pos; }
	void SetPos(const sf::Vector2f& _pos) { m_obj_pos = _pos; }

	float GetScale() { return m_obj_scale; }
	void SetScale(float _scale) { m_obj_scale = _scale; }

	Object()
		:m_obj_scale{10.f}
		, m_obj_pos{}
	{}
	virtual ~Object() {};
public:
	// =====================
	// Object Template
	// =====================
	template<typename T> 
	static std::unique_ptr<T> CreatePtr() {	return std::make_unique<T>();}
};

class Circle : public Object {
public:
	Circle() { std::cout << "Circle 생성" << std::endl; }
	~Circle() { std::cout << "Circle 소멸" << std::endl; }
	Circle(const Circle& other) { std::cout << "Circle 복사생성자 " << std::endl; }

private:
	void Update() override;
	void Render() override;
	void ProcessColl(const Object& other) override;
};