#pragma once
#include <math.h>
#include <vector>
#include <array>
#include <assert.h>
#include <iostream>

static size_t NO_KEY{std::hash<std::string>()("NULL")};

struct Vec2
{
    union {
        struct{
            float x;
            float y; 
        };

        float scalr[2]{};
    };

    Vec2():x{},y{}{}
    Vec2(float _x, float _y ): x{_x}, y{_y}{};
  
    // 복사 생성자
    Vec2(const Vec2& other): x{other.x}, y{other.y} {}

    Vec2 operator- (const Vec2& rhs){return Vec2(x-rhs.x, y-rhs.y);}
    Vec2 operator+ (const Vec2& rhs){return Vec2(x+rhs.x, y+rhs.y);}
    Vec2 operator* (float rhs){return Vec2(x*rhs, y*rhs);}
    Vec2 operator/(float rhs){return Vec2(x / rhs, y / rhs);}
    Vec2 operator* (const Vec2& rhs){return Vec2(x*rhs.x, y*rhs.y);}
    Vec2& operator -= (const Vec2& rhs){x -= rhs.x; y -= rhs.y;return *this;}
    Vec2& operator += (const Vec2& rhs){x += rhs.x; y += rhs.y; return *this;}
    Vec2& operator *= (const float rhs){x*= rhs; y*= rhs; return *this;}
    Vec2& operator *= (const Vec2& rhs){x*= rhs.x; y*= rhs.y; return *this;}
    Vec2& operator /= (float rhs){x /= rhs; y /= rhs; return *this; }
    bool operator != (const Vec2& rhs){
        if ((x== rhs.x) || (y==rhs.y) ) 
            return false;
        return true;
    }
    bool operator == (const Vec2& rhs){
        if ((x!= rhs.x) || (y!=rhs.y) ) 
            return false;
        return true;
    }
};

struct Vec3{
    union {
        struct{
            float x;
            float y; 
            float z;
        };

        float scalr[3]{};
    };

    constexpr Vec3(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {};
    constexpr Vec3(const Vec3& other): x{other.x}, y{other.y}, z{other.z} {}

    constexpr float operator[](size_t index) const {
        assert(index < 3);
        return scalr[index];
    }

    Vec2 ToVec2() const {
        assert(z!=0);
        return Vec2(x/z, y/z);
    }

    Vec3 &operator-=(const Vec3 &rhs);
    Vec3 &operator+=(const Vec3 &rhs);
    Vec3 &operator*=(const float rhs);
    Vec3 &operator*=(const Vec3 &rhs);
    Vec3 &operator/=(float rhs);

    bool operator!=(const Vec3 &rhs) const;
    bool operator==(const Vec3 &rhs) const;
};

struct Mat3
{
    std::array<Vec3, 3> row;
    constexpr Mat3() : row{{Vec3(), Vec3(), Vec3()}} {}
    constexpr Mat3(const Vec3& r0, const Vec3& r1, const Vec3& r2) : row{r0, r1, r2} {}

    constexpr Vec3 operator[] (size_t index) const {    // coll vector 
        assert(index < 3);
        return row[index];
    } 

    [[nodiscard]] constexpr Mat3 Transpose() const {
        return Mat3(
            Vec3(row[0][0], row[1][0], row[2][0]),
            Vec3(row[0][1], row[1][1], row[2][1]),
            Vec3(row[0][2], row[1][2], row[2][2])
        );
    }

    [[nodiscard]] Mat3 Inverse() const;
};

std::ostream& operator<<(std::ostream& os, const Vec2& vec);
std::ostream& operator<<(std::ostream& os, const Vec3& vec);
std::ostream& operator << ( std::ostream& os, const Mat3& mat);

inline constexpr Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
inline constexpr Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
inline constexpr Vec3 operator*(const Vec3& lhs, float rhs);
inline constexpr Vec3 operator*(float lhs, const Vec3& rhs);
inline constexpr Vec3 operator/(const Vec3& lhs, float rhs);
inline constexpr Vec3 operator-(const Vec3& v);

inline constexpr Vec3 operator*(const Mat3& lhs, const Vec3& rhs);
inline constexpr Vec3 operator*(const Mat3& lhs, const Vec2& rhs);

inline constexpr Mat3 operator+(const Mat3 &lhs, const Mat3 &rhs);
inline constexpr Mat3 operator-(const Mat3 &lhs, const Mat3 &rhs);
inline constexpr Mat3 operator*(const Mat3 &lhs, const Mat3 &rhs);

struct Vec4 {
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };

