#pragma once
#include "scalar.hpp"

namespace vk::util::math {
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
using vec2f=vec2;
using vec3f=vec3;
using vec4f=vec4;
}
namespace vk::util::math::vector {
using ::vk::util::math::vec2;
using ::vk::util::math::vec3;
using ::vk::util::math::vec4;
using ::vk::util::math::vec2f;
using ::vk::util::math::vec3f;
using ::vk::util::math::vec4f;
using ::vk::util::math::dot;
using ::vk::util::math::cross;
using ::vk::util::math::length_squared;
using ::vk::util::math::length;
using ::vk::util::math::normalize;
using ::vk::util::math::distance;
using ::vk::util::math::reflect;
using ::vk::util::math::project;
}
