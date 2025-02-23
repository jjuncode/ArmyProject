#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>

class ColorComponent : public Component{
    private:
        sf::Color m_color;
        
    public:
        void Update(float dt) override;

        sf::Color GetColor() { return m_color; }
        void SetColor(const sf::Color &_color) { m_color = _color; }
};