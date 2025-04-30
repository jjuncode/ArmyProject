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

    auto collider = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    if (collider ){
        collider->RotateOBB(offset);
        auto obb = collider->GetOBB();
    }
}

const std::vector<Edge> TransformComponent::CreateEdge()
{
    return Edge::CreateEdge(m_vec_vertex);
}
