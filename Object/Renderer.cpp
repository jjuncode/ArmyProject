#include "Renderer.h"
#include "../pch.h"
#include <algorithm>
#include "../Core.h"

#include "../Mgr/SceneMgr.h"
#include "../Script/CameraScript.h"
#include "../Script/LightScript.h"

#include "Object.h"
#include "Mesh.h"
#include "Texture.h"
#include "Perspective.h"

DrawMode Renderer::m_draw_mode{DrawMode::kDefault_Shading};
std::vector<std::vector<float>> Renderer::m_vec_depth_buffer{}; // 렌더링 포인트 벡터

bool BackFaceCulling(std::array<Vertex, 3> _tri)
{
	auto vec1 = _tri[1].v.ToVec3() - _tri[0].v.ToVec3();
	auto vec2 = _tri[2].v.ToVec3() - _tri[0].v.ToVec3();
	auto normal = Vec::Normalize(Vec::Cross(vec1, vec2));

 	// 삼각형 중심 위치
    auto center = (_tri[0].v.ToVec3() + _tri[1].v.ToVec3() + _tri[2].v.ToVec3()) / 3.f;

	// 시선벡터를 이용해야한다 병신아 
    auto to_face = Vec::Normalize(center);

    if (Vec::Dot(normal, to_face) > 0.f) {
        return true; // Backface → culled
    }

	return false;
}

BoundValue FrustumCulling(const Frustum &_frustum, const Sphere &_sphere)
{
	return _frustum.CheckBound(_sphere); 
}

BoundValue FrustumCulling(const Frustum &_frustum, const Box &_box)
{
	return _frustum.CheckBound(_box);
}

void Renderer::SetDepthBuffer(const Vec2& _v, float _depth)
{
	m_vec_depth_buffer[_v.x][_v.y] = _depth;
}

void Renderer::ClearDepthBuffer()
{
	for (auto &vec : m_vec_depth_buffer)	{
		std::fill(vec.begin(), vec.end(), std::numeric_limits<float>::max());
	}
}

void Renderer::Render()
{
	if ( !m_is_visible ) return;
	
	auto& obj = SceneMgr::GetObject(m_id_owner); 
	auto& transform = obj.GetTransform();
	auto& mesh = Mesh::GetMesh(obj.GetMeshKey());
	
	if ( !mesh.IsValid() ) {
		return; // If transform or mesh is not available, exit the function
	}

	auto vec_vertexs = mesh.GetVertexs();
	
	auto& camera = SceneMgr::GetObject(SceneMgr::GetMainCamera());
	auto& camera_script = *static_cast<CameraScript*>(&camera.GetScript());

	static auto near_plane = camera_script.GetNearPlane();
	static auto far_plane = camera_script.GetFarPlane();

	auto model_matrix = transform.GetModelMatrix();
	auto view_matrix = camera_script.GetViewMatrix();
	static auto projection_matrix = camera_script.GetProjectionMatrix();

	auto VM_matrix = view_matrix * model_matrix;
	auto PVM_matrix = projection_matrix * VM_matrix;
	
	std::array<Plane,6> frustum_planes{
		PVM_matrix[3] + PVM_matrix[0], // Left
		PVM_matrix[3] - PVM_matrix[0], // Right
		PVM_matrix[3] + PVM_matrix[1], // Bottom
		PVM_matrix[3] - PVM_matrix[1], // Top
		PVM_matrix[3] + PVM_matrix[2], // Near
		PVM_matrix[3] - PVM_matrix[2]  // Far
	};
	Frustum frustum{frustum_planes};
	
	sf::Color color_additional{};

	// Frustum Culling
	auto bound_value = FrustumCulling(frustum, mesh.GetBox());
	if ( bound_value == BoundValue::kIntersect){
		color_additional = sf::Color::Red;
	}
	else if ( bound_value == BoundValue::kOutside) {
		return; // If the mesh is outside the frustum, skip rendering
	}

	// Vertex Shader 
	VertexShader(vec_vertexs, VM_matrix);

	uint32_t triangle_count = mesh.GetIndexs().size() / 3;

	for (int i = 0; i < triangle_count; ++i){
		auto v1 = vec_vertexs[mesh.GetIndexs()[i * 3]];
		auto v2 = vec_vertexs[mesh.GetIndexs()[i * 3 + 1]];
		auto v3 = vec_vertexs[mesh.GetIndexs()[i * 3 + 2]];

		std::array<Vertex, 3> tri{v1, v2, v3};
		if (BackFaceCulling(tri)){
			continue;
		}
		VertexShader(tri, projection_matrix);

		std::vector<PerspectiveTest> vec_planes{
			{IsPointOutsideW0, ProjectionToPlaneW0},
			{IsPointOutsidePY, ProjectionToPlanePY},
			{IsPointOutsideMY, ProjectionToPlaneMY},
			{IsPointOutsidePX, ProjectionToPlanePX},
			{IsPointOutsideMX, ProjectionToPlaneMX},
			{IsPointOutsidePZ, ProjectionToPlanePZ},
			{IsPointOutsideMZ, ProjectionToPlaneMZ}
		};

		std::vector<Vertex> vec_clipping{ tri[0], tri[1], tri[2] };

		for (auto& plane : vec_planes){
			plane.Clipping(vec_clipping);
		}

		auto triangle_cnt_clipping = vec_clipping.size() / 3;
		for ( auto j=0; j<triangle_cnt_clipping; ++j ){
			auto v1 = vec_clipping[j*3];
			auto v2 = vec_clipping[j*3 + 1];
			auto v3 = vec_clipping[j*3 + 2];

			std::array<Vertex, 3> tri_cliiping{v1, v2, v3};
			DrawTriangle(tri_cliiping, color_additional);
		}
	}
}

