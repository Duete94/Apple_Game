#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Const.h"

namespace ApplesGame
{
	struct Game;

	struct Apple
	{
		Position2D Position;
		sf::Sprite sprite;
	};

	struct Game;

	void InitApples(Apple& apple, const Game& game);
	void SetApplePosition(Apple& apple, const Position2D& position);
	Circle GetAppleCollider(const Apple& apple);
	void DrawApples(Apple& apple, sf::RenderWindow& window);
	
}

