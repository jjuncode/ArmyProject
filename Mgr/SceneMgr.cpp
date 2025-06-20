#include "SceneMgr.h"
#include "../pch.h"
#include "../Component/Texture.h"
#include "../Component/Mesh.h"

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
}
