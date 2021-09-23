#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"

void Actor::move() {
    if (getDirection() == up) moveTo(getX(), getY() + 1);   // move one step in specified direction
    else if (getDirection() == down) moveTo(getX(), getY() - 1);
    else if (getDirection() == right) moveTo(getX() + 1, getY());
    else moveTo(getX() - 1, getY());
}

void Actor::doSomething() { return; }

waterPool::waterPool(StudentWorld *w, int startX, int startY) : Goody(w, TID_WATER_POOL, startX, startY) {
    setState("temporary");
    int level = getWorld()->getLevel();
    lifeSpan = fmax(100, 300-10*level);
}

void Squirt::doSomething() {
    if (getWorld()->squirtAnnoyProtester(this)) return;
    if (travelDistance <= 0 || !getWorld()->directionClear(getDirection(), getX(), getY())) setState("dead");
    else move();
    travelDistance--;
}

void waterPool::doSomething() {
    Goody::doSomething(SOUND_GOT_GOODIE, 100);
    if (getWorld()->getDistance(getWorld()->getTunnelMan(), getX(), getY()) <= 3.0)
        getWorld()->getTunnelMan()->addWater();
    if (lifeSpan <= 0) setState("dead");
    else lifeSpan--;
}

sonarKit::sonarKit(StudentWorld *w) : Goody(w, TID_SONAR, 0, 60) {
    setState("temporary");
    int level = getWorld()->getLevel();
    lifeSpan = fmax(100, 300-10*level);
}

void sonarKit::doSomething() {
    Goody::doSomething(SOUND_GOT_GOODIE, 75);
    if (getWorld()->getDistance(getWorld()->getTunnelMan(), getX(), getY()) <= 3.0)
        getWorld()->getTunnelMan()->addSonar();
    if (lifeSpan <= 0) setState("dead");
    else lifeSpan--;
}

void goldNugget::doSomething() {
    if (getState() == "dead") return;
    else if (!isVisible() && (getWorld()->getDistance(getWorld()->getTunnelMan(), getX(), getY()) <= 4.0)) {
        setVisible(true);       // made visible when tunnelman near
        return;
    }
    if (getWorld()->getDistance(getWorld()->getTunnelMan(), getX(), getY()) <= 3.0 && getState() != "temporary") {
        setState("dead");       // picked up by tunnelman
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(25);
        getWorld()->getTunnelMan()->addGold();
    }
    if (getState() == "temporary") {
        if (lifeSpan <= 0) setState("dead");    // only able to be picked up by protesters
        lifeSpan--;
        getWorld()->gold(this);
    }
    
}

void Goody::doSomething(int sound, int points) {
    if (getState() == "dead") return;
    else if (!isVisible() && (getWorld()->getDistance(getWorld()->getTunnelMan(), getX(), getY()) <= 4.0)) {
        setVisible(true);       // made visible when tunnelman near
        return;
    }
    else if (getWorld()->getDistance(getWorld()->getTunnelMan(), getX(), getY()) <= 3.0) {
        setState("dead");       // picked up by tunnelman
        GameController::getInstance().playSound(sound);
        getWorld()->increaseScore(points);
    }
    
}

void oilBarrel::doSomething() { Goody::doSomething(SOUND_FOUND_OIL, 1000); }

oilBarrel::~oilBarrel() { getWorld()->gotBarrel(); }

void Boulder::doSomething() {
    if (getState() == "dead")
        return;
    
    if (getState() == "stable")
        if (!getWorld()->earthExists(getX(), getY() - 1) && !getWorld()->earthExists(getX() + 1, getY() - 1) && !getWorld()->earthExists(getX() + 2, getY() - 1) && !getWorld()->earthExists(getX() + 3, getY() - 1))
            setState("waiting");        // if no earth underneath
    
    if (getState() == "waiting") {      // waiting 30 ticks to fall
        if (ticksPassed >= 30) {
            setState("falling");
            GameController::getInstance().playSound(SOUND_FALLING_ROCK);
        }
        else ticksPassed++;
    }
    
    if (getState() == "falling") {
        if (getY() == 0 || getWorld()->earthExists(getX(), getY() - 1) || getWorld()->earthExists(getX() + 1, getY() - 1) || getWorld()->earthExists(getX() + 2, getY() - 1) || getWorld()->earthExists(getX() + 3, getY() - 1))
            setState("dead");       // dead if runs into earth or reaches bottom of screen
        else if (getWorld()->boulderBlocked(this)) setState("dead");        // dead if runs into other boulder
        else move();
        getWorld()->boulder(this);
    }
    
}

