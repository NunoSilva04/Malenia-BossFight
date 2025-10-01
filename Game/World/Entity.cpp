#include "Entity.hpp"
#include "Mesh.hpp"
#include "Position.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include "Camera.h"


int Entity::entityID = 0;

Entity *EntitySystem::createPlayer(IResources *resources, std::string name, std::string fileName){
    Entity *player = new Entity();
    if(player == nullptr){
        std::printf("Couldn't create player entity\n");
        return nullptr;
    }
    player->name = name;
    player->id = Entity::entityID++;
    player->type = Entity::EntityType::PlayerType;

    //Mesh
    player->mesh = new Mesh();
    if(player->mesh == nullptr){
        std::printf("Couldn't create player mesh class\n");
        delete player;
        return nullptr;
    }
    if(!player->mesh->loadMesh(resources, fileName)){
        std::printf("Couldn't load mesh\n");
        delete player->mesh;
        delete player;
        return nullptr;
    }

    //Position
    player->position = new Position();
    if(player->position == nullptr){
        std::printf("Couldn't create player transform class\n");
        player->mesh->destroyMesh();
        delete player->mesh;
        delete player;
        return nullptr;
    }
    player->position->initPosition(player->mesh);

    return player;
}

void EntitySystem::updatePlayerPosition_Camera(Entity *player, ICamera *camera){
    player->position->add(camera->getCamPosition());
}

void EntitySystem::destroyEntity(Entity *entity){
    if(entity != nullptr){
        if(entity->mesh != nullptr){
            entity->mesh->destroyMesh();
            delete entity->mesh;
        }
        if(entity->position != nullptr){
            entity->position->destroyPosition();
            delete entity->position;
        }
        delete entity;
    }
}

void Entity::printEntity(void){
    std::printf("%s, %d, %d\n", this->name.c_str(), this->id, this->type);
    std::printf("\n");
}

const std::string Entity::getName(void){
    return this->name;
}

const int Entity::getId(void){
    return this->id;
}

bool Entity::hasMesh(void){
    if(this->mesh != nullptr){
        return true;
    }

    return false;
}

Mesh *Entity::getMesh(void){
    return this->mesh;
}

bool Entity::hasPosition(void){
    if(this->position == nullptr){
        return false;
    }
    return true;
}

Position *Entity::getPosition(void){
    return this->position;
}

const Entity::EntityType Entity::getType(void){
    return this->type;
}