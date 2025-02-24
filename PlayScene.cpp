#include "PlayScene.h"
#include "pch.h"
#include "Object/CircleObject.h"
#include "Object/GridObject.h"
#include "Object/RectObject.h"

void PlayScene::Init()
{
    CircleObject circle;
    circle.Init(Vec2(25,50),10, sf::Color::Cyan);

    CircleObject circle2;
    circle2.Init(Vec2(50,100), 10, sf::Color::Red);

    CircleObject circle3;
    circle3.Init(Vec2(70,70), 10, sf::Color::White);

    // GridObject grid;
    // grid.Init(20);

    RectObject rect;
    rect.Init(Vec2(400,300), Vec2(50,50), sf::Color::White);
}

void PlayScene::Update(float dt)
{
}