void TunnelMan::doSomething() {
    
    if (hitPoints <= 0) {
        setState("dead");
        GameController::getInstance().playSound(SOUND_PLAYER_GIVE_UP);
    }
    
    int ch;
    if (getWorld()->getKey(ch) == true) {
        
        if (ch == KEY_PRESS_UP) {       // move up
            int newY = getY();
            if (getDirection() != up) setDirection(up);
            else if (getY() + 1 <= 60) newY = getY() + 1;
            if (!getWorld()->tunnelmanBlocked(getX(), newY)) moveTo(getX(), newY);
        }
        
        if (ch == KEY_PRESS_DOWN) {     // move down
            int newY = getY();
            if (getDirection() != down) setDirection(down);
            else if (getY() - 1 >= 0) newY = getY() - 1;
            if (!getWorld()->tunnelmanBlocked(getX(), newY)) moveTo(getX(), newY);
        }
        
        if (ch == KEY_PRESS_RIGHT) {        // move right
            int newX = getX();
            if (getDirection() != right) setDirection(right);
            else if (getX() + 1 <= 60) newX = getX() + 1;
            if (!getWorld()->tunnelmanBlocked(newX, getY())) moveTo(newX, getY());
        }
        
        if (ch == KEY_PRESS_LEFT){      // move left
            int newX = getX();
            if (getDirection() != left) setDirection(left);
            else if (getX() - 1 >= 0) newX = getX() - 1;
            if (!getWorld()->tunnelmanBlocked(newX, getY())) moveTo(newX, getY());
        }
        
        if (ch == KEY_PRESS_ESCAPE)     // quit level
            setState("dead");
        
        if (ch == KEY_PRESS_SPACE) {    // shoot squirt
            if (water <= 0) return;
            GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
            water--;
            
            Squirt *s;
            // add squirt to game world
            if (getDirection() == right) s = new Squirt(getWorld(), getX() + 4, getY(), getDirection());
            else if (getDirection() == up) s = new Squirt(getWorld(), getX(), getY() + 4, getDirection());
            else if (getDirection() == left) s = new Squirt(getWorld(), getX() - 1, getY(), getDirection());
            else s = new Squirt(getWorld(), getX(), getY() - 1, getDirection());
            if (!getWorld()->directionClear(s->getDirection(), s->getX(), s->getY())) delete s;
            else getWorld()->addActor(s);
            
        }
        
        if (ch == 'z' || ch == 'Z') {       // use sonar charge
            if (sonarCharge <= 0) return;
            sonarCharge--;
            getWorld()->makeVisible(getX(), getY());
        }
        
        if (ch == KEY_PRESS_TAB) {      // drop gold nugget
            if (goldNuggets <= 0) return;
            goldNuggets--;
            goldNugget *g = new goldNugget(getWorld(), getX(), getY());
            g->setState("temporary");
            g->setLifeSpan(100);
            getWorld()->addActor(g);
        }
        
        bool earthDestroyed = false;        // destroy earth in path
        for (int i = getX(); i <= getX() + 3; i++)
            for (int j = getY(); j <= getY() + 3; j++) {
                if (j > 59) break;
                if (getWorld()->earthExists(i, j)) {
                    getWorld()->destroyEarth(i, j);
                    earthDestroyed = true;
                }
            }
        if (earthDestroyed == true) GameController::getInstance().playSound(SOUND_DIG);
    }
}

Protester::Protester(StudentWorld *w, int imageID, int hitPts) : Actor(w, imageID, 60, 60, left, 1.0, 0), tickCount(0), lastShout(0), hitPoints(hitPts), lastTurn(0) {
    w->addProtester(1);
    numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
}

//TODO fix
void HardcoreProtester::doSomething() { /*
    if (getState() != "dead" && getTick() >= fmax(0, 3 - getWorld()->getLevel()/4) && getState() != "leaveOilField" && (getWorld()->getDistance(getWorld()->getTunnelMan(), getX(), getY()) > 4.0 || getShout() < 15 || !getWorld()->facingTunnelman(this))) {
        setTick(0);
        
        getWorld()->findPath(this, getWorld()->getTunnelMan()->getX(), getWorld()->getTunnelMan()->getY(), path);
        if (path.size() / 2 <= 16 + getWorld()->getLevel() * 2) {
            int newX = path[0];
            int newY = path[1];
            path.erase(path.begin());
            path.erase(path.begin());
            if (newY < getY()) setDirection(down);
            else if (newY > getY()) setDirection(up);
            else if (newX > getX()) setDirection(right);
            else if (newX < getX()) setDirection(left);
            moveTo(newX, newY);
            return;
        } */
        Protester::doSomething();
    }


