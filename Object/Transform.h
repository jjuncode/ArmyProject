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

struct TransformInfo{
    Vec3 m_pos;
    Vec3 m_scale;

    Quaternion m_quaternion; // quaternion for rotation
    Vec3 m_rotate;           // pitch yaw roll

    Vec3 m_forward;
    Vec3 m_up;
    Vec3 m_right;

    TransformInfo(Vec3 _pos = Vec3())
        : m_pos{_pos}, m_scale{1, 1, 1}, m_rotate{}, m_forward{Vec3(0, 0, 1)}, m_up{Vec3(0, 1, 0)}, m_right{Vec3(1, 0, 0)}
    {}

    void AddRotate(Vec3 _offset);
    void SetRotate(const Quaternion &_quaternion);

    const Mat4 GetModelMatrix() const;
    TransformInfo GetInverse() const;
};

class Transform {
private:
    uint32_t m_owner_id; // owner id

    TransformInfo m_local_transform;
    TransformInfo m_world_transform;

public:
    Transform(Vec3 _pos = Vec3())
        : m_local_transform{_pos}
        , m_world_transform{_pos}
        , m_parent{nullptr}
        , m_children{}
    {}

    void SetOwner(uint32_t _id) { m_owner_id = _id; }

    const Vec3 GetPos() const { return m_world_transform.m_pos; }
    void SetPos(const Vec3 &_pos) {    m_world_transform.m_pos = _pos; }
    void AddPos(const Vec3 &offset) {  m_world_transform.m_pos += offset; }

    void AddRotate(Vec3 _offset){m_world_transform.AddRotate(_offset);};
    Quaternion GetRotate() { return m_world_transform.m_quaternion; }

    Vec3 GetScale() { return m_world_transform.m_scale; }
    void SetScale(const Vec3 _scale) { m_world_transform.m_scale = _scale; }
    void SetRotate(const Quaternion &_quaternion){m_world_transform.SetRotate(_quaternion);};

    const Vec3 GetForward() const { return m_world_transform.m_forward; }
    const Vec3 GetUp() const { return m_world_transform.m_up; }
    const Vec3 GetRight() const { return m_world_transform.m_right; }

    void SetForward(const Vec3 &_forward) { m_world_transform.m_forward = _forward; };
    void SetUp(const Vec3 &_up) { m_world_transform.m_up = _up; };
    void SetRight(const Vec3 &_right) { m_world_transform.m_right = _right; };

    const Mat4 GetModelMatrix() const{ return m_world_transform.GetModelMatrix();};
    TransformInfo GetInverse() const{ return m_world_transform.GetInverse(); };

    // ==============================
    // Animation Hierarchy Structure
    // ==============================
private:
    Transform *m_parent{nullptr};
    std::vector<Transform*> m_children;

public:
    void SetParent(Transform *_parent);

    auto &GetChildren() { return m_children; }

    const TransformInfo &GetLocalTransform() { return m_local_transform; }
    const TransformInfo &GetWorldTransform() { return m_world_transform; }

    void SetLocalPosition(const Vec3 &_pos);
    void SetLocalScale(const Vec3 &_scale);

    void SetLocalTransform(const TransformInfo &_transform);
    void SetWorldTransform(const TransformInfo &_transform);

private:
    [[nodiscard]] TransformInfo UpdateWorldTransformFromLocal(); // by parent world
    [[nodiscard]] TransformInfo UpdateLocalTransformFromWorld(); // by parent world
    void UpdateChildrenWorldTransform();

    void UpdateWorld();
    void UpdateLocal();
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
