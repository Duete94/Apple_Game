#include "Stone.h"
# include "Game.h"


namespace ApplesGame
{
	void InitStones(Stone& stone, const Game& game)
	{

		stone.sprite.setTexture(game.stoneTexture);
		SetSpriteSize(stone.sprite, STONES_SIZE, STONES_SIZE);
		SetSpriteRelativeOrigin(stone.sprite, 0.5f, 0.5f);
		
	}

	void SetStonePosition(Stone& stone, const Position2D& position)
	{
		stone.Position = position;
	}
	
	Rectangle GetStoneCollider(const Stone& stone)
	{
		return 
		{ 
			{ stone.Position.x - STONES_SIZE / 2.f, stone.Position.y - STONES_SIZE / 2.f },
					{ STONES_SIZE, STONES_SIZE } 
		};
	}

	void DrawStone(Stone& stone, sf::RenderWindow& window)
	{
		stone.sprite.setPosition(stone.Position.x, stone.Position.y);
		window.draw(stone.sprite);
		
	}
}
