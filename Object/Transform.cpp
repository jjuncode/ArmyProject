#include "Transform.h"
#include "../pch.h"
#include "../Component/ColliderComponent.h"
#include "../Mgr/SceneMgr.h"

void Transform::SetParent(Transform *_parent)
{
    m_local_transform = m_world_transform;

    m_parent = _parent;
    _parent->m_children.push_back(this);

    m_local_transform = UpdateLocalTransformFromWorld();
}

void Transform::SetLocalTransform(const TransformInfo &_transform)
{
    m_local_transform = _transform;
    UpdateWorld();
}

void Transform::SetWorldTransform(const TransformInfo &_transform)
{
    m_world_transform = _transform;
    UpdateLocal();
}

TransformInfo Transform::UpdateWorldTransformFromLocal()
{
    TransformInfo result;
    TransformInfo world_parent = m_parent->GetWorldTransform();
    
    result.m_scale = (world_parent.m_scale * m_local_transform.m_scale);
    result.SetRotate(m_local_transform.m_quaternion * world_parent.m_quaternion );
    result.m_pos = (world_parent.m_quaternion * ( m_local_transform.m_pos * world_parent.m_scale)
                 + world_parent.m_pos);

    return result;
}

TransformInfo Transform::UpdateLocalTransformFromWorld()
{
    TransformInfo world_parent_inv = m_parent->GetWorldTransform().GetInverse();
    TransformInfo result;

    result.m_scale = (world_parent_inv.m_scale * m_world_transform.m_scale);
    result.SetRotate(world_parent_inv.m_quaternion* m_world_transform.m_quaternion);
    result.m_pos = (world_parent_inv.m_quaternion * (m_world_transform.m_pos * world_parent_inv.m_scale)
                    + world_parent_inv.m_pos);
    
    return result;
}

void Transform::UpdateChildrenWorldTransform()
{
    for (auto& child : m_children) {
        child->SetWorldTransform(child->UpdateWorldTransformFromLocal());
        child->UpdateChildrenWorldTransform();
    }
}

void Transform::UpdateWorld()
{
    if (m_parent == nullptr) 
        return;

    m_world_transform = UpdateWorldTransformFromLocal();
    UpdateChildrenWorldTransform();
}

void Transform::UpdateLocal()
{
    if (m_parent == nullptr) 
        return;

    m_local_transform = UpdateLocalTransformFromWorld();
    UpdateChildrenWorldTransform();
}

void TransformInfo::AddRotate(Vec3 _offset)
{
    //  // Update OBB rotation
    // auto collider = SceneMgr::GetComponent<ColliderComponent>(m_owner_id);
    // if (collider ){
    //     collider->RotateOBB(offset);
    //     auto obb = collider->GetOBB();
    // }

    // pitch / yaw / roll
    _offset.x = Vec::GetRadian(_offset.x);
    _offset.y = Vec::GetRadian(_offset.y);
    _offset.z = Vec::GetRadian(_offset.z);

    m_rotate += _offset;

    if (m_rotate.x > 360.f ) m_rotate.x -= 360.f;
    if (m_rotate.y > 360.f ) m_rotate.y -= 360.f;
    if (m_rotate.z > 360.f ) m_rotate.z -= 360.f;

    if ( m_rotate.x < 0.f ) m_rotate.x += 360.f;
    if ( m_rotate.y < 0.f ) m_rotate.y += 360.f;
    if ( m_rotate.z < 0.f ) m_rotate.z += 360.f;

    Quaternion quaternion{};
    quaternion.CreateQuaternion(m_rotate);

    m_right = quaternion.RotateVector(Vec3(1,0,0));
    m_up = quaternion.RotateVector(Vec3(0,1,0));
    m_forward = quaternion.RotateVector(Vec3(0,0,1));
}

void TransformInfo::SetRotate(const Quaternion & _quaternion)
{
    m_quaternion = _quaternion;
    
    m_right = _quaternion.RotateVector(Vec3(1,0,0));
    m_up = _quaternion.RotateVector(Vec3(0,1,0));
    m_forward = _quaternion.RotateVector(Vec3(0,0,1));
}

const Mat4 TransformInfo::GetModelMatrix() const
{
    // Create a transformation matrix
    Mat4 t{
        Vec4(1,0,0,m_pos.x),
        Vec4(0,1,0,m_pos.y),
        Vec4(0,0,1,m_pos.z),
        Vec4(0,0,0,1)
    };

    Mat4 s{
        Vec4(m_scale.x, 0, 0, 0),
        Vec4(0, m_scale.y, 0, 0),
        Vec4(0, 0, m_scale.z, 0),
        Vec4(0, 0, 0, 1)
    };

    Mat4 r{
        Vec4(m_right.x, m_up.x, m_forward.x ),
        Vec4(m_right.y, m_up.y, m_forward.y ),
        Vec4(m_right.z, m_up.z, m_forward.z ),
        Vec4(0, 0, 0, 1)
    };


    return t*r*s;
}

TransformInfo TransformInfo::GetInverse() const
{
    TransformInfo result;
    result.m_scale = (1/m_scale);
    result.SetRotate(m_quaternion.GetInverse());
    result.m_pos = (result.m_scale * ( result.m_quaternion * -m_pos ));

    return result;
}

Vec3 Quaternion::RotateVector(const Vec3 &_vec) const
{
    // v' = v + real*t + imaginary x t
    // t = 2(imaginary x v)
    Vec3 q_value = imaginary_part;
    Vec3 t_value = Vec::Cross(q_value, _vec) * 2.f;

    Vec3 result = _vec + (real_part*t_value )+ Vec::Cross(q_value, t_value);
    return result;
}

