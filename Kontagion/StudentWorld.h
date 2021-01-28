#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    double actorDistance(double randx, double randy) const;
    void gameGen(double& randx, double& randy);
    bool genericOverlap(double randx, double randy, int type, int num = 1);
    bool ProjectileDamage(double randx, double randy, int m_damage);
    bool SocratesOverlap(double randx, double randy, int distance = SPRITE_WIDTH);
    bool closefood(double randx, double randy, int& dir);
    int socratesDir(double x, double y);
    void manipBacteriaCount(int num);
    
    
    void SocratesSpray();
    void SocratesFlames();
    void rehealSocrates();
    void decSocratesHealth(int num);
    void giveFlames();
    void newActor(Actor* addAct);
    
    //headerfiles, destructors, const funciton
    
   
private:
    Socrates* m_player;
    std::list<Actor*> m_actors;
    int m_bacteriaCount = 0; //actually work this out
};
#endif // STUDENTWORLD_H_


