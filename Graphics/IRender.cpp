#include "DirectX.h"
#include "Mesh.hpp"
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

void Graphics::createTransformDataArray(const int numObjects){
    transformData = new TransformData_t[numObjects];
    if(transformData == nullptr){
        return;
    }
}

void Graphics::pushEntityDataIntoTransformDataArray(Position *position, const int id){
    transformData[id].transform = position->getTransformMat();
    numTransforms += 1;   
}

void Graphics::sendTransformArrayGPU(void){
    HRESULT hr;

    D3D11_MAPPED_SUBRESOURCE mappedSubResource;
    hr = devCon->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
    if(SUCCEEDED(hr)){
        memcpy(mappedSubResource.pData, transformData, maxObjects * sizeof(TransformData_t));
        devCon->Unmap(transformBuffer, 0);
    }else{
        MessageBoxA(nullptr, "Couldn't map transform buffer\n", "Error", MB_OK);
        return;
    }
    devCon->VSSetShaderResources(TRANSFORMSRV_SLOT, 1, &transformSRV);
}

void Graphics::renderEntity(Mesh *mesh, const int id){
    objectId = id;
    devCon->UpdateSubresource(objectIdBuffer, 0, nullptr, &objectId, 0, 0);
    devCon->VSSetConstantBuffers(OBJECTID_CBSLOT, 1, &objectIdBuffer);

    devCon->IASetInputLayout(mesh->getInputLayout());
    ID3D11Buffer *vertexBuffer = mesh->getVertexBuffer();
    uint32_t strides = {mesh->getVertexBufferStrides()};
    uint32_t offset = {mesh->getVertexBufferOffset()};
    devCon->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offset);
    devCon->IASetIndexBuffer(mesh->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
    devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    devCon->VSSetShader(mesh->getVertexShader(), 0, 0);
    devCon->PSSetShader(mesh->getPixelShader(), 0, 0);

    devCon->DrawIndexed(mesh->getNumIndices(), 0, 0);
}

void Graphics::cleanTransformDataArray(void){
    if(transformData == nullptr) delete[] transformData;
    numTransforms = 0;
}