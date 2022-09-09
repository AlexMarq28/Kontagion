#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
class StudentWorld;

class Actor : public GraphObject{
    public:
        Actor(int imageID, double startX, double startY, Direction dir , int depth, StudentWorld* world );
        virtual void doSomething()=0;
        bool isAlive(){return m_alive;}
        void setDead(){m_alive=false;}
        virtual bool decHealth(int damage) {return false;}
        StudentWorld* getWorld(){return m_world;}
        virtual bool blocksMovement(){return false;};
        virtual bool isEdible(){return false;}
        virtual bool mustDieToEndLevel(){return false;}
    
    private:
        bool m_alive;
        StudentWorld* m_world;
};

class Dirt : public Actor{
    public:
        Dirt(double startX, double startY);
        virtual void doSomething(){}
        virtual bool decHealth(int damage){
            setDead();
            return true;
            
        }
        virtual bool blocksMovement(){return true;};
    
    
};
class Bacterium;
class Pit : public Actor{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool mustDieToEndLevel(){return true;}
    
private:
    int  m_bacteria  [3];
   
};



class Food : public Actor{
    public:
    Food(double startX, double startY);
    virtual void doSomething();
    virtual bool isEdible(){return true;}
   
};

class Projectile : public Actor{
    public:
        Projectile(int imageID, double startX, double startY, Direction dir ,int ticksleft,int dmg, StudentWorld* world );
        virtual void doSomething();
    int getDamage(){return dmg;};
        
    private:
        int remainingTicks;
        int dmg;
        
};

class Flame : public Projectile{
    public:
    
    Flame(double startX, double startY, Direction dir , StudentWorld* world);
   
    
};
class Spray : public Projectile{
    public:
   
    Spray(double startX, double startY, Direction dir , StudentWorld* world);
};
class Goodie : public Actor{
    public:
    Goodie(int imageID, double startX, double startY, StudentWorld* world );
    virtual bool decHealth(int damage){
         setDead();
         return true;
         
     }
    virtual void pickupAction()=0;
    virtual void doSomething();
    private:
    int lifespan;
};
class RestoreHealthGoodie: public Goodie{
public:
    RestoreHealthGoodie(double startX, double startY, StudentWorld* world);
    virtual void pickupAction();
};
class FlamethrowerGoodie: public Goodie{
    public:
    FlamethrowerGoodie(double startX, double startY, StudentWorld* world);
    virtual void pickupAction();
};
class ExtraLifeGoodie: public Goodie{
    public:
    ExtraLifeGoodie(double startX, double startY, StudentWorld* world);
    virtual void pickupAction();
};
class Fungus: public Goodie{
public:
    Fungus(double startX, double startY, StudentWorld* world);
    virtual void pickupAction();
};
class Agent: public Actor {
    public:
    Agent(int imageID,int x, int y,int dir,int depth, int hpl,StudentWorld* world);
    int getHealth(){return m_hp;}
    
    void restoreHealth(){m_hp=m_hplim;}
    
    virtual bool decHealth(int damage);
    private:
    int m_hp;
    int m_hplim;
    
};

class Socrates : public Agent{
    public:
        Socrates(StudentWorld* world);
        virtual void doSomething();
        void addFlameCharges();
    
    int getFlames(){return m_flames;}
    int getSprays(){return m_sprays;}
    virtual bool decHealth(int damage);
    
    private:
       
        Direction m_angle;
        int m_sprays;
        int m_flames;
        
    
};

class Bacterium : public Agent{
public:
    Bacterium(int iid, double startX, double startY, int hpl,int dg,StudentWorld* world);
    virtual void doSomething();
    virtual void split(int x, int y)=0;
    virtual bool moveToSocrates(){return false;};
    virtual bool decHealth(int damage);
    virtual int sound()=0;
    virtual int soundDead()=0;
    virtual bool mustDieToEndLevel(){return true;}
    virtual void doSomething2()=0;
    
    int getPlan(){return m_plan;}
    void setPlan(int plan){m_plan=plan;}
private:
     int  m_foodEaten;
     int  m_plan;
     int  dmg;
    
};

class Salmonella: public Bacterium{
public:
    Salmonella(int x, int y, int hpl,int dmg,StudentWorld* s);
    virtual int sound();
    virtual int soundDead();
    virtual void doSomething2();
    
    
};
class RegularSalmonella: public Salmonella{
public:
     RegularSalmonella(int x, int y, StudentWorld* s);
    virtual void split(int x, int y);
    
};
class AggressiveSalmonella: public Salmonella{
public:
    AggressiveSalmonella(int x, int y, StudentWorld* s);
    virtual bool moveToSocrates();
    virtual void split(int x, int y);
};
class EColi:public Bacterium{
public:
    EColi(int x , int y, StudentWorld* s);
    virtual void split(int x, int y);
    virtual int sound();
    virtual int soundDead();
    virtual void doSomething2();
};
