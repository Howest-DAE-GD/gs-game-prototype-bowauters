#pragma once

#include "Vector2f.h"

class Enemy
{
public:
	enum EnemyType { Normal, Exploding };

	Enemy(Point2f pos, int health, EnemyType type, float radius);

	virtual void Update(float elapsedSec);
	virtual void Draw();

	void UpdateDirection(Vector2f direction);
	const Point2f& GetPos();
	Circlef GetCircle();
	void GotHit();
	int GetHealth();

	EnemyType GetEnemyType();


private:
	EnemyType m_EnemyType;

	Point2f m_Pos;
	Vector2f m_DirectionVector;

	int m_Health;
	float m_Speed;

protected:
	float m_Radius{ 20.f };

	bool m_DrawThingy;

};

