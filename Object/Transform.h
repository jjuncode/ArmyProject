#pragma once
#include "../struct.h"

struct Quaternion{
    float real_part;    // real part
    Vec3 imaginary_part;     // imaginary part

    Vec3 RotateVector(const Vec3& _vec) const;
    void CreateQuaternion(const Vec3& _euler);
    void CreateQuaternion(const Mat4& _mat);

    Quaternion Slerp(const Quaternion& _q1, const Quaternion& _q2, float ratio);

    Vec3 GetEuler() const;
    Mat4 GetRotationMatrix() const;
    Quaternion GetInverse() const;

    Quaternion& operator-();
};

class Transform {
    private:
        uint32_t m_owner_id; // owner id

        Vec3 m_pos;
        Vec3 m_scale;

        Quaternion m_quaternion; // quaternion for rotation
        Vec3 m_rotate;  // pitch yaw roll 

        Vec3 m_forward;
        Vec3 m_up;
        Vec3 m_right;

    public:
        Transform(Vec3 _pos = Vec3()) 
            : m_pos{_pos}
            , m_scale{1,1,1}
            , m_rotate{}
            , m_forward{Vec3(0,0,1)}
            , m_up{Vec3(0,1,0)}
            , m_right{Vec3(1,0,0)}
            {}
        void SetOwner(uint32_t _id) { m_owner_id = _id; }

        const Vec3 GetPos() const { return m_pos; }
        void SetPos(const Vec3& _pos){m_pos = _pos;}
        void AddPos(const Vec3& offset){m_pos += offset;}

        void AddRotate(Vec3 _offset);
        Quaternion GetRotate() { return m_quaternion;}

        Vec3 GetScale(){return m_scale;}
        void SetScale(const Vec3 _scale) { m_scale =_scale;}
        void SetRotate(const Quaternion& _quaternion);

        const Vec3 GetForward() const { return m_forward; }
        const Vec3 GetUp() const { return m_up; }
        const Vec3 GetRight() const { return m_right; }

        void SetLocalAxis(const Vec3& _forward, const Vec3& _up, const Vec3& _right) {
            m_forward = _forward;
            m_up = _up;
            m_right = _right;
        }
        void SetForward(const Vec3& _forward){m_forward = _forward;};
        void SetUp(const Vec3& _up){m_up = _up;};
        void SetRight(const Vec3& _right){m_right = _right;};

        const Mat4 GetModelMatrix() const;
        Transform GetInverse() const;
};

inline Vec3 operator* (const Quaternion& _q, const Vec3& _v);
inline Quaternion operator* (const Quaternion& _q1, const Quaternion& _q2);

inline Vec3 operator*(const Quaternion &_q, const Vec3 &_v)
{
    return _q.RotateVector(_v);
}

inline Quaternion operator*(const Quaternion &_q1, const Quaternion &_q2)
{
    Quaternion result{};
    result.real_part = _q1.real_part * _q2.real_part - Vec::Dot(_q1.imaginary_part, _q2.imaginary_part);
    Vec3 v = _q2.imaginary_part * _q1.real_part + _q1.imaginary_part * _q2.real_part + Vec::Cross(_q1.imaginary_part, _q2.imaginary_part);

    result.imaginary_part.x = v.x;
    result.imaginary_part.y = v.y;
    result.imaginary_part.z = v.z;

    return result;
}
