#pragma once
#include "Component.h"
#include "../struct.h"
#include <unordered_map>
#include <vector>
#include "SFML/Graphics.hpp"

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
    Vec2 width_half; // half width
    Vec2 height_half;// half height
    sf::Color m_color;

    void Rotate(float angle){
        // Rotate
        Vec2 width_new;
        width_new.x = width_half.x * cos(angle) - width_half.y * sin(angle);
        width_new.y = width_half.x * sin(angle) + width_half.y * cos(angle);
    
        Vec2 height_new;
        height_new.x= height_half.x * cos(angle) - height_half.y * sin(angle);
        height_new.y= height_half.x * sin(angle) + height_half.y * cos(angle);

        width_half = width_new;
        height_half = height_new;
    }
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

    void Collision(uint32_t coll_id, float dt); // Collision rhs ID
    void Init(CollisionEntityType _type);
    void SetOBB(Vec2 _size){
        m_obb.width_half = Vec2(_size.x/2,0);
        m_obb.height_half = Vec2(0,_size.y/2);

        m_obb.m_color = sf::Color::Green;
    }

    const OBB& GetOBB() { return m_obb; };
    void RotateOBB(float angle){m_obb.Rotate(angle);}
    
    CollisionStatus GetCollisionStatus(uint32_t coll_entity_id);
    void SetCollisionStatus(uint32_t coll_entity_id, CollisionStatus status);

    void CollisionEnter(uint32_t other_entity_id, float dt);
    void CollisionStay(uint32_t other_entity_id , float dt);
    void CollisionExit(uint32_t other_entity_id , float dt);

    void SetOBBColor(sf::Color color){m_obb.m_color = color;};
};