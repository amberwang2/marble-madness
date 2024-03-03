#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Actor::doSomething()
{
	if (isAlive())
	{
		setVisible(true);
		uniqueAction();
	}
}

void Agent::fire() // plays the appropriate fire sound and fires a pea in the current direction the agent is facing
{
	if (bobotic())
		getWorld()->playSound(SOUND_ENEMY_FIRE);
	else
		getWorld()->playSound(SOUND_PLAYER_FIRE);
	switch (getDirection())
	{
	case left:
		getWorld()->insertActor(new Pea(getWorld(), getX() - 1, getY(), getDirection()));
		break;
	case right:
		getWorld()->insertActor(new Pea(getWorld(), getX() + 1, getY(), getDirection()));
		break;
	case up:
		getWorld()->insertActor(new Pea(getWorld(), getX(), getY() + 1, getDirection()));
		break;
	case down:
		getWorld()->insertActor(new Pea(getWorld(), getX(), getY() - 1, getDirection()));
		break;
	}
}

void Agent::damaged() // damages the agent for 2hp and does the appropriate action depending on if the agent dies or not
{
	setHp(getHp() - 2);
	if (getHp() <= 0) {
		dead();
		kill();
	}
	else
		impact();
}

void Avatar::uniqueAction()
{
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_ESCAPE: // kills the avatar and resets the level
			dead();
			kill();
			break;
		case KEY_PRESS_LEFT: // attempt to move avatar left (for all directions of movement, attempts to push a marble if it is in the way)
			setDirection(left);
			if (getWorld()->moveable(getX() - 1, getY()) || getWorld()->attemptPush(getX() - 1, getY(), left))
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT: // attempt to move avatar right
			setDirection(right);
			if (getWorld()->moveable(getX() + 1, getY()) || getWorld()->attemptPush(getX() + 1, getY(), right))
				moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_UP: // attempt to move avatar up
			setDirection(up);
			if (getWorld()->moveable(getX(), getY() + 1) || getWorld()->attemptPush(getX(), getY() + 1, up))
				moveTo(getX(), getY() + 1);
			break;
		case KEY_PRESS_DOWN: // attempt to move avatar down
			setDirection(down);
			if (getWorld()->moveable(getX(), getY() - 1) || getWorld()->attemptPush(getX(), getY() - 1, down))
				moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_SPACE: // fires a pea if avatar has ammo
		 	if (getAmmo() > 0)
		 	{
				fire();
				setAmmo(getAmmo() - 1);
		 	}
		 	break;
		}
	}

}

void Avatar::impact() { getWorld()->playSound(SOUND_PLAYER_IMPACT); }
void Avatar::dead() { getWorld()->playSound(SOUND_PLAYER_DIE); }

void Bot::uniqueAction() // only lets bots take action when the right number of ticks has passed
{
	if (takeAction())
		if (!botAction())
			moveBot();
}

bool Bot::takeAction() // determines if the bot gets to take action this tick
{
	int ticks = (28 - getWorld()->getLevel()) / 4;
	if (ticks < 3)
		ticks = 3;
	if (m_tickCount == ticks)
	{
		m_tickCount = 0;
		return true;
	}
	else
	{
		m_tickCount++;
		return false;
	}
}

bool Bot::playerShootable() // determines if the bot has a clear shot to the player
{
	switch (getDirection()) // first checks if the bot is facing the player, then if there are no obstacles in the way
	{
	case left:
		if (getWorld()->playerY() == getY() && getWorld()->playerX() < getX())
		{
			for (int i = getWorld()->playerX() + 1; i < getX(); i++)
				if (!getWorld()->passTile(i, getY()))
					return false;
			return true;
		}
		break;
	case right:
		if (getWorld()->playerY() == getY() && getWorld()->playerX() > getX())
		{
			for (int i = getX() + 1; i < getWorld()->playerX(); i++)
				if (!getWorld()->passTile(i, getY()))
					return false;
			return true;
		}
		break;
	case up:
		if (getWorld()->playerX() == getX() && getWorld()->playerY() > getY())
		{
			for (int i = getY() + 1; i < getWorld()->playerY(); i++)
				if (!getWorld()->passTile(getX(), i))
					return false;
			return true;
		}
		break;
	case down:
		if (getWorld()->playerX() == getX() && getWorld()->playerY() < getY())
		{
			for (int i = getWorld()->playerY() + 1; i < getY(); i++)
				if (!getWorld()->passTile(getX(), i))
					return false;
			return true;
		}
		break;
	}
	return false;
}

