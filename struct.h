#pragma once
#include <math.h>
#include <vector>
#include <array>
#include <assert.h>
#include <iostream>

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

    Vec2 operator- (const Vec2& rhs){
        return Vec2(x-rhs.x, y-rhs.y);
    }

    Vec2 operator+ (const Vec2& rhs){
        return Vec2(x+rhs.x, y+rhs.y);
    }
    
    Vec2 operator* (float rhs){
        return Vec2(x*rhs, y*rhs);
    }

    Vec2 operator/(float rhs)    {
        return Vec2(x / rhs, y / rhs);
    }

    Vec2 operator* (const Vec2& rhs){
        return Vec2(x*rhs.x, y*rhs.y);
    }

    Vec2& operator -= (const Vec2& rhs){
       x -= rhs.x;
       y -= rhs.y;
       return *this;
    }
    
    Vec2& operator += (const Vec2& rhs){
        x += rhs.x;
        y += rhs.y;
        return *this;
     }

     Vec2& operator *= (const float rhs){
        x*= rhs;
        y*= rhs;
        return *this;
     }

     Vec2& operator *= (const Vec2& rhs){
        x*= rhs.x;
        y*= rhs.y;
        return *this;
     }

     Vec2& operator /= (float rhs){
        x /= rhs;
        y /= rhs;
        return *this;
     }

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

    constexpr Vec2 ConvertToVec2() const;
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
};

std::ostream& operator<<(std::ostream& os, const Vec3& vec);
std::ostream& operator << ( std::ostream& os, const Mat3& mat);

inline constexpr Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
inline constexpr Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
inline constexpr Vec3 operator*(const Vec3& lhs, float rhs);
inline constexpr Vec3 operator*(float lhs, const Vec3& rhs);
inline constexpr Vec3 operator/(const Vec3& lhs, float rhs);

inline constexpr Vec3 operator*(const Mat3& lhs, const Vec3& rhs);
inline constexpr Vec3 operator*(const Mat3& lhs, const Vec2& rhs);

inline constexpr Mat3 operator+(const Mat3 &lhs, const Mat3 &rhs);
inline constexpr Mat3 operator-(const Mat3 &lhs, const Mat3 &rhs);
inline constexpr Mat3 operator*(const Mat3 &lhs, const Mat3 &rhs);

struct RGBA{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    RGBA(uint8_t _r = 255, uint8_t _g = 255, uint8_t _b = 255, uint8_t _a = 255)
        : r{_r}, g{_g}, b{_b}, a{_a} {}
};

struct Vertex{
    Vec2 v;
    RGBA color;

    Vertex(const Vec2& _v = Vec2(), const RGBA& _color = RGBA())
        : v{_v}, color{_color} {}
};

namespace Vec{
    float LengthSquare(Vec2 _vec);
    float Length(Vec2 _vec);

    Vec2 Normalize(const Vec2 &vec);

    float Dot(const Vec2 &lhs, const Vec2 &rhs);
    float Cross(const Vec2 &lhs, const Vec2 &rhs);
    Vec2 Projection(Vec2 vec_unit, Vec2 rhs);

    Vec2 Normal(const Vec2& vec);

    Vec2 Reverse(const Vec2& vec);

    float GetDegree(Vec2 _v1, Vec2 _v2);
    float GetRadian(float _v);
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