Protester::~Protester() { getWorld()->addProtester(-1);}

void Protester::doSomething() {
    
    if (getState() == "dead") return;
    
    if (tickCount < fmax(0, 3 - getWorld()->getLevel()/4)) {    // only do something every couple ticks
        tickCount ++;
        return;
    }
    
    tickCount = 0;
    lastShout++;
    lastTurn++;
    if (getState() == "leaveOilField") {
        
        if (getX() == 60 && getY() == 60) {
            setState("dead");
            return;
        }
        getWorld()->findPaths();
        Direction d = getWorld()->getDirection(getX(), getY());
        setDirection(d);
        move();
        return;
    }
    
    else if (getWorld()->getDistance(getWorld()->getTunnelMan(), getX(), getY()) <= 4.0 && lastShout >= 15 && getWorld()->facingTunnelman(this)) {
        GameController::getInstance().playSound(SOUND_PROTESTER_YELL);  // yell at tunnelman
        getWorld()->getTunnelMan()->getAnnoyed(2);
        lastShout = 0;
        return;
    }
    // if path to tunnelman, face tunnelman and move
    else if ((getWorld()->getTunnelMan()->getX() == getX() || getWorld()->getTunnelMan()->getY() == getY()) && getWorld()->getDistance(getWorld()->getTunnelMan(), getX(), getY()) > 4 && getWorld()->pathToTunnelman(this) && getWorld()->directionClear(getDirection(), getX(), getY())) {
        if (getWorld()->getTunnelMan()->getX() == getX() && getWorld()->getTunnelMan()->getY() > getY()) setDirection(up);
        else if (getWorld()->getTunnelMan()->getX() == getX() && getWorld()->getTunnelMan()->getY() < getY()) setDirection(down);
        else if (getWorld()->getTunnelMan()->getY() == getY() && getWorld()->getTunnelMan()->getX() < getX()) setDirection(left);
        else if (getWorld()->getTunnelMan()->getY() == getY() && getWorld()->getTunnelMan()->getX() > getX()) setDirection(right);
        move();
        numSquaresToMoveInCurrentDirection = 0;
        return;
    }
    
    numSquaresToMoveInCurrentDirection--;
    
    // find new clear direction to move in
    if (numSquaresToMoveInCurrentDirection <= 0) {
        bool directionFound = false;
        do {
            int n = rand() % 4;
            if (n == 0 && getWorld()->directionClear(up, getX(), getY())) setDirection(up);
            else if (n == 1 && getWorld()->directionClear(down, getX(), getY())) setDirection(down);
            else if (n == 2 && getWorld()->directionClear(right, getX(), getY())) setDirection(right);
            else if (n == 3 && getWorld()->directionClear(left, getX(), getY())) setDirection(left);
            
            if (getWorld()->directionClear(getDirection(), getX(), getY())) {
                numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
                directionFound = true;
            }
        } while(!directionFound);
    }
    // see if can turn perpendicular direction, and turn if so
    else {
        bool canTurn = false;
        if (getDirection() == left || getDirection() == right)
            canTurn = getWorld()->directionClear(up, getX(), getY()) || getWorld()->directionClear(down, getX(), getY());
        else canTurn = getWorld()->directionClear(right, getX(), getY()) || getWorld()->directionClear(left, getX(), getY());
        
        
        if (canTurn && lastTurn > 200) {
            int n = rand() % 2;
            if (getDirection() != left && getDirection() != right) {
                if (n == 0) {
                    if (getWorld()->directionClear(left, getX(), getY())) setDirection(left);
                    else setDirection(right);
                }
                else {
                    if (getWorld()->directionClear(right, getX(), getY())) setDirection(right);
                    else setDirection(left);
                }
            }
            else {
                if (n == 0) {
                    if (getWorld()->directionClear(up, getX(), getY())) setDirection(up);
                    else setDirection(down);
                }
                else {
                    if (getWorld()->directionClear(down, getX(), getY())) setDirection(down);
                    else setDirection(up);
                }
            }
            numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
            lastTurn = 0;
        }
    }
    
    if (!getWorld()->directionClear(getDirection(), getX(), getY())) numSquaresToMoveInCurrentDirection = 0;
    else move();
}


