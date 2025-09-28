#include "Entity.hpp"
#include "Mesh.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

Entity *createPlayer(IResources *resources, std::string name, int id, std::string fileName){
    Entity *player = new Entity();
    if(player == nullptr){
        std::printf("Couldn't create player entity\n");
        return nullptr;
    }
    player->name = name;
    player->id = id;
    player->type = Entity::EntityType::PlayerType;
    player->mesh = new Mesh();
    if(player->mesh == nullptr){
        std::printf("Coudln't create player mesh class\n");
        delete player;
        return nullptr;
    }
    if(!player->mesh->loadMesh(resources, fileName)){
        std::printf("Couldn't load mesh\n");
        delete player->mesh;
        delete player;
        return nullptr;
    }
    return player;
}

void destroyEntity(Entity *entity){
    if(entity != nullptr){
        if(entity->mesh != nullptr){
            entity->mesh->destroyMesh();
            delete entity->mesh;
        }
        delete entity;
    }
    std::printf("Destroyed entity\n");
}

void renderEntity(Entity *entity, IRender *render){
    entity->mesh->renderMesh(render);
}

const std::string Entity::getName(void){
    return this->name;
}

const int Entity::getId(void){
    return this->id;
}

const int Entity::getType(void){
    return static_cast<int>(this->type);
}

bool Entity::hasMesh(void){
    if(this->mesh != nullptr){
        return true;
    }

    return false;
}

void Entity::printEntity(void){
    std::printf("%s, %d, %d\n", this->name.c_str(), this->id, this->type);
    std::printf("\n");
}