#pragma once
#include <math.h>
#include <vector>

struct Vec2
{
    float x;
    float y; 

    Vec2():x{},y{}{}
    Vec2(float _x, float _y ): x{_x}, y{_y}{};
  
    // 복사 생성자
    Vec2(const Vec2& other): x{other.x}, y{other.y} {}

    Vec2 operator- (const Vec2& rhs){
        return Vec2(x-rhs.x, y-rhs.y);
    }

    Vec2 operator+ (const Vec2& rhs){
        return Vec2(x+rhs.x, y+rhs.y);
    }

    Vec2 operator* (float rhs){
        return Vec2(x*rhs, y*rhs);
    }

    Vec2 operator* (const Vec2& rhs){
        return Vec2(x*rhs.x, y*rhs.y);
    }

    Vec2& operator -= (const Vec2& rhs){
       x -= rhs.x;
       y -= rhs.y;
       return *this;
    }
    
    Vec2& operator += (const Vec2& rhs){
        x += rhs.x;
        y += rhs.y;
        return *this;
     }

     Vec2& operator *= (const float rhs){
        x*= rhs;
        y*= rhs;
        return *this;
     }

     Vec2& operator *= (const Vec2& rhs){
        x*= rhs.x;
        y*= rhs.y;
        return *this;
     }

     Vec2 operator / (float rhs){
        return Vec2 ( x/rhs, y/rhs);
     }

     Vec2& operator /= (float rhs){
        x /= rhs;
        y /= rhs;
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
    
    Edge operator- (const Edge& rhs){
        return Edge(start - rhs.start,end - rhs.end);
    }

    Edge operator+ (const Vec2& rhs){
        return Edge(start + rhs, end + rhs);
    }

    Edge operator - (const Vec2& rhs){
        return Edge(start - rhs, end - rhs);
    }

    static std::vector<Edge> CreateEdge(std::vector<Vec2> vec_vertex);
};

namespace Vec{
    float EdgeDistnSquare(Edge _edge);

    float LengthSquare(Vec2 _vec);
    float Length(Vec2 _vec);

    Vec2 ChangeVec(Edge _edge);

    Vec2 NormalizeEdge(Edge _edge);
    Vec2 Normalize(const Vec2 &vec);

    float Dot(const Vec2 &lhs, const Vec2 &rhs);
    float Cross(const Vec2 &lhs, const Vec2 &rhs);
    Vec2 Projection(Vec2 vec_unit, Vec2 rhs);

    Vec2 Normal(const Vec2& vec);

    Vec2 Reverse(const Vec2& vec);
}

struct MTV{
    Vec2 vec;
    float length;
};