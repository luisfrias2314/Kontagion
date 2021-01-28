#include "StudentWorld.h"
#include "GameWorld.h"
#include <string>
#include <math.h>
#include <cmath> //is this the right place for include
#include <sstream>
#include <iomanip>
using namespace std;

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp
GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    m_player = new Socrates(this);
    double randx,randy;
    for(int i = 0; i < getLevel();i++)
    {
        gameGen(randx, randy);
        m_actors.push_back( new pit(randx,randy,this));
    }
    int numofFood = min(5*getLevel(),25);
    for(int i = 0; i < numofFood; i++)
    {
        gameGen(randx, randy);
        m_actors.push_back( new Food(randx,randy,this));
    }
    
    //populate world with dirt cannot over lap food and pits
    int numDirt = max(180-20*getLevel(),20);
    for(int i = 0; i < numDirt;i++)
    {
        gameGen(randx, randy);
        m_actors.push_back(new Dirt(randx,randy,this));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    list<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end();it++)
    {
        if((*it)->isActorAlive())
        {
            (*it)->doSomething();
            if(!m_player->isActorAlive())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(m_bacteriaCount == 0) 
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    m_player->doSomething();
    list<Actor*>::iterator dead;
    for(dead = m_actors.begin(); dead != m_actors.end(); dead++)
    {
        if(!(*dead)->isActorAlive())
        {
            delete(*dead);
            dead = m_actors.erase(dead);
            dead--;
        }
        
    }
    int ChanceFungus = max(510-getLevel()*10,200); //try to create a function
    int randFung =randInt(0,ChanceFungus);
    if(randFung == 0)
    {
        int randAngle = randInt(0, 359);
        double xran = 128+128*cos(randAngle);
        double yran = 128+128*sin(randAngle);
        m_actors.push_back(new fungus(xran,yran,this));
        
    }
    int ChanceGoodie = max(510-getLevel()*10,250);
    int rand =randInt(0,ChanceGoodie);
    if(rand == 0)
    {
        int typeofGood = randInt(1, 100);
        int randAngle = randInt(0, 359);
        double xran = 128+128*cos(randAngle);
        double yran = 128+128*sin(randAngle);
        if(typeofGood <= 60)
            m_actors.push_back(new restoreHealth(xran,yran,this));
        else if(typeofGood <= 90)
            m_actors.push_back(new giveFlame(xran,yran,this));
        else
        {
            m_actors.push_back(new extraLife(xran,yran,this));
        }
    }
    ostringstream oss;
    oss << "Score:" <<setw(1) << "";
    oss.fill('0');
    oss << setw(5) <<  getScore();
    oss.fill(' ');
    oss <<setw(8) << "Level:"<< setw(1) << "" << getLevel();
    oss <<setw(8) << "Lives:" << setw(1) << "" << getLives();
    oss <<setw(9) << "Health:" << setw(1) << ""<<m_player->curHitpoint();
    oss <<setw(9) << "Sprays:" << setw(1) << ""<<m_player->sprayAmt(); //fix spray
    oss <<setw(9) << "Flames:" << setw(1) << ""<<m_player->flameAmt();
    string s = oss.str();
    setGameStatText(s);
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_player;
    m_player = nullptr;
    list<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end();it++)
    {
        delete *it;
    }
    m_actors.clear();
    
}
StudentWorld::~StudentWorld()
{
    cleanUp();
}
double StudentWorld::actorDistance(double randx, double randy) const
{
    return sqrt(pow((128-randx),2)+pow((128-randy), 2));
}

