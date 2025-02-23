#pragma once
#include "Component.h"
#include <memory.h>
#include "SFML/Graphics.hpp"

class RenderComponent : public Component
{
    private:
        std::unique_ptr<sf::CircleShape> m_shape;

    public:
        template<typename T>
        void SetShape(uint32_t radius = 1){m_shape = std::make_unique<T>(radius);}
        void Render() override;
};
