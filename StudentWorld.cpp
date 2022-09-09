#include "StudentWorld.h"
#include "GameConstants.h"
#include <iomanip>  // defines the manipulator setw
#include "Actor.h"
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    
}

int StudentWorld::init()
{
    m_player=new Socrates(this);
   
    m_actors.push_back(m_player);
   
    int y,x;
    for(int i=0;i<getLevel();i++){
        randCoord(x, y);
        
        
        
        m_actors. push_back(new Pit(x,y,this));
    }
    
    bool b=true;
    int numDirt=max(180-20*getLevel(), 20);
    for(int i=0;i<numDirt;i++){
        b=true;
        x=y=0;
        randCoord(x, y);
        for(int j=0;j<getLevel();j++){
            if(getDistance(x, y, m_actors[j+1]->getX(), m_actors[j+1]->getY())<=SPRITE_WIDTH) {
                i--;
                b=false;
                break;
            }
        }
        if(b)  m_actors. push_back(new Dirt(x,y));
        
        
    }
    
    for(int i=0;i< min(5 * getLevel(), 25);i++){
        b=true;
        x=y=0;
        randCoord(x, y);
        for(int j=0;j<i+getLevel();j++){
            if(getDistance(x, y, m_actors[m_actors.size()-j-1]->getX(), m_actors[m_actors.size()-j-1]->getY())<=SPRITE_WIDTH) {
                i--;
                b=false;
                break;
            }
        }
        if(b)m_actors.push_back(new Food(x,y));
        
        
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    vector<Actor*>::iterator it=m_actors.begin();
    for(;it<m_actors.end();it++){
        if(!it.operator*()->isAlive()){
            delete it.operator*();
           it= m_actors.erase(it);
            it--;
            continue;
            
        }
        
        it.operator*()->doSomething();
        if (!m_player->isAlive()){
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        
        
      
    }
    bool levelFinished=true;
    
    for(it=m_actors.begin();it<m_actors.end();it++){
        if(it.operator*()->mustDieToEndLevel())levelFinished=false;
    }
    if(levelFinished){
        return GWSTATUS_FINISHED_LEVEL;
    }
    addGoodie();
    ostringstream oss;
    oss.fill('0');
    oss << setw(7) << "Score: " ;
    
    int score=getScore();
    if(score<0)
    oss<<'-'<<setw(5)<<score*-1;
    else oss<<setw(6)<<score;
    oss << setw(9) << "  Level: " ;
       oss<<getLevel();
    oss << setw(9) << "  Lives: " ;
       oss<<getLives();
    oss << setw(10) << "  Health: ";
       oss<<m_player->getHealth();
   
    oss << setw(10) << "  Sprays: " ;
       oss<<m_player->getSprays();
    oss << setw(9) << "  Flames: ";
       oss<<m_player->getFlames();
    
    setGameStatText(oss.str());
    return GWSTATUS_CONTINUE_GAME;

}






void StudentWorld::addGoodie(){
          int ChanceFungus = max(510 -getLevel() * 10, 200);
          int ChanceGoodie = max(510 -getLevel() * 10, 250);
           ChanceFungus=randInt(0,ChanceFungus-1);
           ChanceGoodie=randInt(0,ChanceGoodie-1);
           
           if(ChanceFungus==0){
               int angle= randInt(0,359);
               int x=VIEW_WIDTH/2+  128*cos(angle*3.14159265/180);
               int y=VIEW_HEIGHT/2+  128*sin(angle*3.14159265/180);
               addActor(new Fungus(x,y,this));
       
       
           }
           if(ChanceGoodie==0){
               int type =randInt(1, 10);
               int angle= randInt(0,359);
               int x=VIEW_WIDTH/2+  128*cos(angle*3.14159265/180);
               int y=VIEW_HEIGHT/2+  128*sin(angle*3.14159265/180);
               switch(type){
                   case 1:
                        addActor(new ExtraLifeGoodie(x,y,this));
             
                       break;
                   case 2:
                   case 3:
                   case 4:
                     
                       addActor(new FlamethrowerGoodie(x,y,this));
                       break;
                   case 5:
                   case 6:
                   case 7:
                   case 8:
                   case 9:
                   case 10:

                         addActor(new RestoreHealthGoodie(x,y,this));
                       break;
               }
           }
     
}



void StudentWorld::cleanUp()
{
    for(int i=0;i<m_actors.size();i++)delete m_actors[i];
    m_actors.clear();
    
    m_player=nullptr;
    
}

bool StudentWorld::damagePoint(int x, int y, int damage){
    vector<Actor *>::iterator it=m_actors.begin();
    it++;
    
    for(int i=1;i<m_actors.size();i++){
        if(getDistance(x, y,it.operator*()->getX(), it.operator*()->getY())<=SPRITE_WIDTH) {
            if(it.operator*()->decHealth(damage)){
            return true;
            }
        }
        it++;
    }
    return false;
}
                            
double getDistance(int x1, int y1, int x2, int y2){
            return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}
                            
StudentWorld::~StudentWorld(){
    cleanUp();
}

void StudentWorld::randCoord(int & x, int& y){
    
    x=y=0;
    
    while(getDistance(x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2)>120){
        y=randInt(VIEW_HEIGHT/2-120,VIEW_HEIGHT/2+120);
        x=randInt(VIEW_WIDTH/2-120, VIEW_WIDTH/2+120);
    }
    
}
bool StudentWorld::eat(int x, int y){
    vector<Actor*>::iterator it=m_actors.begin();
    int pos=-1;
    for(int i=0;i<m_actors.size();i++){
        if(m_actors[i]->isEdible()&&getDistance(x, y, m_actors[i]->getX(),m_actors[i]->getY())<=SPRITE_WIDTH){
            pos=i;
            
            
        }
        
            
    }
    
    if(pos!=-1){
        it=m_actors.begin();
        for(int i=0;i<pos;i++)
            it++;
        delete m_actors[pos];
        m_actors.erase(it);
        return true;
        
    }
    return false;
        
}

bool StudentWorld:: isFree(int x, int y, int distance){
    if( getDistance(x,y,VIEW_WIDTH/2,VIEW_HEIGHT/2) >= VIEW_RADIUS)return false;
    for(int i=0;i<m_actors.size();i++)
        if(m_actors[i]->blocksMovement()&& getDistance(x,y,m_actors[i]->getX(),m_actors[i]->getY())<=distance) return false;
    return true;

}

int StudentWorld:: angleToClosestFood(int x, int y){
    int x2,y2, min,temp;
    bool foodFound=false;
    x2=y2=0;
    min=100000;
    for(int i=0;i<m_actors.size();i++){
        if(!m_actors[i]->isEdible()) continue;
        temp = getDistance(x, y, m_actors[i]->getX(), m_actors[i]->getY());
        if(temp<min&&temp<128 ){
            min=temp;
            x2=m_actors[i]->getX();
            y2=m_actors[i]->getY();
            foodFound=true;
        }
    }
    if(!foodFound) return -1;
    if((x2-x) != 0){
        if(x2-x<0&&y2-y<0) return atan((y2-y)/(x2-x))*180/3.14159+180;
        if(x2-x<0&&y2-y>0) return atan((y2-y)/(x2-x))*180/3.14159+180;
        int angle = atan((y2-y)/(x2-x))*180/3.14159;
        return angle%360;
    }
    else if(y2-y>0) return 90;
    return 270;
}
