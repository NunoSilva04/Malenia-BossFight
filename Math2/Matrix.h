#ifndef __MATH_MATRIX_H__
#define __MATH_MATRIX_H__
//Collumn-Major

#include <cstdio>
#include "Vector.h"

/*

MAT4X4

*/

class Mat4x4{
public:
    Vec4 mat[4];
    Mat4x4(void);
    Mat4x4(const Vec4 &a, const Vec4 &b, const Vec4 &c, const Vec4 &d);
    static Mat4x4 matrixIdentity();
    void print();

    Vec4 operator*(const Vec4& a);
    Mat4x4 operator*(const Mat4x4& a);
    Mat4x4& operator=(const Mat4x4& a);
};

inline Vec4 Mat4x4::operator*(const Vec4& a){
    Vec4 result;
    result.x = (mat[0].x * a.x) + (mat[1].x * a.y) + (mat[2].x * a.z) + (mat[3].x * a.w);
    result.y = (mat[0].y * a.x) + (mat[1].y * a.y) + (mat[2].y * a.z) + (mat[3].y * a.w);
    result.z = (mat[0].z * a.x) + (mat[1].z * a.y) + (mat[2].z * a.z) + (mat[3].z * a.w);
    result.w = (mat[0].w * a.x) + (mat[1].w * a.y) + (mat[2].w * a.z) + (mat[3].w * a.w);
    return result;
}

inline Mat4x4 Mat4x4::operator*(const Mat4x4& a){
    return Mat4x4((*this) * a.mat[0], (*this) * a.mat[1], (*this) * a.mat[2], (*this) * a.mat[3]);
}

inline Mat4x4& Mat4x4::operator=(const Mat4x4& a){
    this->mat[0] = a.mat[0];
    this->mat[1] = a.mat[1];
    this->mat[2] = a.mat[2];
    this->mat[3] = a.mat[3];

    return *this;
}

#endif