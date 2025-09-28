#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include <vector>

class IResources;
class IRender;
class Entity;

class World{
public:
    World();
    ~World();
    bool initWorld(IResources *); 
    void renderWorld(IRender *);
    void destroyWorld(void);

private:
    std::vector<Entity*> entities;
};

#endif