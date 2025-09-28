#include "DirectX.h"
#include "Usage.h"
#include "Vector.h"
#include "Windows_OS.h"
#include <DirectXMath.h>


Graphics::Graphics(){
    dev = nullptr;
    devCon = nullptr;
    swapChain = nullptr;
    renderTargetView = nullptr;
    output = nullptr;
    depthStencilState = nullptr;
    depthStencilView = nullptr;
    blendState = nullptr;
    rasterizerState = nullptr;
    defaultSamplerState = nullptr;
}

Graphics::~Graphics(){
    
}


bool Graphics::initDirectX(HWND hWnd, HMONITOR hMonitor, const windowPreferances& window){
    if(!initDevAndDevCon(hMonitor)){
        std::printf("Couldn't initialize device and devCon\n");
        return false;
    }
    if(!initSwapChain(hWnd, window)){
        std::printf("Couldn't initialize swap chain and render target view\n");
        return false;
    }
    if(!initRenderTargetView(window)){
        std::printf("Couldn't initialize render target view\n");
        return false;
    }
    if(!initBlendState()){
        std::printf("Couldn't initialize blend state\n");
        return false;
    }
    if(!initRasterizerState()){
        std::printf("Couldn't initialize rasterizer state\n");
        return false;
    }
    //if(!initDefaultSamplerState()){
    //    std::printf("Couldn't initialize default sampler state\n");
    //    return false;
    //}

    return true;
}

bool Graphics::initDevAndDevCon(HMONITOR hMonitor){
    HRESULT hr;

    IDXGIFactory2 *factory = nullptr;
    hr = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)&factory);
    if(FAILED(hr)){
        std::printf("Couldn't create dxgi factory\n");
        return false;
    }

    uint8_t i = 0;
    IDXGIAdapter1 *tempAdapter = nullptr, *adapter = nullptr;
    uint64_t mostMemory = 0;
    while(factory->EnumAdapters1(i, &tempAdapter) != DXGI_ERROR_NOT_FOUND){
        DXGI_ADAPTER_DESC1 desc;
        tempAdapter->GetDesc1(&desc);
        uint64_t currMemory = desc.DedicatedVideoMemory;

        std::printf("Adapter %u:\n", i);
        std::wprintf(L"\tDesc: %ls\n"
                     L"\tDevice ID: %u\n"
                     L"\tSubSys ID: %u\n"
                     L"\tDedicated Video Memory: %llu MB\n"
                     L"\tDedicated System Memory: %llu MB\n"
                     L"\tShader System Memory: %llu MB\n"
                     L"\tFlags: %u\n", 
                    desc.Description, desc.DeviceId, desc.SubSysId, desc.DedicatedVideoMemory / (1024 * 1024), 
                    desc.DedicatedSystemMemory /(1024 * 1024), desc.DedicatedSystemMemory /(1024 * 1024), desc.Flags);

        if(currMemory > mostMemory){
            mostMemory = currMemory;
            adapter = tempAdapter;
        }

        IDXGIOutput *tempOutput;
        uint32_t j = 0;
        while(tempAdapter->EnumOutputs(j, &tempOutput) != DXGI_ERROR_NOT_FOUND){
            DXGI_OUTPUT_DESC tempOutputDesc;
            tempOutput->GetDesc(&tempOutputDesc);
            if(hMonitor == tempOutputDesc.Monitor){
                std::wprintf(L"Monitor chosen: %ls\n", tempOutputDesc.DeviceName);
                output = tempOutput;
                output->AddRef();
            }
            j++;
        }

        i++;
    }

    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);
    std::wprintf(L"Adapter Chosen: %ls\n", desc.Description);

    D3D_FEATURE_LEVEL featureLevel[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };
    UINT size = ARRAYSIZE(featureLevel);

    hr = D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevel, size, D3D11_SDK_VERSION, 
                           &dev, nullptr, &devCon);
    if(FAILED(hr)){
        std::printf("Couldn't create d3d11 device\n");
        if(adapter) adapter->Release();
        if(tempAdapter) tempAdapter->Release();
        if(factory) factory->Release();
        return false;
    }

    if(adapter) adapter->Release();
    if(tempAdapter) tempAdapter->Release();
    if(factory) factory->Release();

    return true;
}

