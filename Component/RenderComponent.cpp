#include "RenderComponent.h"
#include "../pch.h"
#include "../Core.h"
#include "../Mgr/SceneMgr.h"
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
		Vec2 scale = transform ->GetScale();

		m_shape->setScale(scale.x,scale.y);
		m_shape->setPosition(pos.x,  pos.y);
		
		if(color)
			m_shape->setFillColor(color->GetColor());
		else
			m_shape->setFillColor(color_basic);

		auto window = Core::GetWindowContext();
		window->draw(*m_shape.get());
	}
}