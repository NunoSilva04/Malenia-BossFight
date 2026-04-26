#include "n_dir.h"
#include <cstdio>
#include <cstring>

#if defined(_WIN32)
    #include <windows.h>
    struct os_dir{
        HANDLE dir;
        char *dir_name;
    };

    static void win_format_dir(const char *directory_name, char **out_string){
        char *temp = (char *)directory_name;
        size_t string_size = strlen(directory_name);
        while(*temp != '\0'){
            temp++;
        }

        if(*out_string == nullptr){
            *out_string = new char[string_size * 2];
            char *p_dir_name = (char *)directory_name;
            size_t i = 0;
            while(*p_dir_name != '\0'){
                if(*p_dir_name == '/'){
                    (*out_string)[i++] = '\\';
                    (*out_string)[i++] = '\\';
                }else{
                    (*out_string)[i++] = *p_dir_name;
                }
                p_dir_name++;
            }
            (*out_string)[i++] = '*';
            (*out_string)[i++] = '\0';
        }
    }

#elif defined(__linux__) || defined(__APPLE__)
    #include <dirent.h>
    struct os_dir{
        DIR *dir; 
        char *dir_name;
    };

#endif

struct Core::n_dir::Directory_Handle_t{
    explicit Directory_Handle_t() noexcept {
        this->data.dir_name = nullptr;
        this->file_paths = nullptr;
        this->file_names = nullptr;
        this->num_files = 0;
    };

    ~Directory_Handle_t() { 
        if(this->data.dir_name != nullptr)
            delete[] this->data.dir_name;
        
        if(this->num_files > 0){
            for(size_t i = 0; i < this->num_files; i++){
                delete[] this->file_paths[i];
                delete[] this->file_names[i];         
            }
            delete[] this->file_paths;
            delete[] this->file_names;
        }
    };

    Directory_Handle_t(const Directory_Handle_t &other) = delete;
    Directory_Handle_t &operator=(const Directory_Handle_t &other) = delete;
    Directory_Handle_t(Directory_Handle_t &&other) = delete;
    Directory_Handle_t& operator=(Directory_Handle_t &&other) = delete;

    os_dir data;
    char **file_paths;
    char **file_names;
    size_t num_files;
};

Core::n_dir::Directory_Handle_t *Core::n_dir::extract_dir_contents(const char *directory_name){
#if defined(_WIN32)
    Directory_Handle_t *handle = new Directory_Handle_t;
    win_format_dir(directory_name, &handle->data.dir_name);

    WIN32_FIND_DATAA data;
    size_t num_files = 0;
    handle->data.dir = FindFirstFileA(handle->data.dir_name, &data);

    if(handle->data.dir == INVALID_HANDLE_VALUE){
        return NULL;
    }else{
        if(strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0){
            num_files++;
        }
        while(FindNextFileA(handle->data.dir, &data)){
            if(strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0){
                num_files++;
            }
        }

        FindClose(handle->data.dir);
        handle->num_files = num_files;

        handle->file_paths = new char *[handle->num_files];
        handle->file_names = new char *[handle->num_files];
        for(size_t i = 0; i < handle->num_files; i++){
            handle->file_paths[i] = new char[256];
            memset(handle->file_paths[i], 0, 256 * sizeof(char));
            
            handle->file_names[i] = new char[256];
            memset(handle->file_names[i], 0, 256 * sizeof(char));
        }

        size_t curr_file = 0;
        handle->data.dir = FindFirstFileA(handle->data.dir_name, &data);
        if(strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0){
            strcat(handle->file_paths[curr_file], directory_name);
            strcat(handle->file_paths[curr_file], data.cFileName);
            strcpy(handle->file_names[curr_file], data.cFileName);
            curr_file++;
        }
        while(FindNextFileA(handle->data.dir, &data)){
            if(strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0){
                strcat(handle->file_paths[curr_file], directory_name);
                strcat(handle->file_paths[curr_file], data.cFileName); 
                strcpy(handle->file_names[curr_file], data.cFileName);       
                curr_file++;
            }
        }

        FindClose(handle->data.dir);
        return handle;
    }

#elif defined(__linux__) || defined(__APPLE__)
   

#endif
}

void Core::n_dir::get_dir_file_paths(Directory_Handle_t *handle, size_t *num_files, char ***file_paths){
#if defined(_WIN32)
    if(handle->num_files > 0){
        *num_files = handle->num_files;
        *file_paths = handle->file_paths;
    }else{  
        *num_files = 0;
        *file_paths = nullptr;
    }

#elif defined(__linux__) || defined(__APPLE__)

#endif

    return;
}

void Core::n_dir::get_dir_file_names(Dir_Handle *handle, size_t *num_files, char ***file_names){
#if defined(_WIN32)
    if(handle->num_files > 0){
        *num_files = handle->num_files;
        *file_names = handle->file_names;
    }else{
        *num_files = 0;
        *file_names = nullptr;
    }

#elif defined(__linux__) || defined(__APPLE__)

#endif

    return;
}

bool Core::n_dir::create_dir(const char *dir_path){
#if defined(_WIN32)
    if(CreateDirectoryA(dir_path, NULL) == 0){
        if(GetLastError() == ERROR_ALREADY_EXISTS){
            return true;
        }else
            return false;
    }
    else
        return true;

#elif defined(__linux__) || defined(__APPLE__)

    return true;
#endif
}