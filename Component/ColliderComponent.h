#pragma once
#include "Component.h"
#include "../struct.h"
#include <unordered_map>
#include <vector>
#include "SFML/Graphics.hpp"
#include "../Mgr/SceneMgr.h"

enum class CollisionEntityType;

enum class CollisionStatus{
    kEnter,
    kStay,
    kExit,
    kNone,
    END
};

union CollisionInfoID{  // Entity ID
    struct{
        uint32_t left;
        uint32_t right;
    };

    uint64_t id;
};

struct OBB{
    OBB(Vec2 _size )
        : width_half{_size.x/2,0}
        , height_half{0,_size.y/2}
        , m_color{ sf::Color::Green}
        , obb_size_half{_size/2}
        {}

    Vec2 width_half; // half width
    Vec2 height_half;// half height
    Vec2 obb_size_half;
    sf::Color m_color;

    void Rotate(float angle);
};

class ColliderComponent : public Component {
    private:
        static std::unordered_map<uint64_t, CollisionStatus> m_map_collision_status;
        CollisionEntityType m_collision_type;
        OBB m_obb; // OBB collision
        
public:
    ColliderComponent(CollisionEntityType _type, Vec2 _size) 
        : m_collision_type(_type)
        , m_obb(_size)
        {}
    ~ColliderComponent();

    void Render() override;

    void Collision(uint32_t coll_id, MTV _mtv, float dt); // Collision rhs ID

    void Init();
    const OBB& GetOBB() { return m_obb; };
    void RotateOBB(float angle){m_obb.Rotate(angle);}
    Vec2 GetOBBSize(){return m_obb.obb_size_half;}
    void SetOBBSize(Vec2 _width, Vec2 _height ){m_obb.width_half = _width;
        m_obb.height_half = _height; }
    
    CollisionStatus GetCollisionStatus(uint32_t coll_entity_id);
    void SetCollisionStatus(uint32_t coll_entity_id, CollisionStatus status);

    CollisionEntityType GetCollisionType() const noexcept{
        return m_collision_type;
    }

    void CollisionEnter(uint32_t other_entity_id,MTV _mtv, float dt);
    void CollisionStay(uint32_t other_entity_id ,MTV _mtv, float dt);
    void CollisionExit(uint32_t other_entity_id ,MTV _mtv, float dt);

    void SetOBBColor(sf::Color color){m_obb.m_color = color;};
};