#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int ID, double X, double Y, int strtDirect, int depth, StudentWorld* studWorld): GraphObject(ID,X,Y,strtDirect,depth)
{
    m_actorAlive = true;
    curWorld = studWorld;
}

bool Actor::isActorAlive() const
{
    return m_actorAlive;
}
bool Actor::edible()
{
    return false;
}
void Actor::setActorDead()
{
    m_actorAlive = false;
}
StudentWorld* Actor::thisStudentWorldpointer()
{
    return curWorld;
}
bool Actor::canBlockMov()
{
    return false;
}
bool Actor::damagableObj()
{
    return true;
}
Actor::~Actor()
{
}
damagingCreatures::damagingCreatures(int ID, double X, double Y, int strtDirect, int hitpoint, StudentWorld* studWorld): Actor(ID,X,Y,strtDirect,0, studWorld)
{
    m_hitpoint = hitpoint;
}
void damagingCreatures::manipScore()
{
    
}
void damagingCreatures::hurtObj(int num)
{
    m_hitpoint = m_hitpoint - num;
    if(m_hitpoint >= 0)
    {
        playhurtSound();
    }
    else
    {
        playdieSound();
        manipScore();
        setActorDead();
        
    }
}
int damagingCreatures::curHitpoint() const
{
    return m_hitpoint;
}
damagingCreatures::~damagingCreatures()
{
    
}
void damagingCreatures::incHealth() 
{
    m_hitpoint = 100;
}
//socrates functions
Socrates::Socrates(StudentWorld* studWorld): damagingCreatures(IID_PLAYER, 0,VIEW_HEIGHT/2,0, 100, studWorld)
{
    flameAmount = 5;
    sprayAmount = 20;
}
void Socrates::incFlames(){
    flameAmount += 5;
}
void Socrates::doSomething()
{    if(isActorAlive())
    {
        int ch;
        if(thisStudentWorldpointer()->getKey(ch))
        {
            switch(ch)
            {
                case KEY_PRESS_RIGHT:
                    moveTo(128, 128);
                    moveAngle(getDirection()+175,128);
                    setDirection(getDirection()-5);
                    break;
                case KEY_PRESS_LEFT:
                    moveTo(128, 128);
                    moveAngle(getDirection()+185,128);;
                    setDirection(getDirection()+5);
                    break;
                case KEY_PRESS_SPACE:
                    if(sprayAmount > 0)
                    {
                        thisStudentWorldpointer()->SocratesSpray();
                        sprayAmount = sprayAmount -1;
                        thisStudentWorldpointer()->playSound(SOUND_PLAYER_SPRAY);
                    }
                    break;
                case KEY_PRESS_ENTER:
                    if(flameAmount > 0)
                    {
                        thisStudentWorldpointer()->SocratesFlames();
                        flameAmount = flameAmount-1;
                        thisStudentWorldpointer()->playSound(SOUND_PLAYER_FIRE);
                    }
                    break;
                default:
                    break;
                    
            }
        }
        else
        {
            if(sprayAmount < 20)
                sprayAmount++;
        }
            

    }
    
}
void Socrates::playhurtSound()
{
    thisStudentWorldpointer()->playSound(SOUND_PLAYER_HURT);
}
void Socrates::playdieSound()
{
    thisStudentWorldpointer()->playSound(SOUND_PLAYER_DIE);
}
Socrates::~Socrates()
{
    
}
//non living functions
nonLivingCreatures::nonLivingCreatures(int ID, double X, double Y, int strtDirect, StudentWorld* studWorld): Actor(ID, X, Y, strtDirect, 1, studWorld)
{
    
}
void nonLivingCreatures::hurtObj(int num)
{
    if (num > 0)
        setActorDead();
}

void nonLivingCreatures::doSomething()
{
    
}
//dirt functions
Dirt::Dirt(double x, double y, StudentWorld* studWorld): nonLivingCreatures(IID_DIRT,x,y,0,studWorld)
{
    
}
bool Dirt::canBlockMov()
{
    return true;
}
void Dirt::doSomething()
{
    
}

