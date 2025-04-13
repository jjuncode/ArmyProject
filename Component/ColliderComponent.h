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

struct OBB{
    Vec2 width_half; // half width
    Vec2 height_half;// half height
};

class ColliderComponent : public Component {
    private:
        static std::unordered_map<uint64_t, CollisionStatus> m_map_collision_status;
        CollisionEntityType m_collision_type;
        OBB m_obb; // OBB collision

public:
    ColliderComponent() = default;
    ~ColliderComponent();

    void Render() override;

    void Collision(uint32_t coll_id); // Collision rhs ID
    void Init(CollisionEntityType _type);
    void SetOBB(Vec2 _size){
        m_obb.width_half = Vec2(_size.x/2,0);
        m_obb.height_half = Vec2(0,-_size.y/2);
    }

    const OBB& GetOBB() { return m_obb; };

    CollisionStatus GetCollisionStatus(uint32_t coll_entity_id);

    void CollisionEnter();
    void CollisionStay();
    void CollisionExit(uint32_t coll_entity_id);
};