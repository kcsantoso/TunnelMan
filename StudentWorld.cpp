#include "StudentWorld.h"
#include <string>
using namespace std;
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <queue>
#include <stack>

const int GAME_WORLD_BOUND = 60;
const int ACTOR_SIZE = 4;
const int EARTH_BOUND = 64;

GameWorld* createStudentWorld(string assetDir) { return new StudentWorld(assetDir); }

int StudentWorld::init()
{
    numProtesters = 0;
    ticksSinceLastProtester = fmax(25, 200 - getLevel());
    
    // create earth
    srand(time(0));
    for (int i = 0; i < EARTH_BOUND; i++)
        for (int j = 0; j < GAME_WORLD_BOUND; j++) {
            if (i < 30 || i > 33 || (j < ACTOR_SIZE)) earthObjects[i][j] = new Earth(this, i, j);
            else earthObjects[i][j] = nullptr;
        }
    
    // create tunnelman
    currentMan = new TunnelMan(this);
    
    // create boulders
    for (int i = 0; i < fmin(getLevel() / 2 + 2, 9); i++) {
        int x = rand() % 61;
        int y = rand() % 37 + 20;
        do {
            x = rand() % 61;
            y = rand() % 37 + 20;
        } while(!validLocation(x, y));
        actorObjects.push_back(new Boulder(this, x, y));
        for (int j = 0; j < ACTOR_SIZE; j++)
            for (int k = 0; k < ACTOR_SIZE; k++) {
                delete earthObjects[actorObjects[i]->getX() + j][actorObjects[i]->getY() + k];
                earthObjects[actorObjects[i]->getX() + j][actorObjects[i]->getY() + k] = nullptr;
            }
    }
    
    // create gold nuggets
    for (int i = 0; i < fmax(5- getLevel()/2, 2); i++) {
        int x = rand() % 61;
        int y = rand() % 57;
        do {
            x = rand() % 61;
            y = rand() % 57;
        } while(!validLocation(x, y));
        goldNugget *g = new goldNugget(this, x, y);
        g->setState("permanent");
        g->setVisible(false);
        actorObjects.push_back(g);
    }
    
    // create oil barrels
    numBarrels = fmin(2 + getLevel(), 21);
    for (int i = 0; i < fmin(2 + getLevel(), 21); i++) {
        int x = rand() % 61;
        int y = rand() % 57;
        do {
            x = rand() % 61;
            y = rand() % 57;
        } while(!validLocation(x, y));
        oilBarrel *o = new oilBarrel(this, x, y);
        o->setVisible(false);
        actorObjects.push_back(o);
    }
    
    
    return GWSTATUS_CONTINUE_GAME;

}

