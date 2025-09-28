#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <fbxsdk.h>
#include <string>
#include <d3d11.h>
#include <cstdint>
#include "Vector.h"
#include "Matrix.h"

class IResources;
class IRender;

class Mesh{
public:
    Mesh();
    ~Mesh();
    bool loadMesh(IResources *, std::string);
    void renderMesh(IRender *render);
    void destroyMesh();

private:
    fbxsdk::FbxManager *manager;
    struct Vertices_t{
        Vec4 vertice;
    };
    Vertices_t *vertices;
    int numVertices;

    struct Indices_t{
        uint32_t index;
    };
    Indices_t *indices;
    int numIndices;
    
    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;
    ID3D11InputLayout *inputLayout;
    ID3D11Buffer *vertexBuffer;
    ID3D11Buffer *indexBuffer;
    ID3D11Buffer *constantBuffer;
    Mat4x4 world;
};

#endif