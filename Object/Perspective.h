#pragma once
#include "../struct.h"
#include <functional>

struct PerspectiveTest{
    std::function<bool(const Vertex&)> IsPointOutsideFunc;
    std::function<Vertex(const Vertex&, const Vertex&)> GetProjectionToPlaneFunc;

    std::array<bool, 3 > result_point;

    PerspectiveTest(std::function<bool(const Vertex&)> _is_outside_func,
                    std::function<Vertex(const Vertex&, const Vertex&)> _projection_func)
        : IsPointOutsideFunc{_is_outside_func}, GetProjectionToPlaneFunc{_projection_func} 
        {}

    void Clipping(std::vector<Vertex>& _vertexs);
    void DivideToTwoTriangles(std::vector<Vertex>& _vertexs, std::size_t _start_idx);
    void ClipTriangle(std::vector<Vertex>& _vertexs, std::size_t _start_idx);
};

static auto IsPointOutsideW0 = [](const Vertex& v) {
    return v.v.w < 0.f;
};

static auto ProjectionToPlaneW0 = [](const Vertex& _start, const Vertex& _end) {
    float p1 = _start.v.w;
    float p2 = _end.v.w;
    float t = p1 / (p1 - p2);

    Vertex result;
    result.v     = _start.v * (1.f - t) + _end.v * t;
    result.color = _start.color * (1.f - t) + _end.color * t;
    result.uv    = _start.uv * (1.f - t) + _end.uv * t;
    return result;
};

static auto IsPointOutsidePY = [](const Vertex& v) {
    return v.v.y > v.v.w;
};

static auto ProjectionToPlanePY = [](const Vertex& _start, const Vertex& _end) {
    float p1 = _start.v.w - _start.v.y;
    float p2 = _end.v.w - _end.v.y;
    float t = p1 / (p1 - p2);

    Vertex result;
    result.v     = _start.v * (1.f - t) + _end.v * t;
    result.color = _start.color * (1.f - t) + _end.color * t;
    result.uv    = _start.uv * (1.f - t) + _end.uv * t;
    return result;
};

static auto IsPointOutsideMY = [](const Vertex& v) {
    return v.v.y < -v.v.w;
};

static auto ProjectionToPlaneMY = [](const Vertex& _start, const Vertex& _end) {
    float p1 = _start.v.y + _start.v.w;
    float p2 = _end.v.y + _end.v.w;
    float t = p1 / (p1 - p2);

    Vertex result;
    result.v     = _start.v * (1.f - t) + _end.v * t;
    result.color = _start.color * (1.f - t) + _end.color * t;
    result.uv    = _start.uv * (1.f - t) + _end.uv * t;
    return result;
};

static auto IsPointOutsidePX = [](const Vertex& v) {
    return v.v.x > v.v.w;
};

static auto ProjectionToPlanePX = [](const Vertex& _start, const Vertex& _end) {
    float p1 = _start.v.w - _start.v.x;
    float p2 = _end.v.w - _end.v.x;
    float t = p1 / (p1 - p2);

    Vertex result;
    result.v     = _start.v * (1.f - t) + _end.v * t;
    result.color = _start.color * (1.f - t) + _end.color * t;
    result.uv    = _start.uv * (1.f - t) + _end.uv * t;
    return result;
};

static auto IsPointOutsideMX = [](const Vertex& v) {
    return v.v.x < -v.v.w;
};

static auto ProjectionToPlaneMX = [](const Vertex& _start, const Vertex& _end) {
    float p1 = _start.v.x + _start.v.w;
    float p2 = _end.v.x + _end.v.w;
    float t = p1 / (p1 - p2);

    Vertex result;
    result.v     = _start.v * (1.f - t) + _end.v * t;
    result.color = _start.color * (1.f - t) + _end.color * t;
    result.uv    = _start.uv * (1.f - t) + _end.uv * t;
    return result;
};

static auto IsPointOutsidePZ = [](const Vertex& v) {
    return v.v.z > v.v.w;
};

static auto ProjectionToPlanePZ = [](const Vertex& _start, const Vertex& _end) {
    float p1 = _start.v.w - _start.v.z;
    float p2 = _end.v.w - _end.v.z;
    float t = p1 / (p1 - p2);

    Vertex result;
    result.v     = _start.v * (1.f - t) + _end.v * t;
    result.color = _start.color * (1.f - t) + _end.color * t;
    result.uv    = _start.uv * (1.f - t) + _end.uv * t;
    return result;
};

static auto IsPointOutsideMZ = [](const Vertex& v) {
    return v.v.z < -v.v.w;
};

static auto ProjectionToPlaneMZ = [](const Vertex& _start, const Vertex& _end) {
    float p1 = _start.v.z + _start.v.w;
    float p2 = _end.v.z + _end.v.w;
    float t = p1 / (p1 - p2);

    Vertex result;
    result.v     = _start.v * (1.f - t) + _end.v * t;
    result.color = _start.color * (1.f - t) + _end.color * t;
    result.uv    = _start.uv * (1.f - t) + _end.uv * t;
    return result;
};
