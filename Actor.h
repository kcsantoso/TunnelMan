#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>

class StudentWorld;

class Actor : public GraphObject {
public:
    
    //Actor constructor
    Actor (StudentWorld* w, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: pure virtual function
    virtual void doSomething()=0;
    
    //Actor destructor
    virtual ~Actor();
        
    //NAME: gotGold
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: just returns, used for protester classes
    virtual void gotGold();
    
    //NAME: getHitPoints
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: returns 0, used for protester and tunnelman classes
    virtual int getHitPoints();
    
    //NAME: blocksTunnelman
    //INPUT: none
    //OUTPUT: boolean
    //DESCRIPTION: returns false
    virtual bool blocksTunnelman();
    
    //NAME: annoyedByBoulderOrSquirt
    //INPUT: none
    //OUTPUT: boolean
    //DESCRIPTION: returns false
    virtual bool annoyedByBoulderOrSquirt();
    
    //NAME: getWorld
    //INPUT: none
    //OUTPUT: pointer to studentworld
    //DESCRIPTION: returns pointer to student world that actor is in
    StudentWorld* getWorld();
    
    //NAME: setState
    //INPUT: string
    //OUTPUT: none
    //DESCRIPTION: sets actor's state to newState
    void setState(std::string newState);
    
    //NAME: getState
    //INPUT: none
    //OUTPUT: string
    //DESCRIPTION: returns state of actor
    std::string getState();
    
    //NAME: getAnnoyed
    //INPUT: integer
    //OUTPUT: none
    //DESCRIPTION: just returns, used in protester and tunnelman classes
    virtual void getAnnoyed(int i);
    
    //NAME: move
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has actor move one step in direction it is facing
    void move();
    
    //NAME: hardcore
    //INPUT: none
    //OUTPUT: boolean
    //DESCRIPTION: returns false
    virtual bool hardcore();
    
private:
    StudentWorld* myWorld;
    std::string state;
};

class Squirt : public Actor {
public:
    //Squirt constructor
    Squirt(StudentWorld* w, int startX, int startY, Direction d);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has squirt function as specified while alive
    virtual void doSomething();
    
    //Squirt destructor
    virtual ~Squirt();
    
private:
    int travelDistance;
};



class Goody : public Actor {
public:
    //Goody constructor
    Goody (StudentWorld* w, int imageID, int startX, int startY);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has goody function as specified while alive
    virtual void doSomething(int sound, int points);
    
    //Goody destructor
    virtual ~Goody() {}
    
};

class waterPool : public Goody {
public:
    //waterPool constructor
    waterPool(StudentWorld *w, int startX, int startY);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has waterpool function as specified while alive
    virtual void doSomething();
    
    //waterPool destructor
    virtual ~waterPool();
private:
    int lifeSpan;
};

class sonarKit : public Goody {
public:
    //sonarKit constructor
    sonarKit(StudentWorld *w);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has goody function as specified while alive
    virtual void doSomething();
    
    //sonarKit destructor
    virtual ~sonarKit();
private:
    int lifeSpan;
};

class oilBarrel : public Goody {
public:
    //oilBarrel constructor
    oilBarrel (StudentWorld* w, int startX, int startY);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has oilbarrel function as specified while alive
    virtual void doSomething();
    
    //oilBarrel destructor
    virtual ~oilBarrel();
};

class goldNugget : public Goody {
public:
    //goldNugget constructor
    goldNugget (StudentWorld* w, int startX, int startY);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has goldnugget function as specified while alive
    virtual void doSomething();
    
    //goldNugget destructor
    virtual ~goldNugget();
    
    //NAME: setLifeSpan
    //INPUT: integer
    //OUTPUT: none
    //DESCRIPTION: sets lifespan to i
    void setLifeSpan(int i);
private:
    int lifeSpan;
};


class Earth : public Actor {
public:
    //Earth constructor
    Earth (StudentWorld* w, int startX, int startY);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has earth function as specified while alive
    virtual void doSomething();
    
