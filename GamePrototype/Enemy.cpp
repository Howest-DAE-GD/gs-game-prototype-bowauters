#include "pch.h"
#include "Enemy.h"

#include <utils.h>
#include <iostream>

using namespace utils;

Enemy::Enemy(Point2f pos, int health) : m_Pos{pos}, m_Health{health}, m_DirectionVector{}, m_Speed{70.f}
{}

void Enemy::Update(float elapsedSec)
{
	m_Pos.x -= m_DirectionVector.x * m_Speed * elapsedSec;
	m_Pos.y -= m_DirectionVector.y * m_Speed * elapsedSec;
}

void Enemy::Draw()
{
	glPushMatrix();
	{
		glTranslatef(m_Pos.x, m_Pos.y, 0.f);
		SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		FillTriangle(Point2f{ 0.f, -17.f }, Point2f{ 7.5f, -26.f }, Point2f{ -7.5f, -26.f });
		FillEllipse(Point2f{}, m_Radius, m_Radius);
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
}

int Enemy::GetHealth()
{
	return m_Health;
}

