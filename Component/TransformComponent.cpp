#include "TransformComponent.h"
#include "../pch.h"

void TransformComponent::Update(float dt)
{
}

void TransformComponent::CreateEdge()
{
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
            m_vec_edge.emplace_back(edge);

            edge.start = dot;
        }
    }

    // Finish Edge
    Edge edge_end{edge.end, start};
    m_vec_edge.emplace_back(edge_end);
}
