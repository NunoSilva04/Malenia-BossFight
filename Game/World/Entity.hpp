#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <string>
#include "Math2.h"

class IResources;
class IRender;
class ICamera;

class Entity;
class Mesh;
class Position;


namespace EntitySystem{
    Entity *createPlayer(IResources *, std::string, std::string);
    void updatePlayerPosition_Camera(Entity *, ICamera *);
    void destroyEntity(Entity *);
};

class Entity{
//Entity System
private:
    friend Entity* EntitySystem::createPlayer(IResources *, std::string, std::string);
    friend void EntitySystem::updatePlayerPosition_Camera(Entity *, ICamera *);
    friend void EntitySystem::destroyEntity(Entity *);

//Entity
public:
    void printEntity(void);
    const std::string getName(void);
    const int getId(void);
    bool hasMesh(void);
    Mesh *getMesh(void);
    bool hasPosition(void);
    Position *getPosition(void);

protected:
    std::string name;
    int id;
    enum class EntityType{
        PlayerType,
        EnemyType,
    };
    EntityType type;
    Mesh *mesh;
    Position *position;

private:
    static int entityID;
    const EntityType getType(void);
};

#endif