#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <string>

class Mesh;
class IResources;
class IRender;

class Entity{
protected:
    std::string name;
    int id;
    enum class EntityType{
        PlayerType,
        EnemyType,
    };
    EntityType type;
    Mesh *mesh;

public:
    //Entity
    void printEntity(void);
    const std::string getName(void);
    const int getId(void);
    const int getType(void);
    //Mesh
    bool hasMesh(void);

private:
    friend Entity* createPlayer(IResources *, std::string, int, std::string);
    friend void destroyEntity(Entity *);
    friend void renderEntity(Entity *, IRender *);
};

//Entity
Entity *createPlayer(IResources *, std::string, int, std::string);
void destroyEntity(Entity *);
//Temporary render system
void renderEntity(Entity *, IRender *);

#endif