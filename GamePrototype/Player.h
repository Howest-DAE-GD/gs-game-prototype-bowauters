#pragma once

#include <Vector2f.h>

class Player
{
public:

	Player();

	void Update(float elapsedSec);
	void Draw();
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);

	const Point2f& GetPos();
	int GetHealth();
	void IncreaseHealth();
	Circlef GetCircle();
	void GotHit();


private:
	Point2f m_Pos;
	Vector2f m_DirectionVector{};
	Point2f m_MousePos{};

	float m_Speed;
	float m_Angle{};
	float m_Radius{ 20.f };

	bool m_IsUp{};
	bool m_IsDown{};
	bool m_IsRight{};
	bool m_IsLeft{};

	int m_Health;

	void ChangeDirection();
	void CalculateAngle();
};

