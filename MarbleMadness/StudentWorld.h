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
  int loadLevel(int n);
  void insertActor(Actor* actor);
  bool moveable(double x, double y);
  bool marbleMoveable(double x, double y);
  int getActorCount() { return m_Actors.size(); }
  void getActor(Actor* &actor, int n) { actor = m_Actors[n]; }
  void marbleAt(Actor* &actor, double x, double y);
  bool attemptPush(double x, double y, int dir);
  void crystalCollected() { m_cCrystals++; }
  bool complete() { return m_cCrystals == m_nCrystals; }
  void levelComplete() { m_complete = true; }
  double playerX() { return m_Actors[0]->getX(); }
  double playerY() { return m_Actors[0]->getY(); }
  void playerRestore();
  void playerAmmo();
  std::string getScoreDis();
  std::string getLevelDis();
  int getBonus() { return m_levelBonus; }

private:
	std::vector<Actor*> m_Actors;
	int m_levelBonus;
	int m_cCrystals;
	int m_nCrystals;
	bool m_complete;
};

#endif // STUDENTWORLD_H_
