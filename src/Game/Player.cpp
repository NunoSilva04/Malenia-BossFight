#include "Player.h"
#include "Assets_Helper.h"
#include "Mesh.h"
#include "debug_helper.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Player_t{
    int healt;
    uint32_t num_meshes;
    Mesh *mesh;
}Player;

Player *create_player(void){
    Player *player = (Player *) malloc(sizeof(Player));
    if(player == NULL)
        return NULL;
    player->healt = 0;
    player->num_meshes = 0;
    player->mesh = NULL;

    File_Error error;
    if(!open_assets_file("assets/assets_src/Sphere.assets", &error)){
        Core::debug::log(Core::debug::Fatal, "Couldn't open player assets file. Error: %d\n", error);
        return NULL;
    }

    if(!get_mesh_data(&player->mesh, &player->num_meshes, &error)){
        Core::debug::log(Core::debug::Fatal, "Couldn't get player mesh data. Error: %d\n", error);
        return NULL;
    }

    close_assets_file();

    for(uint32_t i = 0; i < player->num_meshes; i++){
        Core::debug::log(Core::debug::Info, "Mesh [%d]: Num_Vertices: %d\n", i, player->mesh[i].num_vertices);
        for(uint32_t j = 0; j < player->mesh[i].num_vertices; j++){
            print_vertex(player->mesh[i].vertices[j]);
        }
    }
        
    return player;
}

void destroy_player(Player *player){
    for(uint32_t i = 0; i < player->num_meshes; i++){
        destroy_mesh(&player->mesh[i]);
    }
    free(player->mesh);
    free(player);

    return;
}