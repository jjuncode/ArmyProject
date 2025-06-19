#include "SceneMgr.h"
#include "../pch.h"
#include "../Component/Texture.h"

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