bool Bot::botAction()
{
	if (playerShootable()) // fires a pea if the bot has a clear shot to the player
	{
		fire();
		return true;
	}
	return false;
}

bool Bot::canMoveInDir(int dir) // checks if the bot can move in the specified direction; if it can, it moves the bot
{
	switch (dir)
	{
	case left:
		if (getWorld()->moveable(getX() - 1, getY()))
		{
			setDirection(left);
			moveTo(getX() - 1, getY());
			return true;
		}
		return false;
		break;
	case right:
		if (getWorld()->moveable(getX() + 1, getY()))
		{
			setDirection(right);
			moveTo(getX() + 1, getY());
			return true;
		}
		return false;
		break;
	case up:
		if (getWorld()->moveable(getX(), getY() + 1))
		{
			setDirection(up);
			moveTo(getX(), getY() + 1);
			return true;
		}
		return false;
		break;
	case down:
		if (getWorld()->moveable(getX(), getY() - 1))
		{
			setDirection(down);
			moveTo(getX(), getY() - 1);
			return true;
		}
		return false;
		break;
	}
	return false;
}

void Bot::impact() { getWorld()->playSound(SOUND_ROBOT_IMPACT); }
void Bot::dead()
{
	getWorld()->playSound(SOUND_ROBOT_DIE);
	getWorld()->increaseScore(m_bonus);
}


void RageBot::moveBot() // reverses direction of the RageBot if it has encountered an obstacle
{
	if (!canMoveInDir(getDirection()))
	{
		if (getDirection() < 180)
			setDirection(getDirection() + 180);
		else
			setDirection(getDirection() - 180);
	}
}

bool ThiefBot::botAction() // attempts to steal a goodie if bot is on the same square as a goodie; if so, it deactivates the goodie and stores it away
{
	Actor* actor;
	for (int i = 0; i < getWorld()->getActorCount(); i++)
	{
		getWorld()->getActor(actor, i);
		if (actor->getX() == getX() && actor->getY() == getY() && actor->stealable() && actor->isVisible() && m_goodie == nullptr)
		{
			if (!randInt(0, 9))
			{
				m_goodie = actor;
				m_goodie->setVisible(false);
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
				return true;
			}
		}
	}
	return false;
}

void ThiefBot::moveBot() // handles ThiefBot movement
{
	if (m_squaresMoved < m_turnDist)
	{
		if (canMoveInDir(getDirection())) // attempts to move bot forward if it has not moved turnDist squares yet
		{
			m_squaresMoved++;
			return;
		}
	}
	m_turnDist = randInt(1, 6); // new turnDist is selected if bot has moved turnDist squares or encountered an obstacle
	std::vector<int> dirs = { 0, 1, 2, 3 };
	int tempDir = randInt(0, 3); // selects a random direction
	int initialDir = tempDir;
	for (int i = 3; i >= 0; i--)
	{
		switch (dirs[tempDir]) // attempts to move bot in current random direction
		{
		case 0: // left
			if (canMoveInDir(left))
			{
				m_squaresMoved++;
				return;
			}
			break;
		case 1: // right
			if (canMoveInDir(right))
			{
				m_squaresMoved++;
				return;
			}
			break;
		case 2: // up
			if (canMoveInDir(up))
			{
				m_squaresMoved++;
				return;
			}
			break;
		case 3: // down
			if (canMoveInDir(down))
			{
				m_squaresMoved++;
				return;
			}
			break;
		}
		dirs.erase(dirs.begin() + tempDir);
		tempDir = randInt(0, i - 1); // if the bot was not able to move in the current random direction, pick a new random direction out of the ones that were not chosen yet
	}
	setDirection(initialDir); // turn bot to initial random direction if it was not able to move in any direction
}

void ThiefBot::dead()
{
	if (m_goodie != nullptr) // releases stolen goodie upon death if the bot stole one
	{
		m_goodie->setVisible(true);
		m_goodie->moveTo(getX(), getY());
		m_goodie = nullptr;
	}
	Bot::dead();
}

