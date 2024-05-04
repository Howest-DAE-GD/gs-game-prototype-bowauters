#include "pch.h"
#include "Game.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Texture.h"

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
	m_GameOverTexture = new Texture("Game Over!", "Fonts/DIN-Light.otf", 100, Color4f{ 1.f, 1.f, 1.f, 1.f });

	m_AmountOfWaves = 1;
	m_WaveTexture = new Texture("Wave " + std::to_string(m_AmountOfWaves), "Fonts/DIN-Light.otf", 100, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_ShowWaveTextureBig = true;
}

void Game::Cleanup()
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
	if (!m_GameOver)
	{
		m_TimeBetweenPlaceBullets += elapsedSec;
		m_TimeBetweenPlaceHealth += elapsedSec;

		m_Player->Update(elapsedSec);

		if (m_Bullets.size() > 0)
		{
			for (Bullet* bullet : m_Bullets)
			{
				bullet->Update(elapsedSec);
			}
		}

		PlaceDownBullets();

		if (m_Enemies.size() <= 0 and !m_ShowWaveTextureBig)
		{
			++m_AmountOfWaves;
			m_TimeBetweenWaves += elapsedSec;
			delete m_WaveTexture;
			m_WaveTexture = new Texture("Wave " + std::to_string(m_AmountOfWaves), "Fonts/DIN-Light.otf", 100, Color4f{ 1.f, 1.f, 1.f, 1.f });
			m_ShowWaveTextureBig = true;
		}

		if (m_Enemies.size() <= 0)
		{
			m_TimeBetweenWaves += elapsedSec;
		}

		if (m_TimeBetweenWaves >= 3.f)
		{
			if (m_AmountOfWaves == 1)
			{
				AddBalloons(5);
			}
			else
			{
				AddBalloons(7);
			}
			m_ShowWaveTextureBig = false;
			m_TimeBetweenWaves = 0.f;
		}

		UpdateEnemyDir(elapsedSec);
		CheckHit();

		PickUpBullets();

		if (m_Player->GetHealth() <= 0)
		{
			m_GameOver = true;
		}

		DeleteBulletsOutOfBounds();
	}
}

