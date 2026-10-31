#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

namespace vk::util::math {

constexpr float pi = 3.14159265358979323846f;
constexpr float radians(float degrees) { return degrees * pi / 180.0f; }
constexpr float degrees(float radians_value) { return radians_value * 180.0f / pi; }

template<typename T>
constexpr T clamp(T x, T low, T high) { return std::min(high, std::max(low, x)); }

template<typename T>
constexpr T lerp(T a, T b, float t) { return a + (b - a) * t; }

template<typename T>
constexpr T min(T a, T b) { return std::min(a, b); }

template<typename T>
constexpr T max(T a, T b) { return std::max(a, b); }

struct vec2 {
    float x = 0, y = 0;
    constexpr vec2() = default;
    constexpr vec2(float x, float y) : x(x), y(y) {}
    constexpr vec2 operator+(vec2 b) const { return {x + b.x, y + b.y}; }
    constexpr vec2 operator-(vec2 b) const { return {x - b.x, y - b.y}; }
    constexpr vec2 operator*(float s) const { return {x * s, y * s}; }
    constexpr vec2 operator/(float s) const { return {x / s, y / s}; }
};

struct vec3 {
    float x = 0, y = 0, z = 0;
    constexpr vec3() = default;
    constexpr vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    constexpr vec3 operator+(vec3 b) const { return {x + b.x, y + b.y, z + b.z}; }
    constexpr vec3 operator-(vec3 b) const { return {x - b.x, y - b.y, z - b.z}; }
    constexpr vec3 operator-() const { return {-x, -y, -z}; }
    constexpr vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    constexpr vec3 operator/(float s) const { return {x / s, y / s, z / s}; }
};

struct vec4 {
    float x = 0, y = 0, z = 0, w = 0;
    constexpr vec4() = default;
    constexpr vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    constexpr vec4(vec3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
    constexpr vec4 operator+(vec4 b) const { return {x + b.x, y + b.y, z + b.z, w + b.w}; }
    constexpr vec4 operator-(vec4 b) const { return {x - b.x, y - b.y, z - b.z, w - b.w}; }
    constexpr vec4 operator*(float s) const { return {x * s, y * s, z * s, w * s}; }
    constexpr vec4 operator/(float s) const { return {x / s, y / s, z / s, w / s}; }
};

constexpr vec2 operator*(float s, vec2 v) { return v * s; }
constexpr vec3 operator*(float s, vec3 v) { return v * s; }
constexpr vec4 operator*(float s, vec4 v) { return v * s; }

constexpr float dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
constexpr float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
constexpr float dot(vec4 a, vec4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
constexpr vec3 cross(vec3 a, vec3 b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
constexpr float length_squared(vec2 v) { return dot(v, v); }
constexpr float length_squared(vec3 v) { return dot(v, v); }
constexpr float length_squared(vec4 v) { return dot(v, v); }
inline float length(vec2 v) { return std::sqrt(length_squared(v)); }
inline float length(vec3 v) { return std::sqrt(length_squared(v)); }
inline float length(vec4 v) { return std::sqrt(length_squared(v)); }
inline vec2 normalize(vec2 v) { float n = length(v); return n > 0 ? v / n : vec2{}; }
inline vec3 normalize(vec3 v) { float n = length(v); return n > 0 ? v / n : vec3{}; }
inline vec4 normalize(vec4 v) { float n = length(v); return n > 0 ? v / n : vec4{}; }
inline float distance(vec3 a, vec3 b) { return length(a - b); }
inline float distance(vec2 a, vec2 b) { return length(a - b); }
inline vec3 reflect(vec3 incident, vec3 normal) { return incident - 2.0f * dot(incident, normal) * normal; }
inline vec3 project(vec3 a, vec3 onto) {
    float d = dot(onto, onto);
    return d > 0 ? onto * (dot(a, onto) / d) : vec3{};
}

struct quat {
    float x = 0, y = 0, z = 0, w = 1;
    constexpr quat() = default;
    constexpr quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    constexpr static quat identity() { return {}; }
};

constexpr float dot(quat a, quat b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }
inline quat normalize(quat q) {
    float n = std::sqrt(dot(q, q));
    return n > 0 ? quat{q.x/n,q.y/n,q.z/n,q.w/n} : quat{};
}
constexpr quat conjugate(quat q) { return {-q.x, -q.y, -q.z, q.w}; }
inline quat inverse(quat q) {
    float n = dot(q, q);
    auto c = conjugate(q);
    return n > 0 ? quat{c.x/n,c.y/n,c.z/n,c.w/n} : quat{};
}
constexpr quat operator*(quat a, quat b) {
    return {
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w,
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
    };
}
inline quat angle_axis(float angle, vec3 axis) {
    axis = normalize(axis);
    float s = std::sin(angle * 0.5f);
    return {axis.x*s, axis.y*s, axis.z*s, std::cos(angle*0.5f)};
}
inline quat euler(float pitch, float yaw, float roll) {
    return angle_axis(yaw, {0,1,0}) * angle_axis(pitch, {1,0,0})
         * angle_axis(roll, {0,0,1});
}
inline quat nlerp(quat a, quat b, float t) {
    if (dot(a,b) < 0) b = {-b.x,-b.y,-b.z,-b.w};
    return normalize({lerp(a.x,b.x,t),lerp(a.y,b.y,t),lerp(a.z,b.z,t),lerp(a.w,b.w,t)});
}
inline quat slerp(quat a, quat b, float t) {
    float d = dot(a,b);
    if (d < 0) { b = {-b.x,-b.y,-b.z,-b.w}; d = -d; }
    d = clamp(d, -1.0f, 1.0f);
    if (d > 0.9995f) return nlerp(a,b,t);
    float angle = std::acos(d);
    float sin_angle = std::sin(angle);
    float x = std::sin((1-t)*angle) / sin_angle;
    float y = std::sin(t*angle) / sin_angle;
    return normalize({a.x*x+b.x*y,a.y*x+b.y*y,a.z*x+b.z*y,a.w*x+b.w*y});
}
inline vec3 rotate(quat q, vec3 v) {
    q = normalize(q);
    vec3 u{q.x,q.y,q.z};
    return v + 2.0f * (q.w * cross(u,v) + cross(u,cross(u,v)));
}

struct mat4 {
    std::array<float,16> v{};
    constexpr float &operator()(std::size_t row, std::size_t col) { return v[col*4+row]; }
    constexpr float operator()(std::size_t row, std::size_t col) const { return v[col*4+row]; }
    float *data() { return v.data(); }
    const float *data() const { return v.data(); }
    static constexpr mat4 identity() {
        mat4 m;
        m(0,0)=m(1,1)=m(2,2)=m(3,3)=1;
        return m;
    }
};

constexpr mat4 operator*(mat4 a, mat4 b) {
    mat4 result;
    for (std::size_t r=0; r<4; ++r)
        for (std::size_t c=0; c<4; ++c)
            for (std::size_t k=0; k<4; ++k)
                result(r,c)+=a(r,k)*b(k,c);
    return result;
}
constexpr vec4 operator*(mat4 m, vec4 v) {
    return {
        m(0,0)*v.x + m(0,1)*v.y + m(0,2)*v.z + m(0,3)*v.w,
        m(1,0)*v.x + m(1,1)*v.y + m(1,2)*v.z + m(1,3)*v.w,
        m(2,0)*v.x + m(2,1)*v.y + m(2,2)*v.z + m(2,3)*v.w,
        m(3,0)*v.x + m(3,1)*v.y + m(3,2)*v.z + m(3,3)*v.w
    };
}
inline mat4 transpose(mat4 a) {
    mat4 b;
    for (std::size_t r=0;r<4;++r)
        for (std::size_t c=0;c<4;++c) b(r,c)=a(c,r);
    return b;
}
inline mat4 translate(vec3 t) {
    auto m=mat4::identity();
    m(0,3)=t.x; m(1,3)=t.y; m(2,3)=t.z;
    return m;
}
inline mat4 scale(vec3 s) {
    auto m=mat4::identity();
    m(0,0)=s.x; m(1,1)=s.y; m(2,2)=s.z;
    return m;
}
inline mat4 rotation(quat q) {
    q=normalize(q);
    float x=q.x,y=q.y,z=q.z,w=q.w;
    auto m=mat4::identity();
    m(0,0)=1-2*(y*y+z*z); m(0,1)=2*(x*y-z*w); m(0,2)=2*(x*z+y*w);
    m(1,0)=2*(x*y+z*w); m(1,1)=1-2*(x*x+z*z); m(1,2)=2*(y*z-x*w);
    m(2,0)=2*(x*z-y*w); m(2,1)=2*(y*z+x*w); m(2,2)=1-2*(x*x+y*y);
    return m;
}
inline mat4 trs(vec3 position, quat orientation, vec3 scaling) {
    return translate(position)*rotation(orientation)*scale(scaling);
}
inline mat4 look_at(vec3 eye, vec3 target, vec3 up={0,1,0}) {
    vec3 f=normalize(target-eye), s=normalize(cross(f,up)), u=cross(s,f);
    auto m=mat4::identity();
    m(0,0)=s.x; m(0,1)=s.y; m(0,2)=s.z; m(0,3)=-dot(s,eye);
    m(1,0)=u.x; m(1,1)=u.y; m(1,2)=u.z; m(1,3)=-dot(u,eye);
    m(2,0)=-f.x; m(2,1)=-f.y; m(2,2)=-f.z; m(2,3)=dot(f,eye);
    return m;
}
inline mat4 perspective(float fov_y, float aspect, float near_z, float far_z,
                        bool flip_y=true) {
    mat4 m;
    float f=1.0f/std::tan(fov_y*0.5f);
    m(0,0)=f/aspect;
    m(1,1)=flip_y ? -f : f;
    m(2,2)=far_z/(near_z-far_z);
    m(2,3)=(far_z*near_z)/(near_z-far_z);
    m(3,2)=-1;
    return m;
}
inline mat4 orthographic(float left, float right, float bottom, float top,
                         float near_z, float far_z, bool flip_y=true) {
    auto m=mat4::identity();
    m(0,0)=2/(right-left);
    m(1,1)=(flip_y ? -2.0f : 2.0f)/(top-bottom);
    m(2,2)=1/(near_z-far_z);
    m(0,3)=-(right+left)/(right-left);
    m(1,3)=-(top+bottom)/(top-bottom)*(flip_y ? -1.0f : 1.0f);
    m(2,3)=near_z/(near_z-far_z);
    return m;
}

using vec2f=vec2;
using vec3f=vec3;
using vec4f=vec4;
using mat4f=mat4;
using quatf=quat;

}
