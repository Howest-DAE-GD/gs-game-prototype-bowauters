#pragma once

#include <Vector2f.h>

class Bullet
{
public:
	Bullet(Point2f pos, Vector2f direction);

	void Update(float elapsedSec);
	void Draw();


private:
	Point2f m_Pos;
	Vector2f m_DirectionVector;

	float m_Speed;
};

