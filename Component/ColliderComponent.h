#pragma once
#include "Component.h"
#include <unordered_map>

enum class CollisionEntityType;

enum class CollisionStatus{
    kEnter,
    kStay,
    kExit,
    kNone,
    END
};

union CollisionInfoID{
    struct{
        uint32_t left;
        uint32_t right;
    };

    uint64_t id;
};

class ColliderComponent : public Component {
    private:
        CollisionEntityType m_collision_type;
        static std::unordered_map<uint64_t, CollisionStatus> m_map_collision_status;

    public:
        ColliderComponent() = default;

        void Render() override;

        void Collision(uint32_t coll_id);    // Collision rhs ID
        void Init(CollisionEntityType _type);

        CollisionStatus GetCollisionStatus(uint32_t coll_entity_id);

        void CollisionEnter();
        void CollisionStay();
        void CollisionExit(uint32_t coll_entity_id);

        ~ColliderComponent();
};