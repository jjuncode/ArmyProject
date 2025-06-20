#include "Renderer.h"
#include "../pch.h"
#include "../Core.h"

#include "../Mgr/SceneMgr.h"
#include "../Script/CameraScript.h"

#include "Object.h"
#include "Mesh.h"
#include "Texture.h"

void Renderer::Render()
{
	if ( !m_is_visible ) return;
	
	auto window = Core::GetWindowContext();
	auto resolution = Core::GetWindowSize();
	
	auto& obj = SceneMgr::GetObject(m_id_owner); 
	auto& transform = obj.GetTransform();
	auto texture = Texture::GetTexture(obj.GetTextureKey());
	auto mesh = Mesh::GetMesh(obj.GetMeshKey());
	
	if ( !mesh.IsValid() ) {
		return; // If transform or mesh is not available, exit the function
	}

	uint32_t triangle_count = mesh.GetIndexs().size() / 3;
	auto vec_vertexs = mesh.GetVertexs();
	
	auto& camera = SceneMgr::GetObject(SceneMgr::GetMainCamera());
	auto view_matrix = static_cast<CameraScript*>(&camera.GetScript())->GetViewMatrix();
	auto model_matrix = transform.GetModelMatrix();
	// Vertex Shader 
	VertexShader(vec_vertexs, view_matrix * model_matrix);

	for (int i =0; i < triangle_count; ++i ) {
		auto v1 = vec_vertexs[mesh.GetIndexs()[i*3]];
		auto v2 = vec_vertexs[mesh.GetIndexs()[i*3 + 1]];
		auto v3 = vec_vertexs[mesh.GetIndexs()[i*3 + 2]];
	
		std::array<Vertex, 3> tri {v1,v2,v3};
	
		Vec2 min {std::min({v1.v.x, v2.v.x, v3.v.x}) , std::min({v1.v.y, v2.v.y, v3.v.y})};
		Vec2 max {std::max({v1.v.x, v2.v.x, v3.v.x}) , std::max({v1.v.y, v2.v.y, v3.v.y})};
	
		Vec2 u = tri[1].v - tri[0].v;
		Vec2 v = tri[2].v - tri[0].v;

		// 공통분모
		float udotv = Vec::Dot(u, v);
		float vdotv = Vec::Dot(v,v);
		float udotu = Vec::Dot(u,u);
		float det = udotv * udotv - vdotv * udotu;
		if (det == 0) {
			continue; // If denominator is zero, skip this triangle
		}

		float det_inv = 1.f / det;

		// Get Screen Point
		Vec2 screen_left_bot = Vec::ConvertToScreenCoord(min);
		Vec2 screen_right_top = Vec::ConvertToScreenCoord(max);

		// screen clamping 
		screen_left_bot.x = std::clamp(screen_left_bot.x, 0.f, resolution.x);
		screen_left_bot.y = std::clamp(screen_left_bot.y, 0.f, resolution.y);
		screen_right_top.x = std::clamp(screen_right_top.x, 0.f, resolution.x);
		screen_right_top.y = std::clamp(screen_right_top.y, 0.f, resolution.y);
	
		// Get All the pixel in the triangle
		for (int x =screen_left_bot.x; x<=screen_right_top.x; ++x) {
			for (int y = screen_right_top.y; y <=screen_left_bot.y; ++y) {
				Vec2 frag = Vec2(x, y);
				
				Vec2 test_pt = Vec::ConvertToCartesian(frag);
				Vec2 w = test_pt - tri[0].v;
				float wdotu = Vec::Dot(w,u);
				float wdotv = Vec::Dot(w,v);

				float s= (wdotv * udotv - wdotu * vdotv ) * det_inv;
				float t = (wdotu * udotv - wdotv * udotu ) * det_inv;
				float one_minus_s_t = 1.f - s - t;

				// Check if the pixel is inside the triangle using barycentric coordinates
				if (s>=0.f && s<= 1.f && t>=0.f && t<=1.f
					 && one_minus_s_t >= 0.f && one_minus_s_t <= 1.f) {
					// Inside the triangle

					if ( v1.IsUV() && texture.IsValid()) {
						// 한점만 있어도 다 있다는거니까 ㅇㅇ
						Vec2 target_uv( v1.uv.x * one_minus_s_t + v2.uv.x * s + v3.uv.x * t,
														   v1.uv.y * one_minus_s_t + v2.uv.y * s + v3.uv.y * t);
						
						sf::Vertex point[] = {
							sf::Vertex(sf::Vector2f(frag.x, frag.y)
							,texture.GetPixel(target_uv))};
						window->draw(point, 1, sf::Points);
					}
					else{
						sf::Color color = sf::Color(
							static_cast<sf::Uint8>(v1.color.r * one_minus_s_t + v2.color.r * s + v3.color.r * t),
							static_cast<sf::Uint8>(v1.color.g * one_minus_s_t + v2.color.g * s + v3.color.g * t),
							static_cast<sf::Uint8>(v1.color.b * one_minus_s_t + v2.color.b * s + v3.color.b * t));
						sf::Vertex point[] = {
							sf::Vertex(sf::Vector2f(frag.x, frag.y), color)};
						window->draw(point, 1, sf::Points);
					}
				}
			}
		}
	
		// auto view = camera_script->GetViewMatrix();

		// sf::Vertex line[] = {
		// 	sf::Vertex(sf::Vector2f(v1.v.x, v1.v.y), sf::Color::White),
		// 	sf::Vertex(sf::Vector2f(v2.v.x, v2.v.y), sf::Color::White),
			
		// 	sf::Vertex(sf::Vector2f(v2.v.x, v2.v.y), sf::Color::White),
		// 	sf::Vertex(sf::Vector2f(v3.v.x, v3.v.y), sf::Color::White),
			
		// 	sf::Vertex(sf::Vector2f(v3.v.x, v3.v.y), sf::Color::White),
		// 	sf::Vertex(sf::Vector2f(v1.v.x, v1.v.y), sf::Color::White),
		// };

		// window->draw(line, 6, sf::Lines);

	}
}

void VertexShader(std::vector<Vertex> &_v, const Mat3 &_matrix)
{
	for (auto& v : _v ){
		v.v = (_matrix * v.v).ToVec2();
	}
}
