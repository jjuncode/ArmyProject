#include "RenderComponent.h"
#include "../pch.h"
#include "../Core.h"

#include "../Mgr/SceneMgr.h"

#include "TransformComponent.h"
#include "CameraScript.h"

void RenderComponent::Render()
{
	auto id_owner = GetOwnerID();	// self id
	auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(id_owner);

	auto camera_id = SceneMgr::GetMainCamera();
	auto camera_transform = SceneMgr::GetComponentOrigin<TransformComponent>(camera_id);
	auto camera_script = SceneMgr::GetComponentOrigin<CameraScript>(camera_id);

	auto&& vec_edge = transform->CreateEdge();

	for (auto edge : vec_edge) {
		auto pos = transform ->GetPos();
		
		// Camera Zoom in/out
		auto scale_value = camera_script->GetZoomValue();
		
		// pos scaling
		pos *= scale_value;
		auto resolution = Core::GetWindowSize();
		auto camera_pos = (camera_transform->GetPos() * scale_value) - resolution / 2;

		// edge scaling 
		auto offset_start = edge.start - Vec2(0,0);
		auto offset_end = edge.end - Vec2(0,0);

		edge.start = offset_start * scale_value;
		edge.end = offset_end * scale_value;
		
		auto render_edge = edge + pos - camera_pos;

		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(render_edge.start.x, render_edge.start.y), sf::Color::White),
			sf::Vertex(sf::Vector2f(render_edge.end.x, render_edge.end.y), sf::Color::White)
		};

		auto window = Core::GetWindowContext();
		window->draw(line, 2, sf::Lines);
	}
}
