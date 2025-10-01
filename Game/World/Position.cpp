#include "Position.hpp"
#include "Mesh.hpp"

Position::Position(void){

}

Position::~Position(void){

}

bool Position::initPosition(Mesh *mesh){
    origin = mesh->getOriginPoint();
    transform = Mat4x4::matrixIdentity();

    return true;
}

void Position::add(const Vec4 position){
    origin += position;
    translation.translate(position.x, position.y, position.z);
}

void Position::subtract(const Vec4 position){
    origin -= position;
}

void Position::multiply(const Vec4 position){
    origin *= position;
}

void Position::divide(const Vec4 position){
    origin /= position;
}

const Mat4x4 Position::getTransformMat(void){
    return transform;
}

const Vec4 Position::getPosition(void){
    return origin;
}

void Position::destroyPosition(void){
    
}