bool MeanThiefBot::botAction() // first tries to shoot like a RageBot, then attempts to steal like a ThiefBot
{
	if (Bot::botAction())
		return true;
	if (ThiefBot::botAction())
		return true;
	return false;
}

void Factory::uniqueAction()
{
	if (getNumThiefs() < 3 && randInt(0, 49) == 0) // attempts to spawn a new ThiefBot(depending on type) if there are not too many ThiefBots in the vicinity
	{
		if (m_mean)
			getWorld()->insertActor(new MeanThiefBot(getWorld(), getX(), getY()));
		else
			getWorld()->insertActor(new ThiefBot(getWorld(), getX(), getY()));
		getWorld()->playSound(SOUND_ROBOT_BORN);
	}
}

int Factory::getNumThiefs() // counts number of ThiefBots of all variants within 3 squares inclusive
{
	int count = 0;
	Actor* actor;
	for (int i = 0; i < getWorld()->getActorCount(); i++)
	{
		getWorld()->getActor(actor, i);
		if (actor->willSteal())
		{
			if (actor->getX() == getX() && actor->getY() == getY()) // immediately returns as too many ThiefBots if there is currently a ThiefBot on the factory square
				return 3;
			if (abs(actor->getX() - getX()) <= 3 && abs(actor->getY() - getY()) <= 3)
				count++;
		}
	}
	return count;
}

bool Marble::bePushedTo(int dir) // attempts to push the marble in the specified direction
{
	switch (dir)
	{
	case left:
		if (getWorld()->marbleMoveable(getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
			return true;
		}
		break;
	case right:
		if (getWorld()->marbleMoveable(getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
			return true;
		}
		break;
	case up:
		if (getWorld()->marbleMoveable(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
			return true;
		}
		break;
	case down:
		if (getWorld()->marbleMoveable(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
			return true;
		}
		break;
	}
	return false;
}

void Pit::uniqueAction()
{
	Actor* actor;
	getWorld()->marbleAt(actor, getX(), getY()); // checks if there is a marble on its square
	if (actor != nullptr) // if there is a marble, it swallows the marble and both actors disappear
	{
		actor->kill();
		kill();
	}
}

void Pea::uniqueAction()
{
	peaShooter(); // attempts to attack actors on its square if there are attackable actors
	if (isAlive()) // if it has not damaged any actors yet
	{
		movePea(); // advance pea forward
		peaShooter(); // attempt to attack actors on new square
	}
}

void Pea::peaShooter() // attempts to attack actors on its square if there are attackable actors
{
	Actor* actor;
	bool wasted = false;
	for (int i = 0; i < getWorld()->getActorCount(); i++)
	{
		getWorld()->getActor(actor, i);
		if (actor->attackable() && actor->getX() == getX() && actor->getY() == getY())
		{
			actor->damaged(); // attempts to damage every attackable actor on its square before disappearing
			wasted = true;
		}
	}
	if (wasted)
		kill();
}

void Pea::movePea()
{
	switch (getDirection())
	{
	case left:
		moveTo(getX() - 1, getY());
		break;
	case right:
		moveTo(getX() + 1, getY());
		break;
	case up:
		moveTo(getX(), getY() + 1);
		break;
	case down:
		moveTo(getX(), getY() - 1);
		break;
	}
}

void Item::doSomething()
{
	if (isVisible())
		uniqueAction();
}

void Item::uniqueAction()
{
	if (getWorld()->playerX() == getX() && getWorld()->playerY() == getY() && isVisible())
	{
		getWorld()->increaseScore(m_bonus);
		kill();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		bonusAction();
	}
}

void Crystal::bonusAction()
{
	getWorld()->crystalCollected();
}

void Life::bonusAction()
{
	getWorld()->incLives();
}

void Health::bonusAction()
{
	getWorld()->playerRestore();
}

void Ammo::bonusAction()
{
	getWorld()->playerAmmo();
}

void Exit::doSomething()
{
	if (getWorld()->complete() && !isVisible()) // checks if all crystals have been collected and if it has not been revealed already
	{
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
	}
	if (isVisible() && getWorld()->playerX() == getX() && getWorld()->playerY() == getY()) // if player has stepped on the visible exit, mark level as complete
		getWorld()->levelComplete();
}
