#pragma once

struct Vec2
{
    int x;
    int y; 

    Vec2():x{},y{}{}
    Vec2(int _x, int _y ): x{_x}, y{_y}{};
};

struct Vec2f
{
    float x;
    float y;

    Vec2f():x{},y{}{}
    Vec2f(float _x, float _y ): x{_x}, y{_y}{};
};