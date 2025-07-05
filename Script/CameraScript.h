#pragma once
#include "Script.h"
#include "../struct.h"

class CameraScript : public Script {
    private:
        uint32_t m_target;
        uint32_t m_speed{100};
        uint32_t m_fov{60};
        
        float m_near{5.f};
        float m_far{5000.f}; 

    public:
        CameraScript(uint32_t _target) : m_target(_target) {}

        void Execute(float dt) override; // script execute
        void SetTarget(const uint32_t _target) { m_target = _target; }
        uint32_t GetTarget() { return m_target; }
        
        Vec3 GetMainCameraPos();

        void FollowTargetPos(float dt);
        const Mat4 GetViewMatrix() const;
        const Mat4 GetProjectionMatrix() const;

        const float GetNearPlane() const { return m_near; }
        const float GetFarPlane() const { return m_far; }
};