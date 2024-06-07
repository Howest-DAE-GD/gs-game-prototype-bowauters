#pragma once

#include "Enemy.h"

class GooEnemy : public Enemy
{
public:
	GooEnemy(Point2f pos, float speed);

	virtual void Update(float elapsedSec);
	virtual void Draw();

	void TriggerGoo();
	bool GetTriggerGoo();
	bool GetGooGone();
	bool GetHitByGrenade();
	void SetHitByGrenade(bool value);

private:
	float m_GooTimer{};
	float m_GooMaxTime{10.f};

	bool m_TriggerGoo{};
	bool m_GooGone{};
	bool m_HitByGrenade{};

};

