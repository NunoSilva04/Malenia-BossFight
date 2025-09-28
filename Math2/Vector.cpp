#include "Vector.h"

/*

Vec2

*/

Vec2::Vec2(float a, float b){
    this->x = a; this->y = b;
}

Vec2::Vec2(void){
    this->x = 0; this->y = 0; 
}

void Vec2::Zero(){
    this->x = 0; this->y = 0;
}

const float *Vec2::data() const{
    return &x;
}

/*

VEC3

*/

Vec3::Vec3(float a, float b, float c){
    this->x = a; this->y = b; this->z = c;
}

Vec3::Vec3(void){
    this->x = 0; this->y = 0; this->z = 0;
}

void Vec3::Zero(){
    this->x = 0; this->y = 0; this->z = 0;
}

const float *Vec3::data() const{
    return &x;
}

/*

VEC4

*/

Vec4::Vec4(float a, float b, float c, float d){
    this->x = a; this->y = b; this->z = c; this->w = d;
}

Vec4::Vec4(void){
    this->x = 0; this->y = 0; this->z = 0; this->w = 0;
}

void Vec4::print(){
    std::printf("(%f, %f, %f, %f)\n", this->x, this->y, this->z, this->w);
}

void Vec4::Zero(){
    this->x = 0; this->y = 0; this->z = 0; this->w = 0;
}

const float* Vec4::data() const{
    return &x;
}