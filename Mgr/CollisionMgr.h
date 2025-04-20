#pragma once
#include <list>
#include <stdint.h>
#include "../struct.h"

class ColliderComponent;

struct MTV{
    Vec2 vec;
    float length;
};

class CollisionMgr{
    public:
        static void Collision(float dt);
    private:
        static void CollisionCheck(const std::list<uint32_t>& left
            , const std::list<uint32_t>& right
            , float dt);

        static bool CollisionLogic(ColliderComponent* left, ColliderComponent* right);
        static bool OBBCollision_Logic(ColliderComponent* left, ColliderComponent* right);
};