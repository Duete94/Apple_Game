# include "Apple.h"
# include "Game.h"


namespace ApplesGame
{
	void InitApples(Apple& apple, const Game& game)
	{

		apple.sprite.setTexture(game.appleTexture);
		SetSpriteSize(apple.sprite, APPLE_SIZE, APPLE_SIZE);
		SetSpriteRelativeOrigin(apple.sprite, 0.5f, 0.5f);
	} 

	void SetApplePosition(Apple& apple, const Position2D& position)
	{
		apple.Position = position;
	}

	Circle GetAppleCollider(const Apple& apple)
	{
		return { apple.Position, APPLE_SIZE/2.f };
	}
	
	void DrawApples(Apple& apple, sf::RenderWindow& window)
	{
		apple.sprite.setPosition(apple.Position.x, apple.Position.y);
		window.draw(apple.sprite);
	}
}
 