#include "pch.h"
#include "Object.h"
#include "Core.h"

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