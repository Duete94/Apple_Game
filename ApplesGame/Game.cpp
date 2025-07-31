#include "Game.h"
#include <cassert>

namespace ApplesGame
{
	void AddApple(Game& game)   // Add apple function
	{
		if (game.numApples >= game.appleLimit)
			return;

		Apple newApple;
		InitApples(newApple, game);
		SetApplePosition(newApple, GetRandomPositionInRectangle(game.screenRect));
		game.apples[game.numApples] = newApple;
		game.numApples++;
	}

	
	void RemoveApple(Game& game, int index)      // Delete apple
	{
		if (index < 0 || index >= game.numApples)
			return;

		
		for (int i = index; i < game.numApples - 1; i++)        // Shift all apples one position to the left
		{
			game.apples[i] = game.apples[i + 1];
		}
		game.numApples--;
	}

	// I used to have a ClearApples function, but ClearApples is no longer needed, memory will be freed automatically

	void StartPlayingState(Game& game)
	{
		SetPlayerPosition(game.player, { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f });
		SetPlayerSpeed(game.player, INITIAL_SPEED);
		SetPlayerDirection(game.player, PlayerDirection::Right);

		// Reset all apple positions
		
		for (int i = 0; i < game.numApples; ++i)
		{
			SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
		}

		// Init stones
		
		for (int i = 0; i < NUM_STONES; ++i)
		{
			SetStonePosition(game.stones[i], GetRandomPositionInRectangle(game.screenRect));
		}

		game.numEatenApples = 0;
		game.isGameFinished = false;
		game.timeSinceGameFinish = 0;
		game.scoreText.setString("Apples eaten: " + std::to_string(game.numEatenApples));
	}

