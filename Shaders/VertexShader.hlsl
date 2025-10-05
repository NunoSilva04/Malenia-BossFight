cbuffer CameraMatrix : register(b0){
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
}

cbuffer ObjectId : register(b1){
    int id;
}

struct Transform{
    float4x4 mat;
};
StructuredBuffer<Transform> transforms : register(t0);


struct VS_Input{
    float4 inputPos : POSITION;
};

struct VS_Output{
    float4 outputPos : SV_Position;
};

VS_Output main(VS_Input input){
    VS_Output output;

    float4 tempPoint = mul(transforms[id].mat, input.inputPos);
    tempPoint = mul(viewMatrix, tempPoint);
    tempPoint = mul(projectionMatrix, tempPoint);

    output.outputPos = tempPoint;
    return output;
}