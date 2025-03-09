#pragma once
#include <vector>
#include <stdint.h>

class ColliderComponent;

class CollisionMgr{
    public:
        static void Collision();
    private:
        static void CollisionCheck(const std::vector<uint32_t>& left
            , const std::vector<uint32_t>& right);

        static bool CollisionLogic(ColliderComponent* left, ColliderComponent* right);
};