	void InitGame(Game& game, unsigned int modeFlags)
	{
		game.flags = modeFlags;

		// Defining parameters based on flags

		if (game.flags & FLAG_LIMIT_20_APPLES)
		{
			game.appleLimit = 20;
		}
		else if (game.flags & FLAG_LIMIT_50_APPLES)
		{
			game.appleLimit = 50;
		}
		else if (game.flags & FLAG_RANDOM_APPLES)
		{
			game.appleLimit = rand() % (MAX_RANDOM_APPLES - MIN_RANDOM_APPLES + 1) + MIN_RANDOM_APPLES;
		}
		else if (game.flags & FLAG_INFINITE_APPLES)
		{
			game.appleLimit = 100;
		}
		if (game.flags & FLAG_SPEED_CHANGE)
		{
			game.speedModifier = 1.5f;
		}
		else if (game.flags & FLAG_NORMAL_SPEED)
		{
			game.speedModifier = 1.0f;
		}

		
		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "/Player.png"));
		assert(game.stoneTexture.loadFromFile(RESOURCES_PATH + "/Rock.png"));
		assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "/Apple.png"));
		
		// Font loading
		assert(game.font.loadFromFile(RESOURCES_PATH + "/RobotoMediumItalic.ttf"));

		// Load sound effects
		assert(game.sounds.appleSoundBuffer.loadFromFile(RESOURCES_PATH + "/AppleEat.wav"));
		game.sounds.appleSound.setBuffer(game.sounds.appleSoundBuffer);
		assert(game.sounds.collisionSoundBuffer.loadFromFile(RESOURCES_PATH + "/Death.wav"));
		game.sounds.collisionSound.setBuffer(game.sounds.collisionSoundBuffer);

		game.screenRect = { 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT };

		InitPlayer(game.player, game);

		// Init array of apples
		game.apples = nullptr;
		game.numApples = 0;

		
		// Creating initial number of apples
		game.apples = std::make_unique<Apple[]>(game.appleLimit);
		for (int i = 0; i < game.appleLimit; i++)
		{
			Apple newApple;
			InitApples(newApple, game);
			SetApplePosition(newApple, GetRandomPositionInRectangle(game.screenRect));
			game.apples[i] = newApple;
			game.numApples++;
		}

		// Set initial speed
		SetPlayerSpeed(game.player, INITIAL_SPEED * game.speedModifier);

		// Init stones
		for (int i = 0; i < NUM_STONES; ++i)
		{
			InitStones(game.stones[i], game);
		}

		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		// Init score text
		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(24);
		game.scoreText.setFillColor(sf::Color::White);
		game.scoreText.setPosition(10, 10);

		// Init hint text
		game.helpText.setFont(game.font);
		game.helpText.setCharacterSize(18);
		game.helpText.setFillColor(sf::Color::White);
		game.helpText.setPosition(10, 40);
		game.helpText.setString("Keyboards: Move - Arrows, Mode - Num1 - Num5, Esc - Exit");

		// Init Game Over Text
		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(50);
		game.gameOverText.setFillColor(sf::Color::Red);
		game.gameOverText.setPosition(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50);
		game.gameOverText.setString("GAME OVER");

		StartPlayingState(game);
	}

	void UpdatePlayingState(Game& game, float deltaTime)
	{
		// Handle input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			SetPlayerDirection(game.player, PlayerDirection::Right);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			SetPlayerDirection(game.player, PlayerDirection::Up);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			SetPlayerDirection(game.player, PlayerDirection::Left);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			SetPlayerDirection(game.player, PlayerDirection::Down);
		}

		UpdatePlayer(game.player, deltaTime);
		
		// Game over verification
		if (!(game.flags & FLAG_INFINITE_APPLES)) // Checking only for non-infinite mode
		{
			if (game.numEatenApples >= game.appleLimit)
			{
		
				game.isGameFinished = true;
				game.timeSinceGameFinish = 0.f;

				game.sounds.collisionSound.play();

				game.gameOverText.setString("Game Over: " + std::to_string(game.numEatenApples));

				return;
			}
		}
		
		// Check collisions with apples
		for (int i = 0; i < game.numApples; ++i)
		{
			if (DoShapesCollide(GetPlayerCollider(game.player), GetAppleCollider(game.apples[i])))
			{
			
				RemoveApple(game, i);
				game.numEatenApples++;

				if (game.flags & FLAG_INFINITE_APPLES)
				{
					AddApple(game);
				}
				else
				{
					if (game.numApples < game.appleLimit)
					{
						AddApple(game);
					}
					
				}

				game.scoreText.setString("Apples eaten: " + std::to_string(game.numEatenApples));

				// Accelerate player
				if (game.flags & FLAG_SPEED_CHANGE)
				{
					SetPlayerSpeed(game.player,
					GetPlayerSpeed(game.player) + ACCELERATION * game.speedModifier);
				}

				game.sounds.appleSound.play();
			}
		}

		// Find player collisions with rocks
		for (int i = 0; i < NUM_STONES; ++i)
		{
			if (DoShapesCollide(GetPlayerCollider(game.player), GetStoneCollider(game.stones[i])))
			{
				StartGameoverState(game);
			}
		}

		// Check screen borders collision
		if (!DoShapesCollide(GetPlayerCollider(game.player), game.screenRect))
		{
			StartGameoverState(game);
		}
	}

	void StartGameoverState(Game& game)
	{
		game.isGameFinished = true;
		game.timeSinceGameFinish = 0.f;
		game.sounds.collisionSound.play();
		game.gameOverText.setString("Game Over: " + std::to_string(game.numEatenApples));
	}
	
	// Game mode selection function
	void SelectGameMode(Game& game, int mode)
	{
		unsigned int flags = 0;

		switch (mode)
		{
		case 1:
			flags = FLAG_LIMIT_20_APPLES | FLAG_SPEED_CHANGE;
			break;
		case 2:
			flags = FLAG_LIMIT_20_APPLES | FLAG_NORMAL_SPEED;
			break;
		case 3:
			flags = FLAG_LIMIT_50_APPLES | FLAG_SPEED_CHANGE;
			break;
		case 4:
			flags = FLAG_RANDOM_APPLES | FLAG_SPEED_CHANGE;
			break;
		case 5:
			flags = FLAG_INFINITE_APPLES | FLAG_SPEED_CHANGE;
			break;
		default:
			flags = FLAG_LIMIT_20_APPLES | FLAG_SPEED_CHANGE;
			break;
		}

		game.currentMode = mode;
		game.modeFlags = flags;

		InitGame(game, flags);
	}

	void UpdateGameoverState(Game& game, float deltaTime)
	{
		if (game.timeSinceGameFinish <= PAUSE_LENGHT)
		{
			game.timeSinceGameFinish += deltaTime;
			game.background.setFillColor(sf::Color::Black);
		}
		else
		{
			game.background.setFillColor(sf::Color::Black);
			StartPlayingState(game);
		}
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		// Mode flags processing
		if (game.modeFlags & FLAG_SPEED_CHANGE)
		{
			// Speed change logic
			float speed = 100.0f + game.numEatenApples * 5.0f;
		
		}
		else if (game.modeFlags & FLAG_NORMAL_SPEED)
		{
			float speed = 100.0f;

		}

		// Update game state
		if (!game.isGameFinished)
		{
			UpdatePlayingState(game, deltaTime);
		}
		else
		{
			UpdateGameoverState(game, deltaTime);
		}
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		
		window.draw(game.background);
		window.draw(game.scoreText);  
		window.draw(game.helpText);   

		DrawPlayer(game.player, window);

		// Draw apple
		for (int i = 0; i < game.numApples; ++i)
		{
			game.apples[i].sprite.setPosition(game.apples[i].Position.x, game.apples[i].Position.y);
			window.draw(game.apples[i].sprite);
		}
		// Draw stones
		for (int i = 0; i < NUM_STONES; ++i)
		{
			game.stones[i].sprite.setPosition(game.stones[i].Position.x, game.stones[i].Position.y);
			window.draw(game.stones[i].sprite);
		}
		if (game.isGameFinished == true)
		{
			window.draw(game.gameOverText);
		}
		// Draw Mode info
		std::string modeText;
		switch (game.currentMode)
		{
		case 1:
			modeText = "Mode 1: 20 apples with acceleration";
			break;
		case 2:
			modeText = "Mode 2: 20 apples without acceleration";
			break;
		case 3:
			modeText = "Mode 3: 50 apples with acceleration";
			break;
		case 4:
			modeText = "Mode 4: Random apples with acceleration";
			break;
		case 5:
			modeText = "Mode 5: Infinite apples with acceleration";
			break;
		default:
			modeText = "Default mode";
			break;
		}

		sf::Text textMode;
		textMode.setFont(game.font);
		textMode.setString(modeText);
		textMode.setCharacterSize(20);
		textMode.setPosition(10, 60);

		window.draw(textMode);
	}

	void DeinitializeGame(Game& game)
	{
		
	}

}