void Game::Draw() const
{
	ClearBackground();
	
	if (!m_GameOver)
	{
		m_Player->Draw();
		if (m_Bullets.size() > 0)
		{
			for (Bullet* bullet : m_Bullets)
			{
				bullet->Draw();
			}
		}

		SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		for (int i{}; i < m_MaxAmountOfBullets - m_BulletsShot; ++i)
		{
			FillEllipse(GetViewPort().width - 45.f - 25.f * i, 45.f, 10.f, 10.f);
		}

		for (int i{}; i < m_PickUpBullets.size(); ++i)
		{
			FillEllipse(m_PickUpBullets.at(i));
		}
		
		SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
		for (int i{}; i < m_PickUpHealth.size(); ++i)
		{
			FillEllipse(m_PickUpHealth.at(i));
		}

		for (int j{}; j < m_Enemies.size(); ++j)
		{
			m_Enemies.at(j)->Draw();
		}

		if (m_ShowWaveTextureBig)
		{
			m_WaveTexture->Draw(Rectf{ GetViewPort().width / 2 - m_WaveTexture->GetWidth() / 2, GetViewPort().height / 2 - m_WaveTexture->GetHeight() / 2, m_WaveTexture->GetWidth(), m_WaveTexture->GetHeight() });
		}
		else
		{
			m_WaveTexture->Draw(Rectf{ GetViewPort().width - m_WaveTexture->GetWidth() / 3.f - 40.f, GetViewPort().height - m_WaveTexture->GetHeight() / 3.f - 40.f, m_WaveTexture->GetWidth() / 3.f, m_WaveTexture->GetHeight() / 3.f });
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
		FillRect(Point2f{ 40.f, GetViewPort().height - 70.f }, 30.f * m_Player->GetHealth(), 30.f);
	}
	else 
	{
		m_GameOverTexture->Draw(Rectf{ GetViewPort().width / 2 - m_GameOverTexture->GetWidth() / 2, GetViewPort().height / 2 - m_GameOverTexture->GetHeight() / 2, m_GameOverTexture->GetWidth(), m_GameOverTexture->GetHeight()});
	}
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
	float mag{ CalcMagnitude(m_MousePos) };
	float xDir{ xPos / mag };
	float yDir{ yPos / mag };

	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		if (m_BulletsShot < m_MaxAmountOfBullets)
		{
			m_Bullets.push_back(new Bullet(m_Player->GetPos(), Vector2f{ xDir, yDir }));
			++m_BulletsShot;
		}
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

void Game::ClearBackground() const
{
	glClearColor( 0.0f, 0.0f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

float Game::CalcMagnitude(const Point2f& otherPoint)
{
	float xPos{ otherPoint.x - m_Player->GetPos().x};
	float yPos = { otherPoint.y - m_Player->GetPos().y };

	return float(sqrt((xPos * xPos) + (yPos * yPos)));
}

void Game::UpdateEnemyDir(float elapsedSec)
{
	for (int j{}; j < m_Enemies.size(); ++j)
	{
		float xPos{ m_Enemies.at(j)->GetPos().x - m_Player->GetPos().x };
		float yPos{ m_Enemies.at(j)->GetPos().y - m_Player->GetPos().y };

		float mag{ CalcMagnitude(m_Enemies.at(j)->GetPos()) };

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
		m_Enemies.push_back(new Enemy(Point2f{ rand() % 800 * 1.f + 100.f, rand() % 500 * 1.f + 250.f }, 1));
	}
}

void Game::DeleteBulletsOutOfBounds()
{
	for (int i{}; i < m_Bullets.size(); ++i)
	{
		if (m_Bullets.at(i)->GetCircle().center.x < 0.f or m_Bullets.at(i)->GetCircle().center.x > GetViewPort().width or m_Bullets.at(i)->GetCircle().center.y < 0.f or m_Bullets.at(i)->GetCircle().center.y > GetViewPort().height)
		{
			delete m_Bullets.at(i);
			m_Bullets.at(i) = nullptr;
			m_Bullets.erase(m_Bullets.begin() + i);
		}
	}
}

void Game::PlaceDownBullets()
{
	if (m_BulletsShot > 0 and m_TimeBetweenPlaceBullets >= 5.f)
	{
		m_PickUpBullets.push_back(Ellipsef{ rand() % 1200 * 1.f, rand() % 700 * 1.f, 10.f, 10.f });
		m_TimeBetweenPlaceBullets = 0;
	}

	if (m_Player->GetHealth() < 8 and m_TimeBetweenPlaceHealth >= 10.f)
	{
		m_PickUpHealth.push_back(Ellipsef{ rand() % 1200 * 1.f, rand() % 700 * 1.f, 10.f, 10.f });
		m_TimeBetweenPlaceHealth = 0;
	}
}

void Game::PickUpBullets()
{
	for (int i{}; i < m_PickUpBullets.size(); ++i)
	{
		if (IsOverlapping(m_Player->GetCircle(), Circlef{ m_PickUpBullets.at(i).center, m_PickUpBullets.at(i).radiusX }))
		{
			--m_BulletsShot;
			m_PickUpBullets.erase(m_PickUpBullets.begin() + i);
		}
	}

	for (int i{}; i < m_PickUpHealth.size(); ++i)
	{
		if (IsOverlapping(m_Player->GetCircle(), Circlef{ m_PickUpHealth.at(i).center, m_PickUpHealth.at(i).radiusX }))
		{
			m_Player->IncreaseHealth();
			m_PickUpHealth.erase(m_PickUpHealth.begin() + i);
		}
	}
}
