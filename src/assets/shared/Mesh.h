/**********************************************************************************
*   File: Mesh.h
* 
*   Description: 
*       Responsible for all the Mesh Logic.
* 
*   See: 
*       docs/assets/Mesh.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __MESH_H__
#define __MESH_H__

#include "Vertex.h"
#include <stdint.h>

typedef struct Mesh_t{
    uint32_t num_vertices;
    Vertex *vertices;
}Mesh;

bool create_mesh(Mesh *mesh, const uint32_t num_vertices);
void destroy_mesh(Mesh *mesh);

#endif