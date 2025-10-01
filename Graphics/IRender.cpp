#include "DirectX.h"
#include "Entity.hpp"
#include "Position.hpp"

void Graphics::setInputLayout(ID3D11InputLayout *inputLayout){
    devCon->IASetInputLayout(inputLayout);
}

void Graphics::setVertexBuffer(ID3D11Buffer **vertexBuffer, uint32_t startBuffer, uint32_t numBuffers, uint32_t *strides, uint32_t *offset){
    devCon->IASetVertexBuffers(startBuffer, numBuffers, vertexBuffer, strides, offset);
}

void Graphics::setIndexBuffer(ID3D11Buffer *indexBuffer){
    devCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Graphics::setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitive){
    devCon->IASetPrimitiveTopology(primitive);
}

void Graphics::setVertexShader(ID3D11VertexShader *vertexShader){
    devCon->VSSetShader(vertexShader, nullptr, 0);
}

void Graphics::setPixelShader(ID3D11PixelShader *pixelShader){
    devCon->PSSetShader(pixelShader, nullptr, 0);
}

void Graphics::drawIndexed(uint32_t numIndices, uint32_t firstIndex, uint32_t lastIndex){
    devCon->DrawIndexed(numIndices, firstIndex, lastIndex);
}

void Graphics::updateSubResource(ID3D11Resource *resource, const void *dataSource){
    devCon->UpdateSubresource(resource, 0, nullptr, dataSource, 0, 0);
}

void Graphics::setConstantBuffer(ID3D11Buffer **constantBuffer, uint32_t numBuffers, uint32_t startIndexBuffer){
    devCon->VSSetConstantBuffers(startIndexBuffer, numBuffers, constantBuffer);
}

void Graphics::setDepthStencilState(uint32_t stencilValue, ID3D11DepthStencilState *depthStencilState){
    devCon->OMSetDepthStencilState(depthStencilState, stencilValue);
}

void Graphics::createObjectDataArray(const int numObjects){
    objectData = new ObjectData_t[numObjects];
    if(objectData == nullptr){
        return;
    }
}

void Graphics::pushEntityDataIntoObjectDataArray(Position *position, const int id){
    objectData[id].transform = position->getTransformMat();
    objectData[id].id = id;
    numTransforms += 1;
}

void Graphics::sendObjectArrayGPU(void){
    HRESULT hr;

    D3D11_MAPPED_SUBRESOURCE mappedSubResource;
    hr = devCon->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
    if(SUCCEEDED(hr)){
        memcpy(mappedSubResource.pData, objectData, numTransforms * sizeof(ObjectData_t));
        devCon->Unmap(transformBuffer, 0);
    }else{
        MessageBoxA(nullptr, "Couldn't map transform buffer\n", "Error", MB_OK);
        return;
    }
}

void Graphics::renderEntity(Entity *){
    //TODO
}

void Graphics::cleanObjectDataArray(void){
    //TODO
}