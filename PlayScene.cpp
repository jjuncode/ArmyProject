#include "PlayScene.h"
#include "pch.h"
#include "Entity.h"
#include "SceneMgr.h"
#include "RenderComponent.h"

void PlayScene::Init()
{
    std::unique_ptr<Scene>& cur_scene = SceneMgr::GetCurScene();

    Entity obj;
    auto comp = obj.AddComponent<RenderComponent>();
    cur_scene->AddComponent(std::move(comp));
}

void PlayScene::Update(float dt)
{
}
