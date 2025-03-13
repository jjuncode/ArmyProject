#pragma once
#include "Component.h"
#include "../struct.h"
#include <unordered_map>
#include <vector>

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
        static std::unordered_map<uint64_t, CollisionStatus> m_map_collision_status;
        CollisionEntityType m_collision_type;
        std::vector<Edge> m_vec_entity_edge;     // SAT collision Edge

public:
    ColliderComponent() = default;
    ~ColliderComponent();

    void Render() override;

    void Collision(uint32_t coll_id); // Collision rhs ID
    void Init(CollisionEntityType _type);

    CollisionStatus GetCollisionStatus(uint32_t coll_entity_id);

    void CollisionEnter();
    void CollisionStay();
    void CollisionExit(uint32_t coll_entity_id);

private:
    // ==================
    // SAT Func
    // ==================
    void CreateEdge();


};