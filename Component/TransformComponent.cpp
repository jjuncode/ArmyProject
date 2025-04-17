#include "TransformComponent.h"
#include "../pch.h"
#include "ColliderComponent.h"
#include "../Mgr/SceneMgr.h"

void TransformComponent::Update(float dt)
{
}

void TransformComponent::AddRotate(float offset)
{  
    for (auto& dot : m_vec_vertex){
        // Rotate
        auto x = dot.x * cos(offset) - dot.y * sin(offset);
        auto y = dot.x * sin(offset) + dot.y * cos(offset);
    
        dot.x = x;
        dot.y = y;
    }

    auto collider = SceneMgr::GetComponentOrigin<ColliderComponent>(GetOwnerID());
    if (collider ){
        collider->RotateOBB(offset);
        auto obb = collider->GetOBB();
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
