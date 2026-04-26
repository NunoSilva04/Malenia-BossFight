#include "Vertex.h"
#include <stdio.h>

void print_vertex(const Vertex vertex){
    #ifdef DEBUG
    printf("Vertex: (%f, %f, %f)\n", vertex.vertices[0], vertex.vertices[1], vertex.vertices[2]);
    #endif

    return;
}