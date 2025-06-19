#include "Mesh.h"

Mesh Mesh::Square { {
    Vertex{ Vec3{ -0.5f, -0.5f, 0.0f }, RGBA{ 255, 0, 0 }, Vec2(0.125f, 0.75f) },
    Vertex{ Vec3{  0.5f, -0.5f, 0.0f }, RGBA{ 0, 255, 0 },Vec2(0.25f, 0.75f) },
    Vertex{ Vec3{  0.5f,  0.5f, 0.0f }, RGBA{ 0, 0, 255 }, Vec2(0.25f, 0.875f) },
    Vertex{ Vec3{ -0.5f,  0.5f, 0.0f }, RGBA{ 255, 255, 255 }, Vec2(0.125f, 0.875f) }
}, 
{
    0, 1, 2, 
    0, 2, 3
}};
