#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include "Math2.h"

class Mesh;

class Position{
public:
    Position(void);
    ~Position(void);
    bool initPosition(Mesh *);
    void add(const Vec4);
    void subtract(const Vec4);
    void multiply(const Vec4);
    void divide(const Vec4);
    const Mat4x4 getTransformMat(void);
    const Vec4 getPosition(void);
    void destroyPosition(void);

private:
    Vec4 origin;
    Mat4x4 transform, scale, rotation, translation;
};

#endif