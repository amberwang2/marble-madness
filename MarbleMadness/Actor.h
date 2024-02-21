#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, int dir = right) : GraphObject(imageID, startX, startY, dir), m_world(world) { setVisible(true); }
	virtual void doSomething() { return; }
	virtual bool isPermeable() { return true; }
	virtual void attacked() { return; }
	StudentWorld* getWorld() { return m_world; }
private:
	StudentWorld* m_world;
};

class Dynamic : public Actor
{
public:
	Dynamic(StudentWorld* world, int imageID, int startX, int startY, int dir = right) : Actor(world, IID_PLAYER, startX, startY, dir), m_alive(true) {}
	virtual void doSomething() {
		if (!m_alive)
			return;
		uniqueAction();
	}
	virtual void uniqueAction() { return; }
	virtual void damaged() {
		m_hp -= 2;
		healthCheck();
	}
	virtual void attacked() {
		damaged();
		return;
	}
	virtual void damageAction() { return; }
	void setHealth(int hp) {
		m_hp = hp;
		healthCheck();
	}
	void healthCheck() { if (m_hp == 0) m_alive = false; }
private:
	bool m_alive;
	int m_hp;
};

class Avatar : public Dynamic
{
public:
	Avatar(StudentWorld* world, int startX, int startY) : Dynamic(world, IID_PLAYER, startX, startY), m_peas(20) { setHealth(20); }
	virtual void uniqueAction();
private:
	int m_peas;
};

class Bot : public Dynamic
{
public:
	Bot(StudentWorld* world, int imageID, int startX, int startY, int bonus, int dir = right) : Dynamic(world, imageID, startX, startY, dir), m_bonus(bonus) {}
	virtual bool isPermeable() { return false; }
private:
	int m_bonus;
};

class RageBot : public Bot
{
public:
	RageBot(StudentWorld* world, int startX, int startY, int dir) : Bot(world, IID_RAGEBOT, startX, startY, 100, dir) { setHealth(10); }
	virtual void uniqueAction() { return; }
private:
};

class ThiefBot : public Bot
{
public:
	ThiefBot(StudentWorld* world, int startX, int startY, int bonus, int imageID = IID_THIEFBOT) : Bot(world, imageID, startX, startY, bonus), m_turnDist(randInt(1, 6)) { setHealth(5); }
	virtual void uniqueAction() { return; }
private:
	int m_turnDist;
};

class MeanThiefBot : public ThiefBot
{
public:
	MeanThiefBot(StudentWorld* world, int startX, int startY) : ThiefBot(world, startX, startY, 20, IID_MEAN_THIEFBOT) { setHealth(8); }
	virtual void uniqueAction() { return; }
private:
};

class Pea : public Dynamic
{
public:
	Pea(StudentWorld* world, int startX, int startY, int dir) : Dynamic(world, IID_PEA, startX, startY, dir) {}
	virtual void uniqueAction() { return; }
private:
};

class Marble : public Dynamic
{
public:
	Marble(StudentWorld* world, int startX, int startY) : Dynamic(world, IID_MARBLE, startX, startY) { setHealth(10); }
	virtual bool isPermeable() { } // stuff return true/false; }
private:
};

class Exit : public Dynamic
{
public:
	Exit(StudentWorld* world, int startX, int startY);
private:
};

class Item : public Dynamic
{
public:
	Item(StudentWorld* world, int imageID, int startX, int startY);
private:
};

class Crystal : public Item
{
public:
	Crystal(StudentWorld* world, int startX, int startY);
private:
};

class Goodie : public Item
{
public:
	Goodie(StudentWorld* world, int imageID, int startX, int startY);
private:
};

class Life : public Goodie
{
public:
	Life(StudentWorld* world, int startX, int startY);
private:
};

class Health : public Goodie
{
public:
	Health(StudentWorld* world, int startX, int startY);
private:
};

class Ammo : public Goodie
{
public:
	Ammo(StudentWorld* world, int startX, int startY);
private:
};

class Statics : public Actor
{
public:
	Statics(StudentWorld* world, int imageID, int startX, int startY) : Actor(world, imageID, startX, startY) {}
	virtual bool isPermeable() { return false; }
private:
};

class Pit : public Statics
{
public:
	Pit(StudentWorld* world, int startX, int startY) : Statics(world, IID_PIT, startX, startY) {}
private:
};

class Factory : public Statics
{
public:
	Factory(StudentWorld* world, int startX, int startY);
private:
};

class Wall : public Statics
{
public:
	Wall(StudentWorld* world, int startX, int startY);
private:
};
#endif // ACTOR_H_