//   functions
Food::Food(double x, double y, StudentWorld* studWorld): nonLivingCreatures(IID_FOOD,x,y,90,studWorld)
{
    
}
pit::pit(double x, double y, StudentWorld* studWorld): nonLivingCreatures(IID_PIT,x,y,0,studWorld)
{
    m_samCount = 5;
    m_aggSamCount = 3;
    m_ecoliCount = 2;
    thisStudentWorldpointer()->manipBacteriaCount(10);
    
}
void pit::doSomething()
{
    if(m_samCount == 0 && m_aggSamCount == 0 && m_ecoliCount == 0)
    {
        setActorDead();
    }
    else
    {
        Actor* obj;
        int rand = randInt(0, 49);
        if(rand == 0)
        {
            int num = 1;
            while(num)
            {
                int type = randInt(0, 2);
                switch(type)
                {
                    case 0:
                    {
                        if(m_samCount > 0)
                        {
                            obj = new regularSalmonella(getX(),getY(),thisStudentWorldpointer());
                            m_samCount--;
                            num = 0;
                        }
                        break;
                    }
                    case 1:
                    {
                        if(m_aggSamCount > 0)
                        {
                            obj = new aggSalmonella(getX(),getY(),thisStudentWorldpointer());
                            m_aggSamCount--;
                            num = 0;
                        }
                        break;
                    }
                    case 2:
                    {
                        if(m_ecoliCount > 0)
                        {
                            obj = new ecoli(getX(),getY(),thisStudentWorldpointer());
                            m_ecoliCount--;
                            num = 0;
                        }
                        break;
                    }
                }
            }
            thisStudentWorldpointer()->newActor(obj);
            thisStudentWorldpointer()->playSound(SOUND_BACTERIUM_BORN);
        }
    }
}
bool pit::damagableObj(){
    return false;
}
void Food::doSomething()
{
    
}
bool Food::damagableObj()
{
    return false;
}
bool Food::edible()
{
    return true;
}

Projectiles::Projectiles(int ID, double X, double Y, int strtDirect, int travDis, StudentWorld* studWorld, int amtDamage): nonLivingCreatures(ID, X, Y, strtDirect, studWorld)
{
    m_travelDistance = travDis;
    m_projectDistanceCounter = 0;
    m_amtDamage = amtDamage;
}
void Projectiles::doSomething()
{
    if(isActorAlive())
    {
        if(thisStudentWorldpointer()->ProjectileDamage(getX(),getY(), m_amtDamage))
        {
            setActorDead();
        }
        else
        {
            moveAngle(getDirection(),SPRITE_WIDTH);
            if((m_projectDistanceCounter)*(SPRITE_RADIUS*2) >= m_travelDistance)
            {
                setActorDead();
            }
            m_projectDistanceCounter++;
        }
    }
    return;
}

bool Projectiles::damagableObj()
{
    return false;
}
void Projectiles::hurtObj(int num)
{
    
}

Flame::Flame(double X, double Y, int strtDirect, StudentWorld* studWorld): Projectiles(IID_FLAME,X,Y,strtDirect, 32, studWorld,5)
{
    
}

