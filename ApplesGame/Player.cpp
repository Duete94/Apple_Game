# include "Player.h"
# include "Game.h"

namespace ApplesGame
{
	void InitPlayer(Player& player, const Game& game)
	{
		// Init player
		player.Position = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
		player.Speed = INITIAL_SPEED;
		player.Direction = PlayerDirection::Right;
		
		//init player sprite
		player.sprite.setTexture(game.playerTexture);
		SetSpriteSize(player.sprite, PLAYER_SIZE, PLAYER_SIZE);
		SetSpriteRelativeOrigin(player.sprite, 0.5f, 0.5f);
		

	}
	
	void SetPlayerDirection(Player& player, PlayerDirection Direction)
	{
		player.Direction = Direction;
	}

	void SetPlayerPosition(Player& player, const Position2D& Position)
	{
		player.Position = Position;
	}

	void SetPlayerSpeed(Player& player, float speed)
	{
		player.Speed = speed;
	}

	float GetPlayerSpeed(const Player& player)
	{
		return player.Speed;
	}

	Rectangle GetPlayerCollider(const Player& player)
	{
		return { { player.Position.x - PLAYER_SIZE / 2.f, player.Position.y - PLAYER_SIZE / 2.f },
			{ PLAYER_SIZE, PLAYER_SIZE } };
	}

	void UpdatePlayer(Player& player, float deltaTime)
	{
		switch (player.Direction)
		{
		case PlayerDirection::Right:
		{
			player.Position.x += player.Speed * deltaTime;
			break;
		}
		case PlayerDirection::Up:
		{
			player.Position.y -= player.Speed * deltaTime;
			break;
		}
		case PlayerDirection::Left:
		{
			player.Position.x -= player.Speed * deltaTime;
			break;
		}
		case PlayerDirection::Down:
		{
			player.Position.y += player.Speed * deltaTime;
			break;
		}
		}
	}

	void DrawPlayer(Player& player, sf::RenderWindow& window)
	{
		player.sprite.setPosition(player.Position.x, player.Position.y);
		window.draw(player.sprite);

		if (player.Direction == PlayerDirection::Left)
		{
			SetSpriteSize(player.sprite, -PLAYER_SIZE, PLAYER_SIZE);
			player.sprite.setRotation(0.f);

		}
		else
		{
			SetSpriteSize(player.sprite, PLAYER_SIZE, PLAYER_SIZE);
			player.sprite.setRotation((float)player.Direction * -90.f);
		}
	}
}