bool Graphics::initSwapChain(HWND hWnd, const windowPreferances& window){
    HRESULT hr;

    IDXGIFactory2 *factory = nullptr;
    hr = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)&factory);
    if(FAILED(hr)){
        std::printf("Couldn't create factory\n");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
    swapChainDesc.Width = window.width;
    swapChainDesc.Height = window.height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
    ZeroMemory(&fullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
    fullScreenDesc.RefreshRate.Numerator = window.frequency;
    fullScreenDesc.RefreshRate.Denominator = 1;
    fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    fullScreenDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
    fullScreenDesc.Windowed = true;

    hr = factory->CreateSwapChainForHwnd(dev, hWnd, &swapChainDesc, &fullScreenDesc, nullptr, &swapChain);
    if(FAILED(hr)){
        std::printf("Couldn't create swap chain\n");
        if(factory) factory->Release();
        return false;
    }
    if(factory) factory->Release();

    return true;
}

bool Graphics::initDepthStencilView(const windowPreferances& window){
    HRESULT hr;

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
    textureDesc.Width = window.width;
    textureDesc.Height = window.height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    ID3D11Texture2D *texture = nullptr;
    hr = dev->CreateTexture2D(&textureDesc, nullptr, &texture);
    if(FAILED(hr)){
        std::printf("Couldn't create texture 2D for depth stencil view\n");
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    depthStencilViewDesc.Format = textureDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Flags = 0;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    hr = dev->CreateDepthStencilView(texture, &depthStencilViewDesc, &depthStencilView);
    if(FAILED(hr)){
        std::printf("Couldn't create depth stencil view\n");
        if(texture) texture->Release();
        return false;
    }

    return true;
}

bool Graphics::initDepthStencilState(){
    HRESULT hr;

    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilStateDesc.DepthEnable = true;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    depthStencilStateDesc.StencilEnable = true;
    depthStencilStateDesc.StencilReadMask = 255;
    depthStencilStateDesc.StencilWriteMask = 255;

    depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depthStencilStateDesc.BackFace = depthStencilStateDesc.FrontFace;


    hr = dev->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
    if(FAILED(hr)){
        std::printf("Couldn't create depth stencil state\n");
        return false;
    }
    devCon->OMSetDepthStencilState(depthStencilState, 0);

    return true;
}

bool Graphics::initRenderTargetView(const windowPreferances& window){
    HRESULT hr;

    ID3D11Texture2D *backBuffer = nullptr;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if(FAILED(hr)){
        std::printf("Couldn't get backbuffer\n");
        return false;
    }

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    hr = dev->CreateRenderTargetView(backBuffer, &renderTargetViewDesc, &renderTargetView);
    if(FAILED(hr)){
        std::printf("Couldn't create render target view\n");
        if(backBuffer) backBuffer->Release();
        return false;
    }
    if(backBuffer) backBuffer->Release();

    if(!initDepthStencilView(window)){
        std::printf("Couldn't initialize depth stencil view\n");
        return false;
    }

    if(!initDepthStencilState()){
        std::printf("Couldn't initialize depth stencil state\n");
        return false;
    }
    
    devCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    D3D11_VIEWPORT viewPortDesc;
    viewPortDesc.TopLeftX = 0.0f;
    viewPortDesc.TopLeftY = 0.0f;
    viewPortDesc.Width = static_cast<float>(window.width);
    viewPortDesc.Height = static_cast<float>(window.height);
    viewPortDesc.MinDepth = 0.0f;
    viewPortDesc.MaxDepth = 1.0f;
    devCon->RSSetViewports(1, &viewPortDesc);

    return true;
}

bool Graphics::initBlendState(){
    HRESULT hr;

    D3D11_BLEND_DESC blendStateDesc;
    ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

    blendStateDesc.AlphaToCoverageEnable = false;
    blendStateDesc.IndependentBlendEnable = false;
    blendStateDesc.RenderTarget[0].BlendEnable = true;
    blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = dev->CreateBlendState(&blendStateDesc, &blendState);
    if(FAILED(hr)){
        std::printf("Couldn't create blend state\n");
        return false;
    }
    return true;
}

bool Graphics::initRasterizerState(){
    HRESULT hr;

    D3D11_RASTERIZER_DESC rasterizerStateDesc;
    ZeroMemory(&rasterizerStateDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
    rasterizerStateDesc.FrontCounterClockwise = true;
    rasterizerStateDesc.DepthBias = 0;
    rasterizerStateDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerStateDesc.DepthBiasClamp = 0.0f;
    rasterizerStateDesc.DepthClipEnable = true;
    rasterizerStateDesc.ScissorEnable = false;
    rasterizerStateDesc.MultisampleEnable = false;
    rasterizerStateDesc.AntialiasedLineEnable = false;

    hr = dev->CreateRasterizerState(&rasterizerStateDesc, &rasterizerState);
    if(FAILED(hr)){
        std::printf("Couldn't create rasterizer state\n");
        return false;
    }
    devCon->RSSetState(rasterizerState);
    return true;
}

bool Graphics::initDefaultSamplerState(){
    HRESULT hr;
    
    D3D11_SAMPLER_DESC defaultSamplerStateDesc;
    ZeroMemory(&defaultSamplerStateDesc, sizeof(D3D11_SAMPLER_DESC));
    defaultSamplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    defaultSamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    defaultSamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    defaultSamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    defaultSamplerStateDesc.MipLODBias = 0.0f;
    defaultSamplerStateDesc.MaxAnisotropy = 1;
    defaultSamplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    defaultSamplerStateDesc.BorderColor[0] = 1.0f;
    defaultSamplerStateDesc.BorderColor[1] = 1.0f;
    defaultSamplerStateDesc.BorderColor[2] = 1.0f;
    defaultSamplerStateDesc.BorderColor[3] = 1.0f;
    defaultSamplerStateDesc.MinLOD = 0.0f;
    defaultSamplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = dev->CreateSamplerState(&defaultSamplerStateDesc, &defaultSamplerState);
    if(FAILED(hr)){
        std::printf("Couldn't create default sampler state\n");
        return false;
    }

    devCon->PSSetSamplers(0, 1, &defaultSamplerState);
    return true;
}

void Graphics::cleanDirectX(){
    if(dev) dev->Release();
    if(devCon) devCon->Release();
    if(swapChain) swapChain->Release();
    if(renderTargetView) renderTargetView->Release();
    if(output) output->Release();
    if(depthStencilState) depthStencilState->Release();
    if(depthStencilView) depthStencilView->Release();
    if(blendState) blendState->Release();
    if(rasterizerState) rasterizerState->Release();
    if(defaultSamplerState) defaultSamplerState->Release();
}