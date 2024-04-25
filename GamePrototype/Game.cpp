#include "pch.h"
#include "Game.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

#include "utils.h"
#include <iostream>

using namespace utils;

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Player = new Player();

	AddBalloons(5);
}

void Game::Cleanup( )
{
	delete m_Player;
	for (int i{}; i < m_Bullets.size(); ++i)
	{
		delete m_Bullets.at(i);
		m_Bullets.at(i) = nullptr;
	}

	for (int j{}; j < m_Enemies.size(); ++j)
	{
		delete m_Enemies.at(j);
		m_Enemies.at(j) = nullptr;
	}
}

void Game::Update( float elapsedSec )
{
	m_Player->Update(elapsedSec);

	if (m_Bullets.size() > 0)
	{
		for (Bullet* bullet : m_Bullets)
		{
			bullet->Update(elapsedSec);
		}
	}

	if(m_Enemies.size() <= 0)
	{
		AddBalloons(7);
	}

	UpdateEnemyDir(elapsedSec);
	CheckHit();
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_Player->Draw();
	if (m_Bullets.size() > 0)
	{
		for (Bullet* bullet: m_Bullets)
		{
			bullet->Draw();
		}
	}

	for (int j{}; j < m_Enemies.size(); ++j)
	{
		m_Enemies.at(j)->Draw();
	}

	if (m_Player->GetHealth() <= 2)
	{
		SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	}
	else if (m_Player->GetHealth() <= 5)
	{
		SetColor(Color4f{ 1.f, 0.5f, 0.f, 1.f });
	}
	else 
	{
		SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	}
	FillRect(Point2f{ 40.f, GetViewPort().height - 60.f }, 20.f * m_Player->GetHealth(), 20.f);
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_Player->ProcessKeyDownEvent(e);
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_Player->ProcessKeyUpEvent(e);
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_Player->ProcessMouseMotionEvent(e);
	m_MousePos = Point2f{ float(e.x), float(e.y) };
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	float xPos{ m_MousePos.x - m_Player->GetPos().x };
	float yPos { m_MousePos.y - m_Player->GetPos().y };
	float mag{ CalcMagnitude() };
	float xDir{ xPos / mag };
	float yDir{ yPos / mag };

	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		m_Bullets.push_back(new Bullet(m_Player->GetPos(), Vector2f{ xDir, yDir }));
		break;
	default:
		break;
	}	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

float Game::CalcMagnitude()
{
	float xPos{ m_MousePos.x - m_Player->GetPos().x};
	float yPos = { m_MousePos.y - m_Player->GetPos().y };

	return float(sqrt((xPos * xPos) + (yPos * yPos)));
}

void Game::UpdateEnemyDir(float elapsedSec)
{
	for (int j{}; j < m_Enemies.size(); ++j)
	{
		float xPos{ m_Enemies.at(j)->GetPos().x - m_Player->GetPos().x };
		float yPos{ m_Enemies.at(j)->GetPos().y - m_Player->GetPos().y };
		float mag{ CalcMagnitude() };
		float xDir{ xPos / mag };
		float yDir{ yPos / mag };

		m_Enemies.at(j)->UpdateDirection(Vector2f{ xDir, yDir });
		m_Enemies.at(j)->Update(elapsedSec);
	}
	
}

void Game::CheckHit()
{
	for (int j{}; j < m_Enemies.size(); ++j)
	{
		for (int i{}; i < m_Bullets.size(); ++i)
		{
			if (IsOverlapping(m_Bullets.at(i)->GetCircle(), m_Enemies.at(j)->GetCircle()))
			{
				std::cout << "Got hit" << std::endl;
				m_Enemies.at(j)->GotHit();
				delete m_Bullets.at(i);
				m_Bullets.at(i) = nullptr;
				m_Bullets.erase(m_Bullets.begin() + i);
			}
		}
	}

	for (int j{}; j < m_Enemies.size(); ++j)
	{
		if (m_Enemies.at(j)->GetHealth() == 0)
		{
			delete m_Enemies.at(j);
			m_Enemies.at(j) = nullptr;
			m_Enemies.erase(m_Enemies.begin() + j);
		}
	}

	for (int j{}; j < m_Enemies.size(); ++j)
	{
		if (IsOverlapping(m_Player->GetCircle(), m_Enemies.at(j)->GetCircle()))
		{
			m_Player->GotHit();
			delete m_Enemies.at(j);
			m_Enemies.at(j) = nullptr;
			m_Enemies.erase(m_Enemies.begin() + j);
		}
	}
}

void Game::AddBalloons(int amount)
{
	for (int i{}; i < amount; ++i)
	{
		m_Enemies.push_back(new Enemy(Point2f{ rand() % 800 * 1.f + 100.f, rand() % 500 * 1.f + 250.f }, 2));
	}
}
