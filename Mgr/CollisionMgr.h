#pragma once
#include <list>
#include <stdint.h>
#include "../struct.h"

class ColliderComponent;

class CollisionMgr{
    public:
        static void Collision(float dt);
    private:
        static void CollisionCheck(const std::list<uint32_t>& left
            , const std::list<uint32_t>& right
            , float dt);

        static void CollisionCheckSameGroup(const std::list<uint32_t>& _group, float dt);


        static std::pair<bool, MTV>  CollisionLogic(ColliderComponent* left, ColliderComponent* right);
        static std::pair<bool, MTV> OBBCollision_Logic(ColliderComponent* left, ColliderComponent* right);
};