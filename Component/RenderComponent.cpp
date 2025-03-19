#include "RenderComponent.h"
#include "../pch.h"
#include "../Core.h"
#include "../Mgr/SceneMgr.h"
#include "TransformComponent.h"

void RenderComponent::Render()
{
	auto id_owner = GetOwnerID();	// self id
	auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(id_owner);

	for (const auto& edge : transform->GetEdge()) {
		auto pos = transform ->GetPos();

		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(edge.start.x + pos.x , edge.start.y+pos.y)),
			sf::Vertex(sf::Vector2f(edge.end.x + pos.x , edge.end.y + pos.y))
		};

		auto window = Core::GetWindowContext();
		line->color = sf::Color::Red;
		window->draw(line, 2, sf::Lines);
	}
}
