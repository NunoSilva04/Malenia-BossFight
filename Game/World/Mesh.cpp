#include "Mesh.hpp"
#include "IGraphics.h"

using myManager = fbxsdk::FbxManager;
using myIOSettings = fbxsdk::FbxIOSettings;
using myImporter = fbxsdk::FbxImporter;
using myScene = fbxsdk::FbxScene;

Mesh::Mesh(){

}

Mesh::~Mesh(){

}

bool Mesh::loadMesh(IResources *resources, std::string fileName){
    manager = myManager::Create();
    if(manager == nullptr){
        std::printf("Couldn't create fbx manager\n");
        return false;
    }

    myIOSettings *ios = myIOSettings::Create(manager, IOSROOT);
    if(ios == nullptr){
        manager->Destroy();
        std::printf("Couldn't create fbx ios settings\n");
        return false;
    }
    manager->SetIOSettings(ios);

    myImporter *importer = myImporter::Create(manager, "");
    if(importer == nullptr){
        manager->Destroy();
        std::printf("Couldn't create fbx importer\n");
        return false;
    }

    if(!importer->Initialize(fileName.c_str(), -1, manager->GetIOSettings())){
        std::printf("couldn't initialize with filename\n");
        importer->Destroy();
        manager->Destroy();
        return false;
    }

    myScene *scene = myScene::Create(manager, "MyScene");
    if(scene == nullptr){
        std::printf("Couldn't create my scene");
        importer->Destroy();
        manager->Destroy();
        return false;
    }
    if(!importer->Import(scene)){
        std::printf("Couldn't import my scene\n");
        importer->Destroy();
        manager->Destroy();
        return false;
    }
    importer->Destroy();

    std::printf("Number of nodes in scene = %d\n", scene->GetRootNode()->GetChildCount());

    int childCount = scene->GetRootNode()->GetChildCount();
    for(int i = 0; i < childCount; i++){
        fbxsdk::FbxNode *node = scene->GetRootNode()->GetChild(i);
        fbxsdk::FbxMesh *mesh = node->GetMesh();
        
        if(mesh != nullptr){
            std::printf("Mesh name %s\n", mesh->GetName());
            numVertices = mesh->GetControlPointsCount();
            std::printf("Number of vertices: %d\n", numVertices);

            vertices = new Vertices_t[numVertices];
            if(vertices == nullptr){
                std::printf("Couldn't allocate memory for num of vertices\n");
                manager->Destroy();
                return false;
            }
            for(int currVertice = 0; currVertice < numVertices; currVertice++){
                fbxsdk::FbxVector4 temp = mesh->GetControlPointAt(currVertice);
                vertices[currVertice].vertice = Vec4(static_cast<float>(temp.mData[0]), static_cast<float>(temp.mData[1]), static_cast<float>(temp.mData[2]), 1.0f);
                std::printf("Point %d: ", currVertice);
                vertices[currVertice].vertice.print();
            }

            int numPolygons = mesh->GetPolygonCount();
            numIndices = numPolygons * 3;           //3 because each polygon is a triangle
            indices = new Indices_t[numIndices];   
            int currIndex = 0;
            std::printf("Number of polygons: %d\n", numPolygons);
            for(int currPolygon = 0; currPolygon < numPolygons; currPolygon++){
                int numVertInPolygon = mesh->GetPolygonSize(currPolygon);
                std::printf("Polygon %d has %d number of vertices\n", currPolygon, numVertInPolygon);
                for(int currVertInPolygon = 0; currVertInPolygon < numVertInPolygon; currVertInPolygon++){
                    int controlPointIndex = mesh->GetPolygonVertex(currPolygon, currVertInPolygon);
                    std::printf("CurrPolygon: %d, CurrVertInPolygon: %d, ControlPointindex: %d\n", currPolygon, currVertInPolygon, controlPointIndex);
                    indices[currIndex++].index = controlPointIndex;
                }
            }

        }else{
            std::printf("Child number %d has no mesh\n", i);
        }
    }

    ID3D10Blob *pVertexShader = nullptr;
    //This path is relative to the .exe not to the actual .cpp file
    if(!resources->compileShader(&pVertexShader, L"../../Models/VertexShader.hlsl", "main", "vs_5_0")){
        std::printf("Couldn't compile red cube vertex shader\n");
        MessageBoxA(nullptr, "Couldn't compile red cube vertex shader", "Error", MB_OK);
        if(pVertexShader) pVertexShader->Release();
        return false;
    }

    if(!resources->createVertexShader(&vertexShader, pVertexShader)){
        std::printf("Couldn't create red cube vertex shader\n");
        if(pVertexShader) pVertexShader->Release();
        return false;
    }

    ID3D10Blob *pPixelShader = nullptr;
    if(!resources->compileShader(&pPixelShader, L"../../Models/PixelShader.hlsl", "main", "ps_5_0")){
        std::printf("Couldn't compile red cube pixel shader\n");
        if(pVertexShader) pVertexShader->Release();
        if(pPixelShader) pPixelShader->Release();
        return false;
    }

    if(!resources->createPixelShader(&pixelShader, pPixelShader)){
        std::printf("Couldn't create red cube vertex shader\n");
        if(pVertexShader) pVertexShader->Release();
        if(pPixelShader) pPixelShader->Release();
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    uint32_t size = ARRAYSIZE(inputLayoutDesc);
    if(!resources->createInputLayout(inputLayoutDesc, size, pVertexShader, &inputLayout)){
        std::printf("Couldn't create redcube inputlayout\n");
        if(pVertexShader) pVertexShader->Release();
        if(pPixelShader) pPixelShader->Release();
        return false;
    }

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.ByteWidth = numVertices * sizeof(vertices[0].vertice);
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA vertexBufferSubData;
    ZeroMemory(&vertexBufferSubData, sizeof(D3D11_SUBRESOURCE_DATA));
    vertexBufferSubData.pSysMem = &vertices[0];

    if(!resources->createBuffer(&vertexBufferDesc, &vertexBufferSubData, &vertexBuffer)){
        std::printf("Couldn't create red cube vertex buffer\n");
        if(pVertexShader) pVertexShader->Release();
        if(pPixelShader) pPixelShader->Release();
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = numIndices * sizeof(Indices_t);
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA indexBufferSubData;
    ZeroMemory(&indexBufferSubData, sizeof(D3D11_SUBRESOURCE_DATA));
    indexBufferSubData.pSysMem = &indices[0];

    if(!resources->createBuffer(&indexBufferDesc, &indexBufferSubData, &indexBuffer)){
        std::printf("Couldn't create red cube index buffer\n");
        if(pVertexShader) pVertexShader->Release();
        if(pPixelShader) pPixelShader->Release();
        return false;
    }

    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(Mat4x4);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    if(!resources->createBuffer(&constantBufferDesc, nullptr, &constantBuffer)){
        std::printf("Couldn't create constant buffer\n");
        return false;
    }

    return true;
}

void Mesh::renderMesh(IRender *render){
    render->updateSubResource(constantBuffer, &world);
    render->setConstantBuffer(&constantBuffer, 1, 1);

    render->setInputLayout(inputLayout);
    uint32_t strides = {sizeof(Vertices_t)};
    uint32_t offset = {0};
    render->setVertexBuffer(&vertexBuffer, 0, 1, &strides, &offset);
    render->setIndexBuffer(indexBuffer);
    render->setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    render->setVertexShader(vertexShader);
    render->setPixelShader(pixelShader);

    render->drawIndexed(numIndices, 0, 0);
}

void Mesh::destroyMesh(){
    if(vertices != nullptr) delete[] vertices;
    if(indices != nullptr) delete[] indices;
    if(vertexShader != nullptr) vertexShader->Release();
    if(pixelShader != nullptr) pixelShader->Release();
    if(inputLayout != nullptr) inputLayout->Release();
    if(vertexBuffer != nullptr) vertexBuffer->Release();
    if(indexBuffer != nullptr) indexBuffer->Release();
    if(constantBuffer != nullptr) constantBuffer->Release();
    manager->Destroy();
}