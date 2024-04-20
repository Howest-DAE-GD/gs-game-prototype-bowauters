#include "pch.h"
#include "Bullet.h"

#include "utils.h"
using namespace utils;

Bullet::Bullet(Point2f pos, Vector2f direction) : m_Pos{ pos }, m_DirectionVector{ direction }, m_Speed{ 100.f }
{}

void Bullet::Update(float elapsedSec)
{
	m_Pos.x += m_DirectionVector.x * m_Speed * elapsedSec;
	m_Pos.y += m_DirectionVector.y * m_Speed * elapsedSec;
}

void Bullet::Draw()
{
	glPushMatrix();
	{
		glTranslatef(m_Pos.x, m_Pos.y, 0.f);
		SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		FillEllipse(Point2f{}, 10.f, 10.f);
	}
	glPopMatrix();
}