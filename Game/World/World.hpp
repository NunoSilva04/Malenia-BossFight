#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include <vector>

class IResources;
class IRender;
class Entity;
class IInput;
class ICamera;

class World{
public:
    World();
    ~World();
    bool initWorld(IResources *); 
    void updateWorld(const float, IRender *, IInput *, ICamera *);
    void renderWorld(IRender *);
    void destroyWorld(void);

private:
    Entity *player;
    std::vector<Entity*> entities;
};

#endif