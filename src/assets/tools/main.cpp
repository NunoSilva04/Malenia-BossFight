#include "glTF_helper.h"
#include "debug_helper.h"
#include "n_dir.h"

int main(void){
    if(!Core::n_dir::create_dir(OUTPUT_ASSETS_SRC)){
        Core::debug::log(Core::debug::Fatal, "Couldn't create directory\n");
        return 0;
    }

    const char *dir_path = "assets_src/";
    Core::n_dir::Dir_Handle *handle = Core::n_dir::extract_dir_contents(dir_path);

    char **file_paths = nullptr;
    char **file_names = nullptr;
    size_t num_files = 0;
    Core::n_dir::get_dir_file_paths(handle, &num_files, &file_paths);
    Core::n_dir::get_dir_file_names(handle, &num_files, &file_names);

    for(size_t i = 0; i < num_files; i++){
        read_glTF_file(file_paths[i], file_names[i]);
    }

    return 0;
}