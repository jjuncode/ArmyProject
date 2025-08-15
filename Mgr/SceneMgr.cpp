#include "SceneMgr.h"
#include "../pch.h"
#include "../Object/Texture.h"
#include "../Object/Mesh.h"
#include "../Object/Renderer.h"
#include "InputMgr.h"

#include "../Component/AnimationComponent.h"

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
    Texture::CreateTexture(TextureName::Player);

    Mesh::CreateMesh(MeshName::Cube, {
        // 앞면 (+Z) 
        Vertex{{-0.5f, -0.5f,  0.5f}}, Vertex{{ 0.5f, -0.5f,  0.5f}},
        Vertex{{ 0.5f,  0.5f,  0.5f}}, Vertex{{-0.5f,  0.5f,  0.5f}},

        // 뒷{면(-Z)}
        Vertex{{-0.5f, -0.5f, -0.5f}}, Vertex{{ 0.5f, -0.5f, -0.5f}},
        Vertex{{ 0.5f,  0.5f, -0.5f}}, Vertex{{-0.5f,  0.5f, -0.5f}},

        // 왼{쪽(-X)}
        Vertex{{-0.5f, -0.5f, -0.5f}}, Vertex{{-0.5f, -0.5f,  0.5f}},
        Vertex{{-0.5f,  0.5f,  0.5f}}, Vertex{{-0.5f,  0.5f, -0.5f}},

        // 오{른쪽 (+X)}
        Vertex{{ 0.5f, -0.5f, 0.5f }}, Vertex{{ 0.5f, -0.5f, -0.5f}},
        Vertex{{ 0.5f,  0.5f, -0.5f}}, Vertex{{ 0.5f,  0.5f, 0.5f }},

        // 윗{면(+Y)}
        Vertex{{-0.5f,  0.5f,  0.5f}}, Vertex{{ 0.5f,  0.5f,  0.5f}},
        Vertex{{ 0.5f,  0.5f, -0.5f}}, Vertex{{-0.5f,  0.5f, -0.5f}},

        // 아{랫면 (-Y)}
        Vertex{{-0.5f, -0.5f, 0.5f }}, Vertex{{ 0.5f, -0.5f, 0.5f }},
        Vertex{{ 0.5f, -0.5f, -0.5f}}, Vertex{{-0.5f, -0.5f, -0.5f}}
    }, {
        0, 1, 2, 0, 2, 3,     // 앞 (+Z)
        4, 6, 5, 4, 7, 6,     // 뒤 (-Z)
        8, 9,10, 8,10,11,     // 왼 (-X)
        12,13,14,12,14,15,   // 오 (+X)
        16,17,18,16,18,19,   // 위 (+Y)
        20,22,21,20,23,22    // 아래 (-Y)
    });

    constexpr float pixel_player_image = 64;
    auto get_uv_player = [&pixel_player_image](float x, float y){
        return Vec2(x/pixel_player_image, (pixel_player_image-y)/pixel_player_image);
    };

    // ======================== HEAD ========================
    {
        auto mesh_head = Mesh::GetMesh(MeshName::Cube);
        mesh_head.SetTextureCoordinate(std::vector<Vec2>{
            // +Z Front
            get_uv_player(8, 16),  get_uv_player(16, 16),
            get_uv_player(16, 8),  get_uv_player(8,8),

            // -Z Back
            get_uv_player(24, 16), get_uv_player(32, 16),
            get_uv_player(32, 8),  get_uv_player(24, 8),

            // -X Left
            get_uv_player(0, 16),  get_uv_player(8, 16),
            get_uv_player(8, 8),   get_uv_player(0, 8),

            // +X Right
            get_uv_player(16, 16), get_uv_player(24, 16),
            get_uv_player(24, 8),  get_uv_player(16, 8),

            // +Y Top
            get_uv_player(8, 8),   get_uv_player(16, 8),
            get_uv_player(16, 0),  get_uv_player(8, 0),

            // -Y Bottom
            get_uv_player(16,  8), get_uv_player(24,  8),
            get_uv_player(24,  0), get_uv_player(16,  0)
        });
        Mesh::CreateMesh(MeshName::PlayerHead, mesh_head);
    }
    // ======================== BODY ========================
    {
        auto mesh_body = Mesh::GetMesh(MeshName::Cube);
        mesh_body.SetTextureCoordinate(std::vector<Vec2>{
            // +Z Front
            get_uv_player(20, 32),  get_uv_player(28, 32),
            get_uv_player(28, 20),  get_uv_player(20, 20),

            // -Z Back
            get_uv_player(32, 32), get_uv_player(40, 32),
            get_uv_player(40, 20),  get_uv_player(32, 20),

            // -X Left
            get_uv_player(16, 32),  get_uv_player(20, 32),
            get_uv_player(20, 20),   get_uv_player(16, 20),

            // +X Right
            get_uv_player(28, 32), get_uv_player(32, 32),
            get_uv_player(32, 20),  get_uv_player(28, 20),

            // +Y Top
            get_uv_player(20, 20),   get_uv_player(28, 20),
            get_uv_player(28, 16),  get_uv_player(20, 16),

            // -Y Bottom
            get_uv_player(28,  20), get_uv_player(36,  20),
            get_uv_player(36,  16), get_uv_player(28,  16)

        });
        Mesh::CreateMesh(MeshName::PlayerBody, mesh_body);
    }

    // ======================== RIGHT ARM ========================
    {
        auto mesh_right_arm = Mesh::GetMesh(MeshName::Cube);
        mesh_right_arm.SetTextureCoordinate({
            // +Z Front
            get_uv_player(44, 32), get_uv_player(48, 32), 
            get_uv_player(48, 20), get_uv_player(44, 20),

            // -Z Back
            get_uv_player(52, 32), get_uv_player(56, 32), 
            get_uv_player(56, 20), get_uv_player(52, 20),

            // -X Left
            get_uv_player(40, 32), get_uv_player(44, 32), 
            get_uv_player(44, 20), get_uv_player(40, 20),

            // +X Right
            get_uv_player(52, 32), get_uv_player(56, 32), 
            get_uv_player(56, 20), get_uv_player(52, 20),

            // +Y Up
            get_uv_player(44, 20), get_uv_player(48, 20), 
            get_uv_player(48, 16), get_uv_player(44, 16),

            // -Y Down
            get_uv_player(48, 20), get_uv_player(52, 20), 
            get_uv_player(52, 16), get_uv_player(48, 16)
        });
        Mesh::CreateMesh(MeshName::PlayerRightArm, mesh_right_arm);
    }

    // ======================== Left ARM ========================
    {
        auto mesh_left_arm = Mesh::GetMesh(MeshName::Cube);
        mesh_left_arm.SetTextureCoordinate({
             // +Z Front
            get_uv_player(36, 64), get_uv_player(40, 64), 
            get_uv_player(40, 52), get_uv_player(36, 52),

            // -Z Back
            get_uv_player(44, 64), get_uv_player(48, 64), 
            get_uv_player(48, 52), get_uv_player(44, 52),

            // -X Left
            get_uv_player(32, 64), get_uv_player(36, 64), 
            get_uv_player(32, 52), get_uv_player(36, 52),

            // +X Right
            get_uv_player(40, 64), get_uv_player(44, 64), 
            get_uv_player(44, 52), get_uv_player(40, 52),

            // +Y Up
            get_uv_player(36, 52), get_uv_player(40, 52), 
            get_uv_player(40, 48), get_uv_player(36, 48),

            // -Y Down
            get_uv_player(40, 52), get_uv_player(44, 52), 
            get_uv_player(44, 48), get_uv_player(40, 48)
        });
        Mesh::CreateMesh(MeshName::PlayerLeftArm, mesh_left_arm);
    }

    // ======================== RIGHT LEG ========================
    {
        auto mesh_right_leg = Mesh::GetMesh(MeshName::Cube);
        mesh_right_leg.SetTextureCoordinate({
            // +Z Front
            get_uv_player(36-32, 64-32), get_uv_player(40-32, 64-32), 
            get_uv_player(40-32, 52-32), get_uv_player(36-32, 52-32),

            // -Z Back
            get_uv_player(44-32, 64-32), get_uv_player(48-32, 64-32), 
            get_uv_player(48-32, 52-32), get_uv_player(44-32, 52-32),

            // -X Left
            get_uv_player(32-32, 64-32), get_uv_player(36-32, 64-32), 
            get_uv_player(32-32, 52-32), get_uv_player(36-32, 52-32),

            // +X Right
            get_uv_player(40-32, 64-32), get_uv_player(44-32, 64-32), 
            get_uv_player(44-32, 52-32), get_uv_player(40-32, 52-32),

            // +Y Up
            get_uv_player(36-32, 52-32), get_uv_player(40-32, 52-32), 
            get_uv_player(40-32, 48-32), get_uv_player(36-32, 48-32),

            // -Y Down
            get_uv_player(40-32, 52-32), get_uv_player(44-32, 52-32), 
            get_uv_player(44-32, 48-32), get_uv_player(40-32, 48-32)
        });
        Mesh::CreateMesh(MeshName::PlayerRightLeg, mesh_right_leg);
    }

    // ======================== LEFT LEG ========================
    {
        auto mesh_left_leg = Mesh::GetMesh(MeshName::Cube);
        mesh_left_leg.SetTextureCoordinate({
             // +Z Front
            get_uv_player(36-16, 64), get_uv_player(40-16, 64), 
            get_uv_player(40-16, 52), get_uv_player(36-16, 52),

            // -Z Back
            get_uv_player(44-16, 64), get_uv_player(48-16, 64), 
            get_uv_player(48-16, 52), get_uv_player(44-16, 52),

            // -X Left
            get_uv_player(32-16, 64), get_uv_player(36-16, 64), 
            get_uv_player(32-16, 52), get_uv_player(36-16, 52),

            // +X Right
            get_uv_player(40-16, 64), get_uv_player(44-16, 64), 
            get_uv_player(44-16, 52), get_uv_player(40-16, 52),

            // +Y Up
            get_uv_player(36-16, 52), get_uv_player(40-16, 52), 
            get_uv_player(40-16, 48), get_uv_player(36-16, 48),

            // -Y Down
            get_uv_player(40-16, 52), get_uv_player(44-16, 52), 
            get_uv_player(44-16, 48), get_uv_player(40-16, 48)
        });
        Mesh::CreateMesh(MeshName::PlayerLeftLeg, mesh_left_leg);
    }

    std::vector<std::pair<std::string, Vec3>> vec_bones{
        {"left", Vec3(-0.5f, 0.f, 0.5f)},
        {"right", Vec3(0.5f, 0.f, 0.5f)}
    };

    AnimationComponent::CreateBone(std::move(vec_bones));

    std::vector<Weight> vec_weight{
        Weight(1, {"left"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"left"}, {1.f}),

        Weight(1, {"left"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"left"}, {1.f}),

        Weight(1, {"left"}, {1.f}),
        Weight(1, {"left"}, {1.f}),
        Weight(1, {"left"}, {1.f}),
        Weight(1, {"left"}, {1.f}),

        Weight(1, {"right"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"right"}, {1.f}),

        Weight(1, {"left"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"left"}, {1.f}),
        
        Weight(1, {"left"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"right"}, {1.f}),
        Weight(1, {"left"}, {1.f}),
    };

    AnimationComponent::CreateWeight(AnimationName::Player,std::move(vec_weight));

    Mesh::CreateMesh(MeshName::Plane, {
        Vertex{{-1.f , 0.f, 1.f}, {}, {}},
        Vertex{{ -1.f , 0.f, -1.f}, {}, {}},
        Vertex{{ 1.f , 0.f, -1.f}, {}, {}},
        Vertex{{ 1.f , 0.f, 1.f}, {}, {}}
    
    }, {
        0,2,1,
        0,3,2
    });

}
