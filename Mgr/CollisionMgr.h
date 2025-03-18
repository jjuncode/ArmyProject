#pragma once
#include <list>
#include <stdint.h>
#include "../struct.h"

class ColliderComponent;

struct MTV{
    Vec2 vec;
    float length;
};

struct CollisionInfo{
    bool is_collision;
    MTV mtv;

    CollisionInfo(bool _is_collision, MTV _mtv = MTV()): is_collision{_is_collision}, mtv{_mtv}{}
};

class CollisionMgr{
    public:
        static void Collision();
    private:
        static void CollisionCheck(const std::list<uint32_t>& left
            , const std::list<uint32_t>& right);

        static CollisionInfo CollisionLogic(ColliderComponent* left, ColliderComponent* right);
        static CollisionInfo SATCollision_Logic(ColliderComponent* left, ColliderComponent* right);

};