#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, int dir = right) : GraphObject(imageID, startX, startY, dir), m_alive(true) { setVisible(true); }
	virtual void doSomething() { return; }
private:
	bool m_alive;
};

class Dynamic : public Actor
{
public:
	Dynamic(int imageID, int startX, int startY, int dir = right) : Actor(IID_PLAYER, startX, startY, dir) {}
	void setHealth(int hp) { m_hp = hp; }
private:
	int m_hp;
};

class Avatar : public Dynamic
{
public:
	Avatar(int startX, int startY) : Dynamic(IID_PLAYER, startX, startY), m_peas(20) { setHealth(20); }
	virtual void doSomething() { return; }
private:
	int m_peas;
};

class Bot : public Dynamic
{
public:
	Bot(int imageID, int startX, int startY, int dir = right) : Dynamic(IID_PLAYER, startX, startY, dir) {}
	void damage();
private:
};

class RageBot : public Bot
{
public:
	RageBot(int startX, int startY, int dir) : Bot(IID_RAGEBOT, startX, startY, dir) { setHealth(10); }
	virtual void doSomething() { return; }
private:
};

class ThiefBot : public Bot
{
public:
	ThiefBot(int startX, int startY, int imageID = IID_THIEFBOT) : Bot(imageID, startX, startY), m_turnDist(rand()%6+1) { setHealth(5); }
	virtual void doSomething() { return; }
private:
	int m_turnDist;
};

class MeanThiefBot : public ThiefBot
{
public:
	MeanThiefBot(int startX, int startY) : ThiefBot(startX, startY, IID_MEAN_THIEFBOT) { setHealth(8); }
	virtual void doSomething() { return; }
private:
};

class Pea : public Dynamic
{
public:
	Pea(int startX, int startY, int dir) : Dynamic(IID_PEA, startX, startY, dir) {}
private:
};

class Marble : public Dynamic
{
public:
	Marble(int startX, int startY) : Dynamic(IID_MARBLE, startX, startY) { setHealth(10); }
private:
};

class Statics : public Actor
{
public:
	Statics(int imageID, int startX, int startY) : Actor(imageID, startX, startY) {}
private:
};

class Pit : public Statics
{
public:
	Pit(int startX, int startY) : Statics(IID_PIT, startX, startY) {}
private:
};

class Factory : public Statics
{
public:
	Factory(int startX, int startY);
private:
};

class Exit : public Statics
{
public:
	Exit(int startX, int startY);
private:
};

class Wall : public Statics
{
public:
	Wall(int startX, int startY);
private:
};

class Item : public Statics
{
public:
	Item(int imageID, int startX, int startY);
private:
};

class Crystal : public Item
{
public:
	Crystal(int startX, int startY);
private:
};

class Goodie : public Item
{
public:
	Goodie(int imageID, int startX, int startY);
private:
};

class Life : public Goodie
{
public:
	Life(int startX, int startY);
private:
};

class Health : public Goodie
{
public:
	Health(int startX, int startY);
private:
};

class Ammo : public Goodie
{
public:
	Ammo(int startX, int startY);
private:
};
#endif // ACTOR_H_
