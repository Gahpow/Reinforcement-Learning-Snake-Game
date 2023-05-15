#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

#include "SnakeAI.h"
#include "Food.h"
#include "Input.h"

#define GRID_SIZE 4

class Game
{
private:
	sf::RenderWindow* window_;
	Input* input_;

	// Game objects
	SnakeCharacter player_snake_;
	SnakeAI agent_snake_;
	Food food_;

	// Game variables
	bool player_snake_active_;

	// Text
	sf::Font font_;
	sf::Text text_;
public:
	Game(sf::RenderWindow* window, Input* input);
	~Game(){};

	void Update(float dt);
	void Render();
private:
	void ControlledSnake(float dt);
	void AgentSnake(float dt);
	void UpdateText();
};

