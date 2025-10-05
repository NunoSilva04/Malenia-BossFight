struct PS_Input{
    float4 inputPosition : SV_Position;
};

float4 main(PS_Input input) : SV_Target{
    return float4(0.0f, 1.0f, 0.0f, 1.0f);
}