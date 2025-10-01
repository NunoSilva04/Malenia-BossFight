#include "Camera.h"
#include <limits>
#include "Input.h"
#include "Windows_OS.h"
#include "IGraphics.h"

Camera::Camera(){
    cameraBuffer = nullptr;
    camPosition = camTarget = camUp = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    distanceToTarget = Math::VecDistance(camPosition, camTarget);
    cameraBuffer_t.camView = cameraBuffer_t.camProjection = Mat4x4::matrixIdentity();
    nearPlane = farPlane = FOV = aspectRatio = 0.0f;
    forwardVec = rightVec = upVec = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    cameraSpeed = 0.0001f;
    sensitivity = 0.00002f;
    yaw = pitch = 0.0f;
}

Camera::~Camera(){

}

bool Camera::initCamera(IResources *resources, const windowPreferances window){
    camPosition = Vec4(0.0f, 0.0f, -5.0f, 0.0f);
    camUp = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
    camTarget = Vec4(2.0f, 0.0f, 0.5f, 0.0f);
    cameraBuffer_t.camView = Math::CamLookAtLH(camPosition, camTarget, camUp);

    distanceToTarget = Math::VecDistance(camPosition, camTarget);
    forwardVec = camPosition - camTarget;
    forwardVec.normalize();
    rightVec = Math::cross(forwardVec, worldUp);
    rightVec.normalize();
    upVec = Math::cross(rightVec, forwardVec);
    upVec.normalize();

    nearPlane = 0.1f;
    farPlane = 100.0f;
    FOV = Math::radians(45.0f);
    aspectRatio = static_cast<float>(window.width) / static_cast<float>(window.height);
    cameraBuffer_t.camProjection = Math::CamPerspectiveLH(nearPlane, farPlane, FOV, aspectRatio);

    D3D11_BUFFER_DESC cameraBufferDesc;
    ZeroMemory(&cameraBufferDesc, sizeof(D3D11_BUFFER_DESC));
    cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cameraBufferDesc.ByteWidth = sizeof(cameraBuffer_t);
    cameraBufferDesc.CPUAccessFlags = 0;
    cameraBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    if(!resources->createBuffer(&cameraBufferDesc, nullptr, &cameraBuffer)){
        std::printf("Couldn't create camera constant buffer\n");
        return false;
    }

    return true;
}

void Camera::moveCameraTarget(IInput *input){
    Sint16 deltaX = input->getAxisValue(controllerAxis::RIGHT_X);
    Sint16 deltaY = -input->getAxisValue(controllerAxis::RIGHT_Y);

    yaw += deltaX * sensitivity;
    pitch += deltaY * sensitivity;

    if(pitch > 90.0f){
        pitch = 89.9f;
    }else if(pitch < -90.0f){
        pitch = -89.9f;
    }

    camTarget = Vec4(distanceToTarget * std::cosf(Math::radians(pitch)) * std::sinf(Math::radians(yaw)), 
                     distanceToTarget * std::sinf(Math::radians(pitch)), 
                     distanceToTarget * std::cosf(Math::radians(pitch)) * std::cosf(Math::radians(yaw)),
                     0.0f);
    camTarget = camPosition + camTarget;

    forwardVec = camTarget - camPosition; 
    forwardVec.normalize();
    rightVec = Math::cross(forwardVec, worldUp); 
    rightVec.normalize();
    upVec = Math::cross(rightVec, forwardVec);
    upVec.normalize();

    cameraBuffer_t.camView = Math::CamLookAtLH(camPosition, camTarget, worldUp);
}

void Camera::moveCameraPos(IInput *input, float frame_time){
    Vec4 moveForwardVec = forwardVec;
    Vec4 moveRightVec = rightVec;

    float forwardValue = -static_cast<float>(input->getAxisValue(controllerAxis::LEFT_Y));
    moveForwardVec = moveForwardVec * forwardValue;
    float rightValue = -static_cast<float>(input->getAxisValue(controllerAxis::LEFT_X));
    moveRightVec = moveRightVec * rightValue;

    Vec4 moveVec = moveForwardVec + moveRightVec;
    camPosition = camPosition + (moveVec * frame_time * cameraSpeed);
    cameraBuffer_t.camView = Math::CamLookAtLH(camPosition, camTarget, camUp);
}

void Camera::testMovePosition(void){
    static float angle = 0.0f;
    angle += 0.01f;
    camPosition.x = distanceToTarget * std::cosf(angle) + camTarget.x;
    camPosition.z = distanceToTarget * std::sinf(angle) + camTarget.z;
    camPosition.print();

    cameraBuffer_t.camView = Math::CamLookAtLH(camPosition, camTarget, camUp);
}

const Vec4 Camera::getCamPosition(void){
    return this->camPosition;
}

void Camera::updateCamera(IRender *render, IInput *input, float frame_time){
    moveCameraTarget(input);
    moveCameraPos(input, frame_time);
    //testMovePosition();
    render->updateSubResource(cameraBuffer, &cameraBuffer_t);
    render->setConstantBuffer(&cameraBuffer, 1, 0);
}   

void Camera::cleanCamera(){
    if(cameraBuffer) cameraBuffer->Release();
}