#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  int loadLevel();
  void insertActor(Actor* actor);
  bool moveable(int x, int y);

private:
	Actor* m_Player;
	std::vector<Actor*> m_Actors;
};

#endif // STUDENTWORLD_H_
