#include "pch.h"
#include "ExplodingEnemy.h"

#include "utils.h"

using namespace utils;

ExplodingEnemy::ExplodingEnemy(Point2f pos) : Enemy(pos, 2, Enemy::EnemyType::Exploding, 30.f)
{}

void ExplodingEnemy::Update(float elapsedSec)
{
	if (m_StartExplosion)
	{
		m_TimeElapsedExplosion += elapsedSec * 5.f;
		if (m_Radius < 100.f) m_Radius += 500.f * elapsedSec;
	}

	if (m_TimeElapsedExplosion >= 1.f)
	{
		m_TimeElapsedExplosion = 0.f;
		m_Exploded = true;
	}

	Enemy::Update(elapsedSec);
}

void ExplodingEnemy::Draw()
{
	SetColor(Color4f{ 1.f, 0.5f, 0.f, 1.f });

	if (m_StartExplosion)
	{
		SetColor(Color4f{ 1.f, 0.5f, 0.f, 0.75f });
	}

	Enemy::Draw();
}

void ExplodingEnemy::TriggerExplosion()
{
	m_StartExplosion = true;
	++m_Radius;
	m_DrawThingy = false;
}

bool ExplodingEnemy::GetStartExplosion()
{
	return m_StartExplosion;
}

bool ExplodingEnemy::GetExploded()
{
	return m_Exploded;
}
