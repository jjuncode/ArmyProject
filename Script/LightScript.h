#pragma once
#include "Script.h"
#include "../SFML/Graphics.hpp"

class LightScript : public Script {
private:
    sf::Color m_light_color;
    float m_range;

public:
    LightScript(sf::Color _color) : m_light_color{_color} {}

    void Execute(float dt) override; // script execute
    sf::Color GetLightColor() { return m_light_color; }
    void SetRange(float _r ){m_range = _r ;}
    float GetRange() { return m_range;}
};