#include "RenderComponent.h"
#include "../pch.h"
#include "../Core.h"

#include "../Mgr/SceneMgr.h"

#include "TransformComponent.h"
#include "../Script/CameraScript.h"

void RenderComponent::Render()
{
 	auto window = Core::GetWindowContext();
	auto resolution = Core::GetWindowSize();
	
	auto id_owner = GetOwnerID();	// self id
	auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(id_owner);
	auto pos = transform ->GetPos();
	
	auto camera_id = SceneMgr::GetMainCamera();
	auto camera_transform = SceneMgr::GetComponentOrigin<TransformComponent>(camera_id);
	auto camera_script = SceneMgr::GetComponentOrigin<CameraScript>(camera_id);
	
	auto vec_edge = transform->CreateEdge();
	
	auto scale_value = camera_script->GetZoomValue();
	auto camera_pos = (camera_transform->GetPos() * scale_value) - (resolution / 2);
	
	pos *= scale_value;

	for (auto edge : vec_edge) {
		// Camera Zoom in/out
		// pos scaling
		edge.start *= scale_value;
		edge.end *= scale_value;
		
		auto render_edge = edge + pos - camera_pos;

		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(render_edge.start.x, render_edge.start.y), sf::Color::White),
			sf::Vertex(sf::Vector2f(render_edge.end.x, render_edge.end.y), sf::Color::White)
		};

		window->draw(line, 2, sf::Lines);
	}
}
