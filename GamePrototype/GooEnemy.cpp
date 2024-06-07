#include "pch.h"
#include "GooEnemy.h"

#include <utils.h>

using namespace utils;

GooEnemy::GooEnemy(Point2f pos, float speed)
	: Enemy(pos, 11, Enemy::EnemyType::Goo, 80.f, speed)
{}

void GooEnemy::Update(float elapsedSec)
{
	if (m_TriggerGoo and m_GooTimer <= 0.f)
	{
		m_GooTimer += elapsedSec;
		m_Radius = 150.f;
		m_Update = false;
		m_DrawThingy = false;
	}

	if (m_TriggerGoo)
	{
		m_GooTimer += elapsedSec;
		m_Radius -= elapsedSec;
	}

	if (m_GooTimer >= m_GooMaxTime)
	{
		m_GooGone = true;
	}

	Enemy::Update(elapsedSec);
}

void GooEnemy::Draw()
{
	if (!m_TriggerGoo)
	{
		SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	}
	else
	{
		SetColor(Color4f{ 0.f, 1.f, 0.f, 0.8f });
	}

	Enemy::Draw();
}

void GooEnemy::TriggerGoo()
{
	m_TriggerGoo = true;
}

bool GooEnemy::GetTriggerGoo()
{
	return m_TriggerGoo;
}

bool GooEnemy::GetGooGone()
{
	return m_GooGone;
}
