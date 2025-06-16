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
	auto transform = SceneMgr::GetComponent<TransformComponent>(id_owner);
	auto pos = transform ->GetPos();
	
	// auto camera_id = SceneMgr::GetMainCamera();
	// auto camera_script = SceneMgr::GetScript<CameraScript>(camera_id);
	
	// auto scale_value = camera_script->GetZoomValue();

	// auto camera_pos = (camera_script->GetMainCameraPos() * scale_value) - (resolution / 2);
	
	// pos *= scale_value;

	for (auto v : transform->GetVertexs()) {
		auto model = transform->GetModelMatrix();
		// auto view = camera_script->GetViewMatrix();
		
		auto v_3 = model * v;
		auto render_v = v_3.ConvertToVec2();

		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(render_v.start.x, render_v.start.y), sf::Color::White),
			sf::Vertex(sf::Vector2f(render_v.end.x, render_v.end.y), sf::Color::White)
		};

		window->draw(line, 2, sf::Lines);
	}
}
