/**********************************************************************************
*   File: Vertex.h
* 
*   Description: 
*       Responsible for handling Vertices.
* 
*   See: 
*       docs/Math_Lib/Vertex.md 
*       for full documentation of the header
* 
*************************************************************************************/

#ifndef __VERTEX_H__
#define __VERTEX_H__

typedef struct Vertex{
    float vertices[3];
}Vertex;

void print_vertex(const Vertex vertex);

#endif // __VERTEX_H__