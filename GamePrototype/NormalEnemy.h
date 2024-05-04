#pragma once
#include "Enemy.h"

class NormalEnemy : public Enemy
{
public:
	NormalEnemy(Point2f pos);

	virtual void Draw();
};

