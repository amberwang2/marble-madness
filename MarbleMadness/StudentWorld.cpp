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
    m_levelBonus = 1000;
    m_nCrystals = 0;
    m_cCrystals = 0;
    m_complete = false;
    Level::MazeEntry item;
    for (int i = 0; i < VIEW_WIDTH; i++)
    {
        for (int j = 0; j < VIEW_HEIGHT; j++)
        {
            item = lev.getContentsOf(i, j);
            switch (item)
            {
                case Level::player:
                    m_Actors.insert(m_Actors.begin(), new Avatar(this, i, j));
                    break;
                case Level::horiz_ragebot:
                    insertActor(new RageBot(this, i, j, 0));
                    break;
                case Level::vert_ragebot:
                    insertActor(new RageBot(this, i, j, 270));
                    break;
                case Level::wall:
                    insertActor(new Wall(this, i, j));
                    break;
                case Level::pit:
                    insertActor(new Pit(this, i, j));
                    break;
                case Level::marble:
                    insertActor(new Marble(this, i, j));
                    break;
                case Level::crystal:
                    insertActor(new Crystal(this, i, j));
                    m_nCrystals++;
                    break;
                case Level::extra_life:
                    insertActor(new Life(this, i, j));
                    break;
                case Level::restore_health:
                    insertActor(new Health(this, i, j));
                    break;
                case Level::ammo:
                    insertActor(new Ammo(this, i, j));
                    break;
                case Level::exit:
                    insertActor(new Exit(this, i, j));
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
    Actor* actor;
    for (int i = getActorCount() - 1; i >= 0; i--)
    {
        getActor(actor, i);
        actor->doSomething();
        if (!m_Actors[0]->isAlive())
            return GWSTATUS_PLAYER_DIED;
    }
    for (int i = 0; i < getActorCount(); i++)
    {
        if (!m_Actors[i]->isAlive())
        {
            delete m_Actors[i];
            m_Actors.erase(m_Actors.begin() + i);
            i--;
        }
    }
    if (m_levelBonus > 0)
        m_levelBonus--;
    setGameStatText("Score: " + std::to_string(getScore()) + "  Level: " + std::to_string(getLevel()) + "  Lives: " + std::to_string(getLives()) + "  Health: " + std::to_string(m_Actors[0]->getHp()*100/20) + "%  Ammo: " + std::to_string(m_Actors[0]->getAmmo()) + "  Bonus: " + std::to_string(m_levelBonus));
    if (m_complete)
        return GWSTATUS_FINISHED_LEVEL;
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
        {
            if (!actor->canMoveOn() && actor->isVisible())
                return false;
        }
    }
    return true;
}

bool StudentWorld::marbleMoveable(double x, double y)
{
    Actor* actor;
    for (int i = 0; i < getActorCount(); i++)
    {
        getActor(actor, i);
        if (actor->getX() == x && actor->getY() == y)
        {
            if (!actor->MarbleCanPass() && actor->isVisible())
                return false;
        }
    }
    return true;
}

void StudentWorld::marbleAt(Actor* &actor, double x, double y)
{
    actor = nullptr;
    for (int i = 0; i < getActorCount(); i++)
        if (m_Actors[i]->getX() == x && m_Actors[i]->getY() == y && m_Actors[i]->swallowable() && m_Actors[i]->isVisible())
            actor = m_Actors[i];
}

bool StudentWorld::passTile(double x, double y)
{
    Actor* actor;
    for (int i = 0; i < getActorCount(); i++)
        if (m_Actors[i]->getX() == x && m_Actors[i]->getY() == y && m_Actors[i]->isVisible() && m_Actors[i]->attackable())
            return false;
    return true;
}

bool StudentWorld::attemptPush(double x, double y, int dir)
{
    Actor* actor;
    for (int i = 0; i < getActorCount(); i++)
    {
        getActor(actor, i);
        if (actor->getX() == x && actor->getY() == y)
        {
            if (actor->bePushedTo(dir))
                return true;
        }
    }
    return false;
}

void StudentWorld::playerRestore()
{
    m_Actors[0]->setHp(20);
}

void StudentWorld::playerAmmo()
{
    m_Actors[0]->setAmmo(m_Actors[0]->getAmmo() + 20);
}

std::string StudentWorld::getScoreDis()
{
    if (getScore() < 10)
        return "      " + getScore();
    else if (getScore() < 100)
        return "     " + getScore();
    else if (getScore() < 1000)
        return "    " + getScore();
    else if (getScore() < 10000)
        return "   " + getScore();
    else if (getScore() < 100000)
        return "  " + getScore();
    else if (getScore() < 1000000)
        return " " + getScore();
    else
        return "" + getScore();
}

std::string StudentWorld::getLevelDis()
{
    if (getLevel() < 10)
        return " " + getLevel();
    else
        return "" + getLevel();
}
