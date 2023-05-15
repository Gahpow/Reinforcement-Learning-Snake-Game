#include <iostream>

#include "Game.h"
#include "Input.h"

int main()
{
    srand(time(NULL));

    // Initialise window object
    sf::RenderWindow window(sf::VideoMode(500, 500), "CMP304AI", sf::Style::Titlebar | sf::Style::Close);

    // Initialise game objects
    Input input;
    Game game(&window, &input);
    sf::Clock clock;
    float delta_time = 0;

    // Handle all window events
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                input.SetKeyDown(event.key.code);
                break;

            case sf::Event::KeyReleased:
                input.SetKeyUp(event.key.code);
                break;

            default:
                break;
            }
        }
        
        //Calculate delta time
        delta_time = clock.restart().asSeconds();

        // Clear window with black color
        window.clear(sf::Color::Black);

        // Run game functions
        game.Update(delta_time);
        game.Render();

        window.display();
    }

    return 0;
}