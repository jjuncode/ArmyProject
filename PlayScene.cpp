#include "PlayScene.h"
#include "pch.h"
#include "Entity.h"
#include "SceneMgr.h"
#include "RenderComponent.h"
#include "TransformComponent.h"

void PlayScene::Init()
{
    auto& cur_scene = SceneMgr::GetCurScene();

    Entity obj;
    auto comp = obj.AddComponent<RenderComponent>();
    cur_scene->AddComponent<RenderComponent>(std::move(comp));

    auto transform = obj.AddComponent<TransformComponent>();
    transform->SetPos(Vec2(-25, 10 ));
    transform->SetScale(100);

    cur_scene->AddComponent<TransformComponent>(std::move(transform));
}

void PlayScene::Update(float dt)
{
}
