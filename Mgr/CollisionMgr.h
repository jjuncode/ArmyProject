#pragma once
#include <list>
#include <stdint.h>
#include "../struct.h"

class ColliderComponent;

class CollisionMgr{
    public:
        static void Collision(float dt);
    private:
        static void CollisionCheck(ColliderComponent *left, ColliderComponent *right, float dt);

        static std::pair<bool, MTV>  CollisionLogic(ColliderComponent* left, ColliderComponent* right);
        static std::pair<bool, MTV> OBBCollision_Logic(ColliderComponent* left, ColliderComponent* right);
};