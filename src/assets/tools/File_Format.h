/**********************************************************************************
*   File: File_Format.h
* 
*   Description: 
*       Responsible for handling the ".assets" file.
* 
*   See: 
*       docs/assets/File_Format.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __FILE_FORMAT_H__
#define __FILE_FORMAT_H__

#include "cgltf.h"

namespace glTF_Helper{
    inline const char *gltf_result_to_str(cgltf_result result){
        const char *message;

        switch(result){
            case cgltf_result_success:
                message = "Success";
            break;

            case cgltf_result_data_too_short:
                message = "Data Too Short";
            break;

            case cgltf_result_unknown_format:
                message = "Unknown Format";
            break;

            case cgltf_result_invalid_json:
                message = "Invalid JSON";
            break;

            case cgltf_result_invalid_gltf:
                message = "Invalid glTF";
            break;

            case cgltf_result_invalid_options:
                message = "Invalid Options";
            break;

            case cgltf_result_file_not_found:
                message = "File Not Found";
            break;

            case cgltf_result_io_error:
                message = "IO Error";
            break;

            case cgltf_result_out_of_memory:
                message = "Out Of Memory";
            break;

            case cgltf_result_legacy_gltf:
                message = "Legacy glTF";
            break;

            case cgltf_result_max_enum:
                message = "Max Enum";
            break;
        }

        return message;
    }

    inline const char *gltf_options_file_type_to_str(cgltf_file_type type){
        const char *message;

        switch(type){
            case cgltf_file_type_invalid:
                message = "Invalid";
            break;
            
            case cgltf_file_type_gltf:
                message = "glTF Type";
            break;
            
            case cgltf_file_type_glb:
                message = "glb Type";
            break;

            case cgltf_file_type_max_enum:
                message = "Max Enum";
            break;
        }

        return message;
    }
    
    inline const char *gltf_primitive_type_to_str(cgltf_primitive_type type){
        const char *message;

        switch(type){
            case cgltf_primitive_type_invalid:
                message = "Invalid";
            break;

            case cgltf_primitive_type_points:
                message = "Points";
            break;

            case cgltf_primitive_type_lines:
                message = "Lines";
            break;

            case cgltf_primitive_type_line_loop:
                message = "Line Loop";
            break;

            case cgltf_primitive_type_line_strip:
                message = "Line Strip";
            break;

            case cgltf_primitive_type_triangles:
                message = "Triangles";
            break;

            case cgltf_primitive_type_triangle_strip:
                message = "Triangle Strip";
            break;

            case cgltf_primitive_type_triangle_fan:
                message = "Triangle Fan";
            break;

            case cgltf_primitive_type_max_enum:
                message = "Max Enum";
            break;
        }

        return message;
    }

    inline const char *gltf_component_type_to_str(cgltf_component_type type){
        const char *message;

        switch(type){
            case cgltf_component_type_invalid:
                message = "Invalid";
            break;

            case cgltf_component_type_r_8:
                message = "Signed Byte";
            break;

            case cgltf_component_type_r_8u:
                message = "Unsigned Byte";
            break;

            case cgltf_component_type_r_16:
                message = "Signed Short";
            break;

            case cgltf_component_type_r_16u:
                message = "Unsigned Short";
            break;

            case cgltf_component_type_r_32u:
                message = "Unsigned Integer";
            break;

            case cgltf_component_type_r_32f:
                message = "Float";
            break;

            case cgltf_component_type_max_enum:
                message = "Max Enum";
            break;
        }

        return message;
    }

    inline const char *gltf_bool_to_str(cgltf_bool value){
        if(value == 1) 
            return "True";
        else 
            return "False";
    }

    inline const char *gltf_accessor_type_to_str(cgltf_type type){
        const char *message;

        switch(type){
            case cgltf_type_invalid:
                message = "Invalid";
            break;

            case cgltf_type_scalar:
                message = "Scalar";
            break;

            case cgltf_type_vec2:
                message = "Vector 2";
            break;

            case cgltf_type_vec3:
                message = "Vector 3";
            break;

            case cgltf_type_vec4:
                message = "Vector 4";
            break;

            case cgltf_type_mat2:
                message = "Matrix 2x2";
            break;

            case cgltf_type_mat3:
                message = "Matrix 3x3";
            break;

            case cgltf_type_mat4:
                message = "Matrix 4x4";
            break;

            case cgltf_type_max_enum:
                message = "Max Enum";
            break;
        }

        return message;
    }
    
    inline const char *gltf_buffer_view_type_to_str(cgltf_buffer_view_type type){
        const char *message = "<unnamed>";

        switch(type){
            case cgltf_buffer_view_type_invalid:
                message = "Invalid";
            break;

            case cgltf_buffer_view_type_indices:
                message = "Indices";
            break;

            case cgltf_buffer_view_type_vertices:
                message = "Vertices";
            break;

            case cgltf_buffer_view_type_max_enum:
                message = "Max Enum";
            break;
        }

        return message;
    }
    
    inline const char *gltf_attribute_type_to_str(cgltf_attribute_type type){
        const char *message = "<unnamed>";

        switch(type){
            case cgltf_attribute_type_invalid:
                message = "Invalid";
            break;

            case cgltf_attribute_type_position:
                message = "Position";
            break;

            case cgltf_attribute_type_normal:
                message = "Normal";
            break;

            case cgltf_attribute_type_tangent:
                message = "Tangent";
            break;

            case cgltf_attribute_type_texcoord:
                message = "Texcoord";
            break;

            case cgltf_attribute_type_color:
                message = "Color";
            break;

            case cgltf_attribute_type_joints:
                message = "Joints";
            break;

            case cgltf_attribute_type_weights:
                message = "Weights";
            break;

            case cgltf_attribute_type_custom:
                message = "Custom";
            break;

            case cgltf_attribute_type_max_enum:
                message = "Max Enum";
            break;
        }

        return message;
    }
};

bool create_assets_file(const char *gltf_file_path, const char *gltf_file_name, cgltf_data *const data, const cgltf_options *options);

#endif