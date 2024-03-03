#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// #include "SoundFX.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, int dir = none) : GraphObject(imageID, startX, startY, dir), m_world(world), m_alive(true) { setVisible(true); }
	virtual void doSomething();
	virtual void uniqueAction() { return; } // specialized action of the actor
	virtual bool canMoveOn() const { return false; } // if the actor can be moved on by a player or a bot
	virtual bool marbleCanPass() const { return false; } // if the actor can be moved on by a marble
	virtual bool attackable() const { return false; } // if the actor is able to block a pea
	virtual bool willAttack() const { return false; } // if the actor can shoot peas
	virtual bool willSteal() const { return false; } // if the actor can steal things
	virtual bool swallowable() const { return false; } // if the actor can be swallowed by a pit
	virtual bool bobotic() const { return false; } // if the actor is robotic
	virtual bool stealable() const { return false; } // if the actor is able to be stolen by a ThiefBot or MeanThiefBot
	virtual bool bePushedTo(int dir) { return false; } // if the actor is able to be pushed in the specified direction
	virtual void damaged() { return; } // called when actor is hit by a pea
	virtual void setHp(int hp) { m_hp = hp; }
	void setAmmo(int ammo) { m_ammo = ammo; }
	int getHp() const { return m_hp; }
	int getAmmo() const { return m_ammo; }
	bool isAlive() const { return m_alive; }
	void kill() { m_alive = false; }
	StudentWorld* getWorld() const { return m_world; }
private:
	StudentWorld* m_world; // points to the StudentWorld that the actor is in
	bool m_alive; // keeps track of whether or not the actor should be removed from the game
	int m_hp; // keeps track of actor's hp if it has hp
	int m_ammo; // keeps track of actor's ammo if it has ammo
};

class Agent : public Actor
{
public:
	Agent(StudentWorld* world, int imageID, double startX, double startY, int dir = right) : Actor(world, imageID, startX, startY, dir) {}
	virtual bool attackable() const { return true; }
	virtual bool willAttack() const { return true; }
	virtual void damaged();
	virtual void impact() { return; }; // called when agent is damaged but not dead
	virtual void dead() { return; }; // called when agent is damaged and dies
	void fire();
};

class Avatar : public Agent
{
public:
	Avatar(StudentWorld* world, double startX, double startY) : Agent(world, IID_PLAYER, startX, startY) { setHp(20); setAmmo(20); }
	virtual void uniqueAction();
	virtual void impact();
	virtual void dead();
};

class Bot : public Agent
{
public:
	Bot(StudentWorld* world, int imageID, int startX, int startY, int bonus, int dir = right) : Agent(world, imageID, startX, startY, dir), m_tickCount(0), m_bonus(bonus) {}
	virtual bool bobotic() const { return true; }
	virtual void uniqueAction();
	virtual bool botAction();
	virtual void moveBot() = 0; // handles bot movement
	virtual void impact();
	virtual void dead();
	bool canMoveInDir(int dir); // checks if bot can move in a specified direction; if it can, it moves the bot
private:
	int m_tickCount; // keeps track of the ticks passed since the bot last took action
	int m_bonus; // specifies the bonus points gained for killing the agent
	bool takeAction(); // checks if bot can take action this tick
	bool playerShootable(); // checks if the bot has a clear shot to the player
};

class RageBot : public Bot
{
public:
	RageBot(StudentWorld* world, int startX, int startY, int dir) : Bot(world, IID_RAGEBOT, startX, startY, 100, dir) { setHp(10); }
	virtual void moveBot();
};

