#ifndef __IGRAPHICS_H__
#define __IGRAPHICS_H__

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <cstdio>
#include <cstdint>
#include <cwchar>
#include <stdexcept>

class Vec4;

class IGraphics{
public:
    virtual ~IGraphics(){}
    virtual void setRenderTarget(uint32_t numRTVS) = 0;
    virtual void setRenderTargetDepth(uint32_t numRTVS) = 0;
    virtual void setDefaultDepthStencilState() = 0;
    virtual void clearDepthStencilView(uint32_t clearFlags, float depthValue, uint8_t stencilValue) = 0;
    virtual void setBlendState(const Vec4& blendFactor, uint32_t sampleMask) = 0;
    virtual void clearRenderTargetView(const Vec4& color) = 0;
    virtual void present(uint8_t VSync) = 0;
};

class IResources{
public:
    virtual ~IResources(){}
    virtual bool compileShader(ID3D10Blob **blob, const wchar_t* filename, const char *function, const char *version) = 0;
    virtual bool createVertexShader(ID3D11VertexShader **vertexShader, ID3D10Blob *pVertexShader) = 0;
    virtual bool createPixelShader(ID3D11PixelShader **pixelShader, ID3D10Blob *pPixelShader) = 0;
    virtual bool createInputLayout(D3D11_INPUT_ELEMENT_DESC *layoutDesc, uint32_t layoutSize, ID3D10Blob *pVertexShader, ID3D11InputLayout **inputLayout) = 0;
    virtual bool createBuffer(D3D11_BUFFER_DESC *bufferDesc, D3D11_SUBRESOURCE_DATA *subResourceData, ID3D11Buffer **buffer) = 0;
    virtual bool createDepthStencilState(D3D11_DEPTH_STENCIL_DESC *stencilDesc, ID3D11DepthStencilState **depthStencilState) = 0;
};

class IRender{
public:
    virtual ~IRender(){}
    virtual void setInputLayout(ID3D11InputLayout *inputLayout) = 0;
    virtual void setVertexBuffer(ID3D11Buffer **vertexBuffer, uint32_t startBuffer, uint32_t numBuffers, uint32_t *strides, uint32_t *offset) = 0;
    virtual void setIndexBuffer(ID3D11Buffer *indexBuffer) = 0;
    virtual void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitive) = 0;
    virtual void setVertexShader(ID3D11VertexShader *vertexShader) = 0;
    virtual void setPixelShader(ID3D11PixelShader *pixelShader) = 0;
    virtual void drawIndexed(uint32_t numIndices, uint32_t firstIndex, uint32_t lastIndex) = 0;
    virtual void updateSubResource(ID3D11Resource *resource, const void *dataSource) = 0;
    virtual void setConstantBuffer(ID3D11Buffer **constantBuffer, uint32_t numBuffers, uint32_t startIndexBuffer) = 0;
    virtual void setDepthStencilState(uint32_t stencilValue, ID3D11DepthStencilState *depthStencilState) = 0;

};

#endif