#pragma once
#include "BaseGame.h"

#include <vector>

class Player;
class Bullet;
class Enemy;
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	//Variables
	Player* m_Player{};
	Point2f m_MousePos{};
	Enemy* m_Enemy{};

	bool m_EnemyDead{};

	std::vector<Bullet*> m_Bullets{};
	std::vector<Enemy*> m_Enemies{};

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	float CalcMagnitude(const Point2f& otherPoint);
	void UpdateEnemyDir(float elapsedSec);

	void CheckHit();
	void AddBalloons(int amount);
};