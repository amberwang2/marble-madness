#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::loadLevel(int n)
{
    string curLevel;
    if (n < 10)
        curLevel = "level0" + to_string(n) + ".txt";
    else if (n < 100)
        curLevel = "level" + to_string(n) + ".txt";
    else
        return 1;
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found ||
    result == Level:: load_fail_bad_format)
        return -1; // something bad happened!
    // otherwise the load was successful and you can access the
    // contents of the level – here’s an example
    Level::MazeEntry item;
    for (int i = 0; i < VIEW_WIDTH; i++)
    {
        for (int j = 0; j < VIEW_HEIGHT; j++)
        {
            Level::MazeEntry item = lev.getContentsOf(i, j);
            switch (item)
            {
                case Level::player:
                m_Actors.insert(m_Actors.begin(), new Avatar(this, i, j));
                break;
                case Level::wall:
                insertActor(new Wall(this, i, j));
                break;
            }
        }
    }
    return 0;
}

int StudentWorld::init()
{
    int num = loadLevel(getLevel());
    if (num == -1)
        return GWSTATUS_LEVEL_ERROR;
    else if (num == 1)
        return GWSTATUS_PLAYER_WON;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    setGameStatText("Game will end when you type q");
    Actor* actor;
    for (int i = 0; i < getActorCount(); i++)
    {
        getActor(actor, i);
        actor->doSomething();
    }
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < getActorCount(); i++)
    {
        delete m_Actors[i];
        m_Actors[i] = nullptr;
    }
}

void StudentWorld::insertActor(Actor* actor)
{
    m_Actors.push_back(actor);
}

bool StudentWorld::moveable(double x, double y)
{
    Actor* actor;
    for (int i = 0; i < getActorCount(); i++)
    {
        getActor(actor, i);
        if (actor->getX() == x && actor->getY() == y)
            if (!actor->canMoveOn())
                return false;
    }
    return true;
}
