#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth,StudentWorld* world):GraphObject(imageID, startX, startY,dir,depth){
    m_alive=true;
    m_world=world;
}

Dirt::Dirt(double startX, double startY):Actor(IID_DIRT,startX,startY,0,1,nullptr){
    
}


Socrates::Socrates(StudentWorld* world):Agent(IID_PLAYER,0,128,0,0,100,world){
    m_angle=180;
    m_sprays=20;
    m_flames=5;
    
}
void Socrates::doSomething()
     {
         if(!isAlive())return;
         int input;
         
         if(getWorld()->getKey(input)){
             if(input=='A'||input=='a'||input==KEY_PRESS_LEFT){
                 m_angle+=5;
                 setDirection((m_angle+180)%360);
                 moveTo(VIEW_WIDTH/2+  128*cos(m_angle*3.14159265/180), VIEW_HEIGHT/2+128*sin(m_angle*3.14159265/180));
                 
             }
            else if(input=='D'||input=='d'||input==KEY_PRESS_RIGHT){
                 m_angle-=5;
                 setDirection((m_angle+180)%360);
                 moveTo(VIEW_WIDTH/2+128*cos(m_angle*3.14159265/180), VIEW_HEIGHT/2+128*sin(m_angle*3.14159265/180));
                                  
             }
            else if(input==KEY_PRESS_SPACE&&m_sprays>0){
                 
                 getWorld()->addActor(new Spray(getX()+SPRITE_WIDTH*cos((getDirection())*3.14159/180),getY()+SPRITE_HEIGHT*sin((getDirection())*3.14159/180),getDirection(),getWorld()));
                 getWorld()->playSound(SOUND_PLAYER_SPRAY);
                 m_sprays--;
             }
             else if(input==KEY_PRESS_ENTER&&m_flames>0){
                 getWorld()->playSound(SOUND_PLAYER_FIRE);
                 for(int i=0;i<16;i++){
                     getWorld()->addActor(new Flame(getX()+SPRITE_WIDTH*cos((getDirection()+i*22)*3.14159/180),getY()+SPRITE_HEIGHT*sin((getDirection()+i*22)*3.14159/180),(getDirection()+i*22)%360,getWorld()));
                     
                     }
                 m_flames--;
             }
             
             
         }else{
             if(m_sprays<20) m_sprays++;
         }
        
     }
         
        
         void Socrates::addFlameCharges(){
             m_flames+=5;
             if(m_flames>20)m_flames=20;
             
         }
         
         
Agent::Agent(int imageID,int x, int y,int dir,int depth, int hpl, StudentWorld* world):Actor(imageID,x,y,dir,depth,world){
    
    m_hp=hpl;
    m_hplim=hpl;
    
}

void Projectile::doSomething(){
    if(!isAlive())return;
    
   
   

    
    if( getWorld()->damagePoint(getX(), getY(), getDamage())){
        setDead();
        return;
    }
    moveForward(SPRITE_WIDTH);
    remainingTicks--;
    if(remainingTicks<=0)setDead();
}



Projectile::Projectile(int imageID, double startX, double startY, Direction dir ,int ticksleft,int dg, StudentWorld* world ):Actor(imageID, startX, startY, dir, 1,world){
    remainingTicks=ticksleft;
    dmg=dg;
}

Flame::Flame(double startX, double startY, Direction dir , StudentWorld* world):Projectile(IID_FLAME, startX, startY, dir, 4,5, world){
    
}
Spray::Spray(double startX, double startY, Direction dir , StudentWorld* world):Projectile(IID_SPRAY, startX, startY, dir, 14,2, world){
    
}


Goodie::Goodie(int imageID, double startX, double startY , StudentWorld* world ):Actor(imageID,startX,startY,0,1,world),lifespan(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50)){
    
}

void Goodie::doSomething(){
if(!isAlive())return;
    
if(getDistance(getX(), getY(),getWorld()->getPlayer()->getX() , getWorld()->getPlayer()->getY())<=SPRITE_WIDTH){
   
    setDead();
    pickupAction();
    return;
    
    
}
    lifespan--;
    if(lifespan==0)setDead();

}
Fungus::Fungus(double startX, double startY, StudentWorld* world):Goodie(IID_FUNGUS,startX,startY,world){
    
}

