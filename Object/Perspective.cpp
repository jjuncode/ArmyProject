#include "Perspective.h"

void PerspectiveTest::Clipping(std::vector<Vertex> &_vertexs)
{
    std::size_t tri_cnt = _vertexs.size() / 3;
    for (std::size_t i = 0; i < tri_cnt; ++i){
        std::size_t start_idx = i * 3;
        std::size_t cnt_outside_point = 0;

        for (std::size_t j = 0; j < 3; ++j){
            result_point[j] = IsPointOutsideFunc(_vertexs[start_idx + j]);

            if (result_point[j]){
                ++cnt_outside_point;
            }
        }

        if (cnt_outside_point == 0){
            continue;
        }
        else if (cnt_outside_point == 1){
            // One point is outside the frustum
            DivideToTwoTriangles(_vertexs, start_idx);
        }
        else if (cnt_outside_point == 2){
            // Two points are outside the frustum
            ClipTriangle(_vertexs, start_idx);
        }
        else
        {
            // All points are outside the frustum
            _vertexs.erase(_vertexs.begin() + start_idx, _vertexs.begin() + start_idx + 3);
            --tri_cnt;
            --i;
        }
    }
}

void PerspectiveTest::DivideToTwoTriangles(std::vector<Vertex> &_vertexs, std::size_t _start_idx)
{
    std::size_t idx{};
    if ( result_point[0] ) {
        idx = 0;
    } else if ( result_point[1] ) {
        idx = 1;
    } else {
        idx = 2;
    }

    std::size_t idx_v1 = _start_idx + (idx + 1) % 3;
    std::size_t idx_v2 = _start_idx + (idx + 2) % 3;

    Vertex v1 = _vertexs[idx_v1];
    Vertex v2 = _vertexs[idx_v2];

    Vertex clipped_1 = GetProjectionToPlaneFunc(_vertexs[_start_idx + idx], v1);
    Vertex clipped_2 = GetProjectionToPlaneFunc(_vertexs[_start_idx + idx], v2);

    _vertexs[_start_idx] = clipped_1;
    _vertexs[_start_idx + 1] = v1;
    _vertexs[_start_idx + 2] = v2;

    _vertexs.push_back(clipped_1);
    _vertexs.push_back(v2);
    _vertexs.push_back(clipped_2);
}

void PerspectiveTest::ClipTriangle(std::vector<Vertex> &_vertexs, std::size_t _start_idx)
{
    std::size_t idx{};
    if ( result_point[0] ) {
        idx = 0;
    } else if ( result_point[1] ) {
        idx = 1;
    } else {
        idx = 2;
    }

    std::size_t idx_v1 = _start_idx + (idx + 1) % 3;
    std::size_t idx_v2 = _start_idx + (idx + 2) % 3;

    Vertex v1 = _vertexs[idx_v1];
    Vertex v2 = _vertexs[idx_v2];

    Vertex clipped_1 = GetProjectionToPlaneFunc(_vertexs[_start_idx + idx], v1);
    Vertex clipped_2 = GetProjectionToPlaneFunc(_vertexs[_start_idx + idx], v2);

    _vertexs[idx_v1] = clipped_1;
    _vertexs[idx_v2] = clipped_2;
}
