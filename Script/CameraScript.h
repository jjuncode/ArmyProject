#pragma once
#include "Script.h"
#include "../struct.h"

class CameraScript : public Script {
    private:
        uint32_t m_target;
        float m_zoom_value{1.0f}; // Zoom value
        float m_zoom_speed{10}; // Speed of zooming in/out
        float m_zoom_max{6.0f}; // Maximum zoom level
        float m_zoom_min{0.5f}; // Minimum zoom level

    public:
        CameraScript(uint32_t _target) : m_target(_target) {}

        void Execute(float dt) override; // script execute
        void SetTarget(const uint32_t _target) { m_target = _target; }
        uint32_t GetTarget() { return m_target; }
        
        float GetZoomValue() { return m_zoom_value; }
        Vec2 GetMainCameraPos();

        void FollowTargetPos();

        constexpr Mat3 GetViewMatrix() const;
};