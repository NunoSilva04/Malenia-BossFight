#include "Mesh.hpp"
#include "IGraphics.h"

using myManager = fbxsdk::FbxManager;
using myIOSettings = fbxsdk::FbxIOSettings;
using myImporter = fbxsdk::FbxImporter;
using myScene = fbxsdk::FbxScene;

Mesh::Mesh(){
    sceneData.numMeshes = 0;
    sceneData.numVertices = 0;
    sceneData.numIndices = 0;
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

    scene = myScene::Create(manager, "MyScene");
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

    //Get the total scene vertices and indices
    int childCount = scene->GetRootNode()->GetChildCount();
    std::printf("Number of nodes in scene = %d\n", childCount);
    for(int i = 0; i < childCount; i++){
        fbxsdk::FbxNode *node = scene->GetRootNode()->GetChild(i);
        fbxsdk::FbxMesh *mesh = node->GetMesh();
        if(mesh != nullptr){
            sceneData.numMeshes += 1;
            sceneData.numVertices += mesh->GetControlPointsCount();
            sceneData.numIndices += (mesh->GetPolygonCount() * 3);      //3 because each polygon is a triangle
        }
    }
    sceneData.vertices = new Vec4[sceneData.numVertices];
    sceneData.indices = new uint32_t[sceneData.numIndices];
    sceneData.originPoints = new Vec4[sceneData.numMeshes];

    if((sceneData.vertices == nullptr) || (sceneData.indices == nullptr) || (sceneData.originPoints == nullptr)){
        printf("Couldn't allocate memory for scene data\n");
        manager->Destroy();
        return false;
    }

    int currVertice = 0, currIndice = 0, currMesh = 0, meshOffset = 0;
    for(int i = 0; i < childCount; i++){
        fbxsdk::FbxNode *node = scene->GetRootNode()->GetChild(i);
        fbxsdk::FbxMesh *mesh = node->GetMesh();
        if(mesh != nullptr){
            int meshNumVertices = mesh->GetControlPointsCount();
            for(int v = 0; v < meshNumVertices; v++, currVertice++){
                fbxsdk::FbxVector4 temp = mesh->GetControlPointAt(v);
                sceneData.vertices[currVertice] = Vec4(static_cast<float>(temp.mData[0]), 
                                                       static_cast<float>(temp.mData[1]), 
                                                       static_cast<float>(temp.mData[2]), 
                                                       1.0f);
            }

            int numPolygons = mesh->GetPolygonCount();  
            for(int currPolygon = 0; currPolygon < numPolygons; currPolygon++){
                int numVertInPolygon = mesh->GetPolygonSize(currPolygon);
                for(int currVertInPolygon = 0; currVertInPolygon < numVertInPolygon; currVertInPolygon++, currIndice++){
                    int controlPointIndex = mesh->GetPolygonVertex(currPolygon, currVertInPolygon) + meshOffset;
                    sceneData.indices[currIndice] = controlPointIndex;
                }
            }

            sceneData.originPoints[currMesh] = Vec4(static_cast<float>(node->LclTranslation.Get().mData[0] / 100.0f), 
                                                    static_cast<float>(node->LclTranslation.Get().mData[1] / 100.0f), 
                                                    static_cast<float>(node->LclTranslation.Get().mData[2] / 100.0f),
                                                    1.0f);
            currMesh++;
            meshOffset += meshNumVertices;
        }
    }

    for(int i = 0; i < sceneData.numVertices; i++){
        sceneData.vertices[i].print();
    }
    //for(int i = 0; i < sceneData.numIndices; i++){
    //    printf("Indice %d = %d\n", i, sceneData.indices[i]);
    //}
    //for(int i = 0; i < sceneData.numMeshes; i++){
    //    sceneData.originPoints[i].print();
    //}

    if(!createMeshResources(resources)){
        std::printf("Couldn't create mesh resources\n");
        return false;
    }

    return true;           
}

