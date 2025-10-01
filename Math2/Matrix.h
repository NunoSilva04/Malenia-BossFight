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
    static Mat4x4 zero();
    Mat4x4 translate(const float, const float, const float);
    Mat4x4 rotate(Mat4x4 (*axis)(const float), const float);
    Mat4x4 xAxis(const float);
    Mat4x4 yAxis(const float);
    Mat4x4 zAxis(const float);
    Mat4x4 scale(const float, const float, const float);
    void print();

    Vec4 operator*(const Vec4& a);
    Mat4x4 operator*(const Mat4x4& a);
    Mat4x4& operator=(const Mat4x4& a); 
};

inline Mat4x4 Mat4x4::translate(const float tx, const float ty, const float tz){
    Mat4x4 translation = Mat4x4(Vec4(1, 0, 0, 0), 
                               Vec4(0, 1, 0, 0),
                               Vec4(0, 0, 1, 0),
                               Vec4(tx, ty, tz, 1));
    return (translation * (*this));
}

inline Mat4x4 Mat4x4::rotate(Mat4x4 (*axis)(const float), const float angle){
    Mat4x4 rotation = axis(angle);
    return (rotation * (*this));
}

inline Mat4x4 Mat4x4::xAxis(const float axis){
    //TODO   
}

inline Mat4x4 Mat4x4::yAxis(const float axis){
    //TODO   
}

inline Mat4x4 Mat4x4::zAxis(const float axis){
    //TODO   
}

inline Mat4x4 Mat4x4::scale(const float sx, const float sy, const float sz){
    Mat4x4 scale(Vec4(sx, 0, 0, 0),
                 Vec4(0, sy, 0, 0),
                 Vec4(0, 0, sz, 0),
                 Vec4(0, 0, 0, 1));
    return (scale * (*this));
}

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