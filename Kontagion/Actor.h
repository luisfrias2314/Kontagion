#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//CHECK VIRTUAL DISTRCUTORS AND REPEATING FUNCTIOSN
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor: public GraphObject
{
public:
    Actor(int ID, double X, double Y, int strtDirect, int depth, StudentWorld* studWorld);
    virtual void doSomething() = 0;
    bool isActorAlive() const;
    void setActorDead();
    virtual void hurtObj(int num) = 0;
    StudentWorld* thisStudentWorldpointer();
    virtual bool canBlockMov();
    virtual bool damagableObj();
    virtual bool edible();
    virtual ~Actor();
private:
    bool m_actorAlive;
    StudentWorld* curWorld;
};
class damagingCreatures: public Actor
{
public:
    damagingCreatures(int ID, double X, double Y, int strtDirect, int hitpoint, StudentWorld* studWorld);
    virtual void doSomething() = 0;
    virtual void hurtObj(int num);
    virtual void playhurtSound() = 0;
    virtual void playdieSound() = 0;
    virtual void manipScore();
    virtual int curHitpoint() const;
    virtual void incHealth();
    virtual ~damagingCreatures();
private:
    int m_hitpoint;
   
};
class Socrates: public damagingCreatures
{
public:
    Socrates(StudentWorld* studWorld);
    virtual void doSomething();
    virtual void playhurtSound();
    virtual void playdieSound();
    int sprayAmt() const;
    int flameAmt() const;
    void incFlames();
    virtual ~Socrates();
private:
    int flameAmount;
    int sprayAmount;
};
class Bacteria: public damagingCreatures
{
public:
    Bacteria(int ID, double X, double Y, int hitpoint, StudentWorld* studWorld, int damagVal);
    virtual void doSomething();
    virtual void playhurtSound();
    virtual void playdieSound();
    virtual Actor* createBacteria(double x, double y) = 0;
    int damageVal();
    bool genBacteria();
    virtual void genFood();
    void incFood();
    virtual void manipScore();
    int  movePlanDis();
    void decMovePlan(int num);
    void resetMovePlan();
    virtual ~Bacteria();
private:
    int m_foodEaten;
    int m_movPlanDis;
    int m_damageVal;
};
class regularSalmonella: public Bacteria
{
public:
    regularSalmonella(double X, double Y, StudentWorld* studWorld);
    virtual Actor* createBacteria(double x, double y);
};
class ecoli: public Bacteria
{
public:
    ecoli(double X, double Y, StudentWorld* studWorld);
    virtual void playhurtSound();
    virtual void playdieSound();
    virtual Actor* createBacteria(double x, double y);
};
class aggSalmonella: public Bacteria
{
public:
    aggSalmonella(double X, double Y, StudentWorld* studWorld);
    virtual Actor* createBacteria(double x, double y);
};
class nonLivingCreatures: public Actor
{
public:
    nonLivingCreatures(int ID, double X, double Y, int strtDirect, StudentWorld* studWorld);
    virtual void doSomething() = 0;
    virtual void hurtObj(int num);
    
};
class Dirt: public nonLivingCreatures
{
public:
    Dirt(double x, double y, StudentWorld* studWorld);
    virtual bool canBlockMov();
    virtual void doSomething();
    
};
class Food: public nonLivingCreatures
{
public:
    Food(double x, double y, StudentWorld* studWorld);
    virtual void doSomething();
    virtual bool damagableObj();
    virtual bool edible();
};



class Projectiles: public nonLivingCreatures
{
public:
    Projectiles(int ID, double X, double Y, int strtDirect, int travDis, StudentWorld* studWorld, int amtDamage);
    void doSomething();
    virtual bool damagableObj();
    virtual void hurtObj(int num);
    virtual ~Projectiles();
private:
    int m_projectDistanceCounter;
    int m_travelDistance;
    int m_amtDamage;
};
class Flame: public Projectiles
{
public:
    Flame(double X, double Y, int strtDirect, StudentWorld* studWorld);
    
};
class Spray: public Projectiles
{
public:
    Spray(double X, double Y, int strtDirect, StudentWorld* studWorld);
};


//good to go
class supplyDrops: public nonLivingCreatures
{
public:
    supplyDrops(int ID, double X, double Y, StudentWorld* studWorld, int scoreVal);
    virtual void doSomething();
    virtual void supplyAct() = 0;
    bool declifetime();
    int scoreVal();
    virtual ~supplyDrops();
private:
    int m_scoreVal;
    int m_lifetime;
    
};
class restoreHealth: public supplyDrops
{
public:
    restoreHealth( double X, double Y, StudentWorld* studWorld);
    void supplyAct();
    
};
class giveFlame: public supplyDrops
{
    public:
    giveFlame( double X, double Y, StudentWorld* studWorld);
    void supplyAct();
};
class extraLife: public supplyDrops
{
    public:
    extraLife( double X, double Y, StudentWorld* studWorld);
    void supplyAct();
};
class fungus: public supplyDrops
{
    public:
    fungus( double X, double Y, StudentWorld* studWorld);
    void supplyAct();
};
class pit: public nonLivingCreatures
{
public:
    pit(double x, double y, StudentWorld* studWorld);
    virtual void doSomething();
    virtual bool damagableObj();
private:
    int m_samCount;
    int m_aggSamCount;
    int m_ecoliCount;
};

#endif // ACTOR_H_


