#ifndef __DIRECTX_H__
#define __DIRECTX_H__

#include "IGraphics.h"
#include "Math2.h"

struct windowPreferances;

class Graphics : public IGraphics, public IResources, public IRender{
//DirectX 11 initialization
public:
    Graphics();
    ~Graphics();
    bool initDirectX(HWND hWnd, HMONITOR hMonitor, const windowPreferances& window);
    void cleanDirectX();

private:
    bool initDevAndDevCon(HMONITOR hMonitor);
    bool initSwapChain(HWND hWnd, const windowPreferances& window);
    bool initDepthStencilView(const windowPreferances& window);
    bool initDepthStencilState(void);
    bool initRenderTargetView(const windowPreferances& window);
    bool initBlendState(void);
    bool initRasterizerState(void);
    bool initDefaultSamplerState(void);
    bool initTransformSRV(void);
    ID3D11Device *dev;
    ID3D11DeviceContext *devCon;
    IDXGISwapChain1 *swapChain;
    ID3D11RenderTargetView *renderTargetView;
    IDXGIOutput *output;
    ID3D11DepthStencilState *depthStencilState;
    ID3D11DepthStencilView *depthStencilView;
    ID3D11BlendState *blendState;
    ID3D11RasterizerState *rasterizerState;
    ID3D11SamplerState *defaultSamplerState;

    ID3D11ShaderResourceView *transformSRV;
    ID3D11Buffer *transformBuffer;
    struct ObjectData_t{
        Mat4x4 transform;
        int id;
    };
    ObjectData_t *objectData;
    static constexpr int maxObjects = 100;
    int numTransforms;

//IGraphics
public:
    void setRenderTarget(uint32_t numRTVS) override;
    void setRenderTargetDepth(uint32_t numRTVS) override;
    void setDefaultDepthStencilState() override;
    void clearDepthStencilView(uint32_t clearFlags, float depthValue, uint8_t stencilValue) override;
    void setBlendState(const Vec4& blendFactor, uint32_t sampleMask) override;
    void clearRenderTargetView(const Vec4& color) override;
    void present(uint8_t VSync) override;

//IResources
private:
    bool compileShader(ID3D10Blob **blob, const wchar_t* filename, const char *function, const char *version) override;
    bool createVertexShader(ID3D11VertexShader **vertexShader, ID3D10Blob *pVertexShader) override;
    bool createPixelShader(ID3D11PixelShader **pixelShader, ID3D10Blob *pPixelShader) override;
    bool createInputLayout(D3D11_INPUT_ELEMENT_DESC *layoutDesc, uint32_t layoutSize, ID3D10Blob *pVertexShader, ID3D11InputLayout **inputLayout) override;
    bool createBuffer(D3D11_BUFFER_DESC *bufferDesc, D3D11_SUBRESOURCE_DATA *subResourceData, ID3D11Buffer **buffer) override;
    bool createDepthStencilState(D3D11_DEPTH_STENCIL_DESC *stencilDesc, ID3D11DepthStencilState **depthStencilState) override;

//IRender
private:
    void setInputLayout(ID3D11InputLayout *inputLayout) override;
    void setVertexBuffer(ID3D11Buffer **vertexBuffer, uint32_t startBuffer, uint32_t numBuffers, uint32_t *strides, uint32_t *offset) override;
    void setIndexBuffer(ID3D11Buffer *indexBuffer) override;
    void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitive) override;
    void setVertexShader(ID3D11VertexShader *vertexShader) override;
    void setPixelShader(ID3D11PixelShader *pixelShader) override;
    void drawIndexed(uint32_t numIndices, uint32_t firstIndex, uint32_t lastIndex) override;
    void updateSubResource(ID3D11Resource *resource, const void *dataSource) override;
    void setConstantBuffer(ID3D11Buffer **constantBuffer, uint32_t numBuffers, uint32_t startIndexBuffer) override;
    void setDepthStencilState(uint32_t stencilValue, ID3D11DepthStencilState *depthStencilState) override;
    void createObjectDataArray(const int) override;
    void pushEntityDataIntoObjectDataArray(Position *, const int) override;
    void sendObjectArrayGPU(void) override;
    void renderEntity(Entity *) override;
    void cleanObjectDataArray(void) override;
};

#endif