        float scalr[4]{};
    };

    constexpr Vec4(float _x = 0, float _y = 0, float _z = 0, float _w = 1) : x{_x}, y{_y}, z{_z}, w{_w} {}
    constexpr Vec4(const Vec4& other) : x{other.x}, y{other.y}, z{other.z}, w{other.w} {}

    constexpr float operator[](size_t index) const {
        assert(index < 4);
        return scalr[index];
    }

    float &operator[](size_t index){    
        assert(index < 4);
        return *(&x + index);
    }

    Vec3 ToVec3() const {
        assert(w != 0);
        if ( w== 1 ) return Vec3(x, y, z);
        return Vec3(x / w, y / w, z / w);
    }

    inline constexpr Vec4& operator+=(const Vec4& rhs);
};

struct Mat4
{
    std::array<Vec4, 4> row;

    constexpr Mat4() : row{{Vec4(), Vec4(), Vec4(), Vec4()}} {}

    constexpr Mat4(const Vec4& r0, const Vec4& r1, const Vec4& r2, const Vec4& r3)
        : row{r0, r1, r2, r3} {}

    constexpr const Vec4 &operator[](size_t index) const{
        assert(index < 4);
        return row[index];
    }

    Vec4 &operator[](size_t index){
        assert(index < 4);
        return row[index];
    }

    [[nodiscard]] constexpr Mat4 Transpose() const {
        return Mat4(
            Vec4(row[0][0], row[1][0], row[2][0], row[3][0]),
            Vec4(row[0][1], row[1][1], row[2][1], row[3][1]),
            Vec4(row[0][2], row[1][2], row[2][2], row[3][2]),
            Vec4(row[0][3], row[1][3], row[2][3], row[3][3])
        );
    }

    [[nodiscard]] Mat4 Inverse() const;
};

// 출력 연산자 (Vec4, Mat4)
std::ostream& operator<<(std::ostream& os, const Vec4& vec);
std::ostream& operator<<(std::ostream& os, const Mat4& mat);

// Vec4 연산자
inline constexpr Vec4 operator+(const Vec4& lhs, const Vec4& rhs);
inline constexpr Vec4 operator-(const Vec4& lhs, const Vec4& rhs);
inline constexpr Vec4 operator*(const Vec4& lhs, float rhs);
inline constexpr Vec4 operator*(float lhs, const Vec4& rhs);
inline constexpr Vec4 operator/(const Vec4& lhs, float rhs);
inline constexpr Vec4 operator-(const Vec4& v);

// Mat4 × Vec 연산자
inline constexpr Vec4 operator*(const Mat4& lhs, const Vec4& rhs);
inline constexpr Vec4 operator*(const Mat4& lhs, const Vec3& rhs);

// Mat4 행렬 연산자
inline constexpr Mat4 operator+(const Mat4& lhs, const Mat4& rhs);
inline constexpr Mat4 operator-(const Mat4& lhs, const Mat4& rhs);
inline constexpr Mat4 operator*(const Mat4& lhs, const Mat4& rhs);

struct Plane{
    Vec3 normal;
    float d;

    Plane(const Vec3& _normal = Vec3(0, 1, 0), float _d = 0)
        : normal{_normal}, d{_d} {}

    Plane(const Vec4& _v);

    float DistanceToPoint(const Vec3& point) const;
    bool IsOutside(const Vec3& point) const;
};

struct Sphere{
    Vec3 center;
    float radius;

    Sphere(const Vec3& _center = Vec3(), float _radius = 1.0f)
        : center{_center}, radius{_radius} {}

    bool IsInside(const Vec3& point) const;

    bool Intersect(const Sphere& other) const;
};

enum class BoundValue{
    kInside,
    kOutside,
    kIntersect
};

struct Frustum{
    std::array<Plane, 6> planes;

    Frustum(const std::array<Plane, 6>& _planes = {})
        : planes{_planes} {}

    BoundValue CheckBound(const Sphere& point) const;
};


struct RGBA{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    RGBA(uint8_t _r = 255u, uint8_t _g = 255u, uint8_t _b = 255u, uint8_t _a = 1)
        : r{_r}, g{_g}, b{_b}, a{_a} {}
};

struct Vertex{
    Vec4 v;
    RGBA color;
    Vec2 uv;

    Vertex(const Vec4& _v = Vec4(), const RGBA& _color = RGBA(), const Vec2& _uv = Vec2())
        : v{_v.x , _v.y,_v.z,_v.w }, color{_color}, uv{_uv} {}

    bool IsUV(){
        if ( uv.x >=0 && uv.x <= 1 && uv.y >= 0 && uv.y <= 1) return true;
        return false;
    }
};

namespace Vec{
    float LengthSquare(Vec2 _vec);
    float LengthSquare(Vec3 _vec);
    float Length(Vec2 _vec);
    float Length(Vec3 _vec);

    Vec2 Normalize(const Vec2 &vec);
    Vec3 Normalize(const Vec3 &vec);

