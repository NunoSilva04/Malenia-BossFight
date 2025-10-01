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
    //TODO
}

void World::renderWorld(IRender *render){
    //TODO
}

void World::destroyWorld(void){
    if(player != nullptr) EntitySystem::destroyEntity(player);
    for(int i = 0; i < entities.size(); i++){
        EntitySystem::destroyEntity(entities[i]);
    }
}