Spray::Spray(double X, double Y, int strtDirect, StudentWorld* studWorld): Projectiles(IID_SPRAY,X,Y,strtDirect, 112, studWorld,2)
{
    
}
supplyDrops::supplyDrops(int ID, double X, double Y, StudentWorld* studWorld, int scoreVal): nonLivingCreatures(ID, X,Y,0,studWorld)
{
    m_lifetime = std::max(randInt(0,300-10*thisStudentWorldpointer()->getLevel()-1), 50);
    m_scoreVal = scoreVal;
}
bool supplyDrops::declifetime()
{
    if(m_lifetime <= 0)
        return true;
    m_lifetime--;
    return false;
}
int supplyDrops::scoreVal()
{
    return m_scoreVal;
}
void supplyDrops::doSomething()
{
    if(isActorAlive())
    {
        if(!declifetime())
        {
            if(thisStudentWorldpointer()->SocratesOverlap(getX(), getY()))
            {
                thisStudentWorldpointer()->increaseScore(m_scoreVal);
                setActorDead();
                if(scoreVal() > 0 )
                {
                    thisStudentWorldpointer()->playSound(SOUND_GOT_GOODIE);
                }
                supplyAct();
                return;
            }
        }
           else
        {
            setActorDead();
        }
    }
    return;
}
restoreHealth::restoreHealth(double X, double Y, StudentWorld* studWorld): supplyDrops(IID_RESTORE_HEALTH_GOODIE,  X, Y,studWorld,250)
{
}
void restoreHealth::supplyAct()
{
    thisStudentWorldpointer()->rehealSocrates();
}
giveFlame::giveFlame(double X, double Y, StudentWorld* studWorld): supplyDrops(IID_FLAME_THROWER_GOODIE,  X, Y,studWorld,300)
{
}
void giveFlame::supplyAct()
{
    thisStudentWorldpointer()->giveFlames();
}
extraLife::extraLife(double X, double Y, StudentWorld* studWorld): supplyDrops(IID_EXTRA_LIFE_GOODIE,  X, Y,studWorld,500)
{
}
void extraLife::supplyAct()
{
    thisStudentWorldpointer()->incLives();
}
fungus::fungus(double X, double Y, StudentWorld* studWorld): supplyDrops(IID_FUNGUS,  X, Y,studWorld,-50)
{
    
}
void fungus::supplyAct()
{
    thisStudentWorldpointer()->decSocratesHealth(20);
}
int Socrates::flameAmt() const
{
    return flameAmount;
}
int Socrates::sprayAmt() const
{
    return sprayAmount;
}
Bacteria::Bacteria(int ID, double X, double Y, int hitpoint, StudentWorld* studWorld, int damageVal): damagingCreatures(ID, X, Y, 90, hitpoint, studWorld)
{
    m_foodEaten = 0;
    m_movPlanDis = 0;
    m_damageVal = damageVal;
}
void Bacteria::genFood()
{
    int flip = randInt(0, 1);
    if(flip)
    {
        Actor* bac = new Food(getX(), getY(), thisStudentWorldpointer());
        thisStudentWorldpointer()->newActor(bac);
    }
}
void Bacteria::manipScore()
{
    thisStudentWorldpointer()->increaseScore(100);
    thisStudentWorldpointer()->manipBacteriaCount(-1);
    genFood();
}
bool Bacteria::genBacteria()
{
    if(m_foodEaten == 3)
    {
        m_foodEaten = 0;
        return true;
    }
    return false;
}
void Bacteria::playhurtSound(){
    thisStudentWorldpointer()->playSound(SOUND_SALMONELLA_HURT);
}
void Bacteria::playdieSound()
{
    thisStudentWorldpointer()->playSound(SOUND_SALMONELLA_DIE);
}
void Bacteria::incFood()
{
    m_foodEaten++;
}
int Bacteria::movePlanDis()
{
    return m_movPlanDis;
}
void Bacteria::decMovePlan(int num)
{
    m_movPlanDis = m_movPlanDis - num;
}
void Bacteria::resetMovePlan()
{
    m_movPlanDis = 10;
}
int Bacteria::damageVal()
{
    return m_damageVal;
}
regularSalmonella::regularSalmonella(double X, double Y, StudentWorld* studWorld): Bacteria(IID_SALMONELLA, X, Y, 4, studWorld,1)
{
    
}
void ecoli::playhurtSound()
{
    thisStudentWorldpointer()->playSound(SOUND_ECOLI_HURT);
}
void ecoli::playdieSound()
{
    thisStudentWorldpointer()->playSound(SOUND_ECOLI_DIE);
}
ecoli::ecoli(double X, double Y, StudentWorld* studWorld): Bacteria(IID_ECOLI, X, Y, 5, studWorld,4)
{
}
aggSalmonella::aggSalmonella(double X, double Y, StudentWorld* studWorld): Bacteria(IID_SALMONELLA, X, Y, 10, studWorld,2)
{
    
}
Actor* regularSalmonella::createBacteria(double x, double y)
{
    Actor* obj = new regularSalmonella(x,y,thisStudentWorldpointer());
    return obj;
}
Actor* aggSalmonella::createBacteria(double x, double y)
{
    Actor* obj = new aggSalmonella(x,y,thisStudentWorldpointer());
    return obj;
}
Actor* ecoli::createBacteria(double x, double y)
{
    Actor* obj = new ecoli(x,y,thisStudentWorldpointer());
    return obj;
}

