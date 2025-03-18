#pragma once
#include <list>
#include <stdint.h>

class ColliderComponent;

class CollisionMgr{
    public:
        static void Collision();
    private:
        static void CollisionCheck(const std::list<uint32_t>& left
            , const std::list<uint32_t>& right);

        static bool CollisionLogic(ColliderComponent* left, ColliderComponent* right);
        static bool SATCollision(ColliderComponent* left, ColliderComponent* right);

};