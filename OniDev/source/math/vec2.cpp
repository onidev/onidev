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

/*=============================================================================
	vec2.cpp
=============================================================================*/

#include <onidev/math/vec2.h>
#include <cmath>

namespace od
{
const Vec2 Vec2::Zero (0.f, 0.f);
const Vec2 Vec2::UnitX(1.f, 0.f);
const Vec2 Vec2::UnitY(0.f, 1.f);
const Vec2 Vec2::One  (1.f, 1.f);

Vec2::Vec2():
    Pos(0.f, 0.f)
{
}
Vec2::Vec2(float val):
    Pos(val, val)
{
}
Vec2::Vec2(float x, float y):
    Pos(x, y)
{
}
Vec2::Vec2(const Vec2& o):
    Pos(o.x, o.y)
{
}
Vec2::Vec2(float v[]):
    Pos(v[0], v[1])
{
}

bool Vec2::operator==(const Vec2 & v) {
    return x == v.x && y == v.y;
}
bool Vec2::operator!=(const Vec2 & v) {
    return !(*this == v);
}

const Vec2 Vec2::operator+(const Vec2 & v) const {
    return Vec2(x + v.x, y + v.y);
}
const Vec2 Vec2::operator-(const Vec2 & v) const {
    return Vec2(x - v.x, y - v.y);
}

const Vec2 & Vec2::operator+=(const Vec2 & v) {
    x += v.x;
    y += v.y;
    return *this;
}
const Vec2 & Vec2::operator-=(const Vec2 & v) {
    x -= v.x;
    y -= v.y;
    return *this;
}


// uniform scaling
const Vec2 & Vec2::operator*=(float n) {
    x *= n;
    y *= n;
    return *this;
}
const Vec2 & Vec2::operator/=(float n) {
    x /= n;
    y /= n;
    return *this;
}

const Vec2 Vec2::project(const Vec2 & axe)
{
    float sc = scalar(axe);
    return Vec2(sc * x, sc * y);
}

// dot product
float Vec2::operator*(const Vec2 & v) const {
    return x*v.x + y*v.y;
}
float Vec2::scalar(const Vec2 & v) const {
    return x*v.x + y*v.y;
}
float Vec2::dot(const Vec2 & v) const {
    return x*v.x + y*v.y;
}

float Vec2::cross(const Vec2 & v) const {
    return x*v.y - y*v.x;
}

float Vec2::length() const {
    return sqrtf(x*x + y*y);
}
float Vec2::lengthSquared() const {
    return x*x + y*y;
}
void Vec2::normalize() {
    float len = length();
    x /= len;
    y /= len;
}
const Vec2 Vec2::normalized() const {
    float len = length();
    return Vec2(x/len, y/len);
}
bool Vec2::isNormalized() const {
    return fabs( (x*x + y*y) - 1.f ) < 1e-06f;
}

void Vec2::rotate(float a)
{
    float cosa = cosf(a), sina = sinf(a);
    float tx = x * cosa - y * sina;
    float ty = y * cosa + x * sina;
    x = tx;
    y = ty;
}

void Vec2::rotate(const Vec2 & origin, float theta)
{
    float x = this->x - origin.x;
    float y = this->y - origin.y;
    float cosa = cosf(theta), sina = sinf(theta);
    this->x = origin.x + x*cosa - y*sina;
    this->y = origin.y + x*sina + y*cosa;
}

const Vec2 Vec2::rotated(float theta)
{
    Vec2 t = *this;
    t.rotate(theta);
    return t;
}
const Vec2 Vec2::rotated(const Vec2 & origin, float theta)
{
    Vec2 t = *this;
    t.rotate(origin, theta);
    return t;
}

float Vec2::angle() const
{
    return atan2(y, x);
}
float Vec2::angle(const Vec2 & v) const
{
    return atan2f(x*v.y - y*v.x, x*v.x + y*v.y);
}
float Vec2::angle(const Vec2 & left, const Vec2 & right) const
{
    return (left - *this).angle(right - *this);
}

Vec2 Vec2::barycentric(Vec2 v1, Vec2 v2, Vec2 v3, float amount1, float amount2)
{
    return Vec2(
        (v1.x + amount1 * (v2.x - v1.x)) + amount2 * (v3.x - v1.x),
        (v1.y + amount1 * (v2.y - v1.y)) + amount2 * (v3.y - v1.y)
    );
}
Vec2 Vec2::clamp(Vec2 vec, Vec2 min, Vec2 max)
{
    float x = vec.x;
    x = (x > max.x) ? max.x : x;
    x = (x < min.x) ? min.x : x;
    float y = vec.y;
    y = (y > max.y) ? max.y : y;
    y = (y < min.y) ? min.y : y;
    return Vec2(x, y);
}
float Vec2::distance(Vec2 v1, Vec2 v2)
{
    float x = v2.x - v1.x;
    float y = v2.y - v1.y;
    return sqrtf(x*x + y*y);
}
float Vec2::distanceSquared(Vec2 v1, Vec2 v2)
{
    float x = v2.x - v1.x;
    float y = v2.y - v1.y;
    return x*x + y*y;
}
float Vec2::dot(Vec2 v1, Vec2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float Vec2::cross(Vec2 v1, Vec2 v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}
Vec2 Vec2::lerp(Vec2 v1, Vec2 v2, float amount)
{
    return Vec2(v1.x + (v2.x - v1.x)*amount, v1.y + (v2.y - v1.y)*amount);
}
Vec2 Vec2::smoothStep(Vec2 v1, Vec2 v2, float amount)
{
    amount = (amount > 1.f) ? 1.f : ((amount < 0.f) ? 0.f : amount);
    amount = (amount * amount) * (3.f - 2.f * amount);
    return Vec2(v1.x + (v2.x - v1.x)*amount, v1.y + (v2.y - v1.y)*amount);
}
Vec2 Vec2::hermite(Vec2 vec1, Vec2 tan1, Vec2 vec2, Vec2 tan2, float amount)
{
    float a2 = amount * amount;
    float a3 = a2 * amount;
    float v1 = (2.f * a3) - (3.f * a2) + 1.f;
    float v2 = (-2.f * a3) + (3.f * a2);
    float v3 = (a3 - 2.f * a2) + amount;
    float v4 = a3 - a2;
    
    return Vec2(
        vec1.x * v1 + vec2.x * v2 + tan1.x * v3 + tan2.x * v4,
        vec1.y * v1 + vec2.y * v2 + tan1.y * v3 + tan2.y * v4
    );
}
Vec2 Vec2::catmullRom(Vec2 v1, Vec2 v2, Vec2 v3, Vec2 v4, float amount)
{
    float a2 = amount * amount;
    float a3 = a2 * amount;
    
    return Vec2(
        0.5f * ( 2.f * v2.x + (v3.x - v1.x) * amount + (2.f * v1.x - 5.f * v2.x + 4.f * v3.x - v4.x) * a2 + (-v1.x + 3.f * v2.x - 3.f * v3.x + v4.x) * a3 ),
        0.5f * ( 2.f * v2.y + (v3.y - v1.y) * amount + (2.f * v1.y - 5.f * v2.y + 4.f * v3.y - v4.y) * a2 + (-v1.y + 3.f * v2.y - 3.f * v3.y + v4.y) * a3 )
    );
}
Vec2 Vec2::max(Vec2 v1, Vec2 v2)
{
    return Vec2(
        (v1.x > v2.x) ? v1.x : v2.x,
        (v1.y > v2.y) ? v1.y : v2.y
    );
}
Vec2 Vec2::min(Vec2 v1, Vec2 v2)
{
    return Vec2(
        (v1.x < v2.x) ? v1.x : v2.x,
        (v1.y < v2.y) ? v1.y : v2.y
    );
}
Vec2 Vec2::reflect(Vec2 v, Vec2 normal)
{
    float n = v.x * normal.x + v.y * normal.y;
    return Vec2(
        v.x - (2.f * n * normal.x),
        v.y - (2.f * n * normal.y)
    );
}

float & Vec2::operator[](int ind) {
    if(ind == 0) return x;
    return y;
}
float Vec2::operator[](int ind) const {
    if(ind == 0) return x;
    return y;
}

Vec2 operator*(const Vec2 & vec, float n)
{
    return Vec2(vec.x * n, vec.y * n);
}
Vec2 operator/(const Vec2 & vec, float n)
{
    return Vec2(vec.x / n, vec.y / n);
}
Vec2 operator*(float n, const Vec2 & vec)
{
    return vec * n;
}
Vec2 operator/(float n, const Vec2 & vec)
{
    return vec/n;
}

}
