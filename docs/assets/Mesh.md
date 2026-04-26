# Mesh.h
Responsible for the creation, destruction and manipulation of a Mesh.

## Global Variables
None.

## Types
### `typedef struct Mesh_t Mesh`
Structure that holds the data for a Mesh.

| Members | Type | Access | Description |
|---------| -----| -------|------------|
| num_vertices | uint32_t | public | Number of vertices of a Mesh |
| vertices | Vertex * | public | Pointer to an array of vertices | 


### Functions
#### Function: `bool create_mesh(Mesh *mesh, const uint32_t num_vertices)` 
- **Access:** `Public` 
- **Parameters:**
    - `[in]Mesh *mesh` - The pointer to a valid Mesh.
    - `[in]const uint32_t num_vertices` - The number of vertices of the Mesh.
- **Return:**  `bool`   
- **Description:** Creates a mesh and returns true if it succeeds. Returns false otherwise.

#### Function: `void destroy_mesh(Mesh *mesh)` 
- **Access:** `Public` 
- **Parameters:** `[int]Mesh *mesh` - Pointer to a valid Mesh.
- **Return:**  `void`   
- **Description:** Destroys the mesh.

## Dependencies
- [Vertex](../Math_Lib/Vertex.md)