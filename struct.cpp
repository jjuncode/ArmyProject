#include "struct.h"
#include <algorithm>
#include "Core.h"

float Vec::LengthSquare(Vec2 _vec)
{
    return (_vec.x * _vec.x + _vec.y * _vec.y);
}

float Vec::LengthSquare(Vec3 _vec)
{
    return (_vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z);
}

float Vec::Length(Vec2 _vec)
{
    return sqrt(LengthSquare(_vec));
}

float Vec::Length(Vec3 _vec)
{
    return sqrt(LengthSquare(_vec));
}

Vec2 Vec::Normalize(const Vec2 &vec)
{
    float length = LengthSquare(vec);
    if (length == 0) 
        return Vec2(0, 0);
    return Vec2(vec.x / sqrt(length), vec.y / sqrt(length));
}

Vec3 Vec::Normalize(const Vec3 &vec)
{
    float length = LengthSquare(vec);
    if (length == 0) 
        return Vec3();
    return Vec3(vec.x / sqrt(length), vec.y / sqrt(length), vec.z / sqrt(length));
}

float Vec::Dot(const Vec2 &lhs, const Vec2 &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y ;
}

float Vec::Dot(const Vec3 &lhs, const Vec3 &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y +lhs.z* rhs.z;
}

float Vec::Cross(const Vec2 &lhs, const Vec2 &rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

Vec3 Vec::Cross(const Vec3 &lhs, const Vec3 &rhs)
{
    return Vec3(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}

Vec2 Vec::Normal(const Vec2 &vec)
{
    // return Vec2(-vec.y, vec.x);
    // 시계방향
    return Vec2(vec.y , -vec.x);
}

Vec2 Vec::Projection(Vec2 vec_unit, Vec2 rhs)
{
    float dot = Dot(vec_unit, rhs);
    return Vec2(dot * vec_unit.x, dot * vec_unit.y);
}

Vec2 Vec::Reverse(const Vec2 &vec)
{
    return Vec2(-vec.x, -vec.y);
}

Vec3 Vec::Reverse(const Vec3 &vec)
{
    return Vec3(-vec.x, -vec.y, -vec.z);
}

float Vec::GetDegree(Vec2 _v1, Vec2 _v2)
{
   // 두 벡터 사이의 각도(라디안) 구하기
   auto dot = Vec::Dot(Vec::Normalize(_v1), Vec::Normalize(_v2));
   dot = std::clamp(dot, -1.0f, 1.0f );
   auto angle = std::acos(dot);
   float degree = angle * ( 180.0f / 3.14159265f);
   return degree;
}

float Vec::RadToDegree(float radian)
{
    return radian * (180.0 / M_PI); // M_PI는 3.14159로 정의된 상수
}

float Vec::GetRadian(float _v)
{
   return _v * (3.14159265f / 180.f);
}

Vec2 Vec::ConvertToScreenCoord(const Vec2 &vec)
{
    auto window = Core::GetWindowSize();
    return Vec2(vec.x + window.x / 2, -vec.y + window.y / 2);
}

Vec2 Vec::ConvertToScreenCoord(const Vec3 &vec)
{
    auto window = Core::GetWindowSize();
    return Vec2(vec.x + window.x / 2.0f,-vec.y + window.y / 2.0f);
}

Vec2 Vec::ConvertToCartesian(const Vec2 &vec)
{
    auto window = Core::GetWindowSize();
    return Vec2(vec.x+0.5f - window.x / 2.f, -(vec.y+0.5f) + window.y / 2.f);
}

Vec3 Vec::ConvertToCartesian(const Vec3& vec)
{
    auto window = Core::GetWindowSize();
    return Vec3(
        vec.x + 0.5f - window.x / 2.0f,
        -(vec.y + 0.5f) + window.y / 2.0f,
        0
    );
}

bool Vec::IsNearlyZero(float value)
{
    static constexpr float epsilon = 1e-6f; // 작은 값
    return std::abs(value) < epsilon;
}

Mat3 Mat3::Inverse() const
{
    const Vec3& r0 = row[0];
    const Vec3& r1 = row[1];
    const Vec3& r2 = row[2];

    float det =
          r0.x * (r1.y * r2.z - r1.z * r2.y)
        - r0.y * (r1.x * r2.z - r1.z * r2.x)
        + r0.z * (r1.x * r2.y - r1.y * r2.x);

    assert(std::abs(det) > 1e-6f && "Matrix is not invertible!");

    float inv_det = 1.0f / det;

    return Mat3(
        Vec3(
            (r1.y * r2.z - r1.z * r2.y) * inv_det,
            (r0.z * r2.y - r0.y * r2.z) * inv_det,
            (r0.y * r1.z - r0.z * r1.y) * inv_det
        ),
        Vec3(
            (r1.z * r2.x - r1.x * r2.z) * inv_det,
            (r0.x * r2.z - r0.z * r2.x) * inv_det,
            (r0.z * r1.x - r0.x * r1.z) * inv_det
        ),
        Vec3(
            (r1.x * r2.y - r1.y * r2.x) * inv_det,
            (r0.y * r2.x - r0.x * r2.y) * inv_det,
            (r0.x * r1.y - r0.y * r1.x) * inv_det
        )
    );
}

std::ostream &operator<<(std::ostream &os, const Vec2 &vec)
{
    os << vec.x << ", " << vec.y;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Vec3 &vec)
{
    os << vec.x << ", " << vec.y << ", " << vec.z;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Mat3 &mat)
{
    os << "MAT3:\n";
    os << "[" << mat[0] << "]\n";
    os << "[" << mat[1] << "]\n";
    os << "[" << mat[2] << "]\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vec4& vec)
{
    os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Mat4& mat)
{
    os << "MAT4:\n";
    os << "[" << mat[0] << "]\n";
    os << "[" << mat[1] << "]\n";
    os << "[" << mat[2] << "]\n";
    os << "[" << mat[3] << "]\n";
    return os;
}

Mat4 Mat4::Inverse() const
{
    const float* m = &row[0].x; // 연속된 float[16] 접근 (row-major 기준)
    float inv[16];

    inv[0] =   m[5]  * m[10] * m[15] - 
               m[5]  * m[11] * m[14] - 
               m[9]  * m[6]  * m[15] + 
               m[9]  * m[7]  * m[14] +
               m[13] * m[6]  * m[11] - 
               m[13] * m[7]  * m[10];

    inv[1] =  -m[1]  * m[10] * m[15] + 
               m[1]  * m[11] * m[14] + 
               m[9]  * m[2]  * m[15] - 
               m[9]  * m[3]  * m[14] - 
               m[13] * m[2]  * m[11] + 
               m[13] * m[3]  * m[10];

    inv[2] =   m[1]  * m[6]  * m[15] - 
               m[1]  * m[7]  * m[14] - 
               m[5]  * m[2]  * m[15] + 
               m[5]  * m[3]  * m[14] + 
               m[13] * m[2]  * m[7]  - 
               m[13] * m[3]  * m[6];

    inv[3] =  -m[1]  * m[6]  * m[11] + 
               m[1]  * m[7]  * m[10] + 
               m[5]  * m[2]  * m[11] - 
               m[5]  * m[3]  * m[10] - 
               m[9]  * m[2]  * m[7]  + 
               m[9]  * m[3]  * m[6];

    inv[4] =  -m[4]  * m[10] * m[15] + 
               m[4]  * m[11] * m[14] + 
               m[8]  * m[6]  * m[15] - 
               m[8]  * m[7]  * m[14] - 
               m[12] * m[6]  * m[11] + 
               m[12] * m[7]  * m[10];

    inv[5] =   m[0]  * m[10] * m[15] - 
               m[0]  * m[11] * m[14] - 
               m[8]  * m[2]  * m[15] + 
               m[8]  * m[3]  * m[14] + 
               m[12] * m[2]  * m[11] - 
               m[12] * m[3]  * m[10];

    inv[6] =  -m[0]  * m[6]  * m[15] + 
               m[0]  * m[7]  * m[14] + 
               m[4]  * m[2]  * m[15] - 
               m[4]  * m[3]  * m[14] - 
               m[12] * m[2]  * m[7]  + 
               m[12] * m[3]  * m[6];

    inv[7] =   m[0]  * m[6]  * m[11] - 
               m[0]  * m[7]  * m[10] - 
               m[4]  * m[2]  * m[11] + 
               m[4]  * m[3]  * m[10] + 
               m[8]  * m[2]  * m[7]  - 
               m[8]  * m[3]  * m[6];

    inv[8] =   m[4]  * m[9]  * m[15] - 
               m[4]  * m[11] * m[13] - 
               m[8]  * m[5]  * m[15] + 
               m[8]  * m[7]  * m[13] + 
               m[12] * m[5]  * m[11] - 
               m[12] * m[7]  * m[9];

    inv[9] =  -m[0]  * m[9]  * m[15] + 
               m[0]  * m[11] * m[13] + 
               m[8]  * m[1]  * m[15] - 
               m[8]  * m[3]  * m[13] - 
               m[12] * m[1]  * m[11] + 
               m[12] * m[3]  * m[9];

    inv[10] =  m[0]  * m[5]  * m[15] - 
               m[0]  * m[7]  * m[13] - 
               m[4]  * m[1]  * m[15] + 
               m[4]  * m[3]  * m[13] + 
               m[12] * m[1]  * m[7]  - 
               m[12] * m[3]  * m[5];

    inv[11] = -m[0]  * m[5]  * m[11] + 
               m[0]  * m[7]  * m[9]  + 
               m[4]  * m[1]  * m[11] - 
               m[4]  * m[3]  * m[9]  - 
               m[8]  * m[1]  * m[7]  + 
               m[8]  * m[3]  * m[5];

    inv[12] = -m[4]  * m[9]  * m[14] + 
               m[4]  * m[10] * m[13] + 
               m[8]  * m[5]  * m[14] - 
               m[8]  * m[6]  * m[13] - 
               m[12] * m[5]  * m[10] + 
               m[12] * m[6]  * m[9];

    inv[13] =  m[0]  * m[9]  * m[14] - 
               m[0]  * m[10] * m[13] - 
               m[8]  * m[1]  * m[14] + 
               m[8]  * m[2]  * m[13] + 
               m[12] * m[1]  * m[10] - 
               m[12] * m[2]  * m[9];

    inv[14] = -m[0]  * m[5]  * m[14] + 
               m[0]  * m[6]  * m[13] + 
               m[4]  * m[1]  * m[14] - 
               m[4]  * m[2]  * m[13] - 
               m[12] * m[1]  * m[6]  + 
               m[12] * m[2]  * m[5];

    inv[15] =  m[0]  * m[5]  * m[10] - 
               m[0]  * m[6]  * m[9]  - 
               m[4]  * m[1]  * m[10] + 
               m[4]  * m[2]  * m[9]  + 
               m[8]  * m[1]  * m[6]  - 
               m[8]  * m[2]  * m[5];

    float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    assert(std::abs(det) > 1e-6f && "Matrix not invertible");

    float inv_det = 1.0f / det;

    return Mat4(
        Vec4(inv[0],  inv[1],  inv[2],  inv[3])  * inv_det,
        Vec4(inv[4],  inv[5],  inv[6],  inv[7])  * inv_det,
        Vec4(inv[8],  inv[9],  inv[10], inv[11]) * inv_det,
        Vec4(inv[12], inv[13], inv[14], inv[15]) * inv_det
    );
}


Vec3& Vec3::operator-=(const Vec3& rhs) {
    x -= rhs.x; 
    y -= rhs.y; 
    z -= rhs.z;
    return *this;
}

Vec3& Vec3::operator+=(const Vec3& rhs) {
    x += rhs.x; 
    y += rhs.y; 
    z += rhs.z;
    return *this;
}

Vec3& Vec3::operator*=(const float rhs) {
    x *= rhs; 
    y *= rhs; 
    z *= rhs;
    return *this;
}

Vec3& Vec3::operator*=(const Vec3& rhs) {
    x *= rhs.x; 
    y *= rhs.y; 
    z *= rhs.z;
    return *this;
}

Vec3& Vec3::operator/=(float rhs) {
    x /= rhs; 
    y /= rhs; 
    z /= rhs;
    return *this;
}

bool Vec3::operator==(const Vec3& rhs) const {
    if ( (x != rhs.x) || (y != rhs.y) || (z != rhs.z) )
        return false;
    return true;
}

bool Vec3::operator!=(const Vec3& rhs) const {
    if ( *this == rhs)  return false;
    return true;
}

BoundValue Frustum::CheckBound(const Sphere &_sphere) const
{
    for (const auto &plane : planes){
        auto distn = plane.DistanceToPoint(_sphere.center);

        if (distn > _sphere.radius) {
            return BoundValue::kOutside;
        }
        else if(abs(distn) <=_sphere.radius) {
            return BoundValue::kIntersect; // Point is on the plane
        }
    }

    return BoundValue::kInside;
}

BoundValue Frustum::CheckBound(const Box &_box) const
{
    for (const auto& plane : planes) {
        Vec3 check{};
        Vec3 check_opposite{};

        if ( plane.normal.x > 0 ) {
            check.x = _box.min.x;
            check_opposite.x = _box.max.x;
        }
        else{
            check.x = _box.max.x;
            check_opposite.x = _box.min.x;
        }
        if ( plane.normal.y > 0 ) {
            check.y = _box.min.y;
            check_opposite.y = _box.max.y;
        }
        else{
            check.y = _box.max.y;
            check_opposite.y = _box.min.y;
        }
        if ( plane.normal.z > 0 ) {
            check.z = _box.min.z;
            check_opposite.z = _box.max.z;
        }
        else{
            check.z = _box.max.z;
            check_opposite.z = _box.min.z;
        }

        auto distn = plane.DistanceToPoint(check);
        if ( distn > 0 ){
            return BoundValue::kOutside; // Box is outside the plane
        }
        else if (distn <= 0 && plane.DistanceToPoint(check_opposite) >= 0) {
            return BoundValue::kIntersect; // Box intersects the plane
        }
    }

    return BoundValue::kInside; // Box is inside the frustum
}

Plane::Plane(const Vec4 &_v)
{
    normal = Vec3(_v.x, _v.y, _v.z);
    d = _v.w;
    
    auto length = Vec::Length(normal);
    normal /= length;
    d /= length;

    normal *= -1.0f; // Invert the normal direction
    d *= -1.0f; // Invert the distance
}

float Plane::DistanceToPoint(const Vec3 &point) const
{
    return Vec::Dot(normal, point) + d;
}

bool Plane::IsOutside(const Vec3 &point) const
{
    if (DistanceToPoint(point) > 0) {
        return true; // Point is outside the plane
    }
    return false; // Point is inside or on the plane    
}

bool Sphere::IsInside(const Vec3 & point) const
{
    return (Vec::LengthSquare(point - center) <= radius * radius);
}

bool Sphere::Intersect(const Sphere & other) const
{
    float distance_square = Vec::LengthSquare(other.center - center);
    float radius_sum = radius + other.radius;
    return (distance_square <= radius_sum * radius_sum);
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

Quaternion &Quaternion::operator-()
{
    real_part *= -1;
    imaginary_part *= -1;

    return *this;
}
