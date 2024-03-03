#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  void insertActor(Actor* actor);
  bool moveable(double x, double y) const;
  bool marbleMoveable(double x, double y) const;
  int getActorCount() const { return m_Actors.size(); }
  void getActor(Actor* &actor, int n) const { actor = m_Actors[n]; }
  void marbleAt(Actor* &actor, double x, double y) const;
  bool passTile(double x, double y) const; // checks if square can be passed through by a pea
  bool attemptPush(double x, double y, int dir); // attempts to push the actor on the square
  void crystalCollected() { m_cCrystals++; }
  bool complete() const { return m_cCrystals == m_nCrystals; }
  void levelComplete() { m_complete = true; }
  double playerX() const { return m_Actors[0]->getX(); }
  double playerY() const { return m_Actors[0]->getY(); }
  void playerRestore(); // restores health to full for player
  void playerAmmo(); // adds 20 ammo for player
private:
	std::vector<Actor*> m_Actors; // container for all actors; avatar is always at index 0
	int m_levelBonus; // keeps track of the level time bonus
	int m_cCrystals; // number of crystals collected
	int m_nCrystals; // number of crystals in the level
	bool m_complete; // whether the level has been completed or not
	int loadLevel(int n);
	void setDisplayText(int score, int level, int lives, int health, int ammo, int bonus);
};
#endif // STUDENTWORLD_H_