void Bacteria::doSomething() //change for a generic do while
{
   if(isActorAlive())
    {
        int checkMark = 0;
        if(damageVal() == 1 or damageVal() == 2)
        {
            checkMark = 1;
        }
       if(thisStudentWorldpointer()->SocratesOverlap(getX(), getY(),72) && damageVal() == 2)
        {
            double x,y;
            int dir = thisStudentWorldpointer()->socratesDir(getX(), getY());
            getPositionInThisDirection(dir, 3, x, y);
            if(!thisStudentWorldpointer()->genericOverlap(x, y, 2))
            {
                setDirection(dir);
                moveTo(x, y);
            }
            checkMark = 0;
        }
        if(thisStudentWorldpointer()->SocratesOverlap(getX(), getY()))
            thisStudentWorldpointer()->decSocratesHealth(damageVal());
        else
        {
            if(genBacteria())
            {
                double arr[2] = {getX(),getY()};
                for(int i =0; i < 2; i++)
                {
                    if(arr[i] < VIEW_WIDTH/2)
                        arr[i] += SPRITE_WIDTH;
                    else if(arr[i] > VIEW_WIDTH/2)
                        arr[i]-= SPRITE_HEIGHT;
                }
                thisStudentWorldpointer()->manipBacteriaCount(1);
                thisStudentWorldpointer()->newActor(createBacteria(arr[0], arr[1]));
            }
            else
            {
                if(thisStudentWorldpointer()->genericOverlap(getX(), getY(), 3))
                {
                    incFood();

                }
            }
        }
        if(damageVal() ==  4)
        {
            if(thisStudentWorldpointer()->SocratesOverlap(getX(), getY(), 256))
            {
                int dir = thisStudentWorldpointer()->socratesDir(getX(),getY());
                int counter  = 0;
                while (counter < 10)
                {
                    double x,y;
                    getPositionInThisDirection(dir, 3, x, y);
                    if(!thisStudentWorldpointer()->genericOverlap(x, y, 2,2))
                    {
                        setDirection(dir);
                        moveTo(x, y);
                        return;
                    }
                    counter++;
                }
            }
        }
        if(movePlanDis() > 0 && checkMark == 1)
        {
            decMovePlan(1);
            double x, y;
            getPositionInThisDirection(getDirection(), 3, x, y);
            if(!thisStudentWorldpointer()->genericOverlap(x, y, 2,2) && thisStudentWorldpointer()->actorDistance(x, y) < VIEW_RADIUS)
            {
                moveTo(x, y);
            }
            else
            {
                int randDir = randInt(0, 359);
                setDirection(randDir);
                resetMovePlan();
            }
            return;
        }
        else if(checkMark == 1)
        {
          int dir = 0;
          if(thisStudentWorldpointer()->closefood(getX(), getY(), dir))
              {
                  double x,y;
                  getPositionInThisDirection(dir, 3, x, y); //dirrection 
                  if(!thisStudentWorldpointer()->genericOverlap(x, y, 2,2))
                  {
                      setDirection(dir);
                      moveTo(x, y);
                  }
                  else
                  {
                      int randdir = randInt(0, 359);
                      setDirection(randdir);
                      resetMovePlan();
                      return;
                  }
              }
              else
              {
                  int randdir = randInt(0, 359);
                  setDirection(randdir);
                  resetMovePlan();
                  return;
              }
        }
    }
    return;
}
Projectiles::~Projectiles()
{
    
}
Bacteria::~Bacteria()
{
    
}
supplyDrops::~supplyDrops()
{
    
}
