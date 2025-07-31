#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include <ctime>
#include <iostream>

int main()
{
    using namespace ApplesGame;

    int seed = static_cast<int>(time(nullptr));
    srand(seed);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "ApplesGame");
    if (!window.isOpen())
    {
        std::cerr << "Failed to open window\n";
        return 1;
    }

    // Init game with default mode
    Game game;
    SelectGameMode(game, 1); // Setting mode 1 as default

    sf::Clock gameClock;
    float lastTime = gameClock.getElapsedTime().asSeconds();

    while (window.isOpen())
    {
        
        sf::sleep(sf::milliseconds(16));

        float currentTime = gameClock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) window.close();
                else if (event.key.code == sf::Keyboard::Num1) SelectGameMode(game, 1);
                else if (event.key.code == sf::Keyboard::Num2) SelectGameMode(game, 2);
                else if (event.key.code == sf::Keyboard::Num3) SelectGameMode(game, 3);
                else if (event.key.code == sf::Keyboard::Num4) SelectGameMode(game, 4);
                else if (event.key.code == sf::Keyboard::Num5) SelectGameMode(game, 5);
                break;
            }
        }

        UpdateGame(game, deltaTime);

        window.clear();
        DrawGame(game, window);
        window.display();
    }

    DeinitializeGame(game);
    return 0;
}