RestoreHealthGoodie::RestoreHealthGoodie(double startX, double startY, StudentWorld* world):Goodie(IID_RESTORE_HEALTH_GOODIE,startX,startY,world){
    
}
FlamethrowerGoodie::FlamethrowerGoodie(double startX, double startY, StudentWorld* world):Goodie(IID_FLAME_THROWER_GOODIE,startX,startY,world){
    
}

ExtraLifeGoodie::ExtraLifeGoodie(double startX, double startY, StudentWorld* world):Goodie(IID_EXTRA_LIFE_GOODIE,startX,startY,world){
    
}

void RestoreHealthGoodie::pickupAction(){
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->getPlayer()->restoreHealth();
    getWorld()->increaseScore(250);
}
void ExtraLifeGoodie::pickupAction(){
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->incLives();
    getWorld()->increaseScore(500);
}
void FlamethrowerGoodie::pickupAction(){
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->getPlayer()->addFlameCharges();
    getWorld()->increaseScore(300);
}
void Fungus::pickupAction(){
    getWorld()->getPlayer()->decHealth(20);
    getWorld()->increaseScore(-50);
}

Food::Food(double startX, double startY):Actor(IID_FOOD,startX,startY,90,1,nullptr){
    
}

Bacterium::Bacterium(int iid, double startX, double startY, int hpl, int dg,StudentWorld* world):Agent(iid,startX,startY,90,0,hpl,world){
    m_foodEaten=0;
    m_plan=0;
    dmg=dg;
    
}
Pit::Pit(double startX, double startY, StudentWorld* world):Actor(IID_PIT ,startX,startY,0,1,world) {

    m_bacteria[0]=5;
    m_bacteria[1]=3;
    m_bacteria[2]=2;
}

RegularSalmonella::RegularSalmonella(int x, int y, StudentWorld* s):Salmonella(x, y, 4,1, s){
    
}

Salmonella::Salmonella(int x, int y, int hpl,int dmg, StudentWorld* s):Bacterium(IID_SALMONELLA, x, y,hpl, dmg,s){
    
}
AggressiveSalmonella::AggressiveSalmonella(int x, int y, StudentWorld* s):Salmonella(x, y,10, 2,s){
    
}
EColi::EColi(int x , int y, StudentWorld* s):Bacterium(IID_ECOLI, x, y, 5,4,s){
    
}

bool AggressiveSalmonella::moveToSocrates(){
    int dist=getDistance(getX(), getY(), getWorld()->getPlayer()->getX(),  getWorld()->getPlayer()->getY());
    
    if(dist<=1)return true;
    
    if(dist<=72){
        int x2=getWorld()->getPlayer()->getX();
        int x=getX();
       
        int y2=getWorld()->getPlayer()->getY();
        int y=getY();
        
        int angle= atan2((y2-y),(x2-x))  *180/3.14159;
        angle=angle%360;
        
        
        
        
        

        double dx,dy;
        getPositionInThisDirection(angle, 3, dx, dy);
        if(getWorld()->isFree(dx,dy,SPRITE_WIDTH/2)){
            moveTo(dx, dy);
            setDirection(angle);
           
        }
         return true;
    }
    return false;
}
void Bacterium::doSomething(){
   
    if(!isAlive())return ;
    
    
    bool b=false;
    b=moveToSocrates();
    if(getDistance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY())<=SPRITE_WIDTH){
        getWorld()->getPlayer()->decHealth(dmg);
        }
    if(m_foodEaten>=3){
        int newX=getX();
        int newY=getY();
            
        if(newX< VIEW_WIDTH/2)  newX+=SPRITE_WIDTH/2;
        else newX-=SPRITE_WIDTH/2;
            
        if(newY< VIEW_HEIGHT/2)  newY+=SPRITE_WIDTH/2;
        else newY-=SPRITE_WIDTH/2;
       
        split(newX,newY);
        m_foodEaten=0;
            
        
    }else  if(getWorld()->eat(getX(), getY())) m_foodEaten++;
    

    if(b)return;
    doSomething2();

    }


