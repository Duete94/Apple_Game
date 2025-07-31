#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Const.h"

namespace ApplesGame
{
	struct Stone
	{
		Position2D Position;
		sf::Sprite sprite;
	};

	struct Game;

	void InitStones(Stone& stone, const Game& game);
	void SetStonePosition(Stone& stone, const Position2D& position);
	Rectangle GetStoneCollider(const Stone& stone);
	void DrawStone(Stone& stone, sf::RenderWindow& window);
}