void Quaternion::CreateQuaternion(const Vec3 &_euler)
{
    float pitch_sin{}, pitch_cos{};
    float yaw_sin{}, yaw_cos{};
    float roll_sin{}, roll_cos{};

    Vec3 theta = _euler * 0.5f;

    pitch_sin = sin(theta.x);
    pitch_cos = cos(theta.x);

    yaw_sin = sin(theta.y);
    yaw_cos = cos(theta.y);

    roll_sin = sin(theta.z);
    roll_cos = cos(theta.z);

    real_part = yaw_sin * pitch_sin * roll_sin + yaw_cos * pitch_cos * roll_cos;
    imaginary_part.x = yaw_sin * roll_sin * pitch_cos + pitch_sin * yaw_cos * roll_cos;
    imaginary_part.y = yaw_sin * pitch_cos * roll_cos - pitch_sin * roll_sin * yaw_cos;
    imaginary_part.z = -yaw_sin * pitch_sin * roll_cos + roll_sin * yaw_cos * pitch_cos;
}

void Quaternion::CreateQuaternion(const Mat4 & _mat)
{
    float root = 0.f;
    float trace = _mat[0][0] + _mat[1][1] + _mat[2][2];

    if ( trace > 0.f ) {
        root = sqrtf(trace+1.f);
        real_part = 0.5f * root;
        root = 0.5f / root;
    
        imaginary_part.x = ( _mat[1][2] - _mat[2][1] ) * root;
        imaginary_part.y = ( _mat[2][0] - _mat[0][2] ) * root;
        imaginary_part.z = ( _mat[0][1] - _mat[1][0] ) * root;
    }
    else {
        int i=0;

        if ( _mat[1][1] > _mat[0][0] ) { i = 1; }
        if ( _mat[2][2] > _mat[i][i] ) { i = 2; }

        static const int next[3] = { 1,2, 0 };
        int j = next[i];
        int k = next[j];

        root = sqrtf( _mat[i][i] - _mat[j][j] - _mat[k][k] + 1.f );

        float* qt[3] = { &imaginary_part.x, &imaginary_part.y , &imaginary_part.z };
        *qt[i] = 0.5f * root;

        root = 0.5f / root;

        *qt[j] = (_mat[i][j] + _mat[j][i] ) * root;
        *qt[k] = ( _mat[i][k] + _mat[k][i]) * root;

        real_part = ( _mat[j][k] - _mat[k][j] ) * root;
    }

}

Quaternion Quaternion::Slerp(const Quaternion & _q1, const Quaternion & _q2, float ratio)
{
    Quaternion q1 = _q1; 
    Quaternion q2 = _q2;

    float dot = Vec::Dot(q1.imaginary_part, q2.imaginary_part) + + q1.real_part * q2.real_part;

    if ( dot < 0.0f ) {
        q1 = -q1;
        dot = -dot;
    }

    float alpha = 1.f; 
    float beta = 0.f;

    if ( dot > 0.99f ) {    // linear slerp
        alpha = 1.0f - ratio;
        beta = ratio;
    }
    else{
        const float theta = acosf(dot);
        const float inv_sin = 1.f / sinf(theta); 
        alpha = sinf((1.f - ratio) * theta) * inv_sin;

        beta = sinf(ratio * theta ) * inv_sin;
    }

    Quaternion result;
    result.imaginary_part.x = alpha * q1.imaginary_part.x + beta * q2.imaginary_part.x;
    result.imaginary_part.y = alpha * q1.imaginary_part.y + beta * q2.imaginary_part.y;
    result.imaginary_part.z = alpha * q1.imaginary_part.z + beta * q2.imaginary_part.z;
    result.real_part = alpha * q1.real_part + beta * q2.real_part;

    return result;
}

Vec3 Quaternion::GetEuler() const
{
    Vec3 result;
    float v1_roll = 2* (real_part * imaginary_part.z + imaginary_part.x * imaginary_part.y );
    float v2_roll = 1 - 2 * (imaginary_part.z  * imaginary_part.z + imaginary_part.x * imaginary_part.x);
    result.z = Vec::RadToDegree(atan2f(v1_roll,v2_roll));

    float pitch = real_part * imaginary_part.x - imaginary_part.y * imaginary_part.z;
    float max_range = 0.49f;
    if ( pitch < -max_range ) 
        result.x = -90.f;
    else if (pitch > max_range )
        result.x = 90.f;
    else
        result.x = Vec::RadToDegree(asinf(2*pitch));

    float v1_yaw = 2*(real_part * imaginary_part.y + imaginary_part.x * imaginary_part.z);
    float v2_yaw = 1.f - 2*(imaginary_part.x * imaginary_part.x + imaginary_part.y * imaginary_part.y );
    result.y = Vec::RadToDegree(atan2f(v1_yaw, v2_yaw));

    return result;
}

Mat4 Quaternion::GetRotationMatrix() const
{
    Vec3 rotate_x = RotateVector(Vec3(1,0,0));
    Vec3 rotate_y = RotateVector(Vec3(0,1,0));
    Vec3 rotate_z = RotateVector(Vec3(0,0,1));

    Mat4 rotate_matrix{
        Vec4(rotate_x.x , rotate_y.x, rotate_z.x, 0),
        Vec4(rotate_x.y , rotate_y.y, rotate_z.y, 0),
        Vec4(rotate_x.z , rotate_y.z, rotate_z.z, 0),
        Vec4(0,0,0,1)
    };

    return rotate_matrix;
}

Quaternion Quaternion::GetInverse() const
{
    Quaternion result;
    result.real_part = real_part;
    result.imaginary_part = -imaginary_part;
    return result;
}

Quaternion &Quaternion::operator-()
{
    real_part *= -1;
    imaginary_part *= -1;

    return *this;
}
