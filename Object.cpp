#include "pch.h"
#include "Object.h"
#include "Core.h"

void Wall::Update()
{

}

void Wall::Render()
{
}

void Wall::ProcessColl(const Object& other)
{
}

void Car::Update()
{
}

void Car::Render()
{
}

void Car::ProcessColl(const Object& other)
{
}

void Circle::Update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		auto pos = GetPos();
		SetPos(sf::Vector2<float>(pos.x + 1, pos.y + 1));
	}
}

void Circle::Render()
{
	sf::CircleShape circle{GetScale()};
	circle.setFillColor(sf::Color::Green);
	auto window = Core::GetWindowContext();
	circle.setPosition(GetPos());
	window->draw(circle);
}

void Circle::ProcessColl(const Object& other)
{
}