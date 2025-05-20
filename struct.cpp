#include "struct.h"
#include <algorithm>

float Vec::EdgeDistnSquare(Edge _edge)
{
        Vec2 vec = _edge.end - _edge.start;
        float length = (vec.x * vec.x) + (vec.y * vec.y);
        return length;
}

float Vec::LengthSquare(Vec2 _vec)
{
    return (_vec.x * _vec.x + _vec.y * _vec.y);
}

float Vec::Length(Vec2 _vec)
{
    return sqrt(LengthSquare(_vec));
}

Vec2 Vec::ChangeVec(Edge _edge)
{
    return Vec2(_edge.end.x - _edge.start.x, _edge.end.y - _edge.start.y);
}

Vec2 Vec::NormalizeEdge(Edge _edge)
{
    float distn = EdgeDistnSquare(_edge);
    Vec2 vec = _edge.end - _edge.start;
    return Vec2(vec.x / sqrt(distn), vec.y / sqrt(distn));
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

std::vector<Edge> Edge::CreateEdge(std::vector<Vec2> vec_vertex)
{
    std::vector<Edge> vec_edge;
	// Vec2 start;
	// Edge edge;

    // int cnt{};
    // for (const auto &dot : vec_vertex){
    //     ++cnt;
    //     if (cnt == 1){
    //         edge.start = dot;
    //         start = dot;
    //     }
    //     else{
    //         edge.end = dot;
    //         vec_edge.emplace_back(edge);

    //         edge.start = dot;
    //     }
    // }

    // // Finish Edge
    // Edge edge_end{edge.end, start};
    // vec_edge.emplace_back(edge_end);

    for (size_t i = 0; i < vec_vertex.size(); ++i) {
        vec_edge.push_back(Edge(vec_vertex[i], vec_vertex[(i + 1) % vec_vertex.size()]));
    }
    
    return vec_edge;
}
