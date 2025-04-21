#pragma once
#include "Component.h"
#include "../struct.h"

// https://www.youtube.com/watch?v=XdVT-SsP1rU

class Rigidbody : public Component {
    public:
        Rigidbody(float _mass = 1.f, float _fric = -0.1f )
            : m_gravity{0,-25'000.f}
            , m_mass{_mass}
            , m_fric{_fric}
            , m_velocity_max{100'000, 100'000}
            {};

    private:
        Vec2 m_velocity;             // 현재 속도
        Vec2 m_accel;           // 현재 가속도
        Vec2 m_force;                // 현재 힘
        Vec2 m_gravity;              // 중력
        Vec2 m_velocity_max;      // 최대 속도

        float m_mass;                // 질량
        float m_fric;            // 마찰력

public:
    void Update(float dt) override;

    void AddForce(const Vec2& force) { m_force += force; }

    Vec2 GetGravity() { return m_gravity; }
    Vec2 GetVelocity() { return m_velocity; }

    void SetVelocity(const Vec2& velocity) { m_velocity = velocity; }


};