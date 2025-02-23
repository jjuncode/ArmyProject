#include "PlayScene.h"
#include "pch.h"
#include "CircleObject.h"

void PlayScene::Init()
{
    CircleObject circle;
    circle.Init(Vec2(-50,100), 50, sf::Color::Cyan);
}

void PlayScene::Update(float dt)
{
}