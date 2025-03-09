#pragma once
#include "Component.h"

enum class CollisionEntityType;

class ColliderComponent : public Component {
    private:
        CollisionEntityType m_type;

    public:
        ColliderComponent() = default;

        void Render() override;

        void Collision(uint32_t coll_entity_id);    // Collision rhs ID
        void Init(CollisionEntityType _type);

        ~ColliderComponent();
};