#pragma once
#include "vector.hpp"

namespace vk::util::math {
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
using quatf=quat;
}
namespace vk::util::math::quaternion {
using ::vk::util::math::quat;
using ::vk::util::math::quatf;
using ::vk::util::math::dot;
using ::vk::util::math::normalize;
using ::vk::util::math::conjugate;
using ::vk::util::math::inverse;
using ::vk::util::math::angle_axis;
using ::vk::util::math::euler;
using ::vk::util::math::nlerp;
using ::vk::util::math::slerp;
using ::vk::util::math::rotate;
}
