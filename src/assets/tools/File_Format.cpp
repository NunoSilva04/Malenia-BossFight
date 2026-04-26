#include "File_Format.h"
#include <stdio.h>
#include <string.h>
#include "debug_helper.h"
#include "Vertex.h"
#include "Assets_Helper.h"

static void replace_filename_suffix(const char *file_name, const char *suffix, char **out_string){
    int out_str_index = 0;
    while(file_name[out_str_index] != '.'){
        (*out_string)[out_str_index] = file_name[out_str_index];
        out_str_index++;
    }

    int suffix_index = 0;
    while(suffix[suffix_index] != '\0'){
        (*out_string)[out_str_index] = suffix[suffix_index];
        out_str_index++;
        suffix_index++;
    }
    (*out_string)[out_str_index] = '\0';

    return;
}

static FILE *create_file(const char *file_name, const char *suffix){
    // File path creation
    char *final_file_path = nullptr;
    size_t final_file_path_size = strlen(OUTPUT_ASSETS_SRC) * 2;
    final_file_path = new char[final_file_path_size];
    memset(final_file_path, 0, sizeof(char) * final_file_path_size);
    strcpy(final_file_path, OUTPUT_ASSETS_SRC);
    
    char *final_file_name = nullptr;
    size_t final_file_name_size = strlen(file_name) * 2;
    final_file_name = new char[final_file_name_size];
    memset(final_file_name, 0, sizeof(char) * final_file_name_size);
    replace_filename_suffix(file_name, suffix, &final_file_name);

    strcat(final_file_path, final_file_name);

    // Actual FILE creation
    FILE *file = fopen(final_file_path, "wb");

    delete[] final_file_path;
    delete[] final_file_name;
    return file;
}

static void close_file(FILE *file){
    fclose(file);

    return;
}

/**********************************************************************************
*
*       Mesh Data
* 
*************************

************************************************************/

enum class Mesh_Data_Error{
    INVALID_DATA,
    INVALID_MESH,
    VERT_ALLOC_ERROR,
    SUCCESS,
};

enum class Mesh_File_Error{
    INVALID_FILE,
    INVALID_DATA,
    SUCCESS,
};  

typedef struct Mesh_Data{
    char mesh_name[MAX_MESH_NAME];
    uint32_t num_vertices;
    Vertex *vertices; 
}Mesh_Data;

typedef struct Mesh_Header{
    uint32_t num_meshes;
    Mesh_Data *meshes;
}Mesh_Header;

static bool allocate_mesh_data(Mesh_Data *data, uint32_t num_vertices){
    data->num_vertices = num_vertices;
    data->vertices = new Vertex[num_vertices];
    if(data->vertices == nullptr)
        return false;

    return true;
}

static void free_mesh_data(Mesh_Data *data){
    delete[] data->vertices;

    return;
}

static uint32_t get_num_child_node_mesh(cgltf_data *const data){ 
    size_t num_children = 0;
    for(size_t i = 0; i < data->scenes[0].nodes[0]->children_count; i++){
        if(data->scenes[0].nodes[0]->children[i]->mesh != nullptr){
            num_children++;
        }
    }

    return (uint32_t)num_children;
} 