void Renderer::DrawTriangle(const std::array<Vertex,3> &_vertex, sf::Color color_additional)
{
	auto window = Core::GetWindowContext();
	auto resolution = Core::GetWindowSize();
	
	auto& obj = SceneMgr::GetObject(m_id_owner); 
	auto& transform = obj.GetTransform();
	auto& texture = Texture::GetTexture(obj.GetTextureKey());
	
	auto& camera = SceneMgr::GetObject(SceneMgr::GetMainCamera());
	auto& camera_script = *static_cast<CameraScript*>(&camera.GetScript());

	static auto near_plane = camera_script.GetNearPlane();
	static auto far_plane = camera_script.GetFarPlane();

	auto view_matrix = camera_script.GetViewMatrix();
	auto view_matrix_inv = view_matrix.Inverse();

	if ( m_draw_mode != DrawMode::kWireFrame){
		auto v1 = _vertex[0];
		auto v2 = _vertex[1];
		auto v3 = _vertex[2];
		
		if (v1.v.z < 0.f || v2.v.z < 0.f || v3.v.z < 0.f)
		{
			return; // 카메라 뒤쪽에 있는 삼각형은 건너뛰기
		}

		auto inv_z_1 = 1.f / v1.v.z;
		auto inv_z_2 = 1.f / v2.v.z;
		auto inv_z_3 = 1.f / v3.v.z;


		std::array<Vertex, 3> tri {v1,v2,v3};

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
			return; // If denominator is zero, skip this triangle
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

					// Get Depth
					float depth = v1.v.z * one_minus_s_t
								+ v2.v.z * s
								+ v3.v.z * t;

					// //  보정
					float z = inv_z_1 * one_minus_s_t + inv_z_2 * s + inv_z_3 * t;
					float inv_z = 1.f / z;

					// Set Depth Buffer
					if ( m_vec_depth_buffer[frag.x][frag.y] <= depth ) {
						continue; 	// If the current pixel is not closer, skip
									// +Z is Far
					}

					SetDepthBuffer(Vec2(frag.x, frag.y ), depth);

					sf::Vertex point{}; 	// to draw 

					if ( m_draw_mode == DrawMode::kDepthBuffer){

						// White is Close one, Black is far 
						auto color_depth = (depth - near_plane)/(far_plane - near_plane);

						auto color = sf::Color{
							static_cast<sf::Uint8>(255.f * color_depth), 
							static_cast<sf::Uint8>(255.f * color_depth),
							static_cast<sf::Uint8>(255.f * color_depth),
							255u
						};
						point = sf::Vertex{sf::Vector2f(frag.x, frag.y),color};
					}
					else if ( v1.IsUV() && texture.IsValid()) {
						// 한점만 있어도 다 있다는거니까 ㅇㅇ

						Vec2 target_uv( (v1.uv * one_minus_s_t * inv_z_1
										+ v2.uv * s * inv_z_2
										+ v3.uv * t * inv_z_3) * inv_z );
						
						point = sf::Vertex{sf::Vector2f(frag.x, frag.y)
											,texture.GetPixel(target_uv) + color_additional};
						if ( m_is_shading && m_draw_mode == DrawMode::kDefault_Shading)
							FragmentShader(point, transform.GetPos(),view_matrix_inv);
					}
					else{
						sf::Color color = sf::Color(
							static_cast<sf::Uint8>(v1.color.r * one_minus_s_t + v2.color.r * s + v3.color.r * t),
							static_cast<sf::Uint8>(v1.color.g * one_minus_s_t + v2.color.g * s + v3.color.g * t),
							static_cast<sf::Uint8>(v1.color.b * one_minus_s_t + v2.color.b * s + v3.color.b * t));
						point = sf::Vertex{sf::Vector2f(frag.x, frag.y), color + m_color + color_additional};

						if (m_is_shading && m_draw_mode == DrawMode::kDefault_Shading)
							FragmentShader(point, transform.GetPos(), view_matrix_inv);
					}

					window->draw(&point, 1, sf::Points);
				}
			}
		}
	}
	else if ( m_draw_mode == DrawMode::kWireFrame ) {
		// WireFrame
		auto v1 = _vertex[0];
		auto v2 = _vertex[1];
		auto v3 = _vertex[2];

		std::array<Vertex, 3> tri {v1,v2,v3};
		
		// To NDC
		for ( auto& v : tri ) {
			v.v = v.v / v.v.w;
			
			v.v.x *= resolution.x * 0.5f;
			v.v.y *= resolution.y * 0.5f;
		}

		Vec2 v1_screen = Vec::ConvertToScreenCoord(tri[0].v.ToVec3());
		Vec2 v2_screen = Vec::ConvertToScreenCoord(tri[1].v.ToVec3());
		Vec2 v3_screen = Vec::ConvertToScreenCoord(tri[2].v.ToVec3());

		auto self_color = m_color;
		if (color_additional != sf::Color{}){
			self_color = sf::Color{};
		}

		sf::Vertex lines[] = {
			sf::Vertex(sf::Vector2f(v1_screen.x, v1_screen.y), self_color + color_additional),
			sf::Vertex(sf::Vector2f(v2_screen.x, v2_screen.y), self_color + color_additional),
			sf::Vertex(sf::Vector2f(v2_screen.x, v2_screen.y), self_color + color_additional),
			sf::Vertex(sf::Vector2f(v3_screen.x, v3_screen.y), self_color + color_additional),
			sf::Vertex(sf::Vector2f(v3_screen.x, v3_screen.y), self_color + color_additional),
			sf::Vertex(sf::Vector2f(v1_screen.x, v1_screen.y), self_color + color_additional),
		};
		window->draw(lines, 6, sf::Lines);
	}
}

