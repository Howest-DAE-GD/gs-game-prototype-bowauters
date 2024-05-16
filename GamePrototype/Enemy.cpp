#include "pch.h"
#include "Enemy.h"

#include <utils.h>
#include <iostream>

using namespace utils;

Enemy::Enemy(Point2f pos, int health, EnemyType type, float radius) : m_Pos{ pos }, m_Health{ health }, m_DirectionVector{}, m_Speed{ 70.f }, m_EnemyType{ type }, m_DrawThingy{ true }, m_Radius{ radius }
{}

void Enemy::Update(float elapsedSec)
{
	if (m_Update)
	{
		m_Pos.x -= m_DirectionVector.x * m_Speed * elapsedSec;
		m_Pos.y -= m_DirectionVector.y * m_Speed * elapsedSec;
	}
}

void Enemy::Draw()
{
	glPushMatrix();
	{
		if (m_DrawEnemy)
		{
			glTranslatef(m_Pos.x, m_Pos.y, 0.f);
			if (m_DrawThingy) FillTriangle(Point2f{ 0.f, -m_Radius / 2 + 2.f }, Point2f{ 7.5f, -m_Radius - 6.f }, Point2f{ -7.5f, -m_Radius - 6.f });
			FillEllipse(Point2f{}, m_Radius, m_Radius);
		}
	}
	glPopMatrix();
}

void Enemy::UpdateDirection(Vector2f direction)
{
	m_DirectionVector = direction;
}

const Point2f& Enemy::GetPos()
{
	return m_Pos;
}

Circlef Enemy::GetCircle()
{
	Circlef c{ m_Pos, m_Radius };
	return c;
}

void Enemy::GotHit()
{
	std::cout << "HELP" << std::endl;
	m_Health -= 1;
	m_Radius -= 5.f;
}

int Enemy::GetHealth()
{
	return m_Health;
}

Enemy::EnemyType Enemy::GetEnemyType()
{
	return m_EnemyType;
}

void Enemy::SetDrawEnemy(bool val)
{
	m_DrawEnemy = val;
}

bool Enemy::GetDrawEnemy()
{
	return m_DrawEnemy;
}

