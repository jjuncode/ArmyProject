#include "Renderer.h"
#include "../pch.h"
#include "../Core.h"

#include "../Mgr/SceneMgr.h"
#include "../Script/CameraScript.h"
#include "../Script/LightScript.h"

#include "Object.h"
#include "Mesh.h"
#include "Texture.h"

DrawMode Renderer::m_draw_mode{DrawMode::kDefault_Shading};

bool Renderer::BackFaceCulling(std::array<Vertex, 3> _tri)
{
	auto vec1 = _tri[1].v.ToVec3() - _tri[0].v.ToVec3();
	auto vec2 = _tri[2].v.ToVec3() - _tri[0].v.ToVec3();
	auto normal = Vec::Cross(vec1, vec2);

	// auto& camera = SceneMgr::GetObject(SceneMgr::GetMainCamera());
	// auto camera_forward = camera.GetTransform().GetForward();

	auto camera_forward = Vec3(0,0,-1);

	if ( Vec::Dot(normal, camera_forward) >= 0.f ) {
		// Backface Culling
		return true; // Do not render this triangle
	}

	return false;
}

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
	auto projection_matrix = static_cast<CameraScript*>(&camera.GetScript())->GetProjectionMatrix();
	auto view_matrix_inv = view_matrix.Inverse();
	auto model_matrix = transform.GetModelMatrix();
	
	// Vertex Shader 
	VertexShader(vec_vertexs, view_matrix * model_matrix);

	if ( m_draw_mode != DrawMode::kWireFrame){
		for (int i =0; i < triangle_count; ++i ) {
			auto v1 = vec_vertexs[mesh.GetIndexs()[i*3]];
			auto v2 = vec_vertexs[mesh.GetIndexs()[i*3 + 1]];
			auto v3 = vec_vertexs[mesh.GetIndexs()[i*3 + 2]];
		
			std::array<Vertex, 3> tri {v1,v2,v3};
			if ( BackFaceCulling(tri)){
				continue;
			}
			VertexShader(tri, projection_matrix);

			// To NDC
			for ( auto& v : tri ) {
				v.v = v.v / v.v.w;
	
				v.v.x *= resolution.x * 0.5f;
				v.v.y *= resolution.y * 0.5f;
			}

			Vec2 min {std::min({tri[0].v.x, tri[1].v.x, tri[2].v.x}), std::min({tri[0].v.y, tri[1].v.y, tri[2].v.y})};
			Vec2 max {std::max({tri[0].v.x, tri[1].v.x, tri[2].v.x}), std::max({tri[0].v.y, tri[1].v.y, tri[2].v.y})};
		
			Vec3 u = tri[1].v.ToVec3() - tri[0].v.ToVec3();
			Vec3 v = tri[2].v.ToVec3() - tri[0].v.ToVec3();

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

					Vec3 test_pt = Vec::ConvertToCartesian(Vec3(frag.x, frag.y, 0));
					Vec3 w = test_pt - (tri[0].v).ToVec3();
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
							
							sf::Vertex point {sf::Vector2f(frag.x, frag.y)
												,texture.GetPixel(target_uv)};
							if ( m_is_shading && m_draw_mode == DrawMode::kDefault_Shading)
								FragmentShader(point, transform.GetPos(),view_matrix_inv);
							window->draw(&point, 1, sf::Points);
						}
						else{
							sf::Color color = sf::Color(
								static_cast<sf::Uint8>(v1.color.r * one_minus_s_t + v2.color.r * s + v3.color.r * t),
								static_cast<sf::Uint8>(v1.color.g * one_minus_s_t + v2.color.g * s + v3.color.g * t),
								static_cast<sf::Uint8>(v1.color.b * one_minus_s_t + v2.color.b * s + v3.color.b * t));
							sf::Vertex point{sf::Vector2f(frag.x, frag.y), color+ m_color};
							if ( m_is_shading && m_draw_mode == DrawMode::kDefault_Shading)
								FragmentShader(point, transform.GetPos(),view_matrix_inv);
							window->draw(&point, 1, sf::Points);
						}
					}
				}
			}
		}
	}
	else{
		// WireFrame
		for (int i =0; i < triangle_count; ++i ) {
			auto v1 = vec_vertexs[mesh.GetIndexs()[i*3]];
			auto v2 = vec_vertexs[mesh.GetIndexs()[i*3 + 1]];
			auto v3 = vec_vertexs[mesh.GetIndexs()[i*3 + 2]];

			std::array<Vertex, 3> tri {v1,v2,v3};

			VertexShader(tri, projection_matrix);

			// To NDC
			for ( auto& v : tri ) {
				v.v = v.v / v.v.w;
	
				v.v.x *= resolution.x * 0.5f;
				v.v.y *= resolution.y * 0.5f;
			}

			Vec2 v1_screen = Vec::ConvertToScreenCoord(tri[0].v.ToVec3());
			Vec2 v2_screen = Vec::ConvertToScreenCoord(tri[1].v.ToVec3());
			Vec2 v3_screen = Vec::ConvertToScreenCoord(tri[2].v.ToVec3());

			sf::Vertex lines[] = {
				sf::Vertex(sf::Vector2f(v1_screen.x, v1_screen.y), m_color),
				sf::Vertex(sf::Vector2f(v2_screen.x, v2_screen.y), m_color),

				sf::Vertex(sf::Vector2f(v2_screen.x, v2_screen.y), m_color),
				sf::Vertex(sf::Vector2f(v3_screen.x, v3_screen.y), m_color),

				sf::Vertex(sf::Vector2f(v3_screen.x, v3_screen.y), m_color),
				sf::Vertex(sf::Vector2f(v1_screen.x, v1_screen.y), m_color),
			};

			window->draw(lines, 6, sf::Lines);
		}
	}
}

