#pragma once
#include "Component.h"
#include <memory.h>
#include "SFML/Graphics.hpp"

class RenderComponent : public Component
{
    private:
        std::unique_ptr<sf::Shape> m_shape;

    public:
        template<typename T, typename... V>
        void SetShape(V&&... params){
            std::unique_ptr<sf::Shape> shape = std::make_unique<T>(std::forward<V>(params)...);
            m_shape = std::move(shape);
        }
        
        void Render() override;
};
