#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Math.h"
#include "Player.h"
#include "Const.h"
#include "Apple.h"
#include "Stone.h"

namespace ApplesGame
{
	enum GameFlags
	{
		FLAG_LIMIT_20_APPLES = 1 << 2,  
		FLAG_LIMIT_50_APPLES = 1 << 3,  
		FLAG_RANDOM_APPLES = 1 << 1,    
		FLAG_INFINITE_APPLES = 1 << 0,   
		FLAG_SPEED_CHANGE = 1 << 4,     
		FLAG_NORMAL_SPEED = 1 << 5      

	};
	
	struct Game
	{
		int numApples;
		int appleLimit;
		float speedModifier;
		unsigned int flags;
		unsigned int modeFlags;
		int currentMode;

		std::unique_ptr<Apple[]> apples; //Unique pointer to apple array

		Rectangle screenRect;
		Player player;
		Stone stones[NUM_STONES];

		// Global game data
		int numEatenApples = 0;
		bool isGameFinished = false;
		float timeSinceGameFinish = 0.f;
		sf::RectangleShape background;

		struct SoundEffects
		{
			sf::SoundBuffer appleSoundBuffer;
			sf::Sound appleSound;

			sf::SoundBuffer collisionSoundBuffer;
			sf::Sound collisionSound;
		};

		//Text
		sf::Font font;
		sf::Text scoreText;
		sf::Text helpText;
		sf::Text gameOverText;

		SoundEffects sounds;

		//Resources
		sf::Texture playerTexture;
		sf::Texture stoneTexture;
		sf::Texture appleTexture;

	};

	// Adding new functions for dynamic array handling

	void AddApple(Game& game);
	void RemoveApple(Game& game, int index);
	
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinitializeGame(Game& game);

	void StartPlayingState(Game& game);
	void InitGame(Game& game, unsigned int modeFlags);
	void UpdatePlayingState(Game& game, float deltaTime);

	void StartGameoverState(Game& game);
	void SelectGameMode(Game& game, int mode);
	void UpdateGameoverState(Game& game, float deltaTime);
}
