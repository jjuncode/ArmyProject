#pragma once
#include "Script.h"
#include "../struct.h"

class CameraScript : public Script {
    private:
        uint32_t m_target;
        uint32_t m_speed{100};

    public:
        CameraScript(uint32_t _target) : m_target(_target) {}

        void Execute(float dt) override; // script execute
        void SetTarget(const uint32_t _target) { m_target = _target; }
        uint32_t GetTarget() { return m_target; }
        
        Vec2 GetMainCameraPos();

        void FollowTargetPos(float dt);
        const Mat3 GetViewMatrix() const;
};