#include "World.hpp"
#include "IGraphics.h"
#include "Entity.hpp"
#include <cstdio>

World::World(){

}

World::~World(){

}

bool World::initWorld(IResources *resources){
    player = EntitySystem::createPlayer(resources, "Player", "../../Models/Sphere.fbx");
    entities.push_back(player);

    return true;
}   

void World::updateWorld(const float frame_time, IRender *render, IInput *input, ICamera *camera){
    if(player->hasPosition()){
        EntitySystem::updatePlayerPosition_Camera(player, camera);
    }
    render->createObjectDataArray(static_cast<int>(entities.size()));
    for(int i = 0; i < entities.size(); i++){
        render->pushEntityDataIntoObjectDataArray(entities[i]->getPosition(), entities[i]->getId());
    }
    render->sendObjectArrayGPU();
}

void World::renderWorld(IRender *render){
    for(int i = 0; i < entities.size(); i++){
        render->renderEntity(entities[i]);
    }
    render->cleanObjectDataArray();
}

void World::destroyWorld(void){
    if(player != nullptr) EntitySystem::destroyEntity(player);
    for(int i = 0; i < entities.size(); i++){
        EntitySystem::destroyEntity(entities[i]);
    }
}