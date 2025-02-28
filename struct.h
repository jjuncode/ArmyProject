#pragma once

struct Vec2
{
    int x;
    int y; 

    Vec2():x{},y{}{}
    Vec2(int _x, int _y ): x{_x}, y{_y}{};

    Vec2& operator -= (const Vec2& rhs){
       x -= rhs.x;
       y -= rhs.y;
       return *this;
    }

    bool operator != (const Vec2& rhs){
        if ((x== rhs.x) || (y==rhs.y) ) 
            return false;
        return true;
    }

    bool operator == (const Vec2& rhs){
        if ((x!= rhs.x) || (y!=rhs.y) ) 
            return false;
        return true;
    }
};

struct Vec2f
{
    float x;
    float y;

    Vec2f():x{},y{}{}
    Vec2f(float _x, float _y ): x{_x}, y{_y}{};
};