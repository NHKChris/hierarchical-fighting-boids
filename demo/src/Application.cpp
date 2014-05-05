#include "Application.h"

Application::Application(){
    device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480), 32, false, true);
    if (device){
        device->setWindowCaption(L"Boids Demo - 0.1");

        //irrlicht managers
        driver = device->getVideoDriver();
        smgr = device->getSceneManager();
        guienv = device->getGUIEnvironment();
        smgr->setAmbientLight(video::SColorf(0.1f,0.1f,0.1f,1.f));
        smgr->setShadowColor(video::SColor(150,0,0,0));

        device->getCursorControl()->setVisible(false);
        init();

        state = DEMO_INIT;
    }
}

Application::~Application(){
    if(device){
        device->drop();
    }
}

void Application::init(){
    scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 20.0f, 0.01f);
    camera->setTarget(core::vector3df(0., 0., 0.));
    camera->setPosition(core::vector3df(0., 0., -20.));
    camera->setFarValue(100.f);
    video::ITexture* text = driver->getTexture("../textures/ESO_-_Milky_Way_Bottom.bmp");
    scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(text,text,text,text,text,text);
}

#include <iostream>
#include <time.h>

bool Application::run(){
    if(!device){
        return false;
    }

    Simulation sim;
    sim.Init();

    std::vector<Boid*> boids;
    for(int i = 0; i < 200; i++){
        Boid* monBoid = new Boid(smgr);
        boids.push_back(monBoid);
        sim.AddUnit(sim.CreateUnit(Vector3(rand()%100, rand()%100, 0), Vector3(1, 0, 0), 0, monBoid));
    }

    ITimer* irrTimer = device->getTimer();
    u32 TimeStamp = irrTimer->getTime(), DeltaTime = 0;
    while(device->run()){
        DeltaTime = irrTimer->getTime() - TimeStamp;
        TimeStamp = irrTimer->getTime();

        sim.Update(DeltaTime);
        vector<Unit*>* list = sim.GetAllUnits();
        for(unsigned int i = 0; i < list->size(); i++){
            Vector3 pos = list->at(i)->GetPosition();
            ((Boid*)list->at(i)->GetData())->setPosition(core::vector3df(pos.X, pos.Y, pos.Z));
        }

        driver->beginScene();
        smgr->drawAll();
        driver->endScene();
    }

    for(unsigned int i = 0; i < boids.size(); i++){
        delete boids[i];
    }
    sim.Clear();
    return true;
}
