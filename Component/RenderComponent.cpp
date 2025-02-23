#include "RenderComponent.h"
#include "../pch.h"
#include "../Core.h"
#include "../SceneMgr.h"
#include "TransformComponent.h"
#include "ColorComponent.h"

void RenderComponent::Render()
{
	auto id_owner = GetOwnerID();	// self id
	auto transform = SceneMgr::GetComponent<TransformComponent>(id_owner);
	auto color = SceneMgr::GetComponent<ColorComponent>(id_owner);

	sf::Color color_basic{sf::Color::Magenta};

	if(transform){
		auto pos = transform ->GetPos();
		float scale = transform ->GetScale();

		sf::CircleShape circle{scale};
		
		if(color)
			circle.setFillColor(color->GetColor());
		else
			circle.setFillColor(color_basic);

		auto window = Core::GetWindowContext();
		circle.setPosition(pos.x,  pos.y);
		window->draw(circle);
	}
}