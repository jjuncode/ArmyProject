#include "Mesh.h"

Mesh Mesh::Square { {
    Vertex{ Vec3{ -0.5f, -0.5f, 0.0f }, RGBA{ 1, 0, 0 } },
    Vertex{ Vec3{  0.5f, -0.5f, 0.0f }, RGBA{ 0, 1, 0 } },
    Vertex{ Vec3{  0.5f,  0.5f, 0.0f }, RGBA{ 0, 0, 1 } },
    Vertex{ Vec3{ -0.5f,  0.5f, 0.0f }, RGBA{ 1, 1, 1 } }
}, 
{
    3, 2, 1,
    3, 1, 0
}};
