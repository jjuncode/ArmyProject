#pragma once
#include "../struct.h"

class Transform {
    private:
        uint32_t m_owner_id; // owner id

        Vec3 m_pos;
        Vec3 m_scale;

        Vec3 m_rotate;  // pitch yaw roll 

        Vec3 m_forward;
        Vec3 m_up;
        Vec3 m_right;

    public:
        Transform() 
            : m_pos{Vec3()}
            , m_scale{1,1,1}
            , m_rotate{Vec3()}
            , m_forward{Vec3(0,0,1)}
            , m_up{Vec3(0,1,0)}
            , m_right{Vec3(1,0,0)}
            {}
        void SetOwner(uint32_t _id) { m_owner_id = _id; }

        Vec3 GetPos() { return m_pos; }
        void SetPos(const Vec3& _pos){m_pos = _pos;}
        void AddPos(const Vec3& offset){m_pos += offset;}

        void AddRotate(Vec3 _offset);

        Vec3 GetScale(){return m_scale;}
        void SetScale(const Vec3 _scale) { m_scale =_scale;}

        const Vec3 GetForward() const { return m_forward; }
        const Vec3 GetUp() const { return m_up; }
        const Vec3 GetRight() const { return m_right; }

        const Mat4 GetModelMatrix() const;
};