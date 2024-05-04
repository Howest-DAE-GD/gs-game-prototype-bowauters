#include "pch.h"
#include "Player.h"

#include "utils.h"
#include <iostream>
#include "vector"

using namespace utils;

Player::Player() : m_Pos{ Point2f {100.f, 100.f} }, m_Speed{ 150.f }, m_Health{ 10 }
{}

void Player::Update(float elapsedSec)
{
	ChangeDirection();

	m_Pos.x += m_DirectionVector.x * m_Speed * elapsedSec;
	m_Pos.y += m_DirectionVector.y * m_Speed * elapsedSec;

	CalculateAngle();
}

void Player::Draw()
{
	glPushMatrix();
	{
		glTranslatef(m_Pos.x, m_Pos.y, 0.f);
	
		SetColor(Color4f{ 0.f, 0.8f, 1.f, 1.f });
		FillEllipse(0.f, 0.f, m_Radius, m_Radius);

		glRotatef(m_Angle - 90.f, 0.f, 0.f, 1.f);

		FillRect(-5.f, 10.f, 10.f, 30.f);
	}
	glPopMatrix();
}

void Player::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	switch ( e.keysym.sym )
	{
	case SDLK_a:
		m_IsLeft = true;
		break;
	case SDLK_d:
		m_IsRight = true;
		break;
	case SDLK_w:
		m_IsUp = true;
		break;
	case SDLK_s:
		m_IsDown = true;
		break;
	}
}

void Player::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_a:
		m_IsLeft = false;
		break;
	case SDLK_d:
		m_IsRight = false;
		break;
	case SDLK_w:
		m_IsUp = false;
		break;
	case SDLK_s:
		m_IsDown = false;
		break;
	}
}

void Player::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_MousePos = Point2f{ float(e.x), float(e.y) };
}

const Point2f& Player::GetPos()
{
	return m_Pos;
}

int Player::GetHealth()
{
	return m_Health;
}

void Player::IncreaseHealth()
{
	++m_Health;
}

Circlef Player::GetCircle()
{
	Circlef c{ m_Pos, m_Radius };
	return c;
}

void Player::GotHit()
{
	m_Health -= 1;
}

void Player::ChangeDirection()
{
	m_DirectionVector = Vector2f{};

	if (m_IsUp)
	{
		m_DirectionVector.y += 1;
	}
	if (m_IsLeft)
	{
		m_DirectionVector.x -= 1;
	}
	if (m_IsDown)
	{
		m_DirectionVector.y -= 1;
	}
	if (m_IsRight)
	{
		m_DirectionVector.x += 1;
	}
}

void Player::CalculateAngle()
{
	float dx = m_MousePos.x - m_Pos.x;
	float dy = m_MousePos.y - m_Pos.y;

	float angle_rad = std::atan2(dy, dx);

	m_Angle = angle_rad * (180.f / M_PI);
}
