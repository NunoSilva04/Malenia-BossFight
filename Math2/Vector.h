#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__

#include <cstdio>
#include <cstdint>
#include <cmath>

/*

VEC2

*/

class Vec2{
public:
    float x, y;
    Vec2(float, float);
    Vec2(void);
    void Zero();
    const float* data() const; 
    float norm();
    void normalize();
    Vec2& operator=(const Vec2& a);
    Vec2 operator+(const Vec2& a) const;
    Vec2 operator-(const Vec2& a) const;
    float operator*(const Vec2& a) const;
    Vec2 operator/(const Vec2 &a) const;
    Vec2 operator/(const float& a);
    float operator[](const uint8_t index) const;
    float& operator[](const uint8_t);
    Vec2& operator+=(const Vec2&);
    Vec2& operator-=(const Vec2&);
    Vec2& operator*=(const Vec2&);
    Vec2& operator/=(const Vec2&);
};

inline float Vec2::norm(){
    float value = (this->x * this->x) + (this->y * this->y);
    return std::sqrt(value);
}

inline void Vec2::normalize(){
    float lenght = norm();
    this->x = this->x / lenght;
    this->y = this->y / lenght;
}

inline Vec2 Vec2::operator+(const Vec2& a) const{
    return Vec2(this->x + a.x , this->y + a.y);
}

inline Vec2 Vec2::operator-(const Vec2& a) const{
    return Vec2(this->x - a.x, this->y - a.y);
}

inline float Vec2::operator*(const Vec2& a) const{
    return ((this->x * a.x) + (this->y * a.y));
}

inline Vec2 Vec2::operator/(const Vec2& a) const{
    return Vec2(this->x / a.x, this->y / a.y);
}

inline Vec2 Vec2::operator/(const float& a){
    return Vec2(this->x / a, this->y / a);
}

inline float Vec2::operator[](const uint8_t index) const{
    if(index > 2){
        printf("Outside Vec2\n");
        return 0;
    }
    return *(&x + index);
}

inline float& Vec2::operator[](const uint8_t index){
    if(index > 2){
        printf("Outside Vec2\n");
        static float error = 0.0f;
        return error;
    }
    return *(&x + index);
}

inline Vec2& Vec2::operator+=(const Vec2& a){
    this->x += a.x;
    this->y += a.y;

    return *this;
}

inline Vec2& Vec2::operator-=(const Vec2& a){
    this->x -= a.x;
    this->y -= a.y;

    return *this;
}

inline Vec2& Vec2::operator*=(const Vec2& a){
    this->x *= a.x;
    this->y *= a.y;

    return *this;
}

inline Vec2& Vec2::operator/=(const Vec2& a){
    this->x /= a.x;
    this->y /= a.y;

    return *this;
}

inline Vec2& Vec2::operator=(const Vec2& a){
    this->x = a.x;
    this->y = a.y;

    return *this;
}

/*

VEC3

*/

class Vec3{
public:
    float x, y, z;
    Vec3(float, float, float);
    Vec3(void);
    void Zero();
    const float* data() const; 
    float norm();
    void normalize();
    Vec3& operator=(const Vec3& a);
    Vec3 operator+(const Vec3& a) const;
    Vec3 operator-(const Vec3& a) const;
    float operator*(const Vec3& a) const;
    Vec3 operator/(const Vec3 &a) const;
    Vec3 operator/(const float& a);
    float operator[](const uint8_t index) const;
    float& operator[](const uint8_t);
    Vec3& operator+=(const Vec3&);
    Vec3& operator-=(const Vec3&);
    Vec3& operator*=(const Vec3&);
    Vec3& operator/=(const Vec3&);
};

