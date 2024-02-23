#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// #include "SoundFX.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, int dir) : GraphObject(imageID, startX, startY, dir), m_world(world) { setVisible(true); }
	virtual void doSomething();
	virtual void uniqueAction() { return; }
	virtual bool canMoveOn() const { return false; }
	virtual bool isTransient() const { return true; }
	virtual bool isPea() const { return false; }
	virtual bool attackable() const { return false; }
	bool isAlive() const { return m_alive; }
	void kill() { m_alive = false; }
	StudentWorld* getWorld() const { return m_world; }
private:
	StudentWorld* m_world;
	bool m_alive;
};

class Dynamic : public Actor
{
public:
	Dynamic(StudentWorld* world, int imageID, int startX, int startY, int dir = right) : Actor(world, IID_PLAYER, startX, startY, dir) {}
	virtual bool attackable() { return true; }
	virtual void damaged() {
		m_hp -= 2;
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
	void setHealth(int hp) {
		m_hp = hp;
	}
	bool healthCheck() {
		if (m_hp == 0) {
			kill();
			return false;
		}
		return true;
	}
private:
	int m_hp;
	int m_bonus;
};

class Static : public Actor
{
public:
	Static(StudentWorld* world, int imageID, int startX, int startY, int dir = none) : Actor(world, imageID, startX, startY, dir) {}
private:
};

class Avatar : public Dynamic
{
public:
	Avatar(StudentWorld* world, int startX, int startY) : Dynamic(world, IID_PLAYER, startX, startY), m_peas(20) { setHealth(20); }
	virtual void uniqueAction();
private:
	int m_peas;
};

class Wall : public Static
{
public:
	Wall(StudentWorld* world, int startX, int startY) : Static(world, IID_WALL, startX, startY) {};
private:
};

// class Pea : public Static
// {
// public:
// 	Pea(StudentWorld* world, int startX, int startY, int dir) : Static(world, IID_PEA, startX, startY, dir) {}
// 	virtual bool isPea() { return true; }
// 	virtual void uniqueAction() {
// 		Actor* actor;
// 		for (int i = 0; i < getWorld()->getActorCount(); i++)
// 		{
// 			getWorld()->getActor(actor, i);
// 			if (actor->getX() == getX() && actor->getY() == getY())
// 				if (actor->attackable()) // avatar, bot, marble
// 					return;
// 				else if (actor->isTransient()) // wall, factory
// 					return;
// 		}
// 	}
	
// private:
// };

//class Bot : public Dynamic
//{
//public:
//	Bot(StudentWorld* world, int imageID, int startX, int startY, int bonus, int dir = right) : Dynamic(world, imageID, startX, startY, dir), m_bonus(bonus) {}
//	virtual bool isPermeable() { return false; }
//private:
//	int m_bonus;
//};
//
//class RageBot : public Bot
//{
//public:
//	RageBot(StudentWorld* world, int startX, int startY, int dir) : Bot(world, IID_RAGEBOT, startX, startY, 100, dir) { setHealth(10); }
//	virtual void uniqueAction() { return; }
//private:
//};
//
//class ThiefBot : public Bot
//{
//public:
//	ThiefBot(StudentWorld* world, int startX, int startY, int bonus, int imageID = IID_THIEFBOT) : Bot(world, imageID, startX, startY, bonus), m_turnDist(randInt(1, 6)) { setHealth(5); }
//	virtual void uniqueAction() { return; }
//private:
//	int m_turnDist;
//};
//
//class MeanThiefBot : public ThiefBot
//{
//public:
//	MeanThiefBot(StudentWorld* world, int startX, int startY) : ThiefBot(world, startX, startY, 20, IID_MEAN_THIEFBOT) { setHealth(8); }
//	virtual void uniqueAction() { return; }
//private:
//};
//
//class Marble : public Dynamic
//{
//public:
//	Marble(StudentWorld* world, int startX, int startY) : Dynamic(world, IID_MARBLE, startX, startY) { setHealth(10); }
//	virtual bool isPermeable() { } // stuff return true/false; }
//private:
//};
//
//class Exit : public Dynamic
//{
//public:
//	Exit(StudentWorld* world, int startX, int startY);
//private:
//};
//
//class Item : public Dynamic
//{
//public:
//	Item(StudentWorld* world, int imageID, int startX, int startY);
//private:
//};
//
//class Crystal : public Item
//{
//public:
//	Crystal(StudentWorld* world, int startX, int startY);
//private:
//};
//
//class Goodie : public Item
//{
//public:
//	Goodie(StudentWorld* world, int imageID, int startX, int startY);
//private:
//};
//
//class Life : public Goodie
//{
//public:
//	Life(StudentWorld* world, int startX, int startY);
//private:
//};
//
//class Health : public Goodie
//{
//public:
//	Health(StudentWorld* world, int startX, int startY);
//private:
//};
//
//class Ammo : public Goodie
//{
//public:
//	Ammo(StudentWorld* world, int startX, int startY);
//private:
//};
//
//class Pit : public Statics
//{
//public:
//	Pit(StudentWorld* world, int startX, int startY) : Statics(world, IID_PIT, startX, startY) {}
//private:
//};
//
//class Factory : public Statics
//{
//public:
//	Factory(StudentWorld* world, int startX, int startY);
//private:
//};
//
#endif // ACTOR_H_
