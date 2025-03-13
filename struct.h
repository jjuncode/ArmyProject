#pragma once

struct Vec2
{
    float x;
    float y; 

    Vec2():x{},y{}{}
    Vec2(float _x, float _y ): x{_x}, y{_y}{};

    Vec2 operator- (const Vec2& rhs){
        return Vec2(x-rhs.x, y-rhs.y);
    }

    Vec2 operator+ (const Vec2& rhs){
        return Vec2(x+rhs.x, y+rhs.y);
    }

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

struct Edge{
    Vec2 start;
    Vec2 end;

    Edge(Vec2 _start, Vec2 _end ) : start{_start} , end{_end}{}
    Edge(){}
};

// Edge Normalize(const Edge& _edge) {
    
// }
