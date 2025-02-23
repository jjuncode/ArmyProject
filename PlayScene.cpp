#include "PlayScene.h"
#include "pch.h"
#include "Object/CircleObject.h"

void PlayScene::Init()
{
    CircleObject circle;
    circle.Init(Vec2(-50,100), 50, sf::Color::Cyan);

    CircleObject circle2;
    circle2.Init(Vec2(50,100), 50, sf::Color::Red);

    CircleObject circle3;
    circle3.Init(Vec2(-50,-100), 50, sf::Color::White);

}

void PlayScene::Update(float dt)
{
}