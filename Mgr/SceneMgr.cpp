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
    else if (InputMgr::IsTap(sf::Keyboard::F4))
        Renderer::SetDrawMode(DrawMode::kDepthBuffer);
}

void SceneMgr::Render()
{
    m_cur_scene->Render();

    // Clear Depth Buffer
    Renderer::ClearDepthBuffer();
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
        Vertex{{-0.5f, -0.5f,  0.5f}, {}, Vec2(0.125f, 0.75f)},
        Vertex{{ 0.5f, -0.5f,  0.5f}, {}, Vec2(0.25f, 0.75f)},
        Vertex{{ 0.5f,  0.5f,  0.5f}, {}, Vec2(0.25f, 0.875f)},
        Vertex{{-0.5f,  0.5f,  0.5f}, {}, Vec2(0.125f, 0.875f)},
        
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
        Vertex{{ 0.5f, -0.5f, 0.5f}, {}, Vec2(0.25f,  0.75f)},
        Vertex{{ 0.5f, -0.5f, -0.5f}, {}, Vec2(0.375f, 0.75f)},
        Vertex{{ 0.5f,  0.5f, -0.5f}, {}, Vec2(0.375f, 0.875f)},
        Vertex{{ 0.5f,  0.5f, 0.5f}, {}, Vec2(0.25f,  0.875f)},

        // 윗면 (+Y)
        Vertex{{-0.5f,  0.5f,  0.5f}, {}, Vec2(0.125f, 0.875f)},
        Vertex{{ 0.5f,  0.5f,  0.5f}, {}, Vec2(0.25f,  0.875f)},
        Vertex{{ 0.5f,  0.5f, -0.5f}, {}, Vec2(0.25f,  1.0f)},
        Vertex{{-0.5f,  0.5f, -0.5f}, {}, Vec2(0.125f, 1.0f)},

        // 아랫면 (-Y)
        Vertex{{-0.5f, -0.5f, 0.5f}, {}, Vec2(0.25f,  0.875f)},
        Vertex{{ 0.5f, -0.5f, 0.5f}, {}, Vec2(0.375f, 0.875f)},
        Vertex{{ 0.5f, -0.5f, -0.5f}, {}, Vec2(0.375f, 1.0f)},
        Vertex{{-0.5f, -0.5f, -0.5f}, {}, Vec2(0.25f,  1.0f)},
    }, {
        0, 1, 2, 0, 2, 3,     // 앞 (+Z)
        4, 6, 5, 4, 7, 6,     // 뒤 (-Z)
        8, 9,10, 8,10,11,     // 왼 (-X)
        12,13,14,12,14,15,   // 오 (+X)
        16,17,18,16,18,19,   // 위 (+Y)
        20,22,21,20,23,22    // 아래 (-Y)
    });

    Mesh::CreateMesh("plane", {
        Vertex{{-1.f , 0.f, 1.f}, {}, {}},
        Vertex{{ -1.f , 0.f, -1.f}, {}, {}},
        Vertex{{ 1.f , 0.f, -1.f}, {}, {}},
        Vertex{{ 1.f , 0.f, 1.f}, {}, {}}
    }, {
        0,2,1,
        0,3,2
    });

}
