#include "pch.h"
#include "NormalEnemy.h"

#include "utils.h"

using namespace utils;

NormalEnemy::NormalEnemy(Point2f pos) : Enemy(pos, 1, Enemy::EnemyType::Normal, 20.f)
{}

void NormalEnemy::Draw()
{
	SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });

	Enemy::Draw();
}