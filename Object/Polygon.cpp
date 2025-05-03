#include "Polygon.h"
#include "../Component/TransformComponent.h"

void Polygon::CreateVertex(Vec2 _pos, Vec2 _scale)
{
    auto transform = SceneMgr::GetComponent<TransformComponent>(obj.GetEntityID());

    float angleIncrement = 2   * M_PI / sides; // Angle between vertices
    angleIncrement = -angleIncrement; // Reverse the direction to clockwise
    for (int i = 0; i < sides; ++i) {
        float x = cos(i * angleIncrement);
        float y = sin(i * angleIncrement);
        transform->SetVertex(Vec2(x * _scale.x , y * _scale.y));
    }
}