static Mesh_Data_Error fill_mesh_data(cgltf_mesh *const gltf_mesh, Mesh_Data *mesh_data, const char *mesh_name){
    if(gltf_mesh == nullptr)
        return Mesh_Data_Error::INVALID_DATA;

    if(mesh_data == nullptr)
        return Mesh_Data_Error::INVALID_MESH;

    strcpy(mesh_data->mesh_name, mesh_name);
    for(size_t i = 0; i < gltf_mesh->primitives[0].attributes_count; i++){
        if(gltf_mesh->primitives[0].attributes[i].type == cgltf_attribute_type_position){
            cgltf_accessor *accesor = gltf_mesh->primitives[0].attributes[i].data;
            size_t num_floats = cgltf_accessor_unpack_floats(accesor, nullptr, 0);
            size_t num_vertices = accesor->count;
        
            float *vertices_data = new float[num_floats];
            cgltf_accessor_unpack_floats(accesor, vertices_data, num_floats);

            if(!allocate_mesh_data(mesh_data, (uint32_t)num_vertices))
                return Mesh_Data_Error::VERT_ALLOC_ERROR;

            size_t curr_float = 0, curr_vertex = 0;
            while(curr_vertex < num_vertices){
                mesh_data->vertices[curr_vertex].vertices[0] = vertices_data[curr_float];
                mesh_data->vertices[curr_vertex].vertices[1] = vertices_data[curr_float + 1];
                mesh_data->vertices[curr_vertex].vertices[2] = vertices_data[curr_float + 2];

                curr_vertex++;
                curr_float+=3;
            }

            delete[] vertices_data;
        }
    }

    return Mesh_Data_Error::SUCCESS;
}

static Mesh_File_Error write_mesh_data_file(const Mesh_Data *const mesh, FILE *file){
    if(file == nullptr) 
        return Mesh_File_Error::INVALID_FILE;
    
    if(mesh == nullptr)
        return Mesh_File_Error::INVALID_DATA;

    fwrite(mesh->mesh_name, sizeof(char), MAX_MESH_NAME, file);
    fwrite(&mesh->num_vertices, sizeof(uint32_t), 1, file);
    for(uint32_t i = 0; i < mesh->num_vertices; i++){
        fwrite(&mesh->vertices[i], sizeof(Vertex), 1, file);
    }

    return Mesh_File_Error::SUCCESS;
}

static bool load_mesh_data(cgltf_mesh *const mesh){
    printf("Number of primitives: %zu\n", mesh->primitives_count);
    for(size_t i = 0; i < mesh->primitives_count; i++){
        printf("Number of attributes: %zu\n", mesh->primitives[i].attributes_count);
        for(size_t j = 0; j < mesh->primitives[i].attributes_count; j++){
            printf("Attribute[%zu] name: %s\n", j, mesh->primitives[i].attributes[j].name);
            if(mesh->primitives[i].attributes[j].type == cgltf_attribute_type_position){
                printf("In loop\n");
                cgltf_accessor *position = mesh->primitives[i].attributes[j].data;
                
                size_t num_floats = cgltf_accessor_unpack_floats(position, NULL, 0);
                float *floats = new float[num_floats];
                cgltf_accessor_unpack_floats(position, floats, num_floats);

                printf("Number Floats: %zu || Count: %zu\n", num_floats, position->count);
                printf("Offset: %zu\n", position->offset);
                printf("Stride: %zu\n", position->stride);
                printf("Component Type: %s\n", glTF_Helper::gltf_component_type_to_str(position->component_type));
                printf("Type: %s\n", glTF_Helper::gltf_accessor_type_to_str(position->type));
                printf("Buffer size: %zu\n", position->buffer_view->size);
                size_t vertice_num = 0;
                for(size_t curr_vertice = 0; curr_vertice < position->count; curr_vertice++, vertice_num += 3){
                    printf("Vertice[%zu]: (%f, %f, %f)\n", curr_vertice, floats[vertice_num], floats[vertice_num + 1], floats[vertice_num + 2]);
                }

                delete[] floats;
            }

            if(mesh->primitives[i].attributes[j].type == cgltf_attribute_type_normal){
                printf("In loop of normal\n");
                cgltf_accessor *position = mesh->primitives[i].attributes[j].data;
                
                size_t num_floats = cgltf_accessor_unpack_floats(position, NULL, 0);
                float *floats = new float[num_floats];
                cgltf_accessor_unpack_floats(position, floats, num_floats);

                printf("Number Floats: %zu || Count: %zu\n", num_floats, position->count);
                printf("Offset: %zu\n", position->offset);
                printf("Stride: %zu\n", position->stride);
                printf("Component Type: %s\n", glTF_Helper::gltf_component_type_to_str(position->component_type));
                printf("Type: %s\n", glTF_Helper::gltf_accessor_type_to_str(position->type));
                printf("Buffer size: %zu\n", position->buffer_view->size);
                size_t vertice_num = 0;
                for(size_t curr_vertice = 0; curr_vertice < position->count; curr_vertice++, vertice_num += 3){
                    printf("Vertice[%zu]: (%f, %f, %f)\n", curr_vertice, floats[vertice_num], floats[vertice_num + 1], floats[vertice_num + 2]);
                }
            }
        }
    }

    return true;
}

