#pragma once

#include <Vector2f.h>

class Grenade
{
public:
	Grenade(Point2f pos, Vector2f direction, float speed);

	void Update(float elapsedSec);
	void Draw();
	Circlef GetCircle();
	bool GetExploded();
	bool GetStartExplosion();

private:
	Point2f m_Pos;
	Vector2f m_DirectionVector;

	float m_Speed;
	float m_Radius{ 10.f };
	float m_TimeElapsedExplosion{};
	float m_TimeUntilExplosion{};


	bool m_Exploded{};
	bool m_StartExplosion{};
};