bool StudentWorld::genericOverlap(double randx, double randy, int type, int num)
{
    list<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end();it++)
    {
        double ch;
        ch = sqrt(pow((((*it)->getX())-randx),2)+pow((((*it)->getY())-randy),2));
        if(ch <= SPRITE_WIDTH/num)
            switch(type)
            {
                case 1:
                {
                    if(!(*it)->canBlockMov()) //generic overlap
                    {
                        return true;
                    }
                    break;
                }
                case 2:
                    if((*it)->canBlockMov()) //bacteriaL overlap
                    {
                        return true;
                    }
                    break;
                case 3:
                {
                    if((*it)->edible())
                    {
                        (*it)->setActorDead(); //foodoverlap
                        return true;
                    }
                }
                    
            }
    }
    return false;
}
void StudentWorld::gameGen(double &randx, double &randy)
{
    randx = randInt(0,VIEW_WIDTH);
    randy = randInt(0,VIEW_HEIGHT);
    while(actorDistance(randx, randy) > 120 or  genericOverlap(randx, randy, 1))
    {
        randx = randInt(0,VIEW_WIDTH);
        randy = randInt(0,VIEW_HEIGHT);
    }
    return;
}
bool StudentWorld::ProjectileDamage(double randx, double randy, int m_damage)
{
    list<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end();it++)
    {
        double ch;
        ch = sqrt(pow((((*it)->getX())-randx),2)+pow((((*it)->getY())-randy),2));
        if(ch <= SPRITE_WIDTH)
            if((*it)->damagableObj() && (*it)->isActorAlive())
            {
                (*it)->hurtObj(m_damage);
                return true;
            }
    }
    return false;
}
bool StudentWorld::closefood(double randx, double randy, int& dir)
{
    list<Actor*>::iterator it;
        for(it = m_actors.begin(); it != m_actors.end();it++)
        {
            double ch;
            ch = sqrt(pow((((*it)->getX())-randx),2)+pow((((*it)->getY())-randy),2));
            if(ch <= 128)
                if((*it)->edible())
                {
                    double num =  (*it)->getX()-randx;
                    double num2 = (*it)->getY()-randy;
                    if(num == 0)
                    {
                        if(num2 > 0)
                          return (*it)->getDirection();
                        else
                        {
                            return (*it)->getDirection()+180;
                        }
                    }
                    double total = num2/num;
                    double result = atan (total) * 180 / 3.14159265;
                    if(num < 0)
                        result += 180;
                    dir =  result;
                    return true;
                }
        }
        return false;
}
bool StudentWorld::SocratesOverlap(double randx, double randy, int distance)
{
    double ch;
    ch = sqrt(pow((m_player->getX()-randx),2)+pow((m_player->getY()-randy),2));
    if(ch <= distance)
        return true;
    return false;
}

//socrates helper functions
void StudentWorld::rehealSocrates()
{
    m_player->incHealth();
}
void StudentWorld::giveFlames()
{
    m_player->incFlames();
}
void StudentWorld::decSocratesHealth(int num)
{
    m_player->hurtObj(num);
}
void StudentWorld::SocratesSpray()
{
    double X, Y;
     m_player->getPositionInThisDirection(m_player->getDirection(), SPRITE_WIDTH, X, Y);
    m_actors.push_back(new Spray(X,Y,m_player->getDirection(),this));
}
void StudentWorld::SocratesFlames()
{
    
    double X =0;
    double Y = 0;
    int dir  = m_player->getDirection();
    for(int n = 0; n < 16; n++)
    {
        m_player->getPositionInThisDirection(dir, SPRITE_WIDTH, X, Y);
        m_actors.push_back(new Flame(X,Y,dir,this));
        dir+= 22;
    }
        

}

//bacteria helper functions
void StudentWorld::newActor(Actor* addAct)
{
    m_actors.push_back(addAct);
}
int StudentWorld::socratesDir(double x, double y)
{
    double num = m_player->getX()-x;
    double num2 = m_player->getY()-y;
    if(num == 0)
    {
          return m_player->getDirection();
    }
    double total = num2/num;
    double result = atan (total) * 180 / 3.14159265;
    if(num < 0)
        result += 180;
    return result;
    
}
void StudentWorld::manipBacteriaCount(int num)
{
    m_bacteriaCount = m_bacteriaCount + num;
}
