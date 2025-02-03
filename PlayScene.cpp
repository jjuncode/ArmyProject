#include "PlayScene.h"
#include "pch.h"
#include "Entity.h"
#include "SceneMgr.h"
#include "RenderComponent.h"

void PlayScene::Init()
{
    std::unique_ptr<Scene>& cur_scene = SceneMgr::GetCurScene();
    cur_scene->AddComponent<RenderComponent>();
}

void PlayScene::Update(float dt)
{
}
