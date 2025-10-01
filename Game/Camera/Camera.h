#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <d3d11.h>
#include "Math2.h"
#include "Matrix.h"
#include "Vector.h"

class IInput;
class IResources;
class IRender;
struct windowPreferances;

class ICamera{
public:
    virtual ~ICamera(void){}
    virtual const Vec4 getCamPosition(void) = 0;
};

class Camera : public ICamera{
public:
    Camera();
    ~Camera();
    bool initCamera(IResources *resources, const windowPreferances window);
    void updateCamera(IRender *render, IInput *input, float frame_time);
    void cleanCamera(void);

private:
    void moveCameraPos(IInput *input, float frame_time);
    void moveCameraTarget(IInput *input);
    void testMovePosition(void);
    const Vec4 getCamPosition(void) override;

    ID3D11Buffer *cameraBuffer;
    Vec4 camPosition, camTarget, camUp;
    float distanceToTarget;
    float nearPlane, farPlane, FOV, aspectRatio;
    const Vec4 worldUp = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
    Vec4 forwardVec, rightVec, upVec;
    float cameraSpeed, yaw, pitch, sensitivity;
    
    struct{
        Mat4x4 camView; 
        Mat4x4 camProjection;
    } cameraBuffer_t;
};

#endif