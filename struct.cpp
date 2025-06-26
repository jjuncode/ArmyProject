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
    return lhs.x * rhs.x + lhs.y * rhs.y +lhs.z* lhs.z;
}

float Vec::Cross(const Vec2 &lhs, const Vec2 &rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
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