void VertexShader(std::vector<Vertex> &_v, const Mat4 &_matrix)
{
	for (auto& v : _v ){
		v.v = _matrix * v.v;
	}
}

void VertexShader(std::array<Vertex,3> &_v, const Mat4 &_matrix)
{
	for (auto& v : _v ){
		v.v = _matrix * v.v;
	}
}

// void VertexShader(std::vector<Vertex> &_v, const Mat4 &_view, const Mat4 &_transform)
// {
// 	auto resolution = Core::GetWindowSize();

// 	auto view_matrix = _view * _transform;

// 	// To view space
// 	for (auto& v : _v ){
// 		v.v = view_matrix * v.v;
// 	}
// }

void FragmentShader(sf::Vertex &_point, Vec3 pos_object, Mat4 &_view_matrix_inv)
{
	// Vec3 v{_point.position.x, _point.position.y, _point.position.z};
	// v = (_view_matrix_inv * Vec::ConvertToCartesian(v)).ToVec3();

	// Vec3 v_normal = Vec::Normalize(v - pos_object);

	// auto& vec_light_id = SceneMgr::GetLightIDs();
	// for (const auto& light_id : vec_light_id){
	// 	auto& light = SceneMgr::GetObject(light_id);
	// 	auto light_script =  static_cast<LightScript*>(&light.GetScript());
		
	// 	auto light_pos = light.GetTransform().GetPos();
	// 	auto light_dir = Vec::Normalize(light_pos - v);
	// 	auto light_color = light_script->GetLightColor();

	// 	auto shading = Vec::Dot(v_normal, light_dir);
	// 	shading = std::clamp(shading, 0.f, 1.f);

	// 	float distance = Vec::Length(light_pos - v);
	// 	float attenuation = std::clamp(1.f - distance / light_script->GetRange(), 0.f, 1.f);

	// 	// 감쇠 적용
	// 	shading *= attenuation;

	// 	sf::Color shading_light {
	// 		static_cast<sf::Uint8>(light_color.r * shading),
	// 		static_cast<sf::Uint8>(light_color.g * shading),
	// 		static_cast<sf::Uint8>(light_color.b * shading)
	// 	};

	// 	sf::Color shading_color {
	// 		static_cast<sf::Uint8>( _point.color.r * shading),
	// 		static_cast<sf::Uint8>( _point.color.g * shading),
	// 		static_cast<sf::Uint8>( _point.color.b * shading)
	// 	};

	// 	_point.color = shading_color + shading_light;
	// 	// _point.color += shading_light;
	// }
}
