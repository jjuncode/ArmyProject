#include "RenderComponent.h"
#include "../pch.h"
#include "../Core.h"
#include "../Mgr/SceneMgr.h"
#include "TransformComponent.h"

void RenderComponent::Render()
{
	auto id_owner = GetOwnerID();	// self id
	auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(id_owner);

	for (auto edge : transform->GetEdge()) {
		auto pos = transform ->GetPos();
		auto render_edge = edge + pos;

		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(render_edge.start.x, render_edge.start.y), sf::Color::Green),
			sf::Vertex(sf::Vector2f(render_edge.end.x, render_edge.end.y), sf::Color::Green)
		};

		auto window = Core::GetWindowContext();
		window->draw(line, 2, sf::Lines);
	}
}
