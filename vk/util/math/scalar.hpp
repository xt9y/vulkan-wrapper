#pragma once
#include "../../common.hpp"
#include <cmath>
#include <algorithm>
#include <array>

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
}
namespace vk::util::math::scalar {
using ::vk::util::math::pi;
using ::vk::util::math::radians;
using ::vk::util::math::degrees;
using ::vk::util::math::clamp;
using ::vk::util::math::lerp;
using ::vk::util::math::min;
using ::vk::util::math::max;
}
