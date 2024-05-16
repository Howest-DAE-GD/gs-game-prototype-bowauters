#pragma once

#include "Enemy.h"

class GooEnemy : public Enemy
{
public:
	GooEnemy(Point2f pos);

	virtual void Update(float elapsedSec);
	virtual void Draw();

	void TriggerGoo();
	bool GetTriggerGoo();
	bool GetGooGone();

private:
	float m_GooTimer{};
	float m_GooMaxTime{10.f};

	bool m_TriggerGoo{};
	bool m_GooGone{};

};