int StudentWorld::move()
{
    setDisplayText();
    
    // create sonar kit or water pool
    int g = rand() % (getLevel()*25 + 300);
    if (g == 0) {
        int h = rand() % 5;
        if (h == 0) actorObjects.push_back(new sonarKit(this));
        else {
            bool locationFound = true;
            do {
                int x = rand() % GAME_WORLD_BOUND;
                int y = rand() % GAME_WORLD_BOUND;
                locationFound = true;
                
                for (int i = 0; i < ACTOR_SIZE; i++) {
                    for (int j = 0; j < ACTOR_SIZE; j++)
                        if (earthExists(x + j, y + i)) locationFound = false;
                }
                
                if (locationFound) actorObjects.push_back(new waterPool(this, x, y));
            } while (!locationFound);
        }
    }
    
    
    // create new protester
    if (ticksSinceLastProtester >= fmax(25, 200 - getLevel()) && numProtesters <= fmin(15, 2 + getLevel() * 1.5)) {
        numProtesters++;
        ticksSinceLastProtester = 0;
        int probabilityOfHardcore = fmin(90, getLevel() * 10 + 30);
        int i = rand() % 100;
        if (i < probabilityOfHardcore) actorObjects.push_back(new HardcoreProtester(this));
        else actorObjects.push_back(new Protester(this));
    }
    else ticksSinceLastProtester++;
    
    // have each actor in game world do something
    currentMan->doSomething();
    
    for (int i = 0; i < actorObjects.size(); i++)
        actorObjects[i]->doSomething();
    // delete dead actors
    bool done = false;
    do {
        for (int i = 0; i < actorObjects.size(); i++) {
        if (actorObjects[i]->getState() == "dead") {
            delete actorObjects[i];
            actorObjects.erase(actorObjects.begin() + i);
            break;
        }}
        done = true;
    } while (done == false);
    
    
    // if tunnelman dead, level is lost
    if (currentMan->getState() == "dead") {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
        // if all barrels collected, level complete
        if (numBarrels <= 0) {
            GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    // otherwise continue game
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete currentMan;
    // delete tunnelman and earth
    for (int i = 0; i < EARTH_BOUND; i++)
        for (int j = 0; j < GAME_WORLD_BOUND; j++) {
            if (earthObjects[i][j] != nullptr)
                delete earthObjects[i][j];
        }
    // delete all other actors
    for (int i = 0; i < actorObjects.size(); i++)
        if (actorObjects[i] != nullptr) delete actorObjects[i];
    actorObjects.clear();
}

StudentWorld::~StudentWorld() {
    // delete tunnelman and eart
    if (currentMan != nullptr) delete currentMan;
    for (int i = 0; i < EARTH_BOUND; i++)
        for (int j = 0; j < GAME_WORLD_BOUND; j++) {
            if (earthObjects[i][j] != nullptr)
                delete earthObjects[i][j];
        }
    // delete all other actors
    for (int i = 0; i < actorObjects.size(); i++)
        if (actorObjects[i] != nullptr) delete actorObjects[i];
    actorObjects.clear();
}


// see if earth exists at (x, y)
bool StudentWorld::earthExists(int x, int y) {
    if (y > 59) return false;
    else return earthObjects[x][y] != nullptr;
    
}

void StudentWorld::destroyEarth(int x, int y) {
    delete earthObjects[x][y];
    earthObjects[x][y] = nullptr;
}


bool StudentWorld::validLocation(int x, int y) {
    for (int j = 0; j < actorObjects.size(); j++) // checking location to initialize goodies
        if (getDistance(actorObjects[j], x, y) <= 6.0) // cannot be within a radius of 6 of each other
            return false;
    if (x >= 27 && x <= 33 && y >= ACTOR_SIZE)   // cannot be in middle column
        return false;
    return true;
}


void StudentWorld::setDisplayText()
{
    std::ostringstream oss;
    oss << "Lvl: " << std::setw(2) << getLevel();
    oss << "  Lives: " << getLives();
    oss << "  Hlth: " << std::setw(3) << currentMan->getHitPoints() * 10 << "%";
    oss << "  Wtr: " << std::setw(2) << currentMan->getWater();
    oss << "  Gld: " << std::setw(2) << currentMan->getGold();
    oss << "  Oil Left: " << std::setw(2) << numBarrels;
    oss << "  Sonar: " << std::setw(2) << currentMan->getSonarCharge();
    oss.fill('0');
    oss << "  Scr: " << std::setw(6) << getScore();
    std::string s = oss.str();
    setGameStatText(s);
}

void StudentWorld::makeVisible(int x, int y) {
    for (int i = 0; i < actorObjects.size(); i++)       // when sonar used
        if (getDistance(actorObjects[i], x, y) < 12) actorObjects[i]->setVisible(true);
}


void StudentWorld::gold(goldNugget* g) {
    for (int i = 0; i < actorObjects.size(); i++)       // alerts game world that gold nugget is in world
        if (getDistance(actorObjects[i], g->getX(), g->getY()) <= 3 && g->getState() == "temporary" && actorObjects[i]->annoyedByBoulderOrSquirt()) {
            g->setState("dead");
            actorObjects[i]->gotGold();
            return;
        }
}



bool StudentWorld::squirtAnnoyProtester(Squirt *s) {    // annoys protester if in range of squirt
    for (int i = 0; i < actorObjects.size(); i++)
        if (getDistance(actorObjects[i], s->getX(), s->getY()) <= 3 && actorObjects[i]->annoyedByBoulderOrSquirt()) {
            actorObjects[i]->getAnnoyed(2);
            if (actorObjects[i]->getHitPoints() <= 0 && !actorObjects[i]->hardcore()) increaseScore(100);
            else if (actorObjects[i]->getHitPoints() <= 0 && actorObjects[i]->hardcore()) increaseScore(250);
            s->setState("dead");
            return true;
        }
    return false;
}

void StudentWorld::boulder(Boulder *b) {    // alerts game world that boulder is falling in world
    if (getDistance(getTunnelMan(), b->getX(), b->getY()) <= 3)
        getTunnelMan()->getAnnoyed(100);
    for (int i = 0; i < actorObjects.size(); i++)
        if (getDistance(actorObjects[i], b->getX(), b->getY()) <= 3 && actorObjects[i]->annoyedByBoulderOrSquirt()) {
            actorObjects[i]->getAnnoyed(100);
            if (actorObjects[i]->getHitPoints() <= 0) increaseScore(500);
        }
}

bool StudentWorld::tunnelmanBlocked(int x, int y) {
    for (int i = 0; i < actorObjects.size(); i++)   // see if tunnelman blocked by boulder
        if (getDistance(actorObjects[i], x, y) <= 3 && actorObjects[i]->blocksTunnelman())
            return true;
    return false;
}

bool StudentWorld::boulderBlocked(Boulder *b) { // see if boulder blocked by other boulder
    for (int i = 0; i < actorObjects.size(); i++)
        if ((actorObjects[i]->getX() - b->getX() <= 3 || actorObjects[i]->getX() - b->getX() >= -3) && actorObjects[i]->getY() + ACTOR_SIZE >= b->getY() && actorObjects[i]->blocksTunnelman() && actorObjects[i] != b)
            return true;
    return false;
}

bool StudentWorld::facingTunnelman(Protester *p) {  // check if p is facing tunnelman
    if (p->getDirection() == GraphObject::up && currentMan->getY() > p->getY()) return true;
    else if (p->getDirection() == GraphObject::down && currentMan->getY() < p->getY()) return true;
    else if (p->getDirection() == GraphObject::right && currentMan->getX() > p->getX()) return true;
    else if (p->getDirection() == GraphObject::left && currentMan->getX() < p->getX()) return true;
    else return false;
}

bool StudentWorld::pathToTunnelman(Actor *a) {
    // checks for straight clear path to tunnelman above a
    if (a->getDirection() == GraphObject::up) {
        for (int i = a->getY(); i < currentMan->getY(); i++) {
            for (int j = 0; j < actorObjects.size(); j++)
                if (getDistance(actorObjects[j], a->getX(), i) <= 3 && actorObjects[j]->blocksTunnelman())
                    return false;
            for (int k = 0; k < ACTOR_SIZE; k++) {
                if (earthExists(a->getX() + k, i))
                    return false;
            }
        }
    }
    // checks for straight clear path to tunnelman below a
    else if (a->getDirection() == GraphObject::down) {
        for (int i = a->getY(); i > currentMan->getY(); i--) {
            for (int j = 0; j < actorObjects.size(); j++)
                if (getDistance(actorObjects[j], a->getX(), i) <= 3 && actorObjects[j]->blocksTunnelman())
                    return false;
            for (int k = 0; k < ACTOR_SIZE; k++) {
                if (earthExists(a->getX() + k, i))
                    return false;
            }
        }
    }
    
    // checks for straight clear path to tunnelman to right of a
    else if (a->getDirection() == GraphObject::right) {
        for (int i = a->getX(); i < currentMan->getX(); i++) {
            for (int j = 0; j < actorObjects.size(); j++)
                if (getDistance(actorObjects[j], i, a->getY()) <= 3 && actorObjects[j]->blocksTunnelman())
                    return false;
            for (int k = 0; k < ACTOR_SIZE; k++) {
                if (earthExists(i, a->getY() + k))
                    return false;
            }
        }
    }
    
    // checks for straight clear path to tunnelman to left of a
    else {
        for (int i = a->getX(); i > currentMan->getX(); i--) {
            for (int j = 0; j < actorObjects.size(); j++)
                if (getDistance(actorObjects[j], i, a->getY()) <= 3 && actorObjects[j]->blocksTunnelman())
                    return false;
            for (int k = 0; k < ACTOR_SIZE; k++) {
                if (earthExists(i, a->getY() + k))
                    return false;
            }
        }
    }
    
    return true;
}

// TODO: see if issues with this (sometimes protesters walk into earth)
bool StudentWorld::directionClear(GraphObject::Direction d, int x, int y) {
    if (d == GraphObject:: up) {
        if (y + 1 > GAME_WORLD_BOUND) return false;   // out of bounds
        for (int i = 0; i < actorObjects.size(); i++)
            if (getDistance(actorObjects[i], x, y + 1) <= 3 && actorObjects[i]->blocksTunnelman())
                return false;       // blocked by boulder
        for (int i = 0; i < ACTOR_SIZE; i++)
            if (earthExists(x + i, y + ACTOR_SIZE))
                return false;       // blocked by earth
    }
    
    else if (d == GraphObject::down) {
        if (y - 1 < 0) return false;        // out of bounds
        for (int i = 0; i < actorObjects.size(); i++)
            if (getDistance(actorObjects[i], x, y - 1) <= 3 && actorObjects[i]->blocksTunnelman())
                return false;       // blocked by boulder
        for (int i = 0; i < ACTOR_SIZE; i++)
            if (earthExists(x + i, y - 1))
                return false;       // blocked by earth
    }
    
    else if (d == GraphObject::left) {
        if (x - 1 < 0) return false;        // out of bounds
        for (int i = 0; i < actorObjects.size(); i++)
            if (getDistance(actorObjects[i], x - 1, y) <= 3 && actorObjects[i]->blocksTunnelman())
                return false;       // blocked by boulder
        for (int i = 0; i < ACTOR_SIZE; i++)
            if (earthExists(x - 1, y + i))
                return false;       // blocked by earth
    }
    
    else if (d == GraphObject::right) {
        if (x + 1 > GAME_WORLD_BOUND) return false;       // out of bounds
        for (int i = 0; i < actorObjects.size(); i++)
            if (getDistance(actorObjects[i], x + ACTOR_SIZE, y) <= 3 && actorObjects[i]->blocksTunnelman())
                return false;       // blocked by boulder
        for (int i = 0; i < ACTOR_SIZE; i++)
            if (earthExists(x + ACTOR_SIZE, y + i))
                return false;       // blocked by eart
    }
    
    return true;        // path exists
}


void StudentWorld::findPaths() {
    for (int i = 0; i < 61; i++)
        for (int j = 0; j < 61; j++)
            paths[i][j] = 'e';
    queue<int> q;
    paths[GAME_WORLD_BOUND][GAME_WORLD_BOUND] = 'x';
    q.push(GAME_WORLD_BOUND);
    q.push(GAME_WORLD_BOUND);
    
    while (!q.empty()) {
        
        int x = q.front();
        q.pop();
        int y = q.front();
        q.pop();
        if (directionClear(GraphObject::up, x, y) && paths[x][y + 1] == 'e') {
            paths[x][y + 1] = 'd';
            q.push(x);
            q.push(y + 1);
        }
        if (directionClear(GraphObject::down, x, y) && paths[x][y - 1] == 'e') {
            paths[x][y - 1] = 'u';
            q.push(x);
            q.push(y - 1);
        }
        if (directionClear(GraphObject::right, x, y) && paths[x + 1][y] == 'e') {
            paths[x + 1][y] = 'l';
            q.push(x + 1);
            q.push(y);
        }
        if (directionClear(GraphObject::left, x, y) && paths[x - 1][y] == 'e') {
            paths[x - 1][y] = 'r';
            q.push(x - 1);
            q.push(y);
        }
        
    }
}


double StudentWorld::getDistance(Actor* actor1, int x, int y) { return (sqrt(pow((actor1->getX() - x), 2) + pow((actor1->getY() - y), 2)));}

TunnelMan* StudentWorld::getTunnelMan() { return currentMan; }

void StudentWorld::gotBarrel() { numBarrels--; }

void StudentWorld::addActor(Actor* a) {actorObjects.push_back(a);}

void StudentWorld::addProtester(int i) {numProtesters += i;}

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir) {}

GraphObject::Direction StudentWorld::getDirection(int x, int y) {
    if (paths[x][y] == 'u') return GraphObject::up;
    else if (paths[x][y] == 'd') return GraphObject::down;
    else if (paths[x][y] == 'r') return GraphObject::right;
    else return GraphObject::left;
    
}
