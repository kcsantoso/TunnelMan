#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GameController.h"
#include "Actor.h"
#include <vector>
#include <cmath>


class StudentWorld : public GameWorld
{
public:
    
    //StudentWorld constructor
    StudentWorld(std::string assetDir);
    
    //NAME: init
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: initializes the game world and returns a value telling the game to continue running
    virtual int init();
    
    //NAME: move
    //INPUT: none
    //OUTPUT: integer
    //DESCRIPTION: has each actor in the game world perform their respective functions during a tick, and returns a value telling the game to continue or end
    virtual int move();
    
    //NAME: cleanUp
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: clears the game world and deletes its actors
    virtual void cleanUp();
    
    //StudentWorld destructor
    virtual ~StudentWorld();
        
    //NAME: earthExists
    //INPUT: two integers
    //OUTPUT: boolean
    //DESCRIPTION: returns true if an earth object exists at the position (x, y) and false otherwise
    bool earthExists(int x, int y);
    
    //NAME: destroyEarth
    //INPUT: two integers
    //OUTPUT: boolean
    //DESCRIPTION: deletes an earth object at the position (x, y)
    void destroyEarth(int x, int y);
    
    //NAME: getDistance
    //INPUT: pointer to an actor, two integers
    //OUTPUT: double
    //DESCRIPTION: returns the distance between actor1 and the position (x, y)
    double getDistance(Actor* actor1, int x, int y);
    
    //NAME: validLocation
    //INPUT: two integers
    //OUTPUT: boolean
    //DESCRIPTION: returns true if a goody can be initialized at the position (x, y) and false otherwise
    bool validLocation(int x, int y);
    
    //NAME: getTunnelMan
    //INPUT: none
    //OUTPUT: pointer to a tunnelman
    //DESCRIPTION: returns a pointer to this game world's tunnelman object
    TunnelMan* getTunnelMan();
    
    //NAME: gotBarrel
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: tells the gameworld that the tunnelman found a barrel
    void gotBarrel();
    
    //NAME: setDisplayText
    //INPUT: none
    //OUTPUT: none
    //DESCRIPTION: displays top textbar with game's current stats
    void setDisplayText();
    
    //NAME: addActor
    //INPUT: pointer to actor
    //OUTPUT: none
    //DESCRIPTION: adds actor a to the gameworld
    void addActor(Actor* a);
    
    //NAME: makeVisible
    //INPUT: two integers
    //OUTPUT: none
    //DESCRIPTION: makes actor at position (x, y) visible
    void makeVisible(int x, int y);
    
    //NAME: addProtester
    //INPUT: integer
    //OUTPUT: none
    //DESCRIPTION: updates the count of how many protesters exist in the world
    void addProtester(int i);
    
    //NAME: gold
    //INPUT: pointer to goldnugget
    //OUTPUT: none
    //DESCRIPTION: tells gameworld that goldnugget g was placed in the world and responds properly
    void gold(goldNugget* g);

    //NAME: tunnelmanBlocked
    //INPUT: two integers
    //OUTPUT: boolean
    //DESCRIPTION: returns false if tunnelman can move one step in its direction, true if otherwise
    bool tunnelmanBlocked(int x, int y);
    
    //NAME: squirAnnoyProtester
    //INPUT: pointer to squirt
    //OUTPUT: boolean
    //DESCRIPTION: has squirt annoy protesters within its radius
    bool squirtAnnoyProtester(Squirt* s);
    
    //NAME: boulder
    //INPUT: pointer to boulder
    //OUTPUT: none
    //DESCRIPTION: tells gameworld that boulder b was placed in the world and responds properly
    void boulder(Boulder *b);
    
    //NAME: boulderBlocked
    //INPUT: pointer to boulder
    //OUTPUT: boolean
    //DESCRIPTION: returns false if boulder can continue falling, true if otherwise
    bool boulderBlocked(Boulder *b);
    
    //NAME: actorBlocked
    //INPUT: pointer to actor
    //OUTPUT: boolean
    //DESCRIPTION: returns false if actor can move one step in its direction, true if otherwise
    bool actorBlocked(Actor *s);
    
    //NAME: facingTunnelman
    //INPUT: pointer to protester
    //OUTPUT: boolean
    //DESCRIPTION: returns true if protester is facing the tunnelman, false if otherwise
    bool facingTunnelman(Protester *p);
    
    //NAME: pathToTunnelman
    //INPUT: pointer to actor
    //OUTPUT: boolean
    //DESCRIPTION: returns true if there is a straight clear path from actor a to the tunnelman, false if otherwise
    bool pathToTunnelman(Actor *a);
    
    //NAME: directionClear
    //INPUT: a direction, two integers
    //OUTPUT: boolean
    //DESCRIPTION: returns true if an object at position (x, y) can move one step in direction d, false if otherwise
    bool directionClear(GraphObject::Direction d, int x, int y);
    
    //NAME: findPath
    //INPUT: pointer ot actor, two integers, vector of integers passed by reference
    //OUTPUT: none
    //DESCRIPTION: finds paths to exit
    void findPaths();
    
    //NAME: getDirection
    //INPUT: 2 integers
    //OUTPUT: direction
    //DESCRIPTION: tells protester which direction to go to get to exit
    GraphObject::Direction getDirection(int x, int y);
    
private:
    Earth* earthObjects[64][60];
    TunnelMan* currentMan;
    std::vector<Actor *> actorObjects;
    int numBarrels;
    int numProtesters;
    int ticksSinceLastProtester;
    char paths[61][61];
    
};


#endif // STUDENTWORLD_H_
