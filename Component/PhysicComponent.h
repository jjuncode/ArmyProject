#pragma once
#include "Component.h"
#include "../struct.h"

class PhysicComponent : public Component{
    private:
        static int m_gravity;
        static float m_air_friction;
        
        Vec2 m_velocity;
        Vec2 m_accel;

    public:
        void Update(float dt ) override;
        void SetVelocity(Vec2 rhs){m_velocity = rhs;}
        void SetAccel(Vec2 rhs){m_accel = rhs;}
        Vec2 GetVelocity(){return m_velocity;}
        Vec2 GetAccel(){return m_accel;}

        void OnGround();
};