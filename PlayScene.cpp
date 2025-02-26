#include "PlayScene.h"
#include "pch.h"
#include "Object/CircleObject.h"
#include "Object/GridObject.h"
#include "Object/RectObject.h"
#include "Mgr/InputMgr.h"

void PlayScene::Init()
{
    CircleObject circle;
    circle.Init(Vec2(25,50),10, sf::Color::Cyan);

    CircleObject circle2;
    circle2.Init(Vec2(50,100), 10, sf::Color::Red);

    CircleObject circle3;
    circle3.Init(Vec2(70,70), 10, sf::Color::White);

    GridObject grid;
    constexpr uint32_t gird_offset{40};
    grid.Init(gird_offset);

    RectObject rect;
    rect.Init(Vec2(60-20,60-20), Vec2(gird_offset,gird_offset)
            , sf::Color::Red);
}

void PlayScene::Update(float dt)
{
}