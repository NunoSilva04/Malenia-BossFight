#include "DirectX.h"

bool Graphics::compileShader(ID3D10Blob **blob, const wchar_t* filename, const char *function, const char *version){
    HRESULT hr;

    hr = D3DCompileFromFile(filename, 0, 0, function, version, 0, 0, blob, nullptr);
    if(FAILED(hr)){
        return false;
    }
    return true;
}

bool Graphics::createVertexShader(ID3D11VertexShader **vertexShader, ID3D10Blob *pVertexShader){
    HRESULT hr;

    hr = dev->CreateVertexShader(pVertexShader->GetBufferPointer(), pVertexShader->GetBufferSize(), nullptr, vertexShader);
    if(FAILED(hr)){
        return false;
    }
    return true;
}

bool Graphics::createPixelShader(ID3D11PixelShader **pixelShader, ID3D10Blob *pPixelShader){
    HRESULT hr;

    hr = dev->CreatePixelShader(pPixelShader->GetBufferPointer(), pPixelShader->GetBufferSize(), nullptr, pixelShader);
    if(FAILED(hr)){
        return false;
    }
    return true;
}

bool Graphics::createInputLayout(D3D11_INPUT_ELEMENT_DESC *layoutDesc, uint32_t layoutSize, ID3D10Blob *pVertexShader, ID3D11InputLayout **inputLayout){
    HRESULT hr;

    hr = dev->CreateInputLayout(layoutDesc, layoutSize, pVertexShader->GetBufferPointer(), pVertexShader->GetBufferSize(), inputLayout);
    if(FAILED(hr)){
        return false;
    }
    return true;
}

bool Graphics::createBuffer(D3D11_BUFFER_DESC *bufferDesc, D3D11_SUBRESOURCE_DATA *subResourceData, ID3D11Buffer **buffer){
    HRESULT hr;

    hr = dev->CreateBuffer(bufferDesc, subResourceData, buffer);
    if(FAILED(hr)){
        return false;
    }
    return true;
}

bool Graphics::createDepthStencilState(D3D11_DEPTH_STENCIL_DESC *stencilDesc, ID3D11DepthStencilState **depthStencilState){
    HRESULT hr;
    
    hr = dev->CreateDepthStencilState(stencilDesc, depthStencilState);
    if(FAILED(hr)){
        return false;
    }
    return true;
}