void Protester::getAnnoyed(int i) {
    hitPoints -= i;
    if (hitPoints > 0) {        // protester annoyed
        GameController::getInstance().playSound(SOUND_PROTESTER_ANNOYED);
        int level = getWorld()->getLevel();
        tickCount = -fmax(50, 100 - level * 10) + fmax(0, 3 - getWorld()->getLevel()/4);
    }
    else {                      // protester dead
        GameController::getInstance().playSound(SOUND_PROTESTER_GIVE_UP);
        setState("leaveOilField");
        tickCount = fmax(0, 3 - getWorld()->getLevel()/4);
    }
}


void Protester::gotGold() {
    GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    setState("leaveOilField");
}

void HardcoreProtester::gotGold() {
    GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(50);
    setTick(-fmax(50, 100  - getWorld()->getLevel() * 10) + fmax(0, 3 - getWorld()->getLevel()/4));
}



Actor::Actor (StudentWorld* w, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : GraphObject(imageID, startX, startY, dir, size, depth), myWorld(w), state("alive") { setVisible(true); }

Actor::~Actor() {}
    
void Actor::gotGold() {return;}

int Actor::getHitPoints() {return 0;}

bool Actor::blocksTunnelman() {return false;}

bool Actor::annoyedByBoulderOrSquirt() {return false;}

StudentWorld* Actor::getWorld() {return myWorld;}

void Actor::setState(std::string newState) {state = newState;}

std::string Actor::getState() {return state;}

void Actor::getAnnoyed(int i) {return;}

bool Actor::hardcore() {return false;}

Squirt::Squirt(StudentWorld* w, int startX, int startY, Direction d) : Actor(w, TID_WATER_SPURT, startX, startY, d, 1.0, 1), travelDistance(4) {}

Squirt::~Squirt() {}

Goody::Goody (StudentWorld* w, int imageID, int startX, int startY) : Actor(w, imageID, startX, startY, right, 1.0, 2) {}

waterPool::~waterPool() {}

sonarKit::~sonarKit() {}

oilBarrel::oilBarrel (StudentWorld* w, int startX, int startY) : Goody(w, TID_BARREL, startX, startY) {}

goldNugget::goldNugget (StudentWorld* w, int startX, int startY) : Goody(w, TID_GOLD, startX, startY) { setState("permanent"); }

goldNugget::~goldNugget() {}

void goldNugget::setLifeSpan(int i) {lifeSpan = i;}

Earth::Earth (StudentWorld* w, int startX, int startY) : Actor (w, TID_EARTH, startX, startY, right, 0.25, 3) {}

void Earth::doSomething() { return; }

Earth::~Earth() {}

Boulder::Boulder (StudentWorld* w, int startX, int startY) : Actor (w, TID_BOULDER, startX, startY, down, 1.0, 1),  ticksPassed(0) { setState("stable"); }

bool Boulder::blocksTunnelman() {return true;}

Boulder::~Boulder() {}

TunnelMan::TunnelMan (StudentWorld* w) : Actor (w, TID_PLAYER, 30, 60), hitPoints(10), water(5), sonarCharge(0), goldNuggets(0) {}

TunnelMan::~TunnelMan() {}

int TunnelMan::getWater() { return water; }

int TunnelMan::getSonarCharge() { return sonarCharge; }

int TunnelMan::getGold() { return goldNuggets; }

void TunnelMan::addSonar() {sonarCharge++;}

void TunnelMan::addWater() {water += 5;}

void TunnelMan::addGold() { goldNuggets++; }

void TunnelMan::getAnnoyed(int i) {hitPoints -= i;}

int TunnelMan::getHitPoints() {return hitPoints;}

bool Protester::annoyedByBoulderOrSquirt() {return true;}

int Protester::getHitPoints() {return hitPoints;}

void Protester::setTick(int i) {tickCount = i;}

int Protester::getTick() {return tickCount;}

int Protester::getShout() {return lastShout;}

void Protester::setHitPoints(int i) {hitPoints = i;}

HardcoreProtester::HardcoreProtester(StudentWorld *w, int imageID, int hitPts) : Protester(w, imageID, hitPts) {}

bool HardcoreProtester::hardcore() {return true;}

HardcoreProtester::~HardcoreProtester() {}
