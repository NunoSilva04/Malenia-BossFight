#include "World.hpp"
#include "IGraphics.h"
#include "Entity.hpp"
#include <cstdio>

World::World(){

}

World::~World(){

}

bool World::initWorld(IResources *resources){
    Entity *newEntity = createPlayer(resources, "Player", 1, "../../Models/Sphere.fbx");
    entities.push_back(newEntity);
    entities[0]->printEntity();

    return true;
}   

void World::renderWorld(IRender *render){
    if(entities[0]->hasMesh()){
        renderEntity(entities[0], render);
    }
}

void World::destroyWorld(void){
    for(int i = 0; i < entities.size(); i++){
        destroyEntity(entities[i]);
    }
}