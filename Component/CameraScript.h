#pragma once
#include "Component.h"

class CameraScript : public Component {
    private:
        uint32_t m_target;
        float m_zoom_value{1.0f}; // Zoom value
        float m_zoom_speed{10}; // Speed of zooming in/out
        float m_zoom_max{2.0f}; // Maximum zoom level
        float m_zoom_min{0.5f}; // Minimum zoom level

    public:
        void Update(float dt ) override;
        void SetTarget(const uint32_t _target) { m_target = _target; }
        uint32_t GetTarget() { return m_target; }
        float GetZoomValue() { return m_zoom_value; }

        void FollowTargetPos();
};