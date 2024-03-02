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

void Agent::fire()
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

void Avatar::uniqueAction() {
	int ch;
	if (getWorld()->getKey(ch))
	{
		// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_ESCAPE:
			kill();
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (getWorld()->moveable(getX() - 1, getY()) || getWorld()->attemptPush(getX() - 1, getY(), left))
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (getWorld()->moveable(getX() + 1, getY()) || getWorld()->attemptPush(getX() + 1, getY(), right))
				moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (getWorld()->moveable(getX(), getY() + 1) || getWorld()->attemptPush(getX(), getY() + 1, up))
				moveTo(getX(), getY() + 1);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (getWorld()->moveable(getX(), getY() - 1) || getWorld()->attemptPush(getX(), getY() - 1, down))
				moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_SPACE:
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

bool Bot::takeAction()
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

void Bot::uniqueAction()
{
	if (takeAction())
	{
		botAction();
		moveBot();
	}
	else
		return;
}

void Bot::botAction()
{
	if (playerShootable())
		fire();
}

bool Bot::playerShootable()
{
	switch (getDirection())
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

bool Bot::canMoveInDir()
{
	switch (getDirection())
	{
	case left:
		if (getWorld()->moveable(getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
			return true;
		}
		return false;
		break;
	case right:
		if (getWorld()->moveable(getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
			return true;
		}
		return false;
		break;
	case up:
		if (getWorld()->moveable(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
			return true;
		}
		return false;
		break;
	case down:
		if (getWorld()->moveable(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
			return true;
		}
		return false;
		break;
	}
}

void Bot::impact() { getWorld()->playSound(SOUND_ROBOT_IMPACT); }
void Bot::dead()
{
	getWorld()->playSound(SOUND_ROBOT_DIE);
	getWorld()->increaseScore(m_bonus);
}


void RageBot::moveBot()
{
	if (!canMoveInDir())
	{
		if (getDirection() < 180)
			setDirection(getDirection() + 180);
		else
			setDirection(getDirection() - 180);
	}
}

bool Marble::bePushedTo(int dir)
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
	getWorld()->marbleAt(actor, getX(), getY());
	if (actor != nullptr)
	{
		actor->kill();
		kill();
	}
}

void Pea::peaShooter()
{
	Actor* actor;
	bool wasted = false;
	for (int i = 0; i < getWorld()->getActorCount(); i++)
	{
		getWorld()->getActor(actor, i);
		if (actor->attackable() && actor->getX() == getX() && actor->getY() == getY())
		{
			actor->damaged();
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

void Item::uniqueAction()
{
	if (getWorld()->playerX() == getX() && getWorld()->playerY() == getY())
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
	if (getWorld()->complete() && !isVisible())
	{
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
	}
	if (isVisible() && getWorld()->playerX() == getX() && getWorld()->playerY() == getY())
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(2000);
		getWorld()->levelComplete();
		getWorld()->increaseScore(getWorld()->getBonus());
	}
}
