// OniDev - Copyright (c) 2013-2016 Boris Marmontel
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.

#ifndef ONIDEV_MATH_VEC2_H_INCLUDED
#define ONIDEV_MATH_VEC2_H_INCLUDED

#include <onidev/core/primitives.h>

namespace od
{

struct Vec2 : Pos
{
    static const Vec2 Zero;
    static const Vec2 UnitX;
    static const Vec2 UnitY;
    static const Vec2 One;
    
    Vec2();
    Vec2(float val); // cppcheck: add explicit
    Vec2(float x, float y);
    Vec2(const Vec2 & v);
    Vec2(float v[]); // cppcheck: add explicit
    
    bool operator==(const Vec2 & v);
    bool operator!=(const Vec2 & v);
    
    const Vec2 operator+(const Vec2 & v) const;
    const Vec2 operator-(const Vec2 & v) const;
    const Vec2 & operator+=(const Vec2 & v);
    const Vec2 & operator-=(const Vec2 & v);
    
    // projection
    const Vec2 project(const Vec2 & axe);
    
    // uniform scaling
    const Vec2 & operator*=(float n);
    const Vec2 & operator/=(float n);
    
    // dot product
    float operator*(const Vec2 & v) const;
    float scalar(const Vec2 & v) const;
    float dot(const Vec2 & v) const;
    float cross(const Vec2 & v) const;
    float length() const;
    float lengthSquared() const;
    void normalize();
    const Vec2 normalized() const;
    bool isNormalized() const;
    
    void rotate(float theta);
    void rotate(const Vec2 & origin, float theta);
    
    const Vec2 rotated(float theta);
    const Vec2 rotated(const Vec2 & origin, float theta);
    
    // Angle in radians
    float angle() const;
    float angle(const Vec2 & v) const;
    float angle(const Vec2 & left, const Vec2 & right) const;
    
    static Vec2 barycentric(Vec2 v1, Vec2 v2, Vec2 v3, float amount1, float amount2);
    static Vec2 clamp(Vec2 vec, Vec2 min, Vec2 max);
    static float distance(Vec2 v1, Vec2 v2);
    static float distanceSquared(Vec2 v1, Vec2 v2);
    static float dot(Vec2 v1, Vec2 v2);
    static float cross(Vec2 v1, Vec2 v2);
    static Vec2 lerp(Vec2 v1, Vec2 v2, float amount);
    static Vec2 smoothStep(Vec2 v1, Vec2 v2, float amount);
    static Vec2 hermite(Vec2 vec1, Vec2 tan1, Vec2 vec2, Vec2 tan2, float amount);
    static Vec2 catmullRom(Vec2 v1, Vec2 v2, Vec2 v3, Vec2 v4, float amount);
    static Vec2 max(Vec2 v1, Vec2 v2);
    static Vec2 min(Vec2 v1, Vec2 v2);
    static Vec2 reflect(Vec2 v, Vec2 normal);
    
    float & operator[](int ind);
    float operator[](int ind) const;
};

Vec2 operator*(const Vec2 & vec, float n);
Vec2 operator/(const Vec2 & vec, float n);
Vec2 operator*(float n, const Vec2 & vec);
Vec2 operator/(float n, const Vec2 & vec);

}

#endif
