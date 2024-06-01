#pragma once
#include "BaseGame.h"

#include <vector>

class Player;
class Bullet;
class Enemy;
class Texture;
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
	Texture* m_GameOverTexture{};
	Texture* m_GameOverTexture2{};
	Texture* m_IntroTexture{};
	Texture* m_IntroTexture2{};
	Texture* m_IntroTexture3{};
	Texture* m_IntroTexture4{};
	Texture* m_IntroTexture5{};
	Texture* m_WaveTexture{};

	bool m_EnemyDead{};
	bool m_GameOver{};
	bool m_ShowWaveTextureBig{};
	bool m_PlayerHitByGoo{};
	bool m_GameStart{};

	float m_TimeBetweenWaves{};
	float m_TimeBetweenPlaceBullets{};
	float m_TimeBetweenPlaceHealth{};
	float m_TimeGameIntro{};

	int m_AmountOfWaves{};
	int m_MaxAmountOfBullets{15};
	int m_BulletsShot{};

	std::vector<Bullet*> m_Bullets{};
	std::vector<Enemy*> m_Enemies{};
	std::vector<Ellipsef> m_PickUpBullets{};
	std::vector<Ellipsef> m_PickUpHealth{};

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	float CalcMagnitude(const Point2f& otherPoint);
	void UpdateEnemyDir(float elapsedSec);

	void CheckHit();
	void AddBalloons(int amount);
	void DeleteBulletsOutOfBounds();
	void PlaceDownBullets();
	void PickUpBullets();
};