bool Mesh::createMeshResources(IResources *resources){
    ID3D10Blob *pVertexShader = nullptr;
    //This path is relative to the .exe not to the actual .cpp file
    if(!resources->compileShader(&pVertexShader, L"../../Shaders/VertexShader.hlsl", "main", "vs_5_0")){
        std::printf("Couldn't compile red cube vertex shader\n");
        return false;
    }

    if(!resources->createVertexShader(&vertexShader, pVertexShader)){
        std::printf("Couldn't create red cube vertex shader\n");
        return false;
    }

    ID3D10Blob *pPixelShader = nullptr;
    if(!resources->compileShader(&pPixelShader, L"../../Shaders/PixelShader.hlsl", "main", "ps_5_0")){
        std::printf("Couldn't compile red cube pixel shader\n");
        if(pVertexShader) pVertexShader->Release();
        return false;
    }

    if(!resources->createPixelShader(&pixelShader, pPixelShader)){
        std::printf("Couldn't create red cube vertex shader\n");
        if(pVertexShader) pVertexShader->Release();
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
    vertexBufferDesc.ByteWidth = sceneData.numVertices * sizeof(sceneData.vertices[0]);
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA vertexBufferSubData;
    ZeroMemory(&vertexBufferSubData, sizeof(D3D11_SUBRESOURCE_DATA));
    vertexBufferSubData.pSysMem = sceneData.vertices;

    if(!resources->createBuffer(&vertexBufferDesc, &vertexBufferSubData, &vertexBuffer)){
        std::printf("Couldn't create red cube vertex buffer\n");
        if(pVertexShader) pVertexShader->Release();
        if(pPixelShader) pPixelShader->Release();
        if(inputLayout) inputLayout->Release();
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = sceneData.numIndices * sizeof(sceneData.indices[0]);
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA indexBufferSubData;
    ZeroMemory(&indexBufferSubData, sizeof(D3D11_SUBRESOURCE_DATA));
    indexBufferSubData.pSysMem = sceneData.indices;

    if(!resources->createBuffer(&indexBufferDesc, &indexBufferSubData, &indexBuffer)){
        std::printf("Couldn't create red cube index buffer\n");
        if(pVertexShader) pVertexShader->Release();
        if(pPixelShader) pPixelShader->Release();
        if(inputLayout) inputLayout->Release();
        if(vertexBuffer != nullptr) vertexBuffer->Release();
        return false;
    }

    return true;
}

const Vec4 Mesh::getOriginPoint(void){
    return sceneData.originPoints[0];
}

ID3D11VertexShader *Mesh::getVertexShader(void){
    return vertexShader;
}

ID3D11PixelShader *Mesh::getPixelShader(void){
    return pixelShader;
}   

ID3D11InputLayout *Mesh::getInputLayout(void){
    return inputLayout;
}

ID3D11Buffer *Mesh::getVertexBuffer(void){
    return vertexBuffer;
}

ID3D11Buffer *Mesh::getIndexBuffer(void){
    return indexBuffer;
}

uint32_t Mesh::getVertexBufferStrides(void){
    return sizeof(sceneData.vertices[0]);
}

uint32_t Mesh::getVertexBufferOffset(void){
    return 0;
}

int Mesh::getNumVertices(void){
    return sceneData.numVertices;
}

int Mesh::getNumIndices(void){
    return sceneData.numIndices;
}

void Mesh::destroyMesh(){
    if(sceneData.vertices != nullptr) delete[] sceneData.vertices;
    if(sceneData.indices != nullptr) delete[] sceneData.indices;
    if(sceneData.originPoints != nullptr) delete[] sceneData.originPoints;
    if(vertexShader != nullptr) vertexShader->Release();
    if(pixelShader != nullptr) pixelShader->Release();
    if(inputLayout != nullptr) inputLayout->Release();
    if(vertexBuffer != nullptr) vertexBuffer->Release();
    if(indexBuffer != nullptr) indexBuffer->Release();
    scene->Destroy();
    manager->Destroy();
}