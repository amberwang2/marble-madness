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
	virtual void uniqueAction() { return; }
	virtual bool canMoveOn() const { return false; }
	virtual bool canMove() const { return false; }
	virtual bool MarbleCanPass() const { return false; }
	virtual bool attackable() const { return false; }
	virtual bool swallowable() const { return false; }
	virtual bool bobotic() const { return false; }
	virtual bool stealable() const { return false; }
	virtual bool bePushedTo(int dir) { return false; }
	virtual void damaged() { return; }
	virtual void setHp(int hp) { m_hp = hp; }
	int getHp() const { return m_hp; }
	void setAmmo(int ammo) { m_ammo = ammo; }
	int getAmmo() const { return m_ammo; }
	bool isAlive() const { return m_alive; }
	void kill() { m_alive = false; }
	void revive() { m_alive = true; }
	StudentWorld* getWorld() const { return m_world; }
private:
	StudentWorld* m_world;
	bool m_alive;
	int m_hp;
	int m_ammo;
};

class Agent : public Actor
{
public:
	Agent(StudentWorld* world, int imageID, double startX, double startY, int dir = right) : Actor(world, imageID, startX, startY, dir) {}
	virtual bool canMove() const { return true; }
	virtual bool attackable() const { return true; }
	virtual void damaged() {
		setHp(getHp() - 2);
		damagedAction();
	}
	virtual void damagedAction() {
		if (healthCheck())
		{
			impact();
			// something
		}
		else
		{
			dead();
			// dead
		}
	}
	virtual void impact() {}; // play impact sound
	virtual void dead() {}; // play death sound
	bool healthCheck() {
		if (getHp() <= 0) {
			kill();
			return false;
		}
		return true;
	}
	void fire();
private:
	int m_bonus;
};

class Avatar : public Agent
{
public:
	Avatar(StudentWorld* world, double startX, double startY) : Agent(world, IID_PLAYER, startX, startY) { setHp(20); setAmmo(20); }
	virtual void uniqueAction();
	virtual void impact();
	virtual void dead();
private:
};

class Bot : public Agent
{
public:
	Bot(StudentWorld* world, int imageID, int startX, int startY, int bonus, int dir = right) : Agent(world, imageID, startX, startY, dir), m_tickCount(0), m_bonus(bonus) {}
	virtual bool bobotic() const { return true; }
	virtual void uniqueAction();
	virtual bool botAction();
	virtual void moveBot() = 0;
	bool canMoveInDir();
	bool playerShootable();
	virtual void impact();
	virtual void dead();
	bool takeAction();
private:
	int m_tickCount;
	int m_bonus;
};

class RageBot : public Bot
{
public:
	RageBot(StudentWorld* world, int startX, int startY, int dir) : Bot(world, IID_RAGEBOT, startX, startY, 100, dir) { setHp(10); }
	virtual void moveBot();
private:
};

class ThiefBot : public Bot
{
public:
	ThiefBot(StudentWorld* world, int startX, int startY, int bonus, int imageID = IID_THIEFBOT) : Bot(world, imageID, startX, startY, bonus), m_turnDist(randInt(1, 6)), m_squaresMoved(0), m_goodie(nullptr) { setHp(5); }
	virtual bool botAction();
	virtual void moveBot();
private:
	int m_turnDist;
	int m_squaresMoved;
	Actor* m_goodie;
};

class Marble : public Agent
{
public:
	Marble(StudentWorld* world, double startX, double startY) : Agent(world, IID_MARBLE, startX, startY, none) { setHp(10); }
	virtual bool canMoveOn() const { return false; }
	virtual bool swallowable() const { return true; }
	virtual bool bePushedTo(int dir);
private:
};

class Pit : public Actor
{
public:
	Pit(StudentWorld* world, double startX, double startY) : Actor(world, IID_PIT, startX, startY) {}
	virtual void uniqueAction();
	virtual bool MarbleCanPass() const { return true; }
private:
};

class Wall : public Actor
{
public:
	Wall(StudentWorld* world, double startX, double startY) : Actor(world, IID_WALL, startX, startY) {}
	virtual bool attackable() const { return true; }
private:
};

class Pea : public Actor
 {
 public:
	Pea(StudentWorld* world, double startX, double startY, int dir) : Actor(world, IID_PEA, startX, startY, dir) {}
 	virtual void uniqueAction()
	{
		peaShooter();
		if (isAlive())
		{
			movePea();
			peaShooter();
		}
 	}
	
 private:
	 void peaShooter();
	 void movePea();
 };

class Item : public Actor
{
public:
	Item(StudentWorld* world, int imageID, int startX, int startY, int bonus) : Actor(world, imageID, startX, startY), m_bonus(bonus) {}
	virtual bool canMoveOn() const { return true; }
	virtual bool stealable() const { return true; }
	virtual void uniqueAction();
	virtual void bonusAction() = 0;
private:
	int m_bonus;
};

class Crystal : public Item
{
public:
	Crystal(StudentWorld* world, int startX, int startY) : Item(world, IID_CRYSTAL, startX, startY, 50) {}
	virtual bool stealable() const { return false; }
	virtual void bonusAction();
private:
};

class Life : public Item
{
public:
	Life(StudentWorld* world, int startX, int startY) : Item(world, IID_EXTRA_LIFE, startX, startY, 1000) {}
	virtual void bonusAction();
private:
};

class Health : public Item
{
public:
	Health(StudentWorld* world, int startX, int startY) : Item(world, IID_RESTORE_HEALTH, startX, startY, 500) {}
	virtual void bonusAction();
private:
};

class Ammo : public Item
{
public:
	Ammo(StudentWorld* world, int startX, int startY) : Item(world, IID_AMMO, startX, startY, 100) {}
	virtual void bonusAction();
private:
};

class Exit : public Actor
{
public:
	Exit(StudentWorld* world, int startX, int startY) : Actor(world, IID_EXIT, startX, startY) { setVisible(false); }
	virtual bool canMoveOn() const { return true; }
	virtual void doSomething();
private:
};

//
//class MeanThiefBot : public ThiefBot
//{
//public:
//	MeanThiefBot(StudentWorld* world, int startX, int startY) : ThiefBot(world, startX, startY, 20, IID_MEAN_THIEFBOT) { setHealth(8); }
//	virtual void uniqueAction() { return; }
//private:
//};
//

//
//class Factory : public Statics
//{
//public:
//	Factory(StudentWorld* world, int startX, int startY);
//private:
//};
//
#endif // ACTOR_H_
