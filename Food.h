#pragma once

#include <SFML/Graphics.hpp>
#include <random>

class Food
{
private:
	sf::RectangleShape sprite_;
	sf::Vector2f sprite_size_;
	sf::Vector2i grid_size_;
public:
	Food();
	~Food();

	void Initialise(sf::RenderWindow* window, sf::Vector2i grid_size);

	void Draw(sf::RenderWindow* window);
	void ResetPosition();

	sf::Vector2f GetPosition();
};

