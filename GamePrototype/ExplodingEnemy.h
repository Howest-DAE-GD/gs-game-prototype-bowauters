#pragma once
#include "Enemy.h"

class ExplodingEnemy : public Enemy
{
public:
	ExplodingEnemy(Point2f pos, float speed);

	virtual void Update(float elapsedSec);
	virtual void Draw();

	void TriggerExplosion();
	bool GetStartExplosion();
	bool GetExploded();

private:
	bool m_StartExplosion{};
	bool m_Exploded{};

	float m_TimeElapsedExplosion{};

};

