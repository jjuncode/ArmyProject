#pragma once
#include "Component.h"
#include "../struct.h"

class Rigidbody : public Component {
    public:
        Rigidbody(float _mass = 1.f, float _fric = 0.9f, float _e = 0.25f)
            : m_gravity{0,-1000.f,0}
            , m_velocity{0,0,0}
            , m_accel{0,0,0}
            , m_force{0,0,0}
            , m_velo_angular{0}
            , m_accel_angular{0}
            , m_mass{_mass}
            , m_fric{_fric}
            , m_elastic{_e}
            , m_velocity_max{2000.f, 2000.f, 2000.f}
            , m_fixed{false}
            , m_acc_impulse{0,0,0}
            {};

            Rigidbody(bool _fixed)
            : m_gravity{0,0,0}
            ,m_mass{0}
            ,m_fric{0.5f}
            ,m_elastic{0.5f}
            ,m_velocity{0,0,0}
            ,m_fixed{_fixed}
            {};

    private:
        Vec3 m_velocity;             // 현재 속도
        Vec3 m_accel;                // 현재 가속도
        Vec3 m_force;                // 현재 힘
        Vec3 m_gravity;              // 중력
        Vec3 m_velocity_max;         // 최대 속도
        Vec3 m_acc_impulse;         // 누적 충격량

        float m_mass;                // 질량
        float m_fric;               // 마찰력
        float m_elastic;            // 탄성계수
        float m_velo_angular;       // 각속도 
        float m_accel_angular;      // 각가속도

        bool m_fixed;

public:
    void Update(float dt) override;

    void AddForce(const Vec3& force) { m_force += force; }
    Vec3 GetForce(){return m_force;}

    void ApplyImpulse(Vec3 impulse) {m_acc_impulse += impulse;}
    void ApplyAngular(float _rhs) { m_accel_angular += _rhs; }

    Vec3 GetGravity() { return m_gravity; }
    
    Vec3 GetVelocity() { return m_velocity; }
    void SetVelocity(const Vec3& velocity) { m_velocity = velocity; }

    Vec3 GetAccel() { return m_accel; }
    void SetAccel(const Vec3& _accel) { m_accel = _accel;}

    float GetAngularVelocity() { return m_velo_angular; }
    void SetAngularVelocity(float _rhs) { m_velo_angular = _rhs; }

    void SetFric(float fric) { m_fric = fric; }
    float GetFric() { return m_fric; }

    void SetElastic(float elastic) { m_elastic = elastic; }
    float GetElastic(){ return m_elastic; }

    void SetMass(float mass) { m_mass = mass; }
    float GetMass() { return m_mass; }

    bool IsFixed() { return m_fixed; }
};

namespace Physic{
    void ProcessPhysicCollision(uint32_t self_entity_id, uint32_t other_entity_id,MTV _mtv, float dt);
    std::pair<Vec2,float> GetGroundVec(Vec2 _width, Vec2 _height, Vec2 _contact_dot, Vec2 _mtv_direction);
};

std::vector<Vec2> GetCollisionCandidate(uint32_t self_entity_id, uint32_t other_entity_id, Vec2 _mtv_vec);