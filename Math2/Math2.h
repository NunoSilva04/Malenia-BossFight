#ifndef __MATH_H__
#define __MATH_H__

#include "Matrix.h"
#include "Vector.h"

namespace Math{
    constexpr float PI = 3.14159265358979323846f;
    float radians(const float angle);
    float VecDistance(const Vec4 a, const Vec4 b);
    Vec3 cross(const Vec3& a, const Vec3& b);
    Vec4 cross(const Vec4& a, const Vec4& b);
    Mat4x4 CamLookAtLH(const Vec4& camPos, const Vec4& camTarget, const Vec4& camUp);
    Mat4x4 CamLookAtRH(const Vec4& camPos, const Vec4& camTarget, const Vec4& camUp);
    Mat4x4 CamPerspectiveLH(float nearPlane, float farPlane, float FOV, float aspectRatio);
    Mat4x4 CamPerspectiveRH(float nearPlane, float farPlane, float FOV, float aspectRatio);
}

inline float Math::radians(const float angle){
    return angle * (PI / 180.0f);
}

inline float Math::VecDistance(const Vec4 a, const Vec4 b){
    Vec4 c = b - a;
    return c.norm();
}

inline Vec3 Math::cross(const Vec3& a, const Vec3& b){
    float x = (a.y * b.z) - (a.z * b.y);
    float y = (a.z * b.x) - (a.x * b.z);
    float z = (a.x * b.y) - (a.y * b.x);
    return Vec3(x, y, z);
}

inline Vec4 Math::cross(const Vec4& a, const Vec4& b){
    float x = (a.y * b.z) - (a.z * b.y);
    float y = (a.z * b.x) - (a.x * b.z);
    float z = (a.x * b.y) - (a.y * b.x);
    return Vec4(x, y, z, 0);
}

inline Mat4x4 Math::CamLookAtLH(const Vec4& camPos, const Vec4& camTarget, const Vec4& camUp){
    Vec4 X, Y, Z;
    Z = (camTarget - camPos) / (camTarget - camPos).norm();
    X = Math::cross(camUp, Z) / Math::cross(camUp, Z).norm();
    Y = Math::cross(Z, X);

    Vec4 translation;
    float tx = 0, ty = 0, tz = 0;
    tx = -(X * camPos);
    ty = -(Y * camPos);
    tz = -(Z * camPos);
    translation = Vec4(tx, ty, tz, 1);

    return Mat4x4(
        Vec4(X.x, Y.x, Z.x, 0.0f),
        Vec4(X.y, Y.y, Z.y, 0.0f),
        Vec4(X.z, Y.z, Z.z, 0.0f),
        Vec4(tx, ty, tz, 1.0f)
    );
}

inline Mat4x4 Math::CamLookAtRH(const Vec4& camPos, const Vec4& camTarget, const Vec4& camUp){
    Vec4 X, Y, Z;
    Z = (camPos - camTarget) / (camPos - camTarget).norm();
    X = Math::cross(camUp, Z) / Math::cross(camUp, Z).norm();
    Y = Math::cross(Z, X);

    Vec4 translation;
    float tx = 0, ty = 0, tz = 0;
    tx = -(X * camPos);
    ty = -(Y * camPos);
    tz = -(Z * camPos);
    translation = Vec4(tx, ty, tz, 1);

    return Mat4x4(X, Y, Z, translation);
}

inline Mat4x4 Math::CamPerspectiveLH(float nearPlane, float farPlane, float FOV, float aspectRatio){
    Vec4 collumn1(1/(aspectRatio * std::tan(FOV / 2)), 0, 0, 0);
    Vec4 collumn2(0, 1/std::tan(FOV / 2), 0, 0);
    Vec4 collumn3(0, 0, farPlane/(farPlane - nearPlane), 1);
    Vec4 collumn4(0, 0, -(farPlane * nearPlane)/(farPlane - nearPlane), 0);

    return Mat4x4(collumn1, collumn2, collumn3, collumn4);
}

inline Mat4x4 Math::CamPerspectiveRH(float nearPlane, float farPlane, float FOV, float aspectRatio){
    Vec4 collumn1(1/(aspectRatio * std::tan(FOV / 2)), 0, 0, 0);
    Vec4 collumn2(0, 1/std::tan(FOV / 2), 0, 0);
    Vec4 collumn3(0, 0, -(farPlane + nearPlane)/(farPlane - nearPlane), -1);
    Vec4 collumn4(0, 0, -(2 * farPlane * nearPlane)/(farPlane - nearPlane), 0);

    return Mat4x4(collumn1, collumn2, collumn3, collumn4);
}

#endif