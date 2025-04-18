#pragma once
#include "Component.h"

class ScriptComponent : public Component {
    virtual void Execute(float dt); // script execute
    
    virtual void ExecuteCollEnter(uint32_t other_entity_id, float dt);
    virtual void ExecuteCollStay(uint32_t other_entity_id, float dt);
    virtual void ExecuteCollExit(uint32_t other_entity_id, float dt);
};