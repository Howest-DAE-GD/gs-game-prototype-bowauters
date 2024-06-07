#include "pch.h"
#include "Grenade.h"

#include "utils.h"
#include <iostream>

using namespace utils;

Grenade::Grenade(Point2f pos, Vector2f direction, float speed)
	: m_Pos{pos}
	, m_DirectionVector{direction}
	, m_Speed{speed}
{}

void Grenade::Update(float elapsedSec)
{
	m_TimeUntilExplosion += elapsedSec;

	if (m_TimeUntilExplosion >= 1.f) m_StartExplosion = true;

	if (m_StartExplosion)
	{
		m_TimeElapsedExplosion += elapsedSec * 5.f;
		if (m_Radius < 200.f) m_Radius += 1000.f * elapsedSec;
	}

	if (m_TimeElapsedExplosion >= 1.f)
	{
		m_TimeElapsedExplosion = 0.f;
		m_Exploded = true;
	}

	if (!m_StartExplosion)
	{
		m_Pos.x += m_DirectionVector.x * m_Speed * elapsedSec;
		m_Pos.y += m_DirectionVector.y * m_Speed * elapsedSec;
	}
}

void Grenade::Draw()
{
	glPushMatrix();
	{
		if (m_Exploded)	SetColor(Color4f{ 1.f, 0.f, 0.6f, .7f });
		else SetColor(Color4f{ .5f, 0.3f, .8f, 1.f });

		glTranslatef(m_Pos.x, m_Pos.y, 0.f);
		FillEllipse(Point2f{}, m_Radius, m_Radius);
	}
	glPopMatrix();
}

Circlef Grenade::GetCircle()
{
	return Circlef{m_Pos.x, m_Pos.y, m_Radius};
}

bool Grenade::GetExploded()
{
	return m_Radius > 200.f;
}

bool Grenade::GetStartExplosion()
{
	return m_StartExplosion;
}

