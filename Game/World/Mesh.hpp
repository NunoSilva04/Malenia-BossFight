#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <fbxsdk.h>
#include <string>
#include <d3d11.h>
#include <cstdint>
#include "Vector.h"
#include "Matrix.h"

class IResources;

class Mesh{
public:
    Mesh();
    ~Mesh();
    bool loadMesh(IResources *, std::string);
    const Vec4 getOriginPoint(void);
    ID3D11VertexShader *getVertexShader(void);
    ID3D11PixelShader *getPixelShader(void);    
    ID3D11InputLayout *getInputLayout(void);
    ID3D11Buffer *getVertexBuffer(void);
    ID3D11Buffer *getIndexBuffer(void);
    uint32_t getVertexBufferStrides(void);
    uint32_t getVertexBufferOffset(void);
    int getNumVertices(void);
    int getNumIndices(void);
    void destroyMesh(void);

private:
    fbxsdk::FbxManager *manager;
    fbxsdk::FbxScene *scene;
    
    bool createMeshResources(IResources *);    
    struct Scene_t{
        int numMeshes;
        Vec4 *originPoints;
        int numVertices;
        Vec4 *vertices;
        int numIndices;
        uint32_t *indices;
    };  
    Scene_t sceneData;


    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;
    ID3D11InputLayout *inputLayout;
    ID3D11Buffer *vertexBuffer;
    ID3D11Buffer *indexBuffer;
};

#endif