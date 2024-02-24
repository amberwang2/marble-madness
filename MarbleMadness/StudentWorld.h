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
  int getActorCount() { return m_Actors.size(); }
  void getActor(Actor* &actor, int n) { actor = m_Actors[n]; }

private:
	std::vector<Actor*> m_Actors;
};

#endif // STUDENTWORLD_H_
