#include "Mesh.h"
#include <stdlib.h>

bool create_mesh(Mesh *mesh, const uint32_t num_vertices){
    if(mesh == NULL)
        return false;

    mesh->num_vertices = num_vertices;
    mesh->vertices = (Vertex *) malloc(num_vertices * sizeof(Vertex));
    if(mesh->vertices == NULL)
        return false;

    return true;
}

void destroy_mesh(Mesh *mesh){
    free(mesh->vertices);

    return;
}