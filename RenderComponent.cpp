#include "RenderComponent.h"
#include "pch.h"
#include "Core.h"
#include "TransformComponent.h"
#include "SceneMgr.h"

void RenderComponent::Render()
{
	auto id = GetOwnerID();	// self id
	auto transform = SceneMgr::GetComponent<TransformComponent>(id);

	if(transform){
		auto pos = transform ->GetPos();
		float scale = transform ->GetScale();

		sf::CircleShape circle{scale};
		circle.setFillColor(sf::Color::Blue);
		auto window = Core::GetWindowContext();
		circle.setPosition(pos.x,  pos.y);
		window->draw(circle);
	}

    // sf::CircleShape circle{15};
	// circle.setFillColor(sf::Color::Blue);
	// auto window = Core::GetWindowContext();
	// circle.setPosition(-10,-10);
	// window->draw(circle);
}