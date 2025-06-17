#include "Mesh.h"

Mesh Mesh::Square { {
    Vertex{ Vec3{ -0.5f, -0.5f, 0.0f }, RGBA{ 255, 0, 0 } },
    Vertex{ Vec3{  0.5f, -0.5f, 0.0f }, RGBA{ 0, 255, 0 } },
    Vertex{ Vec3{  0.5f,  0.5f, 0.0f }, RGBA{ 0, 0, 255 } },
    Vertex{ Vec3{ -0.5f,  0.5f, 0.0f }, RGBA{ 255, 255, 255 } }
}, 
{
    0, 1, 2, 
    0, 2, 3
}};
