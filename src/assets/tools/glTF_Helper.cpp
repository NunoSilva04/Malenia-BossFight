#define CGLTF_IMPLEMENTATION
#include "glTF_helper.h"
#include "debug_helper.h"
#include "File_Format.h"

bool read_glTF_file(const char *filepath, const char *file_name){
    cgltf_options options = {};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, filepath, &data);

    if(result != cgltf_result_success){
        Core::debug::log(Core::debug::Fatal, "Couldn't open glTF file (Result = %s)\n", glTF_Helper::gltf_result_to_str(result));
        return false;
    }

    if(!create_assets_file(filepath, file_name, data, &options)){
        Core::debug::log(Core::debug::Fatal, "Couldn't create mesh file\n");
        return false;
    }

    cgltf_free(data);
    return true;
}

/*


  if (result == cgltf_result_success){
    	printf("SUCCESS\n");

        // Data
        printf("File type :%s\n", glTF_Helper::gltf_options_file_type_to_str(data->file_type));
        printf("File size: %zu\n", data->file_size);
        
        result = cgltf_load_buffers(&options, data, "assets_src/Test.glb");
        printf("Error: %s\n", glTF_Helper::gltf_result_to_str(result));

        print_mesh_data(data->meshes, data->meshes_count);

    	cgltf_free(data);
    }


static void gltf_print_buffer(cgltf_buffer *const buffer){
    printf("Buffer name: %s\n", buffer->name);
    printf("Buffer Size: %zu\n", buffer->size);
    printf("URI: %s\n", buffer->uri);

    return;
}

static void gltf_print_buffer_view(cgltf_buffer_view *const buffer_view){
    printf("Buffer view name: %s\n", buffer_view->name);
    gltf_print_buffer(buffer_view->buffer);
    printf("Buffer View Offset: %zu\n", buffer_view->offset);
    printf("Buffer View size: %zu\n", buffer_view->size);
    printf("Buffer View Stride %zu (0 == automatically determined by accessor) \n", buffer_view->stride);
    printf("Buffer View Type: %s\n", glTF_Helper::gltf_buffer_view_type_to_str(buffer_view->type));
    printf("Number of Extensions: %zu\n", buffer_view->extensions_count);
    for(size_t curr_extensions = 0; curr_extensions < buffer_view->extensions_count; curr_extensions++){
        printf("Extension %zu (Name: %s, Data: %s)\n", curr_extensions, buffer_view->extensions[curr_extensions].name, 
            buffer_view->extensions[curr_extensions].data);
    }

    if(buffer_view->data == NULL){
        printf("Buffer View data is NULL\n");
    }
    if(buffer_view->buffer->data == NULL){
        printf("Buffer data is NULL\n");
    }

    return;
}

static void gltf_print_primitive_attributes(const cgltf_primitive primitive, const size_t num_attributes){
    for(size_t i = 0; i < num_attributes; i++){
        printf("Attribute: %zu:\n", i);
        printf("Attribute Name: %s\n", primitive.attributes[i].name);
        printf("Attribute type: %s\n", glTF_Helper::gltf_attribute_type_to_str(primitive.attributes[i].type));
        printf("Attribute Int: %d\n", primitive.attributes[i].index);
        printf("Attribute Data Type: %s\n", glTF_Helper::gltf_accessor_type_to_str(primitive.attributes[i].data->type));
        printf("Attribute Offset %zu\n", primitive.attributes[i].data->offset);
        printf("Attribute Count: %zu\n", primitive.attributes[i].data->count);
        printf("Attribute Stride: %zu\n", primitive.attributes[i].data->stride);
        gltf_print_buffer_view(primitive.attributes[i].data->buffer_view);
        printf("Attribute Has min: %s\n", glTF_Helper::gltf_bool_to_str(primitive.attributes[i].data->has_min));
        printf("Attribute Has Max: %s\n", glTF_Helper::gltf_bool_to_str(primitive.attributes[i].data->has_max));
        printf("Attribute Is Sparse %s\n", glTF_Helper::gltf_bool_to_str(primitive.attributes[i].data->is_sparse));
    
        cgltf_float *floats = new float[primitive.attributes[i].data->count];
        size_t out = cgltf_accessor_unpack_floats(primitive.attributes[i].data, floats, primitive.attributes[i].data->count);
        if(out == primitive.attributes[i].data->count){
            printf("Out: %zu\n", out);
            for(size_t curr_float = 0; curr_float < primitive.attributes[i].data->count; curr_float += 3){
                printf("Float [%zu] = (%f, %f, %f)\n", curr_float, floats[curr_float], floats[curr_float + 1], floats[curr_float + 2]);
            }
        }

        printf("\n\n");
    }

    return;
}

void print_mesh_data(cgltf_mesh *const mesh, size_t num_meshes){
    for(size_t curr_mesh = 0; curr_mesh < num_meshes; curr_mesh++){
        printf("Mesh number: %zu\n", curr_mesh);
        printf("Mesh name: %s\n", mesh[curr_mesh].name);
        printf("Number of primitives: %zu\n", mesh[curr_mesh].primitives_count);
        for(size_t curr_primitive = 0; curr_primitive < mesh[curr_mesh].primitives_count; curr_primitive++){
            printf("Primitive number: %zu\n", curr_primitive);
            printf("Primitive Type: %s\n", glTF_Helper::gltf_primitive_type_to_str(mesh[curr_mesh].primitives[curr_primitive].type));

            // Indices
            //printf("Indices name: %s\n", mesh[curr_mesh].primitives[curr_primitive].indices->name);
            //printf("Indices component type: %s\n", gltf_component_type_to_str(mesh[curr_mesh].primitives[curr_primitive].indices->component_type));
            //printf("Indices Normalized: %s\n", gltf_bool_to_str(mesh[curr_mesh].primitives[curr_primitive].indices->normalized));
            //printf("Indices Type: %s\n", gltf_accessor_type_to_str(mesh[curr_mesh].primitives[curr_primitive].indices->type));
            //printf("Indices Offset %zu\n", mesh[curr_mesh].primitives[curr_primitive].indices->offset);
            //printf("Indices Count: %zu\n", mesh[curr_mesh].primitives[curr_primitive].indices->count);
            //printf("Indices Stride: %zu\n", mesh[curr_mesh].primitives[curr_primitive].indices->stride);
            //gltf_print_buffer_view(mesh[curr_mesh].primitives[curr_primitive].indices->buffer_view);
            //printf("Indices Has min: %s\n", gltf_bool_to_str(mesh[curr_mesh].primitives[curr_primitive].indices->has_min));
            //printf("Indices Has Max: %s\n", gltf_bool_to_str(mesh[curr_mesh].primitives[curr_primitive].indices->has_max));
            //printf("Indices Is Sparse %s\n", gltf_bool_to_str(mesh[curr_mesh].primitives[curr_primitive].indices->is_sparse));

            // Attributes
            printf("Nmber of Attributes: %zu\n", mesh[curr_mesh].primitives[curr_primitive].attributes_count);
            gltf_print_primitive_attributes(mesh[curr_mesh].primitives[curr_primitive], mesh[curr_mesh].primitives[curr_primitive].attributes_count);
        }
    }

    return;
}


*/