#include "DirectX.h"
#include "Vector.h"

void Graphics::setRenderTarget(uint32_t numRTVS){
    devCon->OMSetRenderTargets(numRTVS, &renderTargetView, nullptr);
}

void Graphics::setRenderTargetDepth(uint32_t numRTVS){
    devCon->OMSetRenderTargets(numRTVS, &renderTargetView, depthStencilView);
}

void Graphics::setDefaultDepthStencilState(){
    devCon->OMSetDepthStencilState(depthStencilState, 0);
}

void Graphics::clearDepthStencilView(uint32_t clearFlags, float depthValue, uint8_t stencilValue){
    devCon->ClearDepthStencilView(depthStencilView, clearFlags, depthValue, stencilValue);
}

void Graphics::setBlendState(const Vec4& blendFactor, uint32_t sampleMask){
    devCon->OMSetBlendState(blendState, blendFactor.data(), sampleMask);
}

void Graphics::clearRenderTargetView(const Vec4& color){
    devCon->ClearRenderTargetView(renderTargetView, color.data());
}

void Graphics::present(uint8_t VSync){
    swapChain->Present(VSync, 0);
}