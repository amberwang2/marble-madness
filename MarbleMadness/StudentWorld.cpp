#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <iostream>
#include <sstream>
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
    if (n < 10) // generates appropriate filename according to level
        curLevel = "level0" + to_string(n) + ".txt";
    else if (n < 100)
        curLevel = "level" + to_string(n) + ".txt";
    else
        return 1;
    m_levelBonus = 1000; // resets level bonus, crystal count, and completion status
    m_nCrystals = 0;
    m_cCrystals = 0;
    m_complete = false;
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found) // game win if next level is not found
        return 1;
    if (result == Level::load_fail_bad_format) // invalid level format
        return -1;
    Level::MazeEntry item;
    for (int i = 0; i < VIEW_WIDTH; i++)
    {
        for (int j = 0; j < VIEW_HEIGHT; j++)
        {
            item = lev.getContentsOf(i, j); // loads in all actors specified in the level data file
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
                case Level::thiefbot_factory:
                    insertActor(new Factory(this, i, j, false));
                    break;
                case Level::mean_thiefbot_factory:
                    insertActor(new Factory(this, i, j, true));
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
    setDisplayText(getScore(), getLevel(), getLives(), m_Actors[0]->getHp(), m_Actors[0]->getAmmo(), m_levelBonus);
    Actor* actor;
    for (int i = getActorCount() - 1; i >= 0; i--) // gives every actor a chance to do something
    {
        getActor(actor, i);
        actor->doSomething();
        if (!m_Actors[0]->isAlive()) // checks if player has died after every actor action
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }
    for (int i = 0; i < getActorCount(); i++) // removes dead actors from world
    {
        if (!m_Actors[i]->isAlive())
        {
            delete m_Actors[i];
            m_Actors.erase(m_Actors.begin() + i);
            i--;
        }
    }
    if (m_levelBonus > 0) // decreases level bonus if it has not reached zero
        m_levelBonus--;
    if (m_complete) // moves on to next level if the current level has completed
    {
        playSound(SOUND_FINISHED_LEVEL);
        increaseScore(2000);
        increaseScore(m_levelBonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < getActorCount(); i++) // frees all memory allocated for actors and clears the m_Actors container
        delete m_Actors[i];
    m_Actors = {};
}

void StudentWorld::insertActor(Actor* actor)
{
    m_Actors.push_back(actor);
}

bool StudentWorld::moveable(double x, double y) const
{
    Actor* actor;
    for (int i = 0; i < getActorCount(); i++)
    {
        getActor(actor, i);
        if (actor->getX() == x && actor->getY() == y)
            if (!actor->canMoveOn() && actor->isVisible()) // checks if an agent can move onto a specified tile
                return false;
    }
    return true;
}

bool StudentWorld::marbleMoveable(double x, double y) const
{
    Actor* actor;
    for (int i = 0; i < getActorCount(); i++)
    {
        getActor(actor, i);
        if (actor->getX() == x && actor->getY() == y)
            if (!actor->marbleCanPass() && actor->isVisible()) // checks if marble can move onto a specified tile
                return false;
    }
    return true;
}

void StudentWorld::marbleAt(Actor* &actor, double x, double y) const
{
    actor = nullptr;
    for (int i = 0; i < getActorCount(); i++)
        if (m_Actors[i]->getX() == x && m_Actors[i]->getY() == y && m_Actors[i]->swallowable() && m_Actors[i]->isVisible()) // checks if there is a marble at the specified tile
            actor = m_Actors[i];
}

bool StudentWorld::passTile(double x, double y) const
{
    for (int i = 0; i < getActorCount(); i++)
        if (m_Actors[i]->getX() == x && m_Actors[i]->getY() == y && m_Actors[i]->isVisible() && m_Actors[i]->attackable()) // checks if a pea can pass through the specified tile
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
            if (actor->bePushedTo(dir)) // attempts to push the actor in the specified direction
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

void StudentWorld::setDisplayText(int score, int level, int lives, int health, int ammo, int bonus) // configures correct text to display game stats
{
    string s_score, s_level, s_lives, s_health, s_ammo, s_bonus;
    s_score = to_string(score);
    s_level = to_string(level);
    s_lives = to_string(lives);
    s_health = to_string(health * 100 / 20) + "%";
    s_ammo = to_string(ammo);
    s_bonus = to_string(bonus);
    if (level < 10)
        s_level = "0" + s_level;
    if (lives < 10)
        s_lives = " " + s_lives;
    for (int i = 0; i < 7; i++)
    {
        if (s_score.length() < 7)
            s_score = "0" + s_score;
        if (s_health.length() < 4)
            s_health = " " + s_health;
        if (s_ammo.length() < 3)
            s_ammo = " " + s_ammo;
        if (s_bonus.length() < 4)
            s_bonus = " " + s_bonus;
    }
    setGameStatText("Score:  " + s_score + "  Level: " + s_level + "  Lives: " + s_lives + "  Health: " + s_health + "  Ammo: " + s_ammo + "  Bonus: " + s_bonus);
}