    //Earth destructor
    virtual ~Earth();
};


class Boulder : public Actor {
public:
    //Boulder constructor
    Boulder (StudentWorld* w, int startX, int startY);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: boolean
    //DESCRIPTION: returns true
    virtual bool blocksTunnelman();
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has boulder function as specified while alive
    virtual void doSomething();
    
    //Boulder destructor
    virtual ~Boulder();
    
private:
    int ticksPassed;
    
};



class TunnelMan : public Actor {
public:
    //TunnelMan constructor
    TunnelMan (StudentWorld* w);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has tunnelman function as specified while alive
    virtual void doSomething();
    
    //TunnelMan destructor
    virtual ~TunnelMan();
    
    //NAME: getWater
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: returns amount of water tunnelman has
    int getWater();
    
    //NAME: getSonarCharge
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: returns amount of sonar charge tunnelman has
    int getSonarCharge();
    
    //NAME: getGold
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: returns number of goldnuggets tunnelman has
    int getGold();
    
    //NAME: addsonar
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: adds one to tunnelman's number of sonar charge
    void addSonar();
    
    //NAME: addWater
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: adds give to tunnelman's amount of water
    void addWater();
    
    //NAME: addGold
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: adds one to tunnelman's number of goldnuggets
    void addGold();
    
    //NAME: getAnnoyed
    //INPUT: integer
    //OUTPUT: none
    //DESCRIPTION: decrements tunnelman's hitpoints by i
    virtual void getAnnoyed(int i);
    
    //NAME: getHitPoints
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: returns number of hitpoints tunnelman has
    virtual int getHitPoints();
    
private:
    int hitPoints;
    int water;
    int sonarCharge;
    int goldNuggets;
};


class Protester : public Actor {
public:
    //Protester constructor
    Protester(StudentWorld *w, int imageID = TID_PROTESTER, int hitPts = 5);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has protester function as specified while alive
    virtual void doSomething();
    
    //Protester destructor
    virtual ~Protester();
    
    //NAME: gotGold
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: tells protester they have picked up a goldnugget
    virtual void gotGold();
    
    //NAME: annoyedByBoulderOrSquirt
    //INPUT: none
    //OUTPUT: boolean
    //DESCRIPTION: return true
    virtual bool annoyedByBoulderOrSquirt();
    
    //NAME: getAnnoyed
    //INPUT: integer i
    //OUTPUT: none
    //DESCRIPTION: decrements hit points by i
    virtual void getAnnoyed(int i);
    
    //NAME: getHitPoints
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: returns protester's hitpoints
    virtual int getHitPoints();
    
    //NAME: setTick
    //INPUT: integer
    //OUTPUT: none
    //DESCRIPTION: sets protesters tickcount to i
    void setTick(int i);
    
    //NAME: getTick
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: returns protester's tickcount
    int getTick();
    
    //NAME: getShout
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: returns number of ticks since protester's last shout
    int getShout();
    
    //NAME: setHitPoints
    //INPUT: integer
    //OUTPUT: none
    //DESCRIPTION: sets protester's hitpoints to i
    void setHitPoints(int i);
    
private:
    int tickCount;
    int lastShout;
    int hitPoints;
    int numSquaresToMoveInCurrentDirection;
    int lastTurn;
    std::vector<int> path;
    
};

class HardcoreProtester : public Protester {
public:
    //HardcoreProtester constructor
    HardcoreProtester(StudentWorld *w, int imageID = TID_HARD_CORE_PROTESTER, int hitPts = 20);
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: has hardcore protester function as specified while alive
    virtual void doSomething();
    
    //NAME: gotGold
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: tells hardcore protester they have picked up a goldnugget
    virtual void gotGold();
    
    //NAME: doSomething
    //INPUT: none
    //OUTPUT: boolean
    //DESCRIPTION: returns true
    virtual bool hardcore();
    
    //HardcoreProtester destructor
    virtual ~HardcoreProtester();
};

#endif // ACTOR_H_
