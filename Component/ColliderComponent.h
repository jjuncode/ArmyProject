#pragma once
#include "Component.h"

enum class CollisionEntityType;

class ColliderComponent : public Component {
    public:
        ColliderComponent(CollisionEntityType _type){
            Init(_type);
        }

        void Render() override;

    private:
        void Init(CollisionEntityType _type);
};