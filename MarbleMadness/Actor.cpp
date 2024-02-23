#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Actor::doSomething()
{
	if (!isAlive())
			return;
		uniqueAction();
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
			if (getWorld()->moveable(getX() - 1, getY()))
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (getWorld()->moveable(getX() + 1, getY()))
				moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (getWorld()->moveable(getX(), getY() - 1))
				moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (getWorld()->moveable(getX(), getY() + 1))
				moveTo(getX(), getY() + 1);
			break;
		// case KEY_PRESS_SPACE:
		// 	if (m_peas > 0)
		// 	{
		// 		switch (getDirection())
		// 		{
		// 		case left:
		// 			getWorld()->insertActor(new Pea(getWorld(), getX() - 1, getY(), getDirection()));
		// 			break;
		// 		case right:
		// 			getWorld()->insertActor(new Pea(getWorld(), getX() + 1, getY(), getDirection()));
		// 			break;
		// 		case up:
		// 			getWorld()->insertActor(new Pea(getWorld(), getX(), getY() - 1, getDirection()));
		// 			break;
		// 		case down:
		// 			getWorld()->insertActor(new Pea(getWorld(), getX(), getY() + 1, getDirection()));
		// 			break;
		// 		}
		// 	}
		// 	break;
			// etc�
		}
	}

}

