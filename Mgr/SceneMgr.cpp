#include "SceneMgr.h"
#include "../pch.h"
#include "../Object/Texture.h"
#include "../Object/Mesh.h"

std::unique_ptr<Scene> SceneMgr::m_cur_scene{};

void SceneMgr::Update(float dt)
{
    m_cur_scene->Update(dt);
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

    Mesh::CreateMesh( "square",
    {
        Vertex{ Vec3{ -0.5f, -0.5f, 0.0f }, RGBA{ 255, 0, 0 }, Vec2(0.125f, 0.75f) },
        Vertex{ Vec3{  0.5f, -0.5f, 0.0f }, RGBA{ 0, 255, 0 },Vec2(0.25f, 0.75f) },
        Vertex{ Vec3{  0.5f,  0.5f, 0.0f }, RGBA{ 0, 0, 255 }, Vec2(0.25f, 0.875f) },
        Vertex{ Vec3{ -0.5f,  0.5f, 0.0f }, RGBA{ 255, 255, 255 }, Vec2(0.125f, 0.875f) }
    }, 
    {
        0, 1, 2, 
        0, 2, 3
    });

    std::vector<Vertex> circleVertices;
    std::vector<uint32_t> circleIndices;

    const int segments = 32;
    const float radius = 0.5f;

    // 중심 정점
    circleVertices.push_back(Vertex{
        Vec3{0.0f, 0.0f, 0.0f},
        RGBA{0, 0, 0, 0},
        Vec2{0, 0}});

    // 원 둘레 정점들
    for (int i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = std::cos(angle) * radius;
        float y = std::sin(angle) * radius;

        circleVertices.push_back(Vertex{
            Vec3{x, y, 0.0f},
            RGBA{0, 0, 0, 0},
            Vec2{0,0}});
    }

    // 인덱스 생성 (Triangle Fan)
    for (uint32_t i = 1; i <= segments; ++i)
    {
        circleIndices.push_back(0);     // 중심점
        circleIndices.push_back(i);     // 현재 둘레점
        circleIndices.push_back(i + 1); // 다음 둘레점
    }

    // Mesh 생성
    Mesh::CreateMesh("circle", std::move(circleVertices), std::move(circleIndices));
}