inline float Vec3::norm(){
    float value = (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
    return std::sqrt(value);
}

inline void Vec3::normalize(){
    float lenght = norm();
    this->x = this->x / lenght;
    this->y = this->y / lenght;
    this->z = this->z / lenght;
}

inline Vec3 Vec3::operator+(const Vec3& a) const{
    return Vec3(this->x + a.x , this->y + a.y, this->z + a.z);
}

inline Vec3 Vec3::operator-(const Vec3& a) const{
    return Vec3(this->x - a.x, this->y - a.y, this->z - a.z);
}

inline float Vec3::operator*(const Vec3& a) const{
    return ((this->x * a.x) + (this->y * a.y) + (this->z * a.z));
}

inline Vec3 Vec3::operator/(const Vec3& a) const{
    return Vec3(this->x / a.x, this->y / a.y, this->z / a.z);
}

inline Vec3 Vec3::operator/(const float& a){
    return Vec3(this->x / a, this->y / a, this->z / a);
}

inline float Vec3::operator[](const uint8_t index) const{
    if(index > 3){
        printf("Outside Vec3\n");
        return 0;
    }
    return *(&x + index);
}

inline float& Vec3::operator[](const uint8_t index){
    if(index > 3){
        printf("Outside Vec3\n");
        static float error = 0.0f;
        return error;
    }
    return *(&x + index);
}

inline Vec3& Vec3::operator+=(const Vec3& a){
    this->x += a.x;
    this->y += a.y;
    this->z += a.z;

    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& a){
    this->x -= a.x;
    this->y -= a.y;
    this->z -= a.z;

    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& a){
    this->x *= a.x;
    this->y *= a.y;
    this->z *= a.z;

    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& a){
    this->x /= a.x;
    this->y /= a.y;
    this->z /= a.z;

    return *this;
}

inline Vec3& Vec3::operator=(const Vec3& a){
    this->x = a.x;
    this->y = a.y;
    this->z = a.z;

    return *this;
}

/*

VEC4

*/

class Vec4{
public:
    float x, y, z, w;
    Vec4(float, float, float, float);
    Vec4(void);
    void print();
    void Zero();
    const float* data() const; 
    float norm();
    void normalize();
    Vec4& operator=(const Vec4& a);
    Vec4 operator+(const Vec4& a) const;
    Vec4 operator-(const Vec4& a) const;
    float operator*(const Vec4& a) const;
    Vec4 operator*(const float& a) const;
    Vec4 operator/(const Vec4 &a) const;
    Vec4 operator/(const float& a);
    float operator[](const uint8_t index) const;
    float& operator[](const uint8_t);
    Vec4& operator+=(const Vec4&);
    Vec4& operator-=(const Vec4&);
    Vec4& operator*=(const Vec4&);
    Vec4& operator/=(const Vec4&);
};

inline float Vec4::norm(){
    float value = (this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w);
    return std::sqrt(value);
}

inline void Vec4::normalize(){
    float lenght = norm();
    this->x = this->x / lenght;
    this->y = this->y / lenght;
    this->z = this->z / lenght;
    this->w = this->w / lenght;
}

inline Vec4 Vec4::operator+(const Vec4& a) const{
    return Vec4(this->x + a.x , this->y + a.y, this->z + a.z, this->w + a.w);
}

inline Vec4 Vec4::operator-(const Vec4& a) const{
    return Vec4(this->x - a.x, this->y - a.y, this->z - a.z, this->w - a.w);
}

inline float Vec4::operator*(const Vec4& a) const{
    return ((this->x * a.x) + (this->y * a.y) + (this->z * a.z) + (this->w * a.w));
}

inline Vec4 Vec4::operator*(const float& a) const{
    return Vec4(this->x * a, this->y * a, this->z * a, this->w * a);
}

inline Vec4 Vec4::operator/(const Vec4& a) const{
    return Vec4(this->x / a.x, this->y / a.y, this->z / a.z, this->w / a.w);
}

inline Vec4 Vec4::operator/(const float& a){
    return Vec4(this->x / a, this->y / a, this->z / a, this->w / a);
}

inline float Vec4::operator[](const uint8_t index) const{
    if(index > 4){
        printf("Outside Vec4\n");
        return 0;
    }
    return *(&x + index);
}

inline float& Vec4::operator[](const uint8_t index){
    if(index > 4){
        printf("Outside Vec4\n");
        static float error = 0.0f;
        return error;
    }
    return *(&x + index);
}

inline Vec4& Vec4::operator+=(const Vec4& a){
    this->x += a.x;
    this->y += a.y;
    this->z += a.z;
    this->w += a.w;

    return *this;
}

inline Vec4& Vec4::operator-=(const Vec4& a){
    this->x -= a.x;
    this->y -= a.y;
    this->z -= a.z;
    this->w -= a.w;

    return *this;
}

inline Vec4& Vec4::operator*=(const Vec4& a){
    this->x *= a.x;
    this->y *= a.y;
    this->z *= a.z;
    this->w *= a.w;

    return *this;
}

inline Vec4& Vec4::operator/=(const Vec4& a){
    this->x /= a.x;
    this->y /= a.y;
    this->z /= a.z;
    this->w /= a.w;

    return *this;
}

inline Vec4& Vec4::operator=(const Vec4& a){
    this->x = a.x;
    this->y = a.y;
    this->z = a.z;
    this->w = a.w;

    return *this;
}

#endif