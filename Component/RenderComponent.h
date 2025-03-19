#pragma once
#include "Component.h"
#include <memory.h>
#include "SFML/Graphics.hpp"
#include "../struct.h"

class RenderComponent : public Component
{
    public:
        RenderComponent(){}
        
        void Render() override;
};
