#pragma once

#include "Vector2f.h"

class Enemy
{
public:
	Enemy(Point2f pos, int health);

	void Update(float elapsedSec);
	void Draw();

	void UpdateDirection(Vector2f direction);
	const Point2f& GetPos();
	Circlef GetCircle();
	void GotHit();
	int GetHealth();


private:
	Point2f m_Pos;
	Vector2f m_DirectionVector;

	int m_Health;
	float m_Speed;
	float m_Radius{ 20.f };
};