static void node_data(cgltf_node **const node, const size_t num_nodes){
    printf("Number of nodes: %zu\n", num_nodes);
    for(size_t i = 0; i < num_nodes; i++){
        printf("Node[%zu] name: %s\n", i, node[i]->name);
        printf("Number of children: %zu\n", node[i]->children_count);
        for(size_t j = 0; j < node[i]->children_count; j++){
            printf("Child[%zu] name: %s\n", j, node[i]->children[j]->name);
            if(node[i]->children[j]->mesh != NULL){
                load_mesh_data(node[i]->children[j]->mesh);
            }       
        }
    }

    return;
}

static void scene_data(cgltf_scene *const scene, const size_t num_scenes){
    printf("Number of scenes: %zu\n", num_scenes);
    for(size_t i = 0; i < num_scenes; i++){
        printf("Scene[%zu] name: %s\n", i, scene[i].name);
        node_data(scene[i].nodes, scene[i].nodes_count);
    }

    return;
}


bool create_assets_file(const char *gltf_file_path, const char *file_name, cgltf_data *const data, const cgltf_options *options){
    FILE *file = create_file(file_name, ".assets");
    if(file == nullptr){
        Core::debug::log(Core::debug::Fatal, "Couldn't create assets file\n");
        return false;
    }

    if(cgltf_load_buffers(options, data, gltf_file_path) != cgltf_result_success){
        Core::debug::log(Core::debug::Fatal, "Couldn't load assets buffers\n");
        return false;
    }
    
    File_Format format;
    fwrite(&format, sizeof(File_Format), 1, file);

    Mesh_Header mesh_h {};
    mesh_h.num_meshes = get_num_child_node_mesh(data);
    mesh_h.meshes = new Mesh_Data[mesh_h.num_meshes];
    for(uint32_t i = 0; i < mesh_h.num_meshes; i++){
        if(
            fill_mesh_data(data->scenes[0].nodes[0]->children[i]->mesh, &mesh_h.meshes[i], data->scenes[0].nodes[0]->children[i]->name) 
            != Mesh_Data_Error::SUCCESS
        ){
            Core::debug::log(Core::debug::Fatal, "Couldn't fill mesh data\n");
            return false;
        }
    }

    fwrite(&mesh_h.num_meshes, sizeof(uint32_t), 1, file);
    for(uint32_t i = 0; i < mesh_h.num_meshes; i++){
        if(write_mesh_data_file(&mesh_h.meshes[i], file) != Mesh_File_Error::SUCCESS)
            return false;
    }

    for(uint32_t i = 0; i < mesh_h.num_meshes; i++)
        free_mesh_data(&mesh_h.meshes[i]);
    delete[] mesh_h.meshes;
    // scene_data(data->scenes, data->scenes_count);

    //for(size_t i = 0; i < num_childs; i++){
    //    size_t num_vertices = 0;
    //    Child_Error error = get_child_position_num_vertices(data, i, &num_vertices);
    //    if(error == SUCCESS){
    //        Vertex *vertices = new Vertex[num_vertices];
    //        if(get_child_position_vertices(data, i, vertices, num_vertices)){
    //            print_vertex_data(vertices, num_vertices);  
    //        }
//
    //        delete[] vertices;
    //    }else if(error == NO_MESH){
    //        printf("No mesh\n"  );
    //    }
    //}   

    close_file(file);
    return true;
}