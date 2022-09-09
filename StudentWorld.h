#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"


#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Socrates;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
    void randCoord(int & x, int& y);
    void freeCoord(int & x, int& y);
    
    bool eat(int x, int y);
    
    bool isFree(int x, int y, int distance);
    int angleToClosestFood(int x, int y);
    
    virtual ~StudentWorld();
    bool damagePoint(int x, int y, int damage);
    Socrates* getPlayer(){return m_player;}
    void addActor(Actor* a){
        m_actors.push_back(a);
    }
    void addGoodie();
    void removeDead();

private:
    
    std::vector<Actor*> m_actors;
    Socrates* m_player;
    
};
double getDistance(int x1, int y1, int x2, int y2);

#endif //STUDENTWORLD_H_
