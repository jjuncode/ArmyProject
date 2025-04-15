#include "TransformComponent.h"
#include "../pch.h"

void TransformComponent::Update(float dt)
{
}

void TransformComponent::AddRotate(float offset)
{  
    for (auto& dot : m_vec_vertex){
        // Rotate
        dot.x = dot.x * cos(offset) - dot.y * sin(offset);
        dot.y = dot.x * sin(offset) + dot.y * cos(offset);
    }
}

const std::vector<Edge> TransformComponent::CreateEdge()
{
    std::vector<Edge> vec_edge;
	Vec2 start;
	Edge edge;

    int cnt{};
    for (const auto &dot : m_vec_vertex){
        ++cnt;
        if (cnt == 1){
            edge.start = dot;
            start = dot;
        }
        else{
            edge.end = dot;
            vec_edge.emplace_back(edge);

            edge.start = dot;
        }
    }

    // Finish Edge
    Edge edge_end{edge.end, start};
    vec_edge.emplace_back(edge_end);

    return vec_edge;
}
