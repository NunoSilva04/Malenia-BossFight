# ASSETS FILE FORMAT
This document presents how the `.assets` file is defined.  
The file is a type of binary file and this the specified layout:
- 1st: [File Header](assets_file_format.md#file-header)
- 2nd: [Mesh Header](assets_file_format.md#mesh-header)
- 3rd: [Mesh Data](assets_file_format.md#mesh-data)

**IMPORTANT:** All of the values stored in the file are in little-endian format. 

## Referenced types
The following types and constants that are used in this file and have been defined and used externally are:
- [MAX_MESH_NAME](Assets_Helper.md#global-variables)
- [Vertex](../Math_Lib/Vertex.md) 

## File Header
Common to all `.assets` files. Specifies the necessary information needed for later validation of the same file when reading it.

Offset | Size |  Type  | Name        | Description |
-------|------|-----  |-------| ------------|
0      | 4    |  uint32_t  | magic  |  Magic number used to validate the file|
4      | 4    |  uint32_t   |version  | Version of the `.assets` file |  

## Mesh Header
Starting point of the data in the mesh.

Offset | Size |  Type  | Name        | Description |
-------|------|-----  |-------| ------------|
8      | 4    |  uint32_t  | num_meshes  | Number of meshes |

## Mesh Data
The data to each Mesh.

Offset | Size |  Type  | Name        | Description |
-------|------|-----  |-------| ------------|
?      |  MAX_MESH_NAME  |  char  | mesh_name  | String with MAX_MESH_NAME size that holds the mesh name |
?      | 4    |  uint32_t   | num_vertices  | Number of vertices of the current mesh|
?      | sizeof(Vertex) * num_vertices    |  Vertex   | vertex  | The vertex data|  
*Note:* Offsets marked with `?` are variable and will depend on the amount of data stored.     

## Visual Diagram
Here is a visual diagram of how a `.assets` file would be implemented. 

```text
+----------------------+ File Header
| magic (4 bytes)      |
+----------------------+
| version (4 bytes)    |
+----------------------+

+----------------------+ Mesh Header
| num_meshes (4 bytes) |
+----------------------+

+----------------------+ Mesh Data 0
| mesh_name            |
+----------------------+
| num_vertices         |
+----------------------+
| vertex[0]            |
| vertex[1]            |
|   .                  |
+----------------------+
``` 