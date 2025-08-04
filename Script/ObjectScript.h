#pragma once
#include "Script.h"

class ObjectScript : public Script {
    public:
        void Execute(float dt) override; // script execute
        
        void ExecuteCollEnter(uint32_t other_entity_id, MTV _mtv,float dt) override;
        void ExecuteCollStay(uint32_t other_entity_id,  MTV _mtv, float dt) override;
        void ExecuteCollExit(uint32_t other_entity_id,  MTV _mtv, float dt) override;
};