void Renderer::CreateRenderingBuffer()
{
	auto window_size = Core::GetWindowSize();
	m_vec_depth_buffer.resize(window_size.x+1);
	for(auto& v : m_vec_depth_buffer){
		v.resize(window_size.y+1, std::numeric_limits<float>::max());
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


Plane::Plane(const Vec4 &_v)
{
    normal = Vec3(_v.x, _v.y, _v.z);
    d = _v.w;
    
    auto length = Vec::Length(normal);
    normal /= length;
    d /= length;

    normal *= -1.0f; // Invert the normal direction
    d *= -1.0f; // Invert the distance
}

float Plane::DistanceToPoint(const Vec3 &point) const
{
    return Vec::Dot(normal, point) + d;
}

bool Plane::IsOutside(const Vec3 &point) const
{
    if (DistanceToPoint(point) > 0) {
        return true; // Point is outside the plane
    }
    return false; // Point is inside or on the plane    
}

BoundValue Frustum::CheckBound(const Sphere &_sphere) const
{
    for (const auto &plane : planes){
        auto distn = plane.DistanceToPoint(_sphere.center);

        if (distn > _sphere.radius) {
            return BoundValue::kOutside;
        }
        else if(abs(distn) <=_sphere.radius) {
            return BoundValue::kIntersect; // Point is on the plane
        }
    }

    return BoundValue::kInside;
}

BoundValue Frustum::CheckBound(const Box &_box) const
{
    for (const auto& plane : planes) {
        Vec3 check{};
        Vec3 check_opposite{};

        if ( plane.normal.x > 0 ) {
            check.x = _box.min.x;
            check_opposite.x = _box.max.x;
        }
        else{
            check.x = _box.max.x;
            check_opposite.x = _box.min.x;
        }
        if ( plane.normal.y > 0 ) {
            check.y = _box.min.y;
            check_opposite.y = _box.max.y;
        }
        else{
            check.y = _box.max.y;
            check_opposite.y = _box.min.y;
        }
        if ( plane.normal.z > 0 ) {
            check.z = _box.min.z;
            check_opposite.z = _box.max.z;
        }
        else{
            check.z = _box.max.z;
            check_opposite.z = _box.min.z;
        }

        auto distn = plane.DistanceToPoint(check);
        if ( distn > 0 ){
            return BoundValue::kOutside; // Box is outside the plane
        }
        else if (distn <= 0 && plane.DistanceToPoint(check_opposite) >= 0) {
            return BoundValue::kIntersect; // Box intersects the plane
        }
    }

    return BoundValue::kInside; // Box is inside the frustum
}