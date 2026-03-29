#include "file_system.h"
#include <iostream>
#include <fstream>

Core::n_vector<char> Core::File::read_binary_file(const char *file_path){
    Core::n_vector<char> file_data;
    
    std::ifstream file(file_path, std::ios::ate | std::ios::binary);
    if(!file.is_open()) return file_data;

    size_t file_size = static_cast<size_t>(file.tellg());
    file_data.vector_resize(file_size);
    file.seekg(0, std::ios::beg);
    file.read(file_data.vector_data(), static_cast<long long>(file_size));
    file.close();

    return file_data;
}