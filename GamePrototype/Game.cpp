#include "pch.h"
#include "Game.h"

#include "Player.h"
#include "Bullet.h"

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
}

void Game::Cleanup( )
{
	delete m_Player;
	delete m_Bullet;
}

void Game::Update( float elapsedSec )
{
	m_Player->Update(elapsedSec);
	if (m_HasShot)	m_Bullet->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_Player->Draw();
	if (m_HasShot)	m_Bullet->Draw();
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
		m_HasShot = true;
		delete m_Bullet;
		m_Bullet = new Bullet(m_Player->GetPos(), Vector2f{ xDir, yDir });
		break;
	/*case SDL_BUTTON_RIGHT:
		std::cout << " right button " << std::endl;
		break;
	case SDL_BUTTON_MIDDLE:
		std::cout << " middle button " << std::endl;
		break;*/
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