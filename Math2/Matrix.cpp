#include "Matrix.h"

Mat4x4::Mat4x4(){
    this->mat[0] = Vec4(1, 0, 0, 0);
    this->mat[1] = Vec4(0, 1, 0, 0);
    this->mat[2] = Vec4(0, 0, 1, 0);
    this->mat[3] = Vec4(0, 0, 0, 1);
}   

Mat4x4::Mat4x4(const Vec4 &a, const Vec4 &b, const Vec4 &c, const Vec4 &d){
    this->mat[0] = a;
    this->mat[1] = b;
    this->mat[2] = c;
    this->mat[3] = d;
}

Mat4x4 Mat4x4::matrixIdentity(){
    return Mat4x4(Vec4(1, 0, 0, 0),
                  Vec4(0, 1, 0, 0),
                  Vec4(0, 0, 1, 0),
                  Vec4(0, 0, 0, 1));
}

void Mat4x4::print(){
    std::printf("(%f, %f, %f, %f)\n"
                "(%f, %f, %f, %f)\n"
                "(%f, %f, %f, %f)\n"
                "(%f, %f, %f, %f)\n\n", 
                this->mat[0].x, this->mat[1].x, this->mat[2].x, this->mat[3].x,
                this->mat[0].y, this->mat[1].y, this->mat[2].y, this->mat[3].y,
                this->mat[0].z, this->mat[1].z, this->mat[2].z, this->mat[3].z,
                this->mat[0].w, this->mat[1].w, this->mat[2].w, this->mat[3].w);
}