    float Dot(const Vec2 &lhs, const Vec2 &rhs);
    float Dot(const Vec3& lhs, const Vec3& rhs);

    float Cross(const Vec2 &lhs, const Vec2 &rhs);
    Vec3 Cross(const Vec3& lhs, const Vec3& rhs);

    Vec2 Projection(Vec2 vec_unit, Vec2 rhs);

    Vec2 Normal(const Vec2& vec);

    Vec2 Reverse(const Vec2& vec);
    Vec3 Reverse(const Vec3& vec);

    float GetDegree(Vec2 _v1, Vec2 _v2);
    float GetRadian(float _v);

    Vec2 ConvertToScreenCoord(const Vec2& vec);
    Vec2 ConvertToScreenCoord(const Vec3& vec);

    Vec2 ConvertToCartesian(const Vec2& vec);
    Vec3 ConvertToCartesian(const Vec3& vec);

    bool IsNearlyZero(float value);
}

struct MTV{
    Vec2 vec;
    float length;
};


inline constexpr Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline constexpr Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
inline constexpr Vec3 operator*(const Vec3& lhs, float rhs) {
    return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
inline constexpr Vec3 operator*(float lhs, const Vec3& rhs) {
    return Vec3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}
inline constexpr Vec3 operator/(const Vec3& lhs, float rhs) {
    assert(rhs != 0); // 0으로 나누는 것을 방지
    return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

inline constexpr Vec3 operator-(const Vec3 &v)
{
    return Vec3(v * -1.0f);
}

inline constexpr Mat3 operator+(const Mat3 &lhs, const Mat3 &rhs)
{
    Mat3 result{
        lhs[0] + rhs[0],
        lhs[1] + rhs[1],
        lhs[2] + rhs[2]
    };
    return result;
}

inline constexpr Mat3 operator-(const Mat3 &lhs, const Mat3 &rhs)
{
    Mat3 result{
        lhs[0] - rhs[0],
        lhs[1] - rhs[1],
        lhs[2] - rhs[2]
    };
    return result;
}

inline constexpr Mat3 operator*(const Mat3 &lhs, const Mat3 &rhs)
{
    Mat3 result{
        Vec3(lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0],
             lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1],
             lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2]),
        Vec3(lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0],
             lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1],
             lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2]),
        Vec3(lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0],
             lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1],
             lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2])
    };
    return result;
}

inline constexpr Vec3 operator*(const Mat3& lhs, const Vec3& rhs) {
    return Vec3(
        lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z,
        lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z,
        lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z
    );
}

inline constexpr Vec3 operator*(const Mat3 &lhs, const Vec2 &rhs)
{
    Vec3 v = Vec3(rhs.x, rhs.y, 1.0f);
    return lhs * v;
}

inline constexpr Vec4 operator+(const Vec4& lhs, const Vec4& rhs) {
    return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

inline constexpr Vec4 operator-(const Vec4& lhs, const Vec4& rhs) {
    return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

inline constexpr Vec4 operator*(const Vec4& lhs, float rhs) {
    return Vec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

inline constexpr Vec4 operator*(float lhs, const Vec4& rhs) {
    return Vec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
}

inline constexpr Vec4 operator/(const Vec4& lhs, float rhs) {
    assert(rhs != 0.0f);
    return Vec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}
inline constexpr Mat4 operator+(const Mat4& lhs, const Mat4& rhs) {
    return Mat4(
        lhs[0] + rhs[0],
        lhs[1] + rhs[1],
        lhs[2] + rhs[2],
        lhs[3] + rhs[3]
    );
}

inline constexpr Vec4 operator-(const Vec4& v){
    return Vec4(-v.x, -v.y, -v.z, -v.w);
}

inline constexpr Mat4 operator-(const Mat4& lhs, const Mat4& rhs) {
    return Mat4(
        lhs[0] - rhs[0],
        lhs[1] - rhs[1],
        lhs[2] - rhs[2],
        lhs[3] - rhs[3]
    );
}

inline constexpr Mat4 operator*(const Mat4& lhs, const Mat4& rhs) {
    Mat4 result{};

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result[row][col] =
                lhs[row][0] * rhs[0][col] +
                lhs[row][1] * rhs[1][col] +
                lhs[row][2] * rhs[2][col] +
                lhs[row][3] * rhs[3][col];
        }
    }

    return result;
}


inline constexpr Vec4 operator*(const Mat4& m, const Vec4& v) {
    return Vec4(
        m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w,
        m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w,
        m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w,
        m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w
    );
}


inline constexpr Vec4 operator*(const Mat4& lhs, const Vec3& rhs) {
    Vec4 v = lhs * Vec4(rhs.x, rhs.y, rhs.z, 1.0f);
    return v;
}

inline constexpr Vec4 & Vec4::operator+=(const Vec4 & rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}
