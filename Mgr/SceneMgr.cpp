#include "SceneMgr.h"
#include "../pch.h"
#include "../Object/Texture.h"
#include "../Object/Mesh.h"
#include "../Object/Renderer.h"
#include "InputMgr.h"

std::unique_ptr<Scene> SceneMgr::m_cur_scene{};

void SceneMgr::Update(float dt)
{
    m_cur_scene->Update(dt);

    if ( InputMgr::IsTap(sf::Keyboard::F1))
        Renderer::SetDrawMode(DrawMode::kDefault_Shading);
    else if (InputMgr::IsTap(sf::Keyboard::F2))
        Renderer::SetDrawMode(DrawMode::kDefault_Shading_None);
    else if (InputMgr::IsTap(sf::Keyboard::F3))
        Renderer::SetDrawMode(DrawMode::kWireFrame);
}

void SceneMgr::Render()
{
    m_cur_scene->Render();
}

void SceneMgr::Exit()
{
    m_cur_scene->Exit();
    m_cur_scene.reset();
}

void SceneMgr::LoadResource()
{
    Texture::CreateTexture("player.png");
    Mesh::CreateMesh("square", {
        // 앞면 (+Z) - 얼굴
        // 테스트용 앞면 (UV 전체)
        // Vertex{{-0.5f, -0.5f,  0.5f}, {}, Vec2(0.125f, 0.75f)},
        // Vertex{{ 0.5f, -0.5f,  0.5f}, {}, Vec2(0.25f, 0.75f)},
        // Vertex{{ 0.5f,  0.5f,  0.5f}, {}, Vec2(0.25f, 0.875f)},
        // Vertex{{-0.5f,  0.5f,  0.5f}, {}, Vec2(0.125f, 0.875f)},
        
        Vertex{{-0.5f, -0.5f,  0.5f}, {}, Vec2(8/64.f,   1-15/64.f)},
        Vertex{{ 0.5f, -0.5f,  0.5f}, {}, Vec2(15/64.f, 1-15/64.f)},
        Vertex{{ 0.5f,  0.5f,  0.5f}, {}, Vec2(15/64.f, 1-8/64.f)},
        Vertex{{-0.5f,  0.5f,  0.5f}, {}, Vec2(8/64.f,   1-8/64.f)},

        // 뒷면 (-Z)
        Vertex{{-0.5f, -0.5f, -0.5f}, {}, Vec2(0.375f, 0.75f)},
        Vertex{{ 0.5f, -0.5f, -0.5f}, {}, Vec2(0.5f,   0.75f)},
        Vertex{{ 0.5f,  0.5f, -0.5f}, {}, Vec2(0.5f,   0.875f)},
        Vertex{{-0.5f,  0.5f, -0.5f}, {}, Vec2(0.375f, 0.875f)},

        // 왼쪽 (-X)
        Vertex{{-0.5f, -0.5f, -0.5f}, {}, Vec2(0.0f,   0.75f)},
        Vertex{{-0.5f, -0.5f,  0.5f}, {}, Vec2(0.125f, 0.75f)},
        Vertex{{-0.5f,  0.5f,  0.5f}, {}, Vec2(0.125f, 0.875f)},
        Vertex{{-0.5f,  0.5f, -0.5f}, {}, Vec2(0.0f,   0.875f)},

        // 오른쪽 (+X)
        Vertex{{ 0.5f, -0.5f, -0.5f}, {}, Vec2(0.25f,  0.75f)},
        Vertex{{ 0.5f, -0.5f,  0.5f}, {}, Vec2(0.375f, 0.75f)},
        Vertex{{ 0.5f,  0.5f,  0.5f}, {}, Vec2(0.375f, 0.875f)},
        Vertex{{ 0.5f,  0.5f, -0.5f}, {}, Vec2(0.25f,  0.875f)},

        // 윗면 (+Y)
        Vertex{{-0.5f,  0.5f,  0.5f}, {}, Vec2(0.125f, 0.875f)},
        Vertex{{ 0.5f,  0.5f,  0.5f}, {}, Vec2(0.25f,  0.875f)},
        Vertex{{ 0.5f,  0.5f, -0.5f}, {}, Vec2(0.25f,  1.0f)},
        Vertex{{-0.5f,  0.5f, -0.5f}, {}, Vec2(0.125f, 1.0f)},

        // 아랫면 (-Y)
        Vertex{{-0.5f, -0.5f, -0.5f}, {}, Vec2(0.25f,  0.75f)},
        Vertex{{ 0.5f, -0.5f, -0.5f}, {}, Vec2(0.375f, 0.75f)},
        Vertex{{ 0.5f, -0.5f,  0.5f}, {}, Vec2(0.375f, 0.875f)},
        Vertex{{-0.5f, -0.5f,  0.5f}, {}, Vec2(0.25f,  0.875f)},
    }, {
        0, 1, 2, 0, 2, 3,     // 앞 (+Z)
        4, 6, 5, 4, 7, 6,     // 뒤 (-Z)
        // 8, 9,10, 8,10,11,     // 왼 (-X)
        // 12,14,13,12,15,14,   // 오 (+X)
        // 16,17,18,16,18,19,   // 위 (+Y)
        // 20,21,22,20,22,23    // 아래 (-Y)
    });

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    auto latitudeSegments = 16;
    auto longitudeSegments = 32;
    auto radius = 0.5f;


    for (int lat = 0; lat <= latitudeSegments; ++lat)
    {
        float v = (float)lat / latitudeSegments;
        float theta = v * M_PI; // 0 ~ PI

        for (int lon = 0; lon <= longitudeSegments; ++lon)
        {
            float u = (float)lon / longitudeSegments;
            float phi = u * 2.0f * M_PI; // 0 ~ 2PI

            float x = std::sin(theta) * std::cos(phi);
            float y = std::cos(theta);
            float z = std::sin(theta) * std::sin(phi);

            Vec3 temp = Vec3(x, y, z)*radius;
            Vec4 position = Vec4(temp.x,temp.y,temp.z, 1.0f);
            Vec2 uv = Vec2{u, v}; // UV 좌표 (spherical mapping)

            vertices.push_back(Vertex{position, RGBA{}, uv});
        }
    }

    // 인덱스 생성
    for (int lat = 0; lat < latitudeSegments; ++lat)
    {
        for (int lon = 0; lon < longitudeSegments; ++lon)
        {
            int curr = lat * (longitudeSegments + 1) + lon;
            int next = curr + longitudeSegments + 1;

            indices.push_back(curr);
            indices.push_back(next);
            indices.push_back(curr + 1);

            indices.push_back(curr + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    Mesh::CreateMesh("circle", std::move(vertices), std::move(indices));
}