class ThiefBot : public Bot
{
public:
	ThiefBot(StudentWorld* world, int startX, int startY, int bonus = 10, int imageID = IID_THIEFBOT) : Bot(world, imageID, startX, startY, bonus), m_turnDist(randInt(1, 6)), m_squaresMoved(0), m_goodie(nullptr) { setHp(5); }
	virtual bool botAction();
	virtual void moveBot();
	virtual void dead();
	virtual bool willAttack() const { return false; }
	virtual bool willSteal() const { return true; }
private:
	int m_turnDist; // specifies the distance the bot should attempt to move before turning
	int m_squaresMoved; // keeps track of the distance the bot has moved since its last turn
	Actor* m_goodie; // keeps track of the goodie the bot has stolen, if it has stolen one
};


class MeanThiefBot : public ThiefBot
{
public:
	MeanThiefBot(StudentWorld* world, int startX, int startY) : ThiefBot(world, startX, startY, 20, IID_MEAN_THIEFBOT) { setHp(8); }
	virtual bool botAction();
	virtual bool willAttack() const { return true; }
};



class Factory : public Actor
{
public:
	Factory(StudentWorld* world, int startX, int startY, bool mean) : Actor(world, IID_ROBOT_FACTORY, startX, startY), m_mean(mean) {}
	virtual bool attackable() const { return true; }
	virtual void uniqueAction();
private:
	bool m_mean; // specifies if factory produces MeanThiefBots or regular ThiefBots
	int getNumThiefs(); // counts number of MeanThiefBots and ThiefBots in the vicinity
};


class Marble : public Agent
{
public:
	Marble(StudentWorld* world, double startX, double startY) : Agent(world, IID_MARBLE, startX, startY, none) { setHp(10); }
	virtual bool canMoveOn() const { return false; }
	virtual bool swallowable() const { return true; }
	virtual bool bePushedTo(int dir);
};

class Pit : public Actor
{
public:
	Pit(StudentWorld* world, double startX, double startY) : Actor(world, IID_PIT, startX, startY) {}
	virtual void uniqueAction();
	virtual bool marbleCanPass() const { return true; }
};

class Wall : public Actor
{
public:
	Wall(StudentWorld* world, double startX, double startY) : Actor(world, IID_WALL, startX, startY) {}
	virtual bool attackable() const { return true; }
};

class Pea : public Actor
 {
 public:
	Pea(StudentWorld* world, double startX, double startY, int dir) : Actor(world, IID_PEA, startX, startY, dir) {}
	virtual void uniqueAction();
 private:
	 void peaShooter(); // handles pea damage to actors
	 void movePea(); // handles pea movement
 };

class Item : public Actor
{
public:
	Item(StudentWorld* world, int imageID, int startX, int startY, int bonus) : Actor(world, imageID, startX, startY), m_bonus(bonus) { setVisible(true); }
	virtual bool canMoveOn() const { return true; }
	virtual bool stealable() const { return true; }
	virtual void doSomething();
	virtual void uniqueAction();
	virtual void bonusAction() = 0; // differentiated action of each item
private:
	int m_bonus; // specifies the bonus points gained for picking up the goodie
};

class Crystal : public Item
{
public:
	Crystal(StudentWorld* world, int startX, int startY) : Item(world, IID_CRYSTAL, startX, startY, 50) {}
	virtual bool stealable() const { return false; }
	virtual void bonusAction();
};

class Life : public Item
{
public:
	Life(StudentWorld* world, int startX, int startY) : Item(world, IID_EXTRA_LIFE, startX, startY, 1000) {}
	virtual void bonusAction();
};

class Health : public Item
{
public:
	Health(StudentWorld* world, int startX, int startY) : Item(world, IID_RESTORE_HEALTH, startX, startY, 500) {}
	virtual void bonusAction();
};

class Ammo : public Item
{
public:
	Ammo(StudentWorld* world, int startX, int startY) : Item(world, IID_AMMO, startX, startY, 100) {}
	virtual void bonusAction();
};

class Exit : public Actor
{
public:
	Exit(StudentWorld* world, int startX, int startY) : Actor(world, IID_EXIT, startX, startY) { setVisible(false); }
	virtual bool canMoveOn() const { return true; }
	virtual void doSomething();
};
#endif // ACTOR_H_
