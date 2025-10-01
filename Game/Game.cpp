#include "Game.h"
#include "Windows_OS.h"
#include "Input.h"
#include "DirectX.h"
#include "Camera.h"
#include "Vector.h"
#include "World.hpp"

Game::Game(){
    input = nullptr;
    gfx = nullptr;
    camera = nullptr;
    world = nullptr;
}

Game::~Game(){
    
}   

bool Game::initializeGame(HWND hWnd, HMONITOR hMonitor, windowPreferances window){
    input = new Input();
    if(!input->initInput()){
        std::printf("Couldn't initialize input\n");
        return false;
    }
    gfx = new Graphics();
    if(!gfx->initDirectX(hWnd, hMonitor, window)){
        std::printf("Couldn't initialize directX\n");
        return false;
    }

    IResources *resources = gfx;
    camera = new Camera();
    if(!camera->initCamera(resources, window)){
        std::printf("Couldn't initialize camera\n");
        return false;
    }

    world = new World();
    if(!world->initWorld(resources)){
        std::printf("Couldn't init world\n");
        return false;
    }

    return true;
}

void Game::updateGame(float frame_time){
    IRender *render = gfx;
    input->update(frame_time);
    camera->updateCamera(render, input, frame_time);
    world->updateWorld(frame_time, render, static_cast<IInput*>(input), static_cast<ICamera*>(camera));
}

void Game::renderGame(){
    gfx->setRenderTargetDepth(1);
    gfx->clearRenderTargetView(Vec4(0.5f, 0.0f, 0.0f, 0.0f));
    gfx->clearDepthStencilView(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    gfx->setBlendState(Vec4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

    IRender *render = gfx;
    world->renderWorld(render);

    gfx->present(1);
}

void Game::cleanGame(){
    input->cleanInput();
    if(input) delete input;
    gfx->cleanDirectX();
    if(gfx) delete gfx;
    camera->cleanCamera();
    if(camera) delete camera;
    world->destroyWorld();
    if(world) delete world;
}