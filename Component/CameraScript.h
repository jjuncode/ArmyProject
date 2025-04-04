#pragma once
#include "Component.h"

class CameraScript : public Component {
    private:
        uint32_t m_target;

    public:
        void Update(float dt ) override;
        void SetTarget(const uint32_t _target) { m_target = _target; }
        uint32_t GetTarget() { return m_target; }
};