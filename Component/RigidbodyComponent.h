#pragma once
#include "Component.h"
#include "../struct.h"

class Rigidbody : public Component {
    public:
        Rigidbody(float _mass = 1.f, float _fric = 0.9f, float _e = 0.9f)
            : m_gravity{0,-1000.f}
            , m_mass{_mass}
            , m_fric{_fric}
            , m_elastic{_e}
            , m_velocity_max{100000.f, 100000.f}
            {};

    private:
        Vec2 m_velocity;             // 현재 속도
        Vec2 m_accel;                // 현재 가속도
        Vec2 m_force;                // 현재 힘
        Vec2 m_gravity;              // 중력
        Vec2 m_velocity_max;         // 최대 속도

        float m_mass;                // 질량
        float m_fric;               // 마찰력
        float m_elastic;

public:
    void Update(float dt) override;

    void AddForce(const Vec2& force) { m_force += force; }
    void ApplyImpulse(Vec2 impulse) {m_velocity += impulse / m_mass;}

    Vec2 GetGravity() { return m_gravity; }
    
    Vec2 GetVelocity() { return m_velocity; }
    void SetVelocity(const Vec2& velocity) { m_velocity = velocity; }

    void SetFric(float fric) { m_fric = fric; }
    float GetFric() { return m_fric; }

    void SetElastic(float elastic) { m_elastic = elastic; }
    float GetElastic(){ return m_elastic; }

    void SetMass(float mass) { m_mass = mass; }
    float GetMass() { return m_mass; }
};

void ProcessImpulseColl(uint32_t self_entity_id, uint32_t other_entity_id,MTV _mtv, float dt);