#include "Polygon.h"
#include "../Component/TransformComponent.h"

void Polygon::SetScript()
{
}

void Polygon::CreateVertex(Vec2 _pos, Vec2 _scale)
{
    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(obj.GetEntityID());

    double angleIncrement = 2   * M_PI / sides; // Angle between vertices
    for (int i = 0; i < sides; ++i) {
        double x = cos(i * angleIncrement);
        double y = sin(i * angleIncrement);
        transform->SetVertex(Vec2(x * _scale.x , y * _scale.y));
    }

    transform->CreateEdge();
}