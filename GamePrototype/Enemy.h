#pragma once

#include "Vector2f.h"

class Enemy
{
public:
	enum EnemyType { Normal, Exploding, Goo };

	Enemy(Point2f pos, int health, EnemyType type, float radius, float speed);

	virtual void Update(float elapsedSec);
	virtual void Draw();

	void UpdateDirection(Vector2f direction);
	const Point2f& GetPos();
	Circlef GetCircle();
	void GotHit();
	int GetHealth();

	EnemyType GetEnemyType();
	void SetDrawEnemy(bool val);
	bool GetDrawEnemy();

private:
	EnemyType m_EnemyType;

	Point2f m_Pos;
	Vector2f m_DirectionVector;

	int m_Health;
	float m_Speed;

protected:
	float m_Radius{ 20.f };

	bool m_DrawThingy;
	bool m_Update{ true };
	bool m_DrawEnemy{ true };
};

