#include "RenderComponent.h"
#include "../pch.h"
#include "../Core.h"

#include "../Mgr/SceneMgr.h"

#include "TransformComponent.h"
#include "Mesh.h"
#include "../Script/CameraScript.h"

void RenderComponent::Render()
{
 	auto window = Core::GetWindowContext();
	auto resolution = Core::GetWindowSize();
	
	auto id_owner = GetOwnerID();	// self id
	auto transform = SceneMgr::GetComponent<TransformComponent>(id_owner);
	auto mesh = SceneMgr::GetComponent<Mesh>(id_owner);
	if ( !transform || !mesh ) {
		return; // If transform or mesh is not available, exit the function
	}

	// auto pos = transform ->GetPos();
	
	// auto camera_id = SceneMgr::GetMainCamera();
	// auto camera_script = SceneMgr::GetScript<CameraScript>(camera_id);
	// auto scale_value = camera_script->GetZoomValue();
	// auto camera_pos = (camera_script->GetMainCameraPos() * scale_value) - (resolution / 2);
	// pos *= scale_value;

	uint32_t triangle_count = mesh->GetIndexs().size() / 3;
	auto vec_vertexs = mesh->GetVertexs();

	auto model = transform->GetModelMatrix();
	for ( auto& v : vec_vertexs){
		v = model * v.v;
	}

	for (int i =0; i < triangle_count; ++i ) {
		auto v1 = vec_vertexs[mesh->GetIndexs()[i*3]];
		auto v2 = vec_vertexs[mesh->GetIndexs()[i*3 + 1]];
		auto v3 = vec_vertexs[mesh->GetIndexs()[i*3 + 2]];

		// auto view = camera_script->GetViewMatrix();
		
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(v1.v.x, v1.v.y), sf::Color::White),
			sf::Vertex(sf::Vector2f(v2.v.x, v2.v.y), sf::Color::White),
			
			sf::Vertex(sf::Vector2f(v2.v.x, v2.v.y), sf::Color::White),
			sf::Vertex(sf::Vector2f(v3.v.x, v3.v.y), sf::Color::White),
			
			sf::Vertex(sf::Vector2f(v3.v.x, v3.v.y), sf::Color::White),
			sf::Vertex(sf::Vector2f(v1.v.x, v1.v.y), sf::Color::White),
		};

		window->draw(line, 6, sf::Lines);
	}
}
