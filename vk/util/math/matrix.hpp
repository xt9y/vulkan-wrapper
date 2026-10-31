#pragma once
#include "quaternion.hpp"

namespace vk::util::math {
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
using mat4f=mat4;
}
namespace vk::util::math::matrix {
using ::vk::util::math::mat4;
using ::vk::util::math::mat4f;
using ::vk::util::math::transpose;
using ::vk::util::math::translate;
using ::vk::util::math::scale;
using ::vk::util::math::rotation;
using ::vk::util::math::trs;
using ::vk::util::math::look_at;
using ::vk::util::math::perspective;
using ::vk::util::math::orthographic;
}
