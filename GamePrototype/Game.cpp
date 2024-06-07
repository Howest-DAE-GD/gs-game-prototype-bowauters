#include "pch.h"
#include "Game.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Texture.h"
#include "NormalEnemy.h"
#include "ExplodingEnemy.h"
#include "GooEnemy.h"
#include "Grenade.h"

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
	m_GameOverTexture2 = new Texture("Press enter to start again or ESC to quit!", "Fonts/DIN-Light.otf", 50, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_IntroTexture = new Texture("Welcome to Inflatable Apocalypse!", "Fonts/DIN-Light.otf", 75, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_IntroTexture2 = new Texture("Use W, A, S, D to move", "Fonts/DIN-Light.otf", 50, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_IntroTexture3 = new Texture("Left click to shoot", "Fonts/DIN-Light.otf", 50, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_IntroTexture4 = new Texture("Stay alive for as long as possible!", "Fonts/DIN-Light.otf", 50, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_IntroTexture5 = new Texture("Have fun!", "Fonts/DIN-Light.otf", 50, Color4f{ 1.f, 1.f, 1.f, 1.f });

	m_HasAbility = true;

	m_AmountOfWaves = 1;
	m_WaveTexture = new Texture("Wave " + std::to_string(m_AmountOfWaves), "Fonts/DIN-Light.otf", 100, Color4f{ 1.f, 1.f, 1.f, 1.f });
	m_ShowWaveTextureBig = true;

	m_GameOver = false;

	std::cout << "Pick up colours: \n";
	std::cout << "- White: Bullet\n";
	std::cout << "- Green: Health pack\n";
	std::cout << "- Blue: Refresh of bullets\n";
	std::cout << "- Purple: Grenade\n";

	std::cout << "\n";
	std::cout << "\n";


}

void Game::Cleanup()
{
	m_AmountOfWaves = 0;
	m_BulletsShot = 0;     

	delete m_GameOverTexture;
	delete m_GameOverTexture2;
	delete m_IntroTexture;
	delete m_IntroTexture2;
	delete m_IntroTexture3;
	delete m_IntroTexture4;
	delete m_IntroTexture5;

	delete m_WaveTexture;

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

	m_Enemies.clear();
	m_PickUpBullets.clear();
	m_PickUpHealth.clear();
}

void Game::Update( float elapsedSec )
{
	if (!m_GameStart)
	{
		m_TimeGameIntro += elapsedSec;

		if (m_TimeGameIntro >= 10.f) m_GameStart = true;
	}
	else if (!m_GameOver and m_GameStart)
	{
		if (m_Enemies.size() <= 0 or (m_Enemies.at(0)->GetEnemyType() == Enemy::EnemyType::Goo and m_Enemies.at(0)->GetHealth() == 0 and m_Enemies.size() == 1))
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
				int temp{ rand() % 7 + 7 };
				AddBalloons(temp);
			}
			m_ShowWaveTextureBig = false;
			m_TimeBetweenWaves = 0.f;
		}

		if (m_AmountOfWaves % 5 == 0 and !m_ChangedSpeed)
		{
			m_Player->ChangeSpeed();
			m_ChangedSpeed = true;
			m_SpeedEnemies *= 1.25f;
			m_SpeedBullets *= 1.25f;
		}
		else if (m_AmountOfWaves % 5 != 0)
		{
			m_ChangedSpeed = false;
		}

		if (m_AmountOfWaves % 10 == 0 and !m_InfoGooEnemy)
		{
			m_InfoGooEnemy = true;
			std::cout << "Congrats on making it to wave 10. \nThis is a very special enemy. \nIt will Leave a pool of goo behind after you kill it\n";
		}

		m_TimeBetweenPlaceBullets += elapsedSec;
		m_TimeBetweenPlaceHealth += elapsedSec;
		m_TimeBetweenPlacePowerUp += elapsedSec;
		m_TimeBetweenPlaceAbility += elapsedSec;

		m_Player->Update(elapsedSec);

		if (m_Bullets.size() > 0)
		{
			for (Bullet* bullet : m_Bullets)
			{
				bullet->Update(elapsedSec);
			}
		}

		if (m_Grenades.size() > 0)
		{
			for (Grenade* grenade : m_Grenades)
			{
				grenade->Update(elapsedSec);
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

		if (m_Enemies.size())
		{
			if (m_Enemies.at(0)->GetEnemyType() == Enemy::EnemyType::Goo and m_Enemies.at(0)->GetHealth() == 0 and m_Enemies.size() == 1 and !m_ShowWaveTextureBig)
			{
				++m_AmountOfWaves;
				m_TimeBetweenWaves += elapsedSec;
				delete m_WaveTexture;
				m_WaveTexture = new Texture("Wave " + std::to_string(m_AmountOfWaves), "Fonts/DIN-Light.otf", 100, Color4f{ 1.f, 1.f, 1.f, 1.f });
				m_ShowWaveTextureBig = true;
			}
		}

		UpdateEnemyDir(elapsedSec);
		CheckHit();

		PickUpBullets();

		if (m_Player->GetHealth() <= 0)
		{
			m_GameOver = true;
		}

		DeleteBulletsOutOfBounds();
		DeleteGrenadesOutOfBounds();
	}
}

void Game::Draw() const
{
	ClearBackground();
	
	if (!m_GameStart)
	{
		m_IntroTexture->Draw(Rectf{ GetViewPort().width / 2 - m_IntroTexture->GetWidth() / 2, GetViewPort().height / 2 + 150.f, m_IntroTexture->GetWidth(), m_IntroTexture->GetHeight() });
		m_IntroTexture2->Draw(Rectf{ GetViewPort().width / 2 - m_IntroTexture2->GetWidth() / 2, GetViewPort().height / 2 + 50.f, m_IntroTexture2->GetWidth(), m_IntroTexture2->GetHeight() });
		m_IntroTexture3->Draw(Rectf{ GetViewPort().width / 2 - m_IntroTexture3->GetWidth() / 2, GetViewPort().height / 2, m_IntroTexture3->GetWidth(), m_IntroTexture3->GetHeight() });
		m_IntroTexture4->Draw(Rectf{ GetViewPort().width / 2 - m_IntroTexture4->GetWidth() / 2, GetViewPort().height / 2 - 50.f, m_IntroTexture4->GetWidth(), m_IntroTexture4->GetHeight() });
		m_IntroTexture5->Draw(Rectf{ GetViewPort().width / 2 - m_IntroTexture5->GetWidth() / 2, GetViewPort().height / 2 - 150.f, m_IntroTexture5->GetWidth(), m_IntroTexture5->GetHeight() });
	}
	else if (!m_GameOver and m_GameStart)
	{
		m_Player->Draw();
		if (m_Bullets.size() > 0)
		{
			for (Bullet* bullet : m_Bullets)
			{
				bullet->Draw();
			}
		}

		if (m_Grenades.size() > 0)
		{
			for (Grenade* grenade : m_Grenades)
			{
				grenade->Draw();
			}
		}

		SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		for (int i{}; i < m_PickUpBullets.size(); ++i)
		{
			FillEllipse(m_PickUpBullets.at(i));
		}
		
		SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
		for (int i{}; i < m_PickUpHealth.size(); ++i)
		{
			FillEllipse(m_PickUpHealth.at(i));
		}

		SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
		for (int i{}; i < m_PickUpPowerUp.size(); ++i)
		{
			FillEllipse(m_PickUpPowerUp.at(i));
		}

		SetColor(Color4f{ .5f, 0.3f, .8f, 1.f });
		for (int i{}; i < m_PickUpAbility.size(); ++i)
		{
			FillEllipse(m_PickUpAbility.at(i));
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

		SetColor(Color4f{ 0.5f, 0.5f, 0.5f, 1.f });
		if (m_BulletsShot >= 0)
		{
			FillRect(GetViewPort().width - 40.f - 25.f * m_MaxAmountOfBullets - 5.f, 45.f - 15.f, 25.f * m_MaxAmountOfBullets + 10.f, 30.f);
		}
		else
		{
			FillRect(GetViewPort().width - 40.f - 25.f * (m_MaxAmountOfBullets + abs(m_BulletsShot)) - 5.f, 45.f - 15.f, 25.f * (m_MaxAmountOfBullets + abs(m_BulletsShot)) + 10.f, 30.f);
		}

		SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		for (int i{}; i < m_MaxAmountOfBullets - m_BulletsShot; ++i)
		{
			FillEllipse(GetViewPort().width - 50.f - 25.f * i, 45.f, 10.f, 10.f);
		}

		SetColor(Color4f{ 0.5f, 0.5f, 0.5f, 1.f });
		FillRect(GetViewPort().width - 40.f - 25.f * m_GrenadesInMag - 5.f, 80.f - 15.f, 25.f * m_GrenadesInMag + 10.f, 30.f);

		SetColor(Color4f{ .5f, 0.3f, .8f, 1.f });
		for (int i{}; i < m_GrenadesInMag; ++i)
		{
			FillEllipse(GetViewPort().width - 50.f - 25.f * i, 80.f, 10.f, 10.f);
		}
	}
	else 
	{
		m_GameOverTexture->Draw(Rectf{ GetViewPort().width / 2 - m_GameOverTexture->GetWidth() / 2, GetViewPort().height / 2 + m_GameOverTexture->GetHeight() / 2, m_GameOverTexture->GetWidth(), m_GameOverTexture->GetHeight()});
		m_GameOverTexture2->Draw(Rectf{ GetViewPort().width / 2 - m_GameOverTexture2->GetWidth() / 2, GetViewPort().height / 2 - m_GameOverTexture2->GetHeight(), m_GameOverTexture2->GetWidth(), m_GameOverTexture2->GetHeight()});
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_Player->ProcessKeyDownEvent(e);

	switch (e.keysym.sym)
	{
	case SDLK_RETURN:
		if (m_GameOver)
		{
			Cleanup();
			Initialize();
		}
		if (!m_GameStart)
		{
			m_GameStart = true;
		}
		break;
	case SDLK_ESCAPE:
		//SDL_Quit();
		break;
	default:
		break;
	}
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
			m_Bullets.push_back(new Bullet(m_Player->GetPos(), Vector2f{ xDir, yDir }, m_SpeedBullets));
			++m_BulletsShot;
		}
		break;
	case SDL_BUTTON_RIGHT:
		if (m_GrenadesInMag > 0)
		{
			--m_GrenadesInMag;
			m_Grenades.push_back(new Grenade(m_Player->GetPos(), Vector2f{ xDir, yDir }, m_SpeedBullets));
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
			if (IsOverlapping(m_Bullets.at(i)->GetCircle(), m_Enemies.at(j)->GetCircle()) and m_Enemies.at(j)->GetHealth() > 0 and m_Enemies.at(j)->GetDrawEnemy())
			{
				m_Enemies.at(j)->GotHit();

				int temp{ rand() % 15 };
				if (temp == 1)
				{
					m_PickUpPowerUp.push_back(Ellipsef{ m_Enemies.at(j)->GetPos().x, m_Enemies.at(j)->GetPos().y, 10.f, 10.f});
					m_TimeBetweenPlacePowerUp = 0;
				}

				delete m_Bullets.at(i);
				m_Bullets.at(i) = nullptr;
				m_Bullets.erase(m_Bullets.begin() + i);
			}
		}

		for (int i{}; i < m_Grenades.size(); ++i)
		{
			if (IsOverlapping(m_Grenades.at(i)->GetCircle(), m_Enemies.at(j)->GetCircle()) and m_Enemies.at(j)->GetHealth() > 0 and m_Enemies.at(j)->GetDrawEnemy() and m_Grenades.at(i)->GetStartExplosion())
			{
				if (m_Enemies.at(j)->GetEnemyType() == Enemy::Goo)
				{
					GooEnemy* temp = static_cast<GooEnemy*>(m_Enemies.at(j));

					if (!temp->GetHitByGrenade())
					{
						m_Enemies.at(j)->GotHit();
						m_Enemies.at(j)->GotHit();
						m_Enemies.at(j)->GotHit();
						temp->SetHitByGrenade(true);
					}
				}
				else
				{
					m_Enemies.at(j)->GotHit();

					int temp{ rand() % 15 };
					if (temp == 1)
					{
						m_PickUpPowerUp.push_back(Ellipsef{ m_Enemies.at(j)->GetPos().x, m_Enemies.at(j)->GetPos().y, 10.f, 10.f });
						m_TimeBetweenPlacePowerUp = 0;
					}
				}

				/*delete m_Grenades.at(i);
				m_Grenades.at(i) = nullptr;
				m_Grenades.erase(m_Grenades.begin() + i);*/
			}
		}
	}

	for (int j{}; j < m_Enemies.size(); ++j)
	{
		bool blub{};

		if (m_Enemies.at(j)->GetEnemyType() == Enemy::EnemyType::Exploding and m_Enemies.at(j)->GetHealth() <= 0)
		{
			ExplodingEnemy* test{ static_cast<ExplodingEnemy*> (m_Enemies.at(j)) };

			if (!test->GetStartExplosion())
			{
				test->TriggerExplosion();
			}

			if (test->GetExploded())
			{
				m_Enemies.at(j)->SetDrawEnemy(false);

				blub = true;
			}
		}

		if (m_Enemies.at(j)->GetEnemyType() == Enemy::EnemyType::Goo and m_Enemies.at(j)->GetHealth() <= 0)
		{
			GooEnemy* test{ static_cast<GooEnemy*> (m_Enemies.at(j)) };

			if (!IsOverlapping(m_Player->GetCircle(), m_Enemies.at(j)->GetCircle()))
			{
				m_PlayerHitByGoo = false;
			}

			if (!test->GetTriggerGoo())
			{
				test->TriggerGoo();
			}

			if (test->GetGooGone())
			{
				m_Enemies.at(j)->SetDrawEnemy(false);

				blub = true;
			}
		}

		if (!blub)
		{
			if (m_Enemies.at(j)->GetHealth() == 0 and m_Enemies.at(j)->GetEnemyType() == Enemy::EnemyType::Normal)
			{
				m_Enemies.at(j)->SetDrawEnemy(false);
			}
		}
	}


	for (int j{}; j < m_Enemies.size(); ++j)
	{
		if (IsOverlapping(m_Player->GetCircle(), m_Enemies.at(j)->GetCircle()) and m_Enemies.at(j)->GetDrawEnemy())
		{

			if (!m_PlayerHitByGoo) m_Player->GotHit();

			if (m_Enemies.at(j)->GetEnemyType() == Enemy::Goo) m_PlayerHitByGoo = true;

			if (m_Enemies.at(j)->GetEnemyType() != Enemy::Goo)
			{
				m_Enemies.at(j)->SetDrawEnemy(false);
			}
		}

		if (!IsOverlapping(m_Player->GetCircle(), m_Enemies.at(j)->GetCircle()) and !m_Enemies.at(j)->GetDrawEnemy())
		{
			if (m_Enemies.at(j)->GetEnemyType() == Enemy::Goo) m_PlayerHitByGoo = false;
		}
	}

	for (int j{}; j < m_Enemies.size(); ++j)
	{
		if (!m_Enemies.at(j)->GetDrawEnemy())
		{
			delete m_Enemies.at(j);
			m_Enemies.at(j) = nullptr;
		}
	}
	m_Enemies.erase(std::remove_if(m_Enemies.begin(), m_Enemies.end(), [](Enemy* coin) { return coin == nullptr; }), m_Enemies.end());
}

void Game::AddBalloons(int amount)
{
	if (m_AmountOfWaves % 5 != 0)
	{
		if (m_AmountOfWaves == 1 or m_AmountOfWaves == 2)
		{
			for (int i{}; i < amount; ++i)
			{
				m_Enemies.push_back(new NormalEnemy(Point2f{ rand() % 800 * 1.f + 100.f, rand() % 500 * 1.f + 250.f }, m_SpeedEnemies));
			}
		}
		else
		{
			int temp{ rand() % (amount - 2) };

			for (int i{}; i < amount - temp; ++i)
			{
				m_Enemies.push_back(new NormalEnemy(Point2f{ rand() % 800 * 1.f + 100.f, rand() % 500 * 1.f + 250.f }, m_SpeedEnemies));
			}
			for (int i{}; i < temp; ++i)
			{
				m_Enemies.push_back(new ExplodingEnemy(Point2f{ rand() % 800 * 1.f + 100.f, rand() % 500 * 1.f + 250.f }, m_SpeedEnemies));
			}
		}
	}
	else if (m_AmountOfWaves % 10 == 0)
	{
		m_Enemies.push_back(new GooEnemy(Point2f{ rand() % 800 * 1.f + 100.f, rand() % 500 * 1.f + 250.f }, m_SpeedEnemies));
	}
	else
	{
		for (int i{}; i < amount; ++i)
		{
			m_Enemies.push_back(new ExplodingEnemy(Point2f{ rand() % 800 * 1.f + 100.f, rand() % 500 * 1.f + 250.f }, m_SpeedEnemies));
		}
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

void Game::DeleteGrenadesOutOfBounds()
{
	for (int i{}; i < m_Grenades.size(); ++i)
	{
		if (m_Grenades.at(i)->GetCircle().center.x < 0.f or m_Grenades.at(i)->GetCircle().center.x > GetViewPort().width or m_Grenades.at(i)->GetCircle().center.y < 0.f or m_Grenades.at(i)->GetCircle().center.y > GetViewPort().height or m_Grenades.at(i)->GetExploded())
		{
			delete m_Grenades.at(i);
			m_Grenades.at(i) = nullptr;
			m_Grenades.erase(m_Grenades.begin() + i);

			for (int j{}; j < m_Enemies.size(); ++j)
			{
				if (m_Enemies.at(j)->GetEnemyType() == Enemy::Goo)
				{
					GooEnemy* temp = static_cast<GooEnemy*>(m_Enemies.at(j));

					temp->SetHitByGrenade(false);
				}
			}
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

	if (m_BulletsShot > 0 and m_TimeBetweenPlacePowerUp >= 15.f)
	{
		m_PickUpPowerUp.push_back(Ellipsef{ rand() % 1200 * 1.f, rand() % 700 * 1.f, 10.f, 10.f });
		m_TimeBetweenPlacePowerUp = 0;
	}

	if (m_TimeBetweenPlacePowerUp >= 15.f)
	{
		m_PickUpPowerUp.push_back(Ellipsef{ rand() % 1200 * 1.f, rand() % 700 * 1.f, 10.f, 10.f });
		m_TimeBetweenPlacePowerUp = 0;
	}

	if (m_TimeBetweenPlaceAbility >= 25.f)
	{
		m_PickUpAbility.push_back(Ellipsef{ rand() % 1200 * 1.f, rand() % 700 * 1.f, 10.f, 10.f });
		m_TimeBetweenPlaceAbility = 0;
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

	for (int i{}; i < m_PickUpPowerUp.size(); ++i)
	{
		if (IsOverlapping(m_Player->GetCircle(), Circlef{ m_PickUpPowerUp.at(i).center, m_PickUpPowerUp.at(i).radiusX }))
		{
			if (m_BulletsShot >= 0)	m_BulletsShot = 0;
			m_PickUpPowerUp.erase(m_PickUpPowerUp.begin() + i);
		}
	}

	for (int i{}; i < m_PickUpAbility.size(); ++i)
	{
		if (IsOverlapping(m_Player->GetCircle(), Circlef{ m_PickUpAbility.at(i).center, m_PickUpAbility.at(i).radiusX }))
		{
			++m_GrenadesInMag;
			m_PickUpAbility.erase(m_PickUpAbility.begin() + i);
		}
	}
}