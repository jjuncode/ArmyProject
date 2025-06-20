#include "struct.h"
#include <algorithm>
#include "Core.h"

float Vec::LengthSquare(Vec2 _vec)
{
    return (_vec.x * _vec.x + _vec.y * _vec.y);
}

float Vec::Length(Vec2 _vec)
{
    return sqrt(LengthSquare(_vec));
}

Vec2 Vec::Normalize(const Vec2 &vec)
{
    float length = LengthSquare(vec);
    if (length == 0) 
        return Vec2(0, 0);
    return Vec2(vec.x / sqrt(length), vec.y / sqrt(length));
}

float Vec::Dot(const Vec2 &lhs, const Vec2 &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

float Vec::Cross(const Vec2 &lhs, const Vec2 &rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

Vec2 Vec::Normal(const Vec2 &vec)
{
    // return Vec2(-vec.y, vec.x);
    // 시계방향
    return Vec2(vec.y , -vec.x);
}

Vec2 Vec::Projection(Vec2 vec_unit, Vec2 rhs)
{
    float dot = Dot(vec_unit, rhs);
    return Vec2(dot * vec_unit.x, dot * vec_unit.y);
}

Vec2 Vec::Reverse(const Vec2 &vec)
{
    return Vec2(-vec.x, -vec.y);
}

float Vec::GetDegree(Vec2 _v1, Vec2 _v2)
{
   // 두 벡터 사이의 각도(라디안) 구하기
   auto dot = Vec::Dot(Vec::Normalize(_v1), Vec::Normalize(_v2));
   dot = std::clamp(dot, -1.0f, 1.0f );
   auto angle = std::acos(dot);
   float degree = angle * ( 180.0f / 3.14159265f);
   return degree;
}

float Vec::GetRadian(float _v)
{
   return _v * (3.14159265f / 180.f);
}

Vec2 Vec::ConvertToScreenCoord(const Vec2 &vec)
{
    auto window = Core::GetWindowSize();
    return Vec2(vec.x + window.x / 2, -vec.y + window.y / 2);
}

Vec2 Vec::ConvertToCartesian(const Vec2 &vec)
{
    auto window = Core::GetWindowSize();
    return Vec2(vec.x+0.5f - window.x / 2.f, -(vec.y+0.5f) + window.y / 2.f);
}

std::ostream &operator<<(std::ostream &os, const Vec2 &vec)
{
    os << vec.x << ", " << vec.y;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Vec3 &vec)
{
    os << vec.x << ", " << vec.y << ", " << vec.z;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Mat3 &mat)
{
    os << "MAT3:\n";
    os << "[" << mat[0] << "]\n";
    os << "[" << mat[1] << "]\n";
    os << "[" << mat[2] << "]\n";
    return os;
}