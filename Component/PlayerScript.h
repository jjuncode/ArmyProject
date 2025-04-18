#pragma once
#include "ScriptComponent.h"

class PlayerScript : public ScriptComponent {
    public:
        void Execute(float dt) override; // script execute
        
        void ExecuteCollEnter(uint32_t other_entity_id, float dt) override;
        void ExecuteCollStay(uint32_t other_entity_id, float dt) override;
        void ExecuteCollExit(uint32_t other_entity_id, float dt) override;
};