void Salmonella::doSomething2(){
     double x,y;
    if(getPlan()>0){
           
       
           getPositionInThisDirection(getDirection(), 3, x, y);
           if (getWorld()->isFree(x, y,SPRITE_WIDTH/2)){
               moveTo(x, y);
               setPlan(getPlan()-1);
               return;
           }else{
               setDirection(randInt(0,359));
               setPlan(10);
               return;
           }
       }
           int angle=getWorld()->angleToClosestFood(getX(), getY());
           
           if(angle==-1){
               setDirection(randInt(0, 359));
               setPlan(10);
               return;
           }
           getPositionInThisDirection(angle, 3, x, y);
           
               
           if (getWorld()->isFree(x,y,SPRITE_WIDTH/2)) {
               setDirection(angle);
               moveTo(x, y);
               return;
               
           }else{
               setDirection(randInt(0, 359));
               setPlan(10);
               return;
           }
}
void EColi::doSomething2(){
    if(getDistance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY())>256)return;
    int angle=atan2(getWorld()->getPlayer()->getY()-getY(), getWorld()->getPlayer()->getX()-getX())*180/3.14159;
    double x,y;
    getPositionInThisDirection(angle, 2, x, y);
    
    for(int i=0;i<10;i++) {
        if(getWorld()->isFree(x, y, 2)){
            moveTo(x,y);
            return;
        }else{
            angle+=10;
            angle%=360;
        }
    }
    
}


void RegularSalmonella::split(int x, int y){
    getWorld()->addActor(new RegularSalmonella(x,y,getWorld()));
}
void AggressiveSalmonella::split(int x, int y){
    getWorld()->addActor(new AggressiveSalmonella(x,y,getWorld()));
}
void EColi::split(int x, int y){
    getWorld()->addActor(new EColi(x,y,getWorld()));
}

void Food::doSomething(){
    
}

void Pit::doSomething(){
    if(m_bacteria[0]==0&&m_bacteria[1]==0&&m_bacteria[2]==0){
        setDead();
        return;
    }
    int chance=randInt(0, 49);
    if(chance==0){
       
        do{
        chance=randInt(0, 2);
        }while(m_bacteria[chance]<=0);
        
        switch( chance){
        case 0:
                getWorld()->addActor(new RegularSalmonella(getX(),getY(),getWorld()));
                m_bacteria[0]--;
                break;
        case 1:
                getWorld()->addActor(new AggressiveSalmonella(getX(),getY(),getWorld()));
                m_bacteria[1]--;
                break;
        case 2:
                getWorld()->addActor(new EColi(getX(),getY(),getWorld()));
                m_bacteria[2]--;
                break;
            
        }
        getWorld()->playSound(SOUND_BACTERIUM_BORN);
    }
    
    
    
}

bool Agent::decHealth(int damage){
      if(damage<=0)return false;
      m_hp-=damage;
      if(m_hp<=0){
          m_hp=0;
          setDead();
      }
      return true;
  }


 bool Bacterium::decHealth(int damage){
     if(!isAlive()) return false;
     
     Agent::decHealth(damage);
     
     

     if(isAlive()) getWorld()->playSound(sound());
     
     else{
         setDead();
         getWorld()->playSound(soundDead());
         getWorld()->increaseScore(100);
            if(randInt(0, 1)){
                getWorld()->addActor(new Food(getX(),getY()));
            }
     }
     
   
     return true;
}

int Salmonella::sound(){
    return SOUND_SALMONELLA_HURT;
}
int EColi::sound(){
    return SOUND_ECOLI_HURT;
}

int Salmonella::soundDead(){
    return SOUND_SALMONELLA_DIE;
}
int EColi::soundDead(){
    return SOUND_ECOLI_DIE;
}
bool Socrates::decHealth(int damage){
    Agent::decHealth(damage);
    if(isAlive()){
        getWorld()->playSound(SOUND_PLAYER_HURT);
        
    }
    else{
        getWorld()->playSound(SOUND_PLAYER_DIE);
        setDead